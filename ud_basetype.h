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
/** ������������ */
typedef enum pdNETWORK_E
{
	PD_NETWORK_CLS = 0, /**< �������� */
	PD_NETWORK_SEG = 1, /**< �ָ����� */
	PD_NETWORK_DET = 2, /**< ������� */
	PD_NETWORK_SIM = 3, /**< ˫�����ƶ����� */
	PD_NETWORK_ANOMALY = 4, /** �쳣������� */
} NETWORK_E;
/** ����������� */
typedef enum pdERRORCODE_E
{
	PD_OK = 0,               // ����ִ�гɹ�
	PD_WRONG_CALL = 1,       // ����ĵ��ã�����˵�ָ�ģ�͵����˷���Ľӿ�
	PD_WRONG_FILE = 2,       // �ļ��Ҳ���������
	PD_WRONG_IMG = 3,        // ����ͼ���ʽ������߲�����
	PD_WRONG_CUDA = 4,       // cudaָ����������Կ�������
	PD_WRONG_TNAME = 5,      // caffe������inputTensorNames����outputTensorNames���ô���
	PD_WRONG_PLAT = 6,       // ƽ̨���ͺ�ģ���ļ���ƥ��
	PD_WRONG_BATCH = 7,      // onnx��batch sizeС��ʵ��ʹ�õ�
	PD_WRONG_GPUMEM = 8,       // �Դ治�㣬�޷������Կ��ڴ�
	PD_UNKNOW_ERROR = 99,    // δ֪��������ϵ�����߽��
} ERRORCODE_E;
/** �����ʼ������ */
typedef struct pdPARAMS_S
{
	NETWORK_E netType = PD_NETWORK_CLS;
	std::string onnxFilePath;	                    // onnx�ļ�·��
	std::string engineFilePath;	                    // �����ļ�����λ�ã����������Զ����ɣ��������Զ�����
	std::string log_path = "./";					// log��־����·��������Ϊ���򲻱�����־�ļ�
	PD_BOOL fp16{ PD_FALSE };	                            // �Ƿ�ʹ�ð뾫�� ʹ�ð뾫�����������ļ��Ƚ�����ʹ�õ�ʱ���
	std::vector<PD_FLOAT> stdValue{ 1.f, 1.f, 1.f };	// ��һ��ʱ�õ��ķ��superAIѵ����ģ�Ͳ���Ҫ����stdValue��meanValue��ʹ�ô�Ĭ��ֵ����
	std::vector<PD_FLOAT> meanValue{ 0.f, 0.0, 0.0 };	// ��һ��ʱ�õ��ľ�ֵ������ѵ����ʽ���ܻ���Ҫ�˲�����
	PD_S32 gpuId{ 0 };		                            // GPU ID��0��ʾ��һ���Կ���1��ʾ��2���Կ�....�Դ����ơ�
	PD_S32 maxThread = 8;				                // mContexts���������������Ϊһ��ģ��֧��ͬʱ��������໭����������16�Ļ������ø��������������Ҫ����
} PARAMS_S;

/** ���෵�ؽ�� */
typedef std::pair<PD_S32, PD_FLOAT> CLASSIFYRESULT;

/** ��ⷵ�ؽ�� */
typedef struct pdBBox_S
{
	PD_FLOAT x1, y1, x2, y2;     // ���Ͻǡ����½�����ֵ
	PD_FLOAT det_confidence;     // �����а���Ŀ������Ŷ�
	PD_FLOAT class_confidence;   // ����Ŀ�����Ϊ��class_id������Ŷ�
	PD_U32 class_id;    // ���
}BBox_S;

#endif // ! PD_TYPE_H



