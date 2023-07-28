#define TYPE_CTrtAPI_API_EXPORTS
#include <afx.h>
#include <windows.h>

#include "ud_trtapi.h"
#include "ud_trtcore.h"

// \! ���캯��
CTrtAPI::CTrtAPI()
{
	m_pTRTCore = nullptr;
	m_pTRTCore = new CTrtCore();
}

// \! ��������
CTrtAPI::~CTrtAPI()
{
	if (m_pTRTCore != nullptr)
	{
		delete m_pTRTCore;
	}
}

// \! ��ʼ��
// \@param:params     ��ʼ������
// \@param:nErrnoFlag ��ʼ�������룬�����params.h
std::shared_ptr<TRTCore_ctx> CTrtAPI::init(const PARAMS_S& params, PD_S32& nErrnoFlag)
{
	// 1. ������־
	loguru::add_file(params.log_path.c_str(), loguru::Append, loguru::Verbosity_MAX);

	// 2. ��ʼ��һ��CTrtCoreʵ��

	if (m_pTRTCore == nullptr) {
		LOG_F(INFO, "new CTrtCore() Error!!!!");
		return nullptr;
	}

	// 3. ��ʼ��m_pTRTCore
	return m_pTRTCore->init(params, nErrnoFlag);
}


// \! �쳣���
// \@param ctx:ִ��������
// \@param vInCCoreImages:����ͼƬ�б�CCoreImage
// \@param vOutCCoreImages:���ͼƬ���飬CCoreImage
// \@param threshold:��ֵ
// \@param maxValue:���ֵ����һ��ʱʹ��
// \@param minValue:��Сֵ����һ��ʱʹ��
// \@param pixel_value:��ֵ��ͼ���ֵ����һ��ʱʹ��
returnResult_S CTrtAPI::anomaly(cv::Mat&InferImg, std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCoreImages, std::vector<CCoreImage*>& vOutCoreImages, const PD_S32 threshold)
{
	LOG_F(INFO, "anomaly start ......");
	cv::Mat M, M1, T, F;
	// 1.���ָ���Ƿ�Ϊ��
	if (ctx == nullptr || m_pTRTCore == nullptr) {
		LOG_F(INFO, "ָ��Ϊ��");
		return returnResult_S(M, M1, T, F, PD_UNKNOW_ERROR);
	}
		// 3.��CoreImageת��Opencv
		std::vector<cv::Mat> input_images, output_images;
		for (PD_S32 i = 0; i < vInCoreImages.size(); i++) {
			cv::Mat cv_img = cv::Mat(
				vInCoreImages[i]->height_,
				vInCoreImages[i]->width_,
				CV_8UC(vInCoreImages[i]->channal_),
				vInCoreImages[i]->imagedata_,
				vInCoreImages[i]->imagestep_).clone();
			/*
			("sad",cv_img);
			cv::waitKey(0);*/
			input_images.push_back(cv_img);
		}
		returnResult_S ImgResults;
		// 4.���Ŀ�����
		ImgResults = m_pTRTCore->anomaly(ctx, input_images, output_images, threshold);

		// 5. opencv -> coreimage
		PD_S32 input_batch, input_channels, input_height, input_width;
		m_pTRTCore->getInputDims(ctx, input_batch, input_channels, input_height, input_width);
		PD_S32 output_batch, output_channel, output_height, output_width;
		m_pTRTCore->getOutputDims(ctx, output_batch, output_channel, output_height, output_width);
		//cv::imshow("Sdd", ImgResults.Img);
		//cv::waitKey(0);
		return ImgResults;
	
}

// \! ��ȡ�Կ�����
// \@param ctx:ִ��������
// \@param number:gpu����
PD_S32 CTrtAPI::getNumberGPU(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& number)
{
	return this->m_pTRTCore->getNumberGPU(ctx, number);
}

// \! ��ȡ����ά��
// \@param ctx:ִ��������
// \@param nBatch:batchsize
// \@param nChannels:channels
// \@param nHeight:height
// \@param nWidth:width
// \@param index:��index�����룬����onnx�ж�����룬��ͨ��index��ָ��
PD_S32 CTrtAPI::getInputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nChannels, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index)
{
	if (ctx == nullptr || m_pTRTCore == nullptr) {
		LOG_F(INFO, "init failed, can't call getDims");
		return PD_UNKNOW_ERROR;
	}
	m_pTRTCore->getInputDims(ctx, nBatch, nChannels, nHeight, nWidth, index);
	return PD_OK;
}

// \! ��ȡ���ά��
// \@param ctx��ִ��������
// \@param nBatch:batchsize
// \@param nHeight:Height
// \@param nWidth:Width
// \@param index:��index�����������onnx�ж���������ͨ��index��ָ��
PD_S32 CTrtAPI::getOutputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index)
{
	if (ctx == nullptr || m_pTRTCore == nullptr) {
		LOG_F(INFO, "init failed, can't call getDims");
		return PD_UNKNOW_ERROR;
	}
	m_pTRTCore->getOutputDims(ctx, nBatch, nHeight, nWidth, index);
	return PD_OK;
}