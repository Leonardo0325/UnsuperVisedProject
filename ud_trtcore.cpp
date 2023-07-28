#include "UnSuperviseDeployDlg.h"
#include "ud_trtcore.h"
#include "ud_engine.h"
#include "ud_utils.h"
//#include<windows.h>
#pragma comment( lib,"winmm.lib" )
#define MY_MSG WM_USER+100
const  int MAX_INFO_SIZE = 20;
// \! 初始化
// \@param params     初始化参数
// \@param nErrnoFlag 初始化错误码，详情见params.h
CTrtCore::CTrtCore() : m_hWnd(nullptr), m_hLogThread(nullptr), m_bStopListening(false)// : m_hWnd(nullptr), m_hLogThread(nullptr), m_bStopListening(false)
{
	m_pcUd = reinterpret_cast<CUnSuperviseDeployDlg*>(AfxGetMainWnd());
	m_hWnd = m_pcUd->GetSafeHwnd();
}
CTrtCore::~CTrtCore()
{
	if (m_hLogThread != nullptr)
	{
		// 等待日志线程结束
		WaitForSingleObject(m_hLogThread, INFINITE);
		CloseHandle(m_hLogThread);
	}
}
//void CTrtCore::tcStartListrning()
//{
//	// 获取MFC界面类的句柄
//	CUnSuperviseDeployDlg* pMainDlg = reinterpret_cast<CUnSuperviseDeployDlg*>(AfxGetMainWnd());
//	m_hWnd = pMainDlg->GetSafeHwnd();
//
//	// 创建日志线程
//	UNIT uDeploy;
//	(void*)_beginthreadex(NULL, 0, tcLogThreadProc, this, 0, &uDeploy);
//}
//UNIT _cdecl CTrtCore::tcLogThreadProc(LPVOID lparam)
//{
//	return 0;
//}
std::shared_ptr<TRTCore_ctx> CTrtCore::init(const PARAMS_S& params, PD_S32& nErrnoFlag)
{
	//::PostMessage(m_hWnd, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	//m_pcUd->m_editLoguru.SetWindowText("asdasdasd");
	std::ofstream outputFile("../loguru.txt"); // 打开output.txt文件用于写入
	if (outputFile.is_open()) { // 检查文件是否成功打开
		// 将输出信息写入文件
		outputFile << "Init Start ......\n";
	}
	LOG_F(INFO, "Init Start ......");
	try {
		// 1. 判断此电脑GPU是否兼容代码否
		if (!IsCompatible(params.gpuId)) {
			nErrnoFlag = PD_WRONG_CUDA;
			
			LOG_F(INFO, "GPU Compatible Error");
			return nullptr;
		}
		std::string log_ = std::string("Using GPU No. ") + std::to_string(params.gpuId);
		outputFile << log_.c_str()<<std::endl;
		LOG_F(INFO, log_.c_str());

		// 2. build Engine. 生成引擎
		std::shared_ptr< CTrtEngine> engine_ptr(new CTrtEngine(params, nErrnoFlag));
		if (nErrnoFlag != PD_OK) {
			outputFile << "Can't build or load engine file \n";
			LOG_F(INFO, "Can't build or load engine file");
			return nullptr;
		}

		// 3.generate Contexts pools， 通过引擎和一些配置参数，获得执行上下文，线程池
		ContextPool<ExecContext> pool;
		for (PD_S32 i = 0; i < params.maxThread; ++i)
		{
			std::unique_ptr<ExecContext> context(new ExecContext(params.gpuId, engine_ptr->Get()));
			pool.Push(std::move(context));
		}
		if (pool.Size() == 0) {
			nErrnoFlag = PD_WRONG_CUDA;
			outputFile << "No suitable CUDA device \n";
			LOG_F(INFO, "No suitable CUDA device");
			return nullptr;
		}

		// 4.产生执行上下文
		std::shared_ptr<TRTCore_ctx> ctx(new TRTCore_ctx{ params,  engine_ptr, std::move(pool) });
		outputFile << "Init Successfully !!!! \n";
		LOG_F(INFO, "Init Successfully !!!!");

		return ctx;
	}
	catch (const std::invalid_argument& ex) {
		outputFile << "Init failed !!! \n";
		LOG_F(INFO, "Init failed !!!!");
		nErrnoFlag = PD_UNKNOW_ERROR;
		return nullptr;
	}
}
cv::Mat Resize(const cv::Mat& src, PD_S32 dst_height, PD_S32 dst_width,
	const std::string& PD_S32erpolation) {
	cv::Mat dst(dst_height, dst_width, src.type());
	if (PD_S32erpolation == "bilinear") {
		cv::resize(src, dst, dst.size(), 0, 0, cv::INTER_LINEAR);
	}
	else if (PD_S32erpolation == "nearest") {
		cv::resize(src, dst, dst.size(), 0, 0, cv::INTER_NEAREST);
	}
	else if (PD_S32erpolation == "area") {
		cv::resize(src, dst, dst.size(), 0, 0, cv::INTER_AREA);
	}
	else if (PD_S32erpolation == "bicubic") {
		cv::resize(src, dst, dst.size(), 0, 0, cv::INTER_CUBIC);
	}
	else if (PD_S32erpolation == "lanczos") {
		cv::resize(src, dst, dst.size(), 0, 0, cv::INTER_LANCZOS4);
	}
	else {
		assert(0);
	}
	return dst;
}
cv::Mat Normalize(cv::Mat& src, const std::vector<PD_FLOAT>& mean, const std::vector<PD_FLOAT>& std,
	bool to_rgb, bool inplace) {
	assert(src.channels() == mean.size());
	assert(mean.size() == std.size());

	cv::Mat dst;

	if (src.depth() == CV_32F) {
		dst = inplace ? src : src.clone();
	}
	else {
		src.convertTo(dst, CV_32FC(src.channels()));
	}

	if (to_rgb && dst.channels() == 3) {
		cv::cvtColor(dst, dst, cv::COLOR_BGR2RGB);
	}

	auto _mean = mean;
	auto _std = std;
	for (auto i = mean.size(); i < 4; ++i) {
		_mean.push_back(0.);
		_std.push_back(1.0);
	}
	cv::Scalar mean_scalar(_mean[0], _mean[1], _mean[2], _mean[3]);
	cv::Scalar std_scalar(1.0 / _std[0], 1.0 / _std[1], 1.0 / _std[2], 1.0 / _std[3]);

	cv::subtract(dst, mean_scalar, dst);
	cv::multiply(dst, std_scalar, dst);
	return dst;
}
//异常检测预处理
cv::Mat pre_process(cv::Mat& image) {
	std::vector<PD_FLOAT> mean = { 0.485, 0.456, 0.406 };
	std::vector<PD_FLOAT> std = { 0.229, 0.224, 0.225 };
	cv::Mat resized_image = Resize(image, 224, 224, "area");
	// 归一化
	// convertTo直接将所有值除以255,normalize的NORM_MINMAX是将原始数据范围变换到0~1之间,convertTo更符合深度学习的做法
	resized_image.convertTo(resized_image, CV_32FC3, 1.0 / 255, 0);
	// 标准化
	resized_image = Normalize(resized_image, mean, std);
	return resized_image;
}



// \! 异常检测
// \@param ctx:执行上下文
// \@param cvImages:输入图片列表，Mat
// \@param outputs:输出图片数组，Mat
returnResult_S CTrtCore::anomaly(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<cv::Mat>& cvImages, std::vector<cv::Mat>& outputs, PD_S32 thresh)
{
	cv::Mat M, M1, T, F;
	// 1.ctx是否初始化成功
	if (ctx == nullptr) {
		LOG_F(INFO, "Init failed, can't call anomaly.");
		return returnResult_S(M, M1, T, F, PD_UNKNOW_ERROR);
	}

	// 2. NetType 是否正确
	//netType=4,表示异常检测
	if (ctx->params.netType != PD_NETWORK_ANOMALY)
	{
		LOG_F(INFO, "Illegal calls，please check your NetWorkType");
		return returnResult_S(M, M1, T, F, PD_WRONG_CALL);
	}

	// 3.Engine信息与输入信息对比
	PD_S32 engine_batch, engine_channels, engine_height, engine_width;
	//this是指当前指向
	this->getInputDims(ctx, engine_batch, engine_channels, engine_height, engine_width);// 获得输入维度信息
	PD_S32 engine_output_batch, engine_output_channel, engine_output_height, engine_output_width;
	this->getOutputDims(ctx, engine_output_batch, engine_output_channel, engine_output_height, engine_output_width);//获得输出维度信息
	auto engine_input_name = this->getInputNames(ctx);  //获得输入名称
	auto engine_output_name = this->getOutputNames(ctx);//获得输出名称
	// 3.1 batchsize判断
	if (cvImages.size() > engine_batch) {
		LOG_F(INFO, "输入图片数组的batchsize超过Engine预定值 ");
		return returnResult_S(M, M1, T, F, PD_WRONG_IMG);
	}
	// 3.2 c,h,w判断
	std::vector<cv::Mat> imgs;//存放最终的imgs
	for (PD_S32 i = 0; i < cvImages.size(); i++) {
		cv::Mat cv_img = cvImages[i].clone();
		if (cv_img.channels() != engine_channels) {
			LOG_F(ERROR, "输入图片的通道数与Engine不符 ");
			return returnResult_S(M, M1, T, F, PD_WRONG_IMG);
		}
		std::cout << cv_img.cols << std::endl;
		if (engine_height != cv_img.cols || engine_width != cv_img.rows) {
			LOG_F(WARNING, "输入的图片尺寸与Engine不相符,自动resize");
			/*uchar* firstRow = cv_img.ptr<uchar>(0, 0);
			std::cout << cv_img << std::endl;*/
			cv::resize(cv_img, cv_img, cv::Size(engine_height, engine_width), cv::INTER_LINEAR); // 使用此方法与onnx（python）时像对应
			PD_S32 height1 = cv_img.rows;
			PD_S32 width1 = cv_img.cols;
			PD_S32 nc = cv_img.channels();
			cv::Mat dst;
			dst.create(cv_img.size(), cv_img.type());
			for (PD_S32 row = 0; row < height1; row++) {
				for (PD_S32 col = 0; col < width1; col++) {
					/*PD_S32 b = image.at<cv::Vec3b>(row, col)[0];
					PD_S32 g = image.at<cv::Vec3b>(row, col)[1];
					PD_S32 r = image.at<cv::Vec3b>(row, col)[2];*/
					PD_S32 r = cv_img.at<cv::Vec3b>(row, col)[2];
					//std::cout << r << std::endl;
					//PD_S32 g = image.at<uchar>(row, col);
					//PD_S32 r = image.at<uchar>(row, col);
					//std::cout << r << std::endl;
					/*dst.at<cv::Vec3b>(row, col)[0] = 255 - b;
					dst.at<cv::Vec3b>(row, col)[1] = 255 - g;
					dst.at<cv::Vec3b>(row, col)[2] = 255 - r;*/
				}
			}
			uchar * firstRow1 = cv_img.ptr<uchar>(0, 0);//获取像素值
			//std::cout << cv_img << std::endl;
		}
		imgs.push_back(cv_img);
	}
	if (imgs.empty()) {
		LOG_F(INFO, "No images, please check");
		return returnResult_S(M, M1, T, F, PD_WRONG_IMG);
	}
	cv::Mat img;
	img = imgs[0];
	// 4. 预处理图片
	samplesCommon::BufferManager buffers(ctx.get()->engine->Get());// 分配显存（输入和输出）
	// 预处理
	if (PD_OK != normalization(buffers, imgs, ctx.get()->params, engine_input_name))
	{
		LOG_F(INFO, "CPU2GPU 内存拷贝失败");
		return returnResult_S(M, M1, T, F, PD_UNKNOW_ERROR);
	}

	buffers.copyInputToDevice();
	DWORD t1, t2;
	//t1 = timeGetTime();
	// 5. 执行推理过程
	// mContext->executeV2 是这个项目中最核心的一句代码，模型检测就是这一步。
	ScopedContext<ExecContext> context(ctx->pool);
	auto ctx_ = context->getContext();
	if (!ctx_->executeV2(buffers.getDeviceBindings().data()))
	{
		LOG_F(INFO, "执行推理失败");
		return returnResult_S(M, M1, T, F, PD_UNKNOW_ERROR);
	}
	//t2 = timeGetTime();
	//std::cout << "模型推理时间 : " << (PD_DOUBLE)(t2 - t1) << "ms" << std::endl;
	// 6. 后处理
	buffers.copyOutputToHost();

	ReturnImgs_S anomalyImg = anomalyPost(
		buffers,
		img,
		outputs,
		engine_output_batch,
		engine_output_channel,
		engine_output_height,
		engine_output_width,
		engine_output_name,
		thresh);
	returnResult_S ImgResults;
	ImgResults.Img = anomalyImg.Img;
	ImgResults.Img1 = anomalyImg.Img1;
	ImgResults.nLY = anomalyImg.nLY;
	ImgResults.FinalImg = anomalyImg.FinalImg;
	ImgResults.ThreshImg = anomalyImg.ThreshImg;
	//std::cout << outputs[1] << std::endl;

	//if (PD_OK != anomalyPostFlag) {
	//	LOG_F(INFO, "GPU2CPU 内存拷贝失败");
	//	return returnResult_S(M, M1, T, F, PD_UNKNOW_ERROR);
	//}

	return ImgResults;

}


// \! 获取显卡数量
// \@param ctx:执行上下文
// \@param number:gpu数量
PD_S32 CTrtCore::getNumberGPU(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& number)
{
	cudaError_t st = cudaGetDeviceCount(&number);
	if (st != cudaSuccess) {
		LOG_F(INFO, "Could not list CUDA devices");
		return 0;
	}
	return PD_OK;
}

// \! 获取输入维度
// \@param ctx:执行上下文
// \@param nBatch:batchsize
// \@param nChannels:channels
// \@param nHeight:height
// \@param nWidth:width
// \@param index:第index个输入，加入onnx有多个输入，则通过index来指定
PD_S32 CTrtCore::getInputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nChannels, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index)
{
	if (ctx == nullptr) {
		LOG_F(INFO, "init failed, can't call getDims");
		return PD_UNKNOW_ERROR;
	}
	auto inputDims = ctx->engine->mInputDims[index];
	nBatch = inputDims.d[0];
	nChannels = inputDims.d[1];
	nHeight = inputDims.d[2];
	nWidth = inputDims.d[3];
	return PD_OK;
}

// \! 获取输出维度
// \@param ctx：执行上下文
// \@param nBatch:batchsize
// \@param nHeight:Height
// \@param nWidth:Width
// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
PD_S32 CTrtCore::getOutputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nChannel, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index)
{
	if (ctx == nullptr) {
		LOG_F(INFO, "init failed, can't call getDims");
		return PD_UNKNOW_ERROR;
	}
	auto outputDims = ctx->engine->mOutputDims[index];
	nBatch = outputDims.d[0];
	nChannel = outputDims.d[1];
	nHeight = outputDims.d[2];
	nWidth = outputDims.d[3];
	return PD_OK;
}
// \! 获取输出维度
// \@param ctx：执行上下文
// \@param nBatch:batchsize
// \@param nNumClass:NumClass 类别数，针对分类
// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
PD_S32 CTrtCore::getOutputDims2(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nNumClass, PD_S32 index)
{
	if (ctx == nullptr) {
		LOG_F(INFO, "init failed, can't call getDims");
		return PD_UNKNOW_ERROR;
	}
	auto outputDims = ctx->engine->mOutputDims[index];
	nBatch = outputDims.d[0];
	nNumClass = outputDims.d[1];
	return PD_OK;
}

// \! 获取输入名称
// \@param ctx：执行上下文
// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
std::string CTrtCore::getInputNames(std::shared_ptr<TRTCore_ctx> ctx, PD_S32 index)
{
	auto engine_input_name = ctx.get()->engine->mInputTensorNames[index];
	return engine_input_name;
}

// \! 获取输出名称
// \@param ctx：执行上下文
// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
std::string CTrtCore::getOutputNames(std::shared_ptr<TRTCore_ctx> ctx, PD_S32 index)
{
	auto engine_output_name = ctx.get()->engine->mOutputTensorNames[index];
	return engine_output_name;
}