#ifndef UD_TRTDEPLOY_H
#define UD_TRTDEPLOY_H


/*****************************************************************************
* @description : TensorRT 调用API
*****************************************************************************/
#pragma once
#include <afxext.h> 
#include<afxwin.h>
#include<string>
#include <opencv2/opencv.hpp> // opencv include
#include <iostream> // system include

#include<math.h>
#include<cstring>
#include "ud_trtapi.h"
#include "ud_basetype.h"
using namespace std;

class TimeTick
{
public:
	TimeTick(PD_VOID)
	{
		QueryPerformanceFrequency(&mFrequency);
	};
private:
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mEndTime;
	LARGE_INTEGER mCurrentTime;
	LARGE_INTEGER mFrequency;  // CPU频率 计时的精度跟频率有关，我电脑频率是10e8，计时精度为10纳秒级别
public:
	PD_DOUBLE mInterval;  // 间隔
public:
	PD_VOID start()
	{
		QueryPerformanceCounter(&mStartTime);
	};
	PD_VOID end()
	{
		QueryPerformanceCounter(&mEndTime);

		mInterval = ((PD_DOUBLE)mEndTime.QuadPart - (PD_DOUBLE)mStartTime.QuadPart) / (PD_DOUBLE)mFrequency.QuadPart;  //秒，10e-8级别

	};
	PD_DOUBLE tick()
	{
		QueryPerformanceCounter(&mCurrentTime);
		return (PD_DOUBLE)mCurrentTime.QuadPart / (PD_DOUBLE)mFrequency.QuadPart;
	};
};

class CTrtDeploy
{
public:
	CTrtDeploy();
	CTrtDeploy(string m_modelPth);
	~CTrtDeploy();
public:
	PD_VOID Init(string ModelPth);
	returnResult_S run(string G_sImgPth, int nThreshValue, string ModelPth);
	returnResult_S TD_AllDeploy(string G_sImgPth);
	CTrtAPI* m_pTrtAPI;
	string ModelPth;
private:
	PARAMS_S params;
	std::shared_ptr<TRTCore_ctx> ctx;

};

#endif // !UD_TRTDEPLOY_H