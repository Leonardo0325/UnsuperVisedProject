#include"ud_trtdeploy.h"
using namespace std;
CTrtDeploy::CTrtDeploy()
{
	m_pTrtAPI = nullptr;
	m_pTrtAPI = new CTrtAPI();
}
CTrtDeploy::CTrtDeploy(string m_modelPth)
{
	ModelPth = m_modelPth;
}
CTrtDeploy::~CTrtDeploy()
{
	if (m_pTrtAPI != nullptr)
	{
		delete m_pTrtAPI;
	}
}
PD_VOID CTrtDeploy::Init(string ModelPth)
{
	params.onnxFilePath = ModelPth + ".onnx";
	params.engineFilePath = ModelPth + ".engine";
	
	//params.onnxFilePath = "..\\model\\LED-PaDim-SIM_384.onnx";
	//params.engineFilePath = "..\\model\\LED-PaDim-SIM_384.engine";
	params.netType = PD_NETWORK_ANOMALY;
	params.fp16 = PD_FALSE;
	params.maxThread = 1;
	params.meanValue = { 0.485,0.456,0.406 };
	params.stdValue = { 0.229,0.224,0.225 };
	params.log_path = "TRT_Log.txt";
	params.gpuId = 0;
	cout << params.log_path << endl;
	//2.初始化
	int nFlag;
	ctx = m_pTrtAPI->init(params, nFlag);
}
returnResult_S CTrtDeploy::run(string G_sImgPth,int nThreshValue,string ModelPth)
{
	//1.配置参数
	cv::Mat SrcImg = cv::imread(G_sImgPth, cv::IMREAD_COLOR);
	////2.初始化
	//PD_S32 nFlag;
	//auto ctx = m_pTrtAPI->init(params, nFlag);
	//3.输入数据
	std::vector<std::string> file_names;
	//cv::glob("H:\\DataSets-Img\\USING\\3.4\\Bad-Deal_Rotate", file_names);
	cv::Mat image = cv::imread("..\\test\\Bad-Deal_Rotate 1\\3000.jpg");
	cv::Mat InferImg1;
	//cv::glob("..\\test\\Bad-Deal_Rotate 1", file_names);
	PD_S32 batch_size, channels, height, width;
	m_pTrtAPI->getInputDims(ctx, batch_size, channels, height, width); // 获得onnx中的输入维度
	//for (PD_S32 nn = 0; nn < file_names.size() / batch_size; nn++) { // 假如图片很多，需要循环多次来处理
	std::vector<CCoreImage*> inputs; // API接口的输入
	std::vector<CCoreImage*> outputs; // API接口的输入
	// 这个是真实存放图片的内存空间
	std::vector<cv::Mat> inputsCvImage; // 存放CV::Mat 在此次循环中保留内存
	std::vector<cv::Mat> outputsCvImage; // 存放CV::Mat 在此次循环中保留内存
	// 这个是将CV图片转成CCoreImage图片的内存空间
	//开辟装下1个batch的内存空间 inputCCoreImage,属于CCoreImage类型
	//CCoreImage中含有的变量：
	//PD_S32 channal,   通道
	//PD_S32 width,   宽
	//PD_S32 height,  高
	//PD_S32 step,  每行字节数
	//unsigned char* data  图像数据
	//下面操作都是放到内存中进行的
	CCoreImage* inputCCoreImage = new CCoreImage[batch_size];// 存放CCoreImage的数组，在此次循环中保留在内存中
														  // 数组中存放图像地址，开辟一块内存空间
	CCoreImage* outputsCCoreImage = new CCoreImage[batch_size];// 存放CCoreImage的数组，在此次循环中保留在内存中

	// 将一个batchsize图片读入-->CVImage-->CCoreImage，最终存放到inputsCvImage和outputsCvImage中
	for (PD_S32 b = 0; b < batch_size; b++) {  // 每次处理多个batchsize，batchsize大小 = onnx中的batchsize大小
		// 3.1 处理batch_size中的一个图片--输入
		//=每读取一张图片，b增加1，最终b的大小由batchsize决定
		//图片最终都会存放在inputsCvImage这个容器中
		//这个代码有点问题，最后b应该放到最后，以便于bs！=1的情况也可以使用
		//现在的意思是默认bs=1
		inputsCvImage.push_back(SrcImg);// 得到CV格式图片，放入内存
		inputCCoreImage[b].SetValue(
			inputsCvImage[b].channels(),
			inputsCvImage[b].cols,
			inputsCvImage[b].rows,
			inputsCvImage[b].step,
			(unsigned char*)inputsCvImage[b].data
		);// 得到CCoreImage格式图片，放入内存
		inputs.push_back(&inputCCoreImage[b]);//转成输入格式

		// 3.2 处理batch_size中的一个图片--输出
		//创建一个mask掩膜，
		outputsCvImage.push_back(cv::Mat::zeros(cv::Size(224, 224), CV_8UC1));
		outputsCCoreImage[b].SetValue(
			outputsCvImage[b].channels(),
			outputsCvImage[b].cols,
			outputsCvImage[b].rows,
			outputsCvImage[b].step,
			(unsigned char*)outputsCvImage[b].data
		);
		outputs.push_back(&outputsCCoreImage[b]);
	}

	// 4. 推理
	TimeTick time;
	time.start();
	returnResult_S ImgResults;
	ImgResults = m_pTrtAPI->anomaly(InferImg1, ctx, inputs, outputs, nThreshValue);
	time.end();
	std::cout << "Infer Time : " << time.mInterval * 1000 << "ms" << std::endl;
	return ImgResults;
}

returnResult_S CTrtDeploy::TD_AllDeploy(string G_sImgPth)
{
	std::vector<std::string> file_names;
	cv::glob(G_sImgPth, file_names);
	cv::Mat InferImg1;
	PD_S32 batch_size, channels, height, width;
	m_pTrtAPI->getInputDims(ctx, batch_size, channels, height, width); // 获得onnx中的输入维度
	for (PD_S32 nn = 0; nn < file_names.size() / batch_size; nn++) { // 假如图片很多，需要循环多次来处理
		std::vector<CCoreImage*> inputs; // API接口的输入
		std::vector<CCoreImage*> outputs; // API接口的输入
		// 这个是真实存放图片的内存空间
		std::vector<cv::Mat> inputsCvImage; // 存放CV::Mat 在此次循环中保留内存
		std::vector<cv::Mat> outputsCvImage; // 存放CV::Mat 在此次循环中保留内存
		// 这个是将CV图片转成CCoreImage图片的内存空间
		//开辟装下1个batch的内存空间 inputCCoreImage,属于CCoreImage类型
		//CCoreImage中含有的变量：
		//PD_S32 channal,   通道
		//PD_S32 width,   宽
		//PD_S32 height,  高
		//PD_S32 step,  每行字节数
		//unsigned char* data  图像数据
		//下面操作都是放到内存中进行的
		CCoreImage* inputCCoreImage = new CCoreImage[batch_size];// 存放CCoreImage的数组，在此次循环中保留在内存中
															  // 数组中存放图像地址，开辟一块内存空间
		CCoreImage* outputsCCoreImage = new CCoreImage[batch_size];// 存放CCoreImage的数组，在此次循环中保留在内存中

		// 将一个batchsize图片读入-->CVImage-->CCoreImage，最终存放到inputsCvImage和outputsCvImage中
		for (PD_S32 b = 0; b < batch_size; b++) {  // 每次处理多个batchsize，batchsize大小 = onnx中的batchsize大小
			// 3.1 处理batch_size中的一个图片--输入
			//=每读取一张图片，b增加1，最终b的大小由batchsize决定
			//图片最终都会存放在inputsCvImage这个容器中
			//这个代码有点问题，最后b应该放到最后，以便于bs！=1的情况也可以使用
			//现在的意思是默认bs=1
			inputsCvImage.push_back(cv::imread(file_names[nn * batch_size + b], cv::IMREAD_COLOR));// 得到CV格式图片，放入内存
			inputCCoreImage[b].SetValue(
				inputsCvImage[b].channels(),
				inputsCvImage[b].cols,
				inputsCvImage[b].rows,
				inputsCvImage[b].step,
				(unsigned char*)inputsCvImage[b].data
			);// 得到CCoreImage格式图片，放入内存
			inputs.push_back(&inputCCoreImage[b]);//转成输入格式

			// 3.2 处理batch_size中的一个图片--输出
			//创建一个mask掩膜，
			outputsCvImage.push_back(cv::Mat::zeros(cv::Size(384,384), CV_8UC1));
			outputsCCoreImage[b].SetValue(
				outputsCvImage[b].channels(),
				outputsCvImage[b].cols,
				outputsCvImage[b].rows,
				outputsCvImage[b].step,
				(unsigned char*)outputsCvImage[b].data
			);
			outputs.push_back(&outputsCCoreImage[b]);
		}

		// 4. 推理
		TimeTick time;
		time.start();
		returnResult_S ImgResults;
		ImgResults = m_pTrtAPI->anomaly(InferImg1, ctx, inputs, outputs, 7);
		time.end();
		std::cout << "Infer Time : " << time.mInterval * 1000 << "ms" << std::endl;
		return ImgResults;
	}

}


//}
