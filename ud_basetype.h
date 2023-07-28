#ifndef UD_BASETYPE_H
#define UD_BASETYPE_H
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>
#include <numeric>
#include <iostream>




typedef unsigned char PD_U8;
typedef unsigned char PD_UCHAR;
typedef unsigned short PD_U16;
typedef unsigned int PD_U32;

#ifndef  _M_IX86
typedef unsigned long long PD_U64;
typedef long long PD_S64;
#else
typedef __int64 PD_U64;
typedef __int64 PD_S64;
#endif // ! _M_IX86
typedef char PD_CHAR;
typedef char* PD_PCHAR;
typedef float PD_FLOAT;
typedef double PD_DOUBLE;
typedef int PD_S32;
typedef short PD_S16;
typedef void PD_VOID;
typedef unsigned long PD_SIZE_T;
typedef unsigned long PD_LENGTH_T;
typedef void* PD_MODULE_HANDLE;
typedef enum {
	PD_FALSE = 0,
	PD_TRUE = 1,
} PD_BOOL;
#ifndef NULL
#define NULL 0L
#endif
#define PD_NULL 0L
#define PD_NULL_PTR 0L
#define PD_SUCCESS 0
#define PD_FAILURE (-1)

/** PD_EXPORTS */
#if defined(PD_API_EXPORTS)
#define PD_EXPORTS __declspec(dllexport)
#elif defined(PD_API_IMPORTS)
#define PD_EXPORTS __declspec(dllimport)
#else
#define PD_EXPORTS extern
#endif


#define PD_ASSERT(x) ( assert(x) )
#define RESTRICT

/** SA_INLINE */
#ifndef PD_INLINE
#if defined __cplusplus
#define PD_INLINE inline
#elif (defined WIN32 || defined WIN64) && !defined __GNUC__
#define PD_INLINE __inline
#else
#define PD_INLINE static
#endif
#endif
typedef unsigned int UNIT;
/** 定义网络类型 */
typedef enum pdNETWORK_E
{
	PD_NETWORK_CLS = 0, /**< 分类网络 */
	PD_NETWORK_SEG = 1, /**< 分割网络 */
	PD_NETWORK_DET = 2, /**< 检测网络 */
	PD_NETWORK_SIM = 3, /**< 双流相似度网络 */
	PD_NETWORK_ANOMALY = 4, /** 异常检测网络 */
} NETWORK_E;
/** 定义错误类型 */
typedef enum pdERRORCODE_E
{
	PD_OK = 0,               // 函数执行成功
	PD_WRONG_CALL = 1,       // 错误的调用，比如说分割模型调用了分类的接口
	PD_WRONG_FILE = 2,       // 文件找不到或者损坏
	PD_WRONG_IMG = 3,        // 输入图像格式错误或者不存在
	PD_WRONG_CUDA = 4,       // cuda指定错误或者显卡不存在
	PD_WRONG_TNAME = 5,      // caffe参数中inputTensorNames或者outputTensorNames设置错误
	PD_WRONG_PLAT = 6,       // 平台类型和模型文件不匹配
	PD_WRONG_BATCH = 7,      // onnx的batch size小于实际使用的
	PD_WRONG_GPUMEM = 8,       // 显存不足，无法申请显卡内存
	PD_UNKNOW_ERROR = 99,    // 未知错误，请联系开发者解决
} ERRORCODE_E;
/** 定义初始化参数 */
typedef struct pdPARAMS_S
{
	NETWORK_E netType = PD_NETWORK_CLS;
	std::string onnxFilePath;	                    // onnx文件路径
	std::string engineFilePath;	                    // 引擎文件保存位置，不存在则自动生成，存在则自动加载
	std::string log_path = "./";					// log日志保存路径，设置为空则不保存日志文件
	PD_BOOL fp16{ PD_FALSE };	                            // 是否使用半精度 使用半精度生成引擎文件比较慢，使用的时候快
	std::vector<PD_FLOAT> stdValue{ 1.f, 1.f, 1.f };	// 归一化时用到的方差。superAI训练的模型不需要设置stdValue和meanValue，使用此默认值即可
	std::vector<PD_FLOAT> meanValue{ 0.f, 0.0, 0.0 };	// 归一化时用到的均值。其他训练方式可能会需要此参数。
	PD_S32 gpuId{ 0 };		                            // GPU ID，0表示第一块显卡，1表示第2块显卡....以此类推。
	PD_S32 maxThread = 8;				                // mContexts的数量，可以理解为一个模型支持同时启动的最多画面数。超过16的话请设置更大的数，否则不需要更改
} PARAMS_S;

/** 分类返回结果 */
typedef std::pair<PD_S32, PD_FLOAT> CLASSIFYRESULT;

/** 检测返回结果 */
typedef struct pdBBox_S
{
	PD_FLOAT x1, y1, x2, y2;     // 左上角、右下角坐标值
	PD_FLOAT det_confidence;     // 检测框中包含目标的置信度
	PD_FLOAT class_confidence;   // 检测框目标类别为第class_id类的置信度
	PD_U32 class_id;    // 类别
}BBox_S;

#endif // ! PD_TYPE_H



