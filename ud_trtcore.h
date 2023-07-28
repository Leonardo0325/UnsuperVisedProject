#ifndef UD_TRTCORE_H
#define UD_TRTCORE_H
/*****************************************************************************
* @description : TensorRT 核心库
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
// \! CTrtCore接口
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
	typedef struct TRTCore_ctx TRTCore_ctx; // 声明需要用到的一个结构体，包含（params、engine、pool）, 定义在engine.h中
	CTrtCore();
	~CTrtCore();
	// \! 初始化
	// \@param params     初始化参数
	// \@param nErrnoFlag 初始化错误码，详情见params.h
	std::shared_ptr<TRTCore_ctx> init(const PARAMS_S & params, PD_S32& nErrnoFlag);

	// \! 分类
	// \@param ctx:执行上下文
	// \@param vInCoreImages:输入图像列表，Mat格式
	// \@param vvOutClsRes:输出结果，ClassifyResult格式
	PD_S32 classify(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<cv::Mat>& cvImages, std::vector<std::vector<CLASSIFYRESULT>>& outputs);

	// \! 分割
	// \@param ctx: 执行上下文
	// \@param vInCoreImages: 输入图片vector，cvImage
	// \@param vOutCoreImages:输出图片vector，cvImage
	// \@param verbose: 如果为true,return the probability graph, else return the class id image
	PD_S32 segment(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<cv::Mat>& cvImages, std::vector<cv::Mat>& outputs, bool verbose = false);

	// \! 目标检测
	PD_S32 detect(
		std::shared_ptr<TRTCore_ctx> ctx,
		const std::vector<cv::Mat>& cvImages,
		std::vector<std::vector<BBox_S>>& outputs
	);

	// \! 异常检测
	returnResult_S anomaly(
		std::shared_ptr<TRTCore_ctx> ctx,
		const std::vector<cv::Mat>& cvImages,
		std::vector<cv::Mat>& outputs,
		PD_S32 thresh
	);

	// \! 获取显卡数量
	// \@param ctx:执行上下文
	// \@param number:gpu数量
	PD_S32 getNumberGPU(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& number);

	// \! 获取输入维度
	// \@param ctx:执行上下文
	// \@param nBatch:batchsize
	// \@param nChannels:channels
	// \@param nHeight:height
	// \@param nWidth:width
	// \@param index:第index个输入，加入onnx有多个输入，则通过index来指定
	PD_S32 getInputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nChannels, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index = 0);

	// \! 获取输出维度
	// \@param ctx：执行上下文
	// \@param nBatch:batchsize
	// \@param nHeight:Height
	// \@param nWidth:Width
	// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
	PD_S32 getOutputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nChannel, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index = 0);

	// \! 获取输出维度
	// \@param ctx：执行上下文
	// \@param nBatch:batchsize
	// \@param nNumClass:NumClass 类别数，针对分类
	// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
	PD_S32 getOutputDims2(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nNumClass, PD_S32 index = 0);

	// \! 获取输入名称
	// \@param ctx：执行上下文
	// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
	std::string getInputNames(std::shared_ptr<TRTCore_ctx> ctx, PD_S32 index = 0);

	// \! 获取输出名称
	// \@param ctx：执行上下文
	// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
	std::string getOutputNames(std::shared_ptr<TRTCore_ctx> ctx, PD_S32 index = 0);
//public:
//	void tcStartListrning(); //监听日志的函数
//private:
//	//static UNIT _cdecl tcLogThreadProc(LPVOID lparam);//日志线程函数
//	HWND m_hWnd;//MFC界面类的句柄
//	HANDLE m_hLogThread; // 日线程的句柄
//	bool m_bStopListening; // 停止监听志
private:
	CUnSuperviseDeployDlg* m_pcUd;
	static UNIT _cdecl tcLogThreadProc(LPVOID lparam);
	HWND m_hWnd; // MFC界面类的句柄
	HANDLE m_hLogThread; // 日线程的句柄
	bool m_bStopListening; // 停止监听志
};
#endif 
