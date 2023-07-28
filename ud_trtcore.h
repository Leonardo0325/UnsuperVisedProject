#ifndef UD_TRTCORE_H
#define UD_TRTCORE_H
/*****************************************************************************
* @description : TensorRT ���Ŀ�
*****************************************************************************/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "ud_loguru.hpp" // https://github.com/emilk/loguru
#include "ud_basetype.h"
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <fstream>
//#include "UnSuperviseDeployDlg.h"
// \! CTrtCore�ӿ�
class CUnSuperviseDeployDlg;
struct returnResult_S
{
	cv::Mat Img;
	cv::Mat Img1;
	cv::Mat ThreshImg;
	cv::Mat FinalImg;
	int nLY;
	returnResult_S() = default;
	returnResult_S(cv::Mat Img_, cv::Mat Img1_, cv::Mat ThreshImg_, cv::Mat FinalImg_, int nLY_) :Img(Img_), Img1(Img1_), ThreshImg(ThreshImg_), FinalImg(FinalImg_), nLY(nLY_) {};
};
class CTrtCore {
	//friend class CUnSuperviseDeployDlg;
public:
	typedef struct TRTCore_ctx TRTCore_ctx; // ������Ҫ�õ���һ���ṹ�壬������params��engine��pool��, ������engine.h��
	CTrtCore();
	~CTrtCore();
	// \! ��ʼ��
	// \@param params     ��ʼ������
	// \@param nErrnoFlag ��ʼ�������룬�����params.h
	std::shared_ptr<TRTCore_ctx> init(const PARAMS_S & params, PD_S32& nErrnoFlag);

	// \! ����
	// \@param ctx:ִ��������
	// \@param vInCoreImages:����ͼ���б�Mat��ʽ
	// \@param vvOutClsRes:��������ClassifyResult��ʽ
	PD_S32 classify(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<cv::Mat>& cvImages, std::vector<std::vector<CLASSIFYRESULT>>& outputs);

	// \! �ָ�
	// \@param ctx: ִ��������
	// \@param vInCoreImages: ����ͼƬvector��cvImage
	// \@param vOutCoreImages:���ͼƬvector��cvImage
	// \@param verbose: ���Ϊtrue,return the probability graph, else return the class id image
	PD_S32 segment(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<cv::Mat>& cvImages, std::vector<cv::Mat>& outputs, bool verbose = false);

	// \! Ŀ����
	PD_S32 detect(
		std::shared_ptr<TRTCore_ctx> ctx,
		const std::vector<cv::Mat>& cvImages,
		std::vector<std::vector<BBox_S>>& outputs
	);

	// \! �쳣���
	returnResult_S anomaly(
		std::shared_ptr<TRTCore_ctx> ctx,
		const std::vector<cv::Mat>& cvImages,
		std::vector<cv::Mat>& outputs,
		PD_S32 thresh
	);

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
	PD_S32 getOutputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nChannel, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index = 0);

	// \! ��ȡ���ά��
	// \@param ctx��ִ��������
	// \@param nBatch:batchsize
	// \@param nNumClass:NumClass ���������Է���
	// \@param index:��index�����������onnx�ж���������ͨ��index��ָ��
	PD_S32 getOutputDims2(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nNumClass, PD_S32 index = 0);

	// \! ��ȡ��������
	// \@param ctx��ִ��������
	// \@param index:��index�����������onnx�ж���������ͨ��index��ָ��
	std::string getInputNames(std::shared_ptr<TRTCore_ctx> ctx, PD_S32 index = 0);

	// \! ��ȡ�������
	// \@param ctx��ִ��������
	// \@param index:��index�����������onnx�ж���������ͨ��index��ָ��
	std::string getOutputNames(std::shared_ptr<TRTCore_ctx> ctx, PD_S32 index = 0);
//public:
//	void tcStartListrning(); //������־�ĺ���
//private:
//	//static UNIT _cdecl tcLogThreadProc(LPVOID lparam);//��־�̺߳���
//	HWND m_hWnd;//MFC������ľ��
//	HANDLE m_hLogThread; // ���̵߳ľ��
//	bool m_bStopListening; // ֹͣ����־
private:
	CUnSuperviseDeployDlg* m_pcUd;
	static UNIT _cdecl tcLogThreadProc(LPVOID lparam);
	HWND m_hWnd; // MFC������ľ��
	HANDLE m_hLogThread; // ���̵߳ľ��
	bool m_bStopListening; // ֹͣ����־
};
#endif 
