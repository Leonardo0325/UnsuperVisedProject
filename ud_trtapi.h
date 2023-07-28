/***************************************************************************************
* @description : CTrtCore ���API����CTrtCore�Ͻ�һ����װ��ȥ����opencv������������Core
****************************************************************************************/
#ifndef UD_TRTAPI_H
#define UD_TRTAPI_H

#if defined (TYPE_CTrtAPI_API_EXPORTS)
#define TYPE_CTrtAPI_API __declspec(dllexport)
#else
#define TYPE_CTrtAPI_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>

#include "ud_basetype.h"
#include "ud_loguru.hpp" // https://github.com/emilk/loguru
#include "ud_trtcore.h"

// \! ͼ��Ļ�������
//����ӿ�
class CCoreImage
{
public:
	PD_VOID SetValue(PD_S32 channal, PD_S32 width, PD_S32 height, PD_S32 step, unsigned char* data)
	{
		channal_ = channal;
		width_ = width;
		height_ = height;
		imagestep_ = step;
		imagedata_ = data;
	};

public:
	//ͼ��ͨ��
	PD_S32 channal_;
	//��	
	PD_S32 width_;
	//��
	PD_S32 height_;
	//ÿ���ֽ���
	PD_S32 imagestep_;
	//ͼ������
	unsigned char* imagedata_;
};

// \! API�ӿ�
class CTrtCore;  // ������ӿ�
class TYPE_CTrtAPI_API CTrtAPI { // Ӧ�ò�ӿ�
public:
	typedef struct TRTCore_ctx TRTCore_ctx;   //������Ҫ�õ���һ���ṹ�壬������params��engine��pool��
	CTrtAPI();	// ���캯��
	~CTrtAPI();	// ��������

	// \! ��ʼ��
	// \@param:params     ��ʼ������
	// \@param:nErrnoFlag ��ʼ�������룬�����params.h
	std::shared_ptr<TRTCore_ctx> init(const PARAMS_S& params, PD_S32& nErrnoFlag);

	// \! ����
	// \@param ctx:ִ��������
	// \@param vInCCoreImages:����ͼ���б�CCoreImage��ʽ
	// \@param vvOutClsRes:��������ClassifyResult��ʽ
	PD_S32 classify(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCCoreImages, std::vector<std::vector<CLASSIFYRESULT>>& vvOutClsRes);

	// \! �ָ�
	// \@param ctx: ִ��������
	// \@param vInCCoreImages: ����ͼƬvector��CCoreImage
	// \@param vOutCCoreImages:���ͼƬvector��CCoreImage
	// \@param verbose: ���Ϊtrue,return the probability graph, else return the class id image
	PD_S32 segment(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCCoreImages, std::vector<CCoreImage*>& vOutCCoreImages, bool verbose = false);

	// \! Ŀ����
	// \@param ctx:ִ��������
	// \@param vInCCoreImages:����ͼƬ���飬CCoreImage
	// \@param vvOutBBoxs:���������飬BBox
	PD_S32 detect(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCCoreImages, std::vector<std::vector<BBox_S>>& vvOutBBoxs);

	// \! �쳣���
	// \@param ctx:ִ��������
	// \@param vInCCoreImages:����ͼƬ�б�CCoreImage
	// \@param vOutCCoreImages:���ͼƬ���飬CCoreImage
	// \@param threshold:��ֵ
	// \@param maxValue:���ֵ����һ��ʱʹ��
	// \@param minValue:��Сֵ����һ��ʱʹ��
	// \@param pixel_value:��ֵ��ͼ���ֵ����һ��ʱʹ��
	returnResult_S anomaly(cv::Mat&InferImg, std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCoreImages, std::vector<CCoreImage*>& vOutCoreImages, const PD_S32 threshold);

	// \! ��ȡ�Կ�����
	// \@param ctx:ִ��������
	// \@param number:gpu����
	PD_S32 getNumberGPU(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& number);

	// \! ��ȡ����ά��
	// \@param ctx:ִ��������
	// \@param nBatch:batchsize
	// \@param nChannels:channels
	// \@param nHeight:height
	// \@param nWidth:width
	// \@param index:��index�����룬����onnx�ж�����룬��ͨ��index��ָ��
	PD_S32 getInputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nChannels, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index = 0);

	// \! ��ȡ���ά��
	// \@param ctx��ִ��������
	// \@param nBatch:batchsize
	// \@param nHeight:Height
	// \@param nWidth:Width
	// \@param index:��index�����������onnx�ж���������ͨ��index��ָ��
	PD_S32 getOutputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index = 0);

private:
	CTrtCore* m_pTRTCore; // Ϊ�˷��������������������������ӿڣ�������ӿڡ�Ӧ�ò�ӿڡ�Ӧ�ò�ӿ����ڿ�����֮�Ϸ�װ�Ķ���ӿڡ�
};
#endif