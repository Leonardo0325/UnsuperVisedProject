#include "pch.h"
#include "ud_preprocess.h"
#include<time.h>
#include<Windows.h>
#include "stdio.h"
#pragma comment(lib, "winmm.lib")
void disp_continue_message(HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);

// Procedures 
// Chapter: Graphics / Text
// Short Description: This procedure displays 'Click 'Run' to continue' in the lower right corner of the screen. 
void disp_continue_message(HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box)
{

	// Local iconic variables

	// Local control variables
	
	if (0 != (hv_Color == HTuple("")))
	{
		//disp_text does not accept an empty string for Color.
		hv_Color = HTuple();
	}
	//
	//Display the message.
	//hv_ContinueMessage = "Press Run (F5) to continue";
	//DispText(hv_WindowHandle, hv_ContinueMessage, "window", "bottom", "right", hv_Color,
	//	hv_GenParamName, hv_GenParamValue);
	return;
}

//#ifndef NO_EXPORT_MAIN
// Main procedure 
void CPreProcess::preAction(CString ModelPth,CString SavePth)
{
	std::string sSavePth((LPCTSTR)SavePth);
	std::string sModelPth((LPCTSTR)ModelPth);
	// 使用std::string构造HTuple
	HTuple hSavePth(sSavePth.c_str());
	HTuple hModelPth(sModelPth.c_str());

	// Local iconic variables
	HObject  ho_Image, ho_GrayImage, ho_ImageGauss;
	HObject  ho_Regions, ho_Rectangle, ho_RegionUnion, ho_ImageReduced;
	HObject  ho_ModelContours, ho_ContoursAffinTrans, ho_Image1;
	HObject  ho_SearchImage, ho_ModelTrans, ho_Region, ho_RegionUnion1;
	HObject  ho_Rectangle1, ho_LEDImage, ho_ImagePart;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_WindowHandle;
	HTuple  hv_num_cir, hv_i, hv_Row1, hv_Column1, hv_Row2;
	HTuple  hv_Column2, hv_Area, hv_regiRow1, hv_regiColumn1;
	HTuple  hv_ModelID, hv_HomMat2D, hv_Row3, hv_Column3, hv_Angle;
	HTuple  hv_Scale, hv_Score, hv_I, hv_HomMat2DIdentity, hv_HomMat2DTranslate;
	HTuple  hv_HomMat2DRotate, hv_HomMat2DScale, hv_Row, hv_Column;
	HTuple  hv_Phi, hv_Length1, hv_Length2;
	{
		HTuple end_val8 = hv_num_cir;
		HTuple step_val8 = 1;
		for (hv_i = 1; hv_i.Continue(end_val8, step_val8); hv_i += step_val8)
		{
			DrawRectangle1(hv_WindowHandle, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
			/*hv_Row1 = 1921.2;
			hv_Column1 = 3821.68;
			hv_Row2 = 2283.64;
			hv_Column2 = 4186.66;*/
			GenRectangle1(&ho_Rectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
			ConcatObj(ho_Regions, ho_Rectangle, &ho_Regions);
		}
	}
	Union1(ho_Regions, &ho_RegionUnion);
	ReduceDomain(ho_ImageGauss, ho_RegionUnion, &ho_ImageReduced);

	AreaCenter(ho_RegionUnion, &hv_Area, &hv_regiRow1, &hv_regiColumn1);

	//【1】创建模板模型
	//如果Metric= 'use_polarity'，图片和模板必须要有相同的对比度。例如，如果模型是一个暗的目标在一个亮的背景上，那么仅仅那些比背景暗的目标可以被找到;
	//如果Metric= 'ignore_global_polarity',图片和模板必须要有相同或相反的对比度
	CreateScaledShapeModel(ho_ImageReduced, 5, HTuple(-10).TupleRad(), HTuple(10).TupleRad(),
		"auto", 0.7, 1.2, "auto", "auto", "ignore_global_polarity", "auto", "auto",
		&hv_ModelID);
	//【2】生成模板轮廓
	GetShapeModelContours(&ho_ModelContours, hv_ModelID, 1);

	//看看生成的轮廓
	if (HDevWindowStack::IsOpen())
		SetColor(HDevWindowStack::GetActive(), "red");
	if (HDevWindowStack::IsOpen())
		SetLineWidth(HDevWindowStack::GetActive(), 2);
	VectorAngleToRigid(0, 0, 0, hv_regiRow1, hv_regiColumn1, 0, &hv_HomMat2D);
	AffineTransContourXld(ho_ModelContours, &ho_ContoursAffinTrans, hv_HomMat2D);
	disp_continue_message(hv_WindowHandle, "black", "true");
	//stop ()
	//time_t start, end;
	//DWORD t1, t2;
	//time(&start);

	ReadImage(&ho_Image1, hModelPth);
	Rgb1ToGray(ho_Image1, &ho_GrayImage);
	GaussImage(ho_GrayImage, &ho_SearchImage, 5);
	if (HDevWindowStack::IsOpen())
		DispObj(ho_SearchImage, HDevWindowStack::GetActive());
	//time(&start);
	double start = GetTickCount();
	//【3】寻找模板，第七个参数为设置匹配的最小分数，第八个参数为设置匹配的个数，0表示有多少个识别多少个
	FindScaledShapeModel(ho_SearchImage, hv_ModelID, HTuple(-10).TupleRad(), HTuple(10).TupleRad(),
		0.5, 1.2, 0.6, 0, 0.5, "least_squares", 5, 0.8, &hv_Row3, &hv_Column3, &hv_Angle,
		&hv_Scale, &hv_Score);

	{
		HTuple end_val46 = (hv_Score.TupleLength()) - 1;
		HTuple step_val46 = 1;
		for (hv_I = 0; hv_I.Continue(end_val46, step_val46); hv_I += step_val46)
		{
			//生成一个单位矩阵
			HomMat2dIdentity(&hv_HomMat2DIdentity);
			//平移
			HomMat2dTranslate(hv_HomMat2DIdentity, HTuple(hv_Row3[hv_I]), HTuple(hv_Column3[hv_I]),
				&hv_HomMat2DTranslate);
			//旋转
			HomMat2dRotate(hv_HomMat2DTranslate, HTuple(hv_Angle[hv_I]), HTuple(hv_Row3[hv_I]),
				HTuple(hv_Column3[hv_I]), &hv_HomMat2DRotate);
			//放缩，得到最终的仿射变换矩阵
			HomMat2dScale(hv_HomMat2DRotate, HTuple(hv_Scale[hv_I]), HTuple(hv_Scale[hv_I]),
				HTuple(hv_Row3[hv_I]), HTuple(hv_Column3[hv_I]), &hv_HomMat2DScale);
			//仿射变换
			AffineTransContourXld(ho_ModelContours, &ho_ModelTrans, hv_HomMat2DScale);
			if (HDevWindowStack::IsOpen())
				DispObj(ho_ModelTrans, HDevWindowStack::GetActive());
			GenRegionContourXld(ho_ModelTrans, &ho_Region, "filled");
			Union1(ho_Region, &ho_RegionUnion1);
			SmallestRectangle2(ho_RegionUnion1, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1,
				&hv_Length2);
			GenRectangle2(&ho_Rectangle1, hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2);
			ReduceDomain(ho_Image1, ho_Rectangle1, &ho_LEDImage);
			CropDomainRel(ho_LEDImage, &ho_ImagePart, -5, -5, -5, -5);
			WriteImage(ho_ImagePart, "jpg", 0, (hSavePth+"/") + "LED_" + (hv_I.TupleString(".")));
		}
	}
	double  end = GetTickCount();
	double cost = end-start;
	printf("%f/n", cost);
}

//
//#ifndef NO_EXPORT_APP_MAIN
//
//#ifdef __APPLE__
//// On OS X systems, we must have a CFRunLoop running on the main thread in
//// order for the HALCON graphics operators to work correctly, and run the
//// action function in a separate thread. A CFRunLoopTimer is used to make sure
//// the action function is not called before the CFRunLoop is running.
//// Note that starting with macOS 10.12, the run loop may be stopped when a
//// window is closed, so we need to put the call to CFRunLoopRun() into a loop
//// of its own.
//HTuple      gStartMutex;
//H_pthread_t gActionThread;
//HBOOL       gTerminate = FALSE;
//
//static void timer_callback(CFRunLoopTimerRef timer, void *info)
//{
//	UnlockMutex(gStartMutex);
//}
//
//static Herror apple_action(void **parameters)
//{
//	// Wait until the timer has fired to start processing.
//	LockMutex(gStartMutex);
//	UnlockMutex(gStartMutex);
//
//	try
//	{
//		action();
//	}
//	catch (HException &exception)
//	{
//		fprintf(stderr, "  Error #%u in %s: %s\n", exception.ErrorCode(),
//			(const char *)exception.ProcName(),
//			(const char *)exception.ErrorMessage());
//	}
//
//	// Tell the main thread to terminate itself.
//	LockMutex(gStartMutex);
//	gTerminate = TRUE;
//	UnlockMutex(gStartMutex);
//	CFRunLoopStop(CFRunLoopGetMain());
//	return H_MSG_OK;
//}
//
//static int apple_main(int argc, char *argv[])
//{
//	Herror                error;
//	CFRunLoopTimerRef     Timer;
//	CFRunLoopTimerContext TimerContext = { 0, 0, 0, 0, 0 };
//
//	CreateMutex("type", "sleep", &gStartMutex);
//	LockMutex(gStartMutex);
//
//	error = HpThreadHandleAlloc(&gActionThread);
//	if (H_MSG_OK != error)
//	{
//		fprintf(stderr, "HpThreadHandleAlloc failed: %d\n", error);
//		exit(1);
//	}
//
//	error = HpThreadCreate(gActionThread, 0, apple_action);
//	if (H_MSG_OK != error)
//	{
//		fprintf(stderr, "HpThreadCreate failed: %d\n", error);
//		exit(1);
//	}
//
//	Timer = CFRunLoopTimerCreate(kCFAllocatorDefault,
//		CFAbsoluteTimeGetCurrent(), 0, 0, 0,
//		timer_callback, &TimerContext);
//	if (!Timer)
//	{
//		fprintf(stderr, "CFRunLoopTimerCreate failed\n");
//		exit(1);
//	}
//	CFRunLoopAddTimer(CFRunLoopGetCurrent(), Timer, kCFRunLoopCommonModes);
//
//	for (;;)
//	{
//		HBOOL terminate;
//
//		CFRunLoopRun();
//
//		LockMutex(gStartMutex);
//		terminate = gTerminate;
//		UnlockMutex(gStartMutex);
//
//		if (terminate)
//			break;
//	}
//
//	CFRunLoopRemoveTimer(CFRunLoopGetCurrent(), Timer, kCFRunLoopCommonModes);
//	CFRelease(Timer);
//
//	error = HpThreadHandleFree(gActionThread);
//	if (H_MSG_OK != error)
//	{
//		fprintf(stderr, "HpThreadHandleFree failed: %d\n", error);
//		exit(1);
//	}
//
//	ClearMutex(gStartMutex);
//	return 0;
//}
//#endif

//int main(int argc, char *argv[])
//{
//	int ret = 0;
//
//	try
//	{
//#if defined(_WIN32)
//		SetSystem("use_window_thread", "true");
//#elif defined(__linux__) && !defined(__arm__)
//		XInitThreads();
//#endif
//
//		// Default settings used in HDevelop (can be omitted) 
//		SetSystem("width", 512);
//		SetSystem("height", 512);
//
////#ifndef __APPLE__
////		CPreProcess::preAction(HTuple ModelPth, CString SavePth)
////#else
////		ret = apple_main(argc, argv);
////#endif
////	}
//	catch (HException &exception)
//	{
//		fprintf(stderr, "  Error #%u in %s: %s\n", exception.ErrorCode(),
//			(const char *)exception.ProcName(),
//			(const char *)exception.ErrorMessage());
//		ret = 1;
//	}
//	return ret;
//}
//
//#endif
//
//
//#endif
//
//
//
