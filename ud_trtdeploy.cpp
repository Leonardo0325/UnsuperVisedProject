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
	//2.��ʼ��
	int nFlag;
	ctx = m_pTrtAPI->init(params, nFlag);
}
returnResult_S CTrtDeploy::run(string G_sImgPth,int nThreshValue,string ModelPth)
{
	//1.���ò���
	cv::Mat SrcImg = cv::imread(G_sImgPth, cv::IMREAD_COLOR);
	////2.��ʼ��
	//PD_S32 nFlag;
	//auto ctx = m_pTrtAPI->init(params, nFlag);
	//3.��������
	std::vector<std::string> file_names;
	//cv::glob("H:\\DataSets-Img\\USING\\3.4\\Bad-Deal_Rotate", file_names);
	cv::Mat image = cv::imread("..\\test\\Bad-Deal_Rotate 1\\3000.jpg");
	cv::Mat InferImg1;
	//cv::glob("..\\test\\Bad-Deal_Rotate 1", file_names);
	PD_S32 batch_size, channels, height, width;
	m_pTrtAPI->getInputDims(ctx, batch_size, channels, height, width); // ���onnx�е�����ά��
	//for (PD_S32 nn = 0; nn < file_names.size() / batch_size; nn++) { // ����ͼƬ�ܶ࣬��Ҫѭ�����������
	std::vector<CCoreImage*> inputs; // API�ӿڵ�����
	std::vector<CCoreImage*> outputs; // API�ӿڵ�����
	// �������ʵ���ͼƬ���ڴ�ռ�
	std::vector<cv::Mat> inputsCvImage; // ���CV::Mat �ڴ˴�ѭ���б����ڴ�
	std::vector<cv::Mat> outputsCvImage; // ���CV::Mat �ڴ˴�ѭ���б����ڴ�
	// ����ǽ�CVͼƬת��CCoreImageͼƬ���ڴ�ռ�
	//����װ��1��batch���ڴ�ռ� inputCCoreImage,����CCoreImage����
	//CCoreImage�к��еı�����
	//PD_S32 channal,   ͨ��
	//PD_S32 width,   ��
	//PD_S32 height,  ��
	//PD_S32 step,  ÿ���ֽ���
	//unsigned char* data  ͼ������
	//����������Ƿŵ��ڴ��н��е�
	CCoreImage* inputCCoreImage = new CCoreImage[batch_size];// ���CCoreImage�����飬�ڴ˴�ѭ���б������ڴ���
														  // �����д��ͼ���ַ������һ���ڴ�ռ�
	CCoreImage* outputsCCoreImage = new CCoreImage[batch_size];// ���CCoreImage�����飬�ڴ˴�ѭ���б������ڴ���

	// ��һ��batchsizeͼƬ����-->CVImage-->CCoreImage�����մ�ŵ�inputsCvImage��outputsCvImage��
	for (PD_S32 b = 0; b < batch_size; b++) {  // ÿ�δ�����batchsize��batchsize��С = onnx�е�batchsize��С
		// 3.1 ����batch_size�е�һ��ͼƬ--����
		//=ÿ��ȡһ��ͼƬ��b����1������b�Ĵ�С��batchsize����
		//ͼƬ���ն�������inputsCvImage���������
		//��������е����⣬���bӦ�÷ŵ�����Ա���bs��=1�����Ҳ����ʹ��
		//���ڵ���˼��Ĭ��bs=1
		inputsCvImage.push_back(SrcImg);// �õ�CV��ʽͼƬ�������ڴ�
		inputCCoreImage[b].SetValue(
			inputsCvImage[b].channels(),
			inputsCvImage[b].cols,
			inputsCvImage[b].rows,
			inputsCvImage[b].step,
			(unsigned char*)inputsCvImage[b].data
		);// �õ�CCoreImage��ʽͼƬ�������ڴ�
		inputs.push_back(&inputCCoreImage[b]);//ת�������ʽ

		// 3.2 ����batch_size�е�һ��ͼƬ--���
		//����һ��mask��Ĥ��
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

	// 4. ����
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
	m_pTrtAPI->getInputDims(ctx, batch_size, channels, height, width); // ���onnx�е�����ά��
	for (PD_S32 nn = 0; nn < file_names.size() / batch_size; nn++) { // ����ͼƬ�ܶ࣬��Ҫѭ�����������
		std::vector<CCoreImage*> inputs; // API�ӿڵ�����
		std::vector<CCoreImage*> outputs; // API�ӿڵ�����
		// �������ʵ���ͼƬ���ڴ�ռ�
		std::vector<cv::Mat> inputsCvImage; // ���CV::Mat �ڴ˴�ѭ���б����ڴ�
		std::vector<cv::Mat> outputsCvImage; // ���CV::Mat �ڴ˴�ѭ���б����ڴ�
		// ����ǽ�CVͼƬת��CCoreImageͼƬ���ڴ�ռ�
		//����װ��1��batch���ڴ�ռ� inputCCoreImage,����CCoreImage����
		//CCoreImage�к��еı�����
		//PD_S32 channal,   ͨ��
		//PD_S32 width,   ��
		//PD_S32 height,  ��
		//PD_S32 step,  ÿ���ֽ���
		//unsigned char* data  ͼ������
		//����������Ƿŵ��ڴ��н��е�
		CCoreImage* inputCCoreImage = new CCoreImage[batch_size];// ���CCoreImage�����飬�ڴ˴�ѭ���б������ڴ���
															  // �����д��ͼ���ַ������һ���ڴ�ռ�
		CCoreImage* outputsCCoreImage = new CCoreImage[batch_size];// ���CCoreImage�����飬�ڴ˴�ѭ���б������ڴ���

		// ��һ��batchsizeͼƬ����-->CVImage-->CCoreImage�����մ�ŵ�inputsCvImage��outputsCvImage��
		for (PD_S32 b = 0; b < batch_size; b++) {  // ÿ�δ�����batchsize��batchsize��С = onnx�е�batchsize��С
			// 3.1 ����batch_size�е�һ��ͼƬ--����
			//=ÿ��ȡһ��ͼƬ��b����1������b�Ĵ�С��batchsize����
			//ͼƬ���ն�������inputsCvImage���������
			//��������е����⣬���bӦ�÷ŵ�����Ա���bs��=1�����Ҳ����ʹ��
			//���ڵ���˼��Ĭ��bs=1
			inputsCvImage.push_back(cv::imread(file_names[nn * batch_size + b], cv::IMREAD_COLOR));// �õ�CV��ʽͼƬ�������ڴ�
			inputCCoreImage[b].SetValue(
				inputsCvImage[b].channels(),
				inputsCvImage[b].cols,
				inputsCvImage[b].rows,
				inputsCvImage[b].step,
				(unsigned char*)inputsCvImage[b].data
			);// �õ�CCoreImage��ʽͼƬ�������ڴ�
			inputs.push_back(&inputCCoreImage[b]);//ת�������ʽ

			// 3.2 ����batch_size�е�һ��ͼƬ--���
			//����һ��mask��Ĥ��
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

		// 4. ����
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
