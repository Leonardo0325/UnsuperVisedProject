#define TYPE_CTrtAPI_API_EXPORTS
#include <afx.h>
#include <windows.h>

#include "ud_trtapi.h"
#include "ud_trtcore.h"

// \! 构造函数
CTrtAPI::CTrtAPI()
{
	m_pTRTCore = nullptr;
	m_pTRTCore = new CTrtCore();
}

// \! 析构函数
CTrtAPI::~CTrtAPI()
{
	if (m_pTRTCore != nullptr)
	{
		delete m_pTRTCore;
	}
}

// \! 初始化
// \@param:params     初始化参数
// \@param:nErrnoFlag 初始化错误码，详情见params.h
std::shared_ptr<TRTCore_ctx> CTrtAPI::init(const PARAMS_S& params, PD_S32& nErrnoFlag)
{
	// 1. 创建日志
	loguru::add_file(params.log_path.c_str(), loguru::Append, loguru::Verbosity_MAX);

	// 2. 初始化一个CTrtCore实例

	if (m_pTRTCore == nullptr) {
		LOG_F(INFO, "new CTrtCore() Error!!!!");
		return nullptr;
	}

	// 3. 初始化m_pTRTCore
	return m_pTRTCore->init(params, nErrnoFlag);
}


// \! 异常检测
// \@param ctx:执行上下文
// \@param vInCCoreImages:输入图片列表，CCoreImage
// \@param vOutCCoreImages:输出图片数组，CCoreImage
// \@param threshold:阈值
// \@param maxValue:最大值，归一化时使用
// \@param minValue:最小值，归一化时使用
// \@param pixel_value:二值化图像的值，归一化时使用

// \! 获取显卡数量
// \@param ctx:执行上下文
// \@param number:gpu数量
PD_S32 CTrtAPI::getNumberGPU(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& number)
{
	return this->m_pTRTCore->getNumberGPU(ctx, number);
}

// \! 获取输入维度
// \@param ctx:执行上下文
// \@param nBatch:batchsize
// \@param nChannels:channels
// \@param nHeight:height
// \@param nWidth:width
// \@param index:第index个输入，加入onnx有多个输入，则通过index来指定
PD_S32 CTrtAPI::getInputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nChannels, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index)
{
	if (ctx == nullptr || m_pTRTCore == nullptr) {
		LOG_F(INFO, "init failed, can't call getDims");
		return PD_UNKNOW_ERROR;
	}
	m_pTRTCore->getInputDims(ctx, nBatch, nChannels, nHeight, nWidth, index);
	return PD_OK;
}

// \! 获取输出维度
// \@param ctx：执行上下文
// \@param nBatch:batchsize
// \@param nHeight:Height
// \@param nWidth:Width
// \@param index:第index个输出，假如onnx有多个输出，则通过index来指定
PD_S32 CTrtAPI::getOutputDims(std::shared_ptr<TRTCore_ctx> ctx, PD_S32& nBatch, PD_S32& nHeight, PD_S32& nWidth, PD_S32 index)
{
	if (ctx == nullptr || m_pTRTCore == nullptr) {
		LOG_F(INFO, "init failed, can't call getDims");
		return PD_UNKNOW_ERROR;
	}
	m_pTRTCore->getOutputDims(ctx, nBatch, nHeight, nWidth, index);
	return PD_OK;
}
