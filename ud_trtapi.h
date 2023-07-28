/***************************************************************************************
* @description : CTrtCore 库的API，在CTrtCore上进一步分装，去除对opencv的依赖，调用Core
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

// \! 图像的基础定义
//定义接口
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
	//图像通道
	PD_S32 channal_;
	//宽	
	PD_S32 width_;
	//高
	PD_S32 height_;
	//每行字节数
	PD_S32 imagestep_;
	//图像数据
	unsigned char* imagedata_;
};

// \! API接口
class CTrtCore;  // 开发层接口
class TYPE_CTrtAPI_API CTrtAPI { // 应用层接口
public:
	typedef struct TRTCore_ctx TRTCore_ctx;   //声明需要用到的一个结构体，包含（params、engine、pool）
	CTrtAPI();	// 构造函数
	~CTrtAPI();	// 析构函数

	// \! 初始化
	// \@param:params     初始化参数
	// \@param:nErrnoFlag 初始化错误码，详情见params.h
	std::shared_ptr<TRTCore_ctx> init(const PARAMS_S& params, PD_S32& nErrnoFlag);

	// \! 分类
	// \@param ctx:执行上下文
	// \@param vInCCoreImages:输入图像列表，CCoreImage格式
	// \@param vvOutClsRes:输出结果，ClassifyResult格式
	PD_S32 classify(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCCoreImages, std::vector<std::vector<CLASSIFYRESULT>>& vvOutClsRes);

	// \! 分割
	// \@param ctx: 执行上下文
	// \@param vInCCoreImages: 输入图片vector，CCoreImage
	// \@param vOutCCoreImages:输出图片vector，CCoreImage
	// \@param verbose: 如果为true,return the probability graph, else return the class id image
	PD_S32 segment(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCCoreImages, std::vector<CCoreImage*>& vOutCCoreImages, bool verbose = false);

	// \! 目标检测
	// \@param ctx:执行上下文
	// \@param vInCCoreImages:输入图片数组，CCoreImage
	// \@param vvOutBBoxs:输出结果数组，BBox
	PD_S32 detect(std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCCoreImages, std::vector<std::vector<BBox_S>>& vvOutBBoxs);

	// \! 异常检测
	// \@param ctx:执行上下文
	// \@param vInCCoreImages:输入图片列表，CCoreImage
	// \@param vOutCCoreImages:输出图片数组，CCoreImage
	// \@param threshold:阈值
	// \@param maxValue:最大值，归一化时使用
	// \@param minValue:最小值，归一化时使用
	// \@param pixel_value:二值化图像的值，归一化时使用
	returnResult_S anomaly(cv::Mat&InferImg, std::shared_ptr<TRTCore_ctx> ctx, const std::vector<CCoreImage*>& vInCoreImages, std::vector<CCoreImage*>& vOutCoreImages, const PD_S32 threshold);

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
	PD_S32 getOutputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index = 0);

private:
	CTrtCore* m_pTRTCore; // 为了方便其他软件开发，定义了两层接口：开发层接口、应用层接口。应用层接口是在开发层之上封装的对外接口。
};
#endif