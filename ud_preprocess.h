#ifndef UD_PREPROCESS
#define UD_PREPROCESS


#pragma once
#include <HalconCPP.h>

using namespace HalconCpp;

class CPreProcess
{

public:
	void preAction(CString ModelPth, CString SavePth);
};

#endif // !UD_PREPROCESS