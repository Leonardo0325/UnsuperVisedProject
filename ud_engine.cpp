
#include "ud_engine.h"

// build�ͼ��������ʱ����Ҫ�õ����
//SampleErrorRecorder gRecorder;
namespace sample
{
	Logger gLogger{ Logger::Severity::kINFO };
	LogStreamConsumer gLogVerbose{ LOG_VERBOSE(gLogger) };
	LogStreamConsumer gLogInfo{ LOG_INFO(gLogger) };
	LogStreamConsumer gLogWarning{ LOG_WARN(gLogger) };
	LogStreamConsumer gLogError{ LOG_ERROR(gLogger) };
	LogStreamConsumer gLogFatal{ LOG_FATAL(gLogger) };

	PD_VOID setReportableSeverity(Logger::Severity severity)
	{
		gLogger.setReportableSeverity(severity);
		gLogVerbose.setReportableSeverity(severity);
		gLogInfo.setReportableSeverity(severity);
		gLogWarning.setReportableSeverity(severity);
		gLogError.setReportableSeverity(severity);
		gLogFatal.setReportableSeverity(severity);
	}
} // namespace sample

// \!���湹��
// \@param ����PARAMS_S����
// \@param �������nErrorFlag
CTrtEngine::CTrtEngine(const PARAMS_S& params, PD_S32& nErrnoFlag)
{
	// 1.��ʼ��Engine��build����load
	std::ifstream fin(params.engineFilePath);
	if (fin) {
		nErrnoFlag = loadEngine(params);  // ����Engine�ļ�
		if (nErrnoFlag != PD_OK)
		{
			LOG_F(INFO, "Loading Engine Failed");
			return;
		}
	}
	else {
		nErrnoFlag = buildONNX(params); // ����Engine�ļ���������
		if (nErrnoFlag != PD_OK) {
			LOG_F(INFO, "Building ONNX Failed");
			return;
		}
	}

	// 2. ����������ά��
	PD_S32 num_input_output = mEngine->getNbBindings();
	for (PD_S32 i = 0; i < num_input_output; i++) {
		if (mEngine->bindingIsInput(i)) {//���������ڵ�
			auto inputName_i = mEngine->getBindingName(i); // ���i������ڵ�����
			mInputTensorNames.push_back(inputName_i);		// ���ڵ����Ʒ���mInputTensorNames
			auto inputDims_i = mEngine->getBindingDimensions(i); // ��ýڵ�i��ά��
			mInputDims.push_back(inputDims_i);
		}
		else {//���������ڵ�
			auto outputName_i = mEngine->getBindingName(i); // ���i������ڵ�����
			mOutputTensorNames.push_back(outputName_i);		// ���ڵ����Ʒ���mOutputTensorNames
			auto outputDims_i = mEngine->getBindingDimensions(i); // ��ýڵ�i��ά��
			mOutputDims.push_back(outputDims_i);
		}
	}
	nErrnoFlag = PD_OK;
}

// \! ��ȡICudaEngineָ��
std::shared_ptr<nvinfer1::ICudaEngine> CTrtEngine::Get() const
{
	return mEngine;
}

// \! ����Engine�ļ�
// \@param  params �����ṹ��
PD_S32 CTrtEngine::loadEngine(const PARAMS_S& params)
{
	std::fstream file;
	ICudaEngine* engine;
	file.open(params.engineFilePath, std::ios::binary | std::ios::in);
	if (!file.is_open())
	{
		LOG_F(INFO, (std::string("Can't load Engine file from: ") + params.engineFilePath).c_str());
		return PD_WRONG_FILE;
	}
	file.seekg(0, std::ios::end);
	PD_S32 length = file.tellg();
	file.seekg(0, std::ios::beg);
	std::unique_ptr<char[]> data(new char[length]);
	file.read(data.get(), length);
	file.close();
	SampleUniquePtr<IRuntime> runTime(createInferRuntime(sample::gLogger));
	if (runTime == nullptr) {
		LOG_F(INFO, "CreateInferRuntime error");
		return PD_UNKNOW_ERROR;
	}
	engine = runTime->deserializeCudaEngine(data.get(), length, nullptr);
	if (engine == nullptr) {
		LOG_F(INFO, "DeserializeCudaEngine error");
		return PD_UNKNOW_ERROR;
	}
	mEngine = std::shared_ptr<nvinfer1::ICudaEngine>(engine, samplesCommon::InferDeleter());
	return PD_OK;
}

// \! �����ʹ洢Engine
// \@param  params �����ṹ��
PD_S32 CTrtEngine::buildONNX(const PARAMS_S& params)
{
	LOG_F(INFO, "Building engine from onnx file, this may take few minutes, please wait ...");
	// 1. builder
	auto builder = SampleUniquePtr<nvinfer1::IBuilder>(nvinfer1::createInferBuilder(sample::gLogger.getTRTLogger()));
	if (!builder)
	{
		LOG_F(INFO, "CreateInferBuilder error");
		return PD_UNKNOW_ERROR;
	}

	// 2. network
	const auto explicitBatch = 1U << static_cast<uint32_t>(NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
	auto network = SampleUniquePtr<nvinfer1::INetworkDefinition>(builder->createNetworkV2(explicitBatch));
	if (!network)
	{
		LOG_F(INFO, "CreateNetworkV2 error");
		return PD_UNKNOW_ERROR;
	}

	// 3. config
	auto config = SampleUniquePtr<nvinfer1::IBuilderConfig>(builder->createBuilderConfig());
	if (!config)
	{
		LOG_F(INFO, "createBuilderConfig error");
		return PD_UNKNOW_ERROR;
	}
	config->setMaxWorkspaceSize(2048_MiB);
	if (params.fp16)
	{
		config->setFlag(BuilderFlag::kFP16);
	}

	// 4. parser
	auto parser = SampleUniquePtr<nvonnxparser::IParser>(nvonnxparser::createParser(*network, sample::gLogger.getTRTLogger()));
	if (!parser)
	{
		LOG_F(INFO, "createParser error");
		return PD_UNKNOW_ERROR;
	}

	// 5. parsed
	//����ONNXģ��
	auto parsed = parser->parseFromFile(params.onnxFilePath.c_str(),
		static_cast<PD_S32>(sample::gLogger.getReportableSeverity()));
	if (!parsed)
	{
		LOG_F(INFO, "parse onnx File error");
		return PD_WRONG_FILE;
	}

	// 6. profileStream
	auto profileStream = samplesCommon::makeCudaStream();
	if (!profileStream)
	{
		LOG_F(INFO, "makeCudaStream error");
		return PD_UNKNOW_ERROR;
	}
	config->setProfileStream(*profileStream);

	// 7. plan
	SampleUniquePtr<IHostMemory> plan{ builder->buildSerializedNetwork(*network, *config) };
	if (!plan)
	{
		LOG_F(INFO, "builder->buildSerializedNetwork error");
		return PD_UNKNOW_ERROR;
	}

	// 8. runtime
	SampleUniquePtr<IRuntime> runtime{ createInferRuntime(sample::gLogger.getTRTLogger()) };
	if (!runtime)
	{
		LOG_F(INFO, "createInferRuntime error");
		return PD_UNKNOW_ERROR;
	}

	// 9. icudaEngine
	mEngine = std::shared_ptr<nvinfer1::ICudaEngine>(
		runtime->deserializeCudaEngine(plan->data(), plan->size()), samplesCommon::InferDeleter());
	if (!mEngine)
	{
		LOG_F(INFO, "deserializeCudaEngine error");
		return PD_UNKNOW_ERROR;
	}

	// 10.save
	SampleUniquePtr<IHostMemory> serializedModel(mEngine->serialize());
	std::ofstream p(params.engineFilePath.c_str(), std::ios::binary);
	p.write((const char*)serializedModel->data(), serializedModel->size());
	p.close();
	return PD_OK;
}