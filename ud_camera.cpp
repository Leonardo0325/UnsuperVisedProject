// ud_camera.cpp: 实现文件
//

#include "pch.h"
#include "UnSuperviseDeploy.h"
#include "ud_camera.h"
#include "afxdialogex.h"

#define GrabNum 10000
#define COLOUR_CAMERA
// ud_camera 对话框

IMPLEMENT_DYNAMIC(CCamera, CDialogEx)

CCamera::CCamera(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	InitializeCriticalSection(&m_hSaveImageMux);
	
}

CCamera::~CCamera()
{

}
BOOL CCamera::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
	DisplayWindowInitial();
	m_editSaveSingle.SubclassDlgItem(IDC_SINGLESAVE_EDIT, this);
	m_editSaveSingle.SetWindowText(strSaveSinglePth);

	m_editSaveMulti.SubclassDlgItem(IDC_MULSAVE_EDIT, this);
	m_editSaveMulti.SetWindowText(strSaveMultiPth);
	nCameraNum = 0;
	handle = NULL;
	return TRUE;
}
void CCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlDeviceCombo);
	//显示参数信息
	DDX_Text(pDX, IDC_EXPOSURE_EDIT, m_dExposureEdit);
	DDX_Text(pDX, IDC_GAIN_EDIT, m_dGainEdit);
	DDX_Text(pDX, IDC_FRAME_RATE_EDIT, m_dFrameRateEdit);
	//DDX_Text(pDX, IDC_EDIT5, m_mEdit1);
	//DDX_Control(pDX, IDC_EDIT5, m_edtEdit2);
	//DDX_Control(pDX, IDC_LIGHTPICTURE_STATIC, m_psPicture);
}


BEGIN_MESSAGE_MAP(CCamera, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON1, &CCamera::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_OPEN_BUTTON, &CCamera::OnBnClickedOpenButton)
	//ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CCamera::OnBnClickedCloseButton)
	//ON_BN_CLICKED(IDC_BUTTON2, &CCamera::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_STOP_GRABBING_BUTTON, &CCamera::OnBnClickedStopGrabbingButton)
	//ON_BN_CLICKED(IDC_GET_PARAMETER_BUTTON, &CCamera::OnBnClickedGetParameterButton)
	//ON_BN_CLICKED(IDC_SET_PARAMETER_BUTTON, &CCamera::OnBnClickedSetParameterButton)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CCamera::OnBnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_OPEN_MFCBUTTON, &CCamera::OnBnClickedOpenMfcbutton)
	ON_BN_CLICKED(IDC_CLOSE_MFCBUTTON, &CCamera::OnBnClickedCloseMfcbutton)
	ON_BN_CLICKED(IDC_MFCBUTTON5, &CCamera::OnBnClickedMfcbutton5)
	ON_BN_CLICKED(IDC_STOP_GRABBING_MFCBUTTON6, &CCamera::OnBnClickedStopGrabbingMfcbutton6)
	//ON_BN_CLICKED(IDC_SINGLEBUTTON, &CCamera::OnBnClickedSinglebutton)
	ON_BN_CLICKED(IDC_GET_PARAMETER_MFCBUTTON9, &CCamera::OnBnClickedGetParameterMfcbutton9)
	ON_BN_CLICKED(IDC_SET_PARAMETER_MFCBUTTON10, &CCamera::OnBnClickedSetParameterMfcbutton10)
	ON_EN_CHANGE(IDC_SINGLESAVE_EDIT, &CCamera::OnEnChangeSinglesaveEdit)
	ON_EN_CHANGE(IDC_MULSAVE_EDIT, &CCamera::OnEnChangeMulsaveEdit)
	ON_BN_CLICKED(IDC_SINGLEMFCBUTTON7, &CCamera::OnBnClickedSinglemfcbutton7)
	ON_BN_CLICKED(IDC_MULMFCBUTTON8, &CCamera::OnBnClickedMulmfcbutton8)
	ON_BN_CLICKED(IDC_MULMFCBUTTON9, &CCamera::OnBnClickedMulmfcbutton9)
END_MESSAGE_MAP()


// ud_camera 消息处理程序

//显示错误信息
void CCamera::ShowErrorMsg(CString csMessage, int nErrorNum)
{
	CString pstrErrorMsg;
	if (nErrorNum == 0)
	{
		pstrErrorMsg.Format(_T("%s"), csMessage);
	}
	else
	{
		pstrErrorMsg.Format(_T("%s: Error = %x: "), csMessage, nErrorNum);
	}

	switch (nErrorNum)
	{
	case MV_E_HANDLE:           pstrErrorMsg += "Error or invalid handle ";                                         break;
	case MV_E_SUPPORT:          pstrErrorMsg += "Not supported function ";                                          break;
	case MV_E_BUFOVER:          pstrErrorMsg += "Cache is full ";                                                   break;
	case MV_E_CALLORDER:        pstrErrorMsg += "Function calling order error ";                                    break;
	case MV_E_PARAMETER:        pstrErrorMsg += "Incorrect parameter ";                                             break;
	case MV_E_RESOURCE:         pstrErrorMsg += "Applying resource failed ";                                        break;
	case MV_E_NODATA:           pstrErrorMsg += "No data ";                                                         break;
	case MV_E_PRECONDITION:     pstrErrorMsg += "Precondition error, or running environment changed ";              break;
	case MV_E_VERSION:          pstrErrorMsg += "Version mismatches ";                                              break;
	case MV_E_NOENOUGH_BUF:     pstrErrorMsg += "Insufficient memory ";                                             break;
	case MV_E_ABNORMAL_IMAGE:   pstrErrorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
	case MV_E_UNKNOW:           pstrErrorMsg += "Unknown error ";                                                   break;
	case MV_E_GC_GENERIC:       pstrErrorMsg += "General error ";                                                   break;
	case MV_E_GC_ACCESS:        pstrErrorMsg += "Node accessing condition error ";                                  break;
	case MV_E_ACCESS_DENIED:	pstrErrorMsg += "No permission ";                                                   break;
	case MV_E_BUSY:             pstrErrorMsg += "Device is busy, or network disconnected ";                         break;
	case MV_E_NETER:            pstrErrorMsg += "Network error ";                                                   break;
	}

	MessageBox(pstrErrorMsg, TEXT("PROMPT"), MB_OK | MB_ICONWARNING);
}
// // ch:最开始时的窗口初始化 | en:Initial window initialization
void CCamera::DisplayWindowInitial()
{
	CWnd* pWnd = GetDlgItem(IDC_DISPLAY_STATIC);
	if (pWnd)
	{
		m_hwndDisplay = pWnd->GetSafeHwnd();
	}

}
void CCamera::OnBnClickedButton1()
{
	CString strMsg;

	m_ctrlDeviceCombo.ResetContent();
	memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

	// ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
	int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
	if (MV_OK != nRet)
	{
		return;
	}

	// ch:将值加入到信息列表框中并显示出来 | en:Add value to the information list box and display
	
	if (0 == m_stDevList.nDeviceNum)
	{
		ShowErrorMsg(TEXT("No device"), 0);
		return;
	}
	m_ctrlDeviceCombo.SetCurSel(0);
}

void CCamera::OnBnClickedOpenButton()
{
	if (TRUE == m_bOpenDevice || NULL != m_pcMvCamera)
	{
		return;
	}
	UpdateData(TRUE);

	int nIndex = m_nDeviceCombo;
	if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
	{
		ShowErrorMsg(TEXT("Please select device"), 0);
		return;
	}

	// ch:由设备信息创建设备实例 | en:Device instance created by device information
	if (NULL == m_stDevList.pDeviceInfo[nIndex])
	{
		ShowErrorMsg(TEXT("Device does not exist"), 0);
		return;
	}

	m_pcMvCamera = new CMvCamera;
	if (NULL == m_pcMvCamera)
	{
		return;
	}

	int nRet = m_pcMvCamera->MC_Open(m_stDevList.pDeviceInfo[nIndex]);
	if (MV_OK != nRet)
	{
		delete m_pcMvCamera;
		m_pcMvCamera = NULL;
		ShowErrorMsg(TEXT("Open Fail"), nRet);
		return;
	}

	// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
	if (m_stDevList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
	{
		unsigned int nPacketSize = 0;
		nRet = m_pcMvCamera->MC_GetOptimalPacketSize(&nPacketSize);
		if (nRet == MV_OK)
		{
			nRet = m_pcMvCamera->MC_SetIntValue("GevSCPSPacketSize", nPacketSize);
			if (nRet != MV_OK)
			{
				ShowErrorMsg(TEXT("Warning: Set Packet Size fail!"), nRet);
			}
		}
		else
		{
			ShowErrorMsg(TEXT("Warning: Get Packet Size fail!"), nRet);
		}
	}

	m_bOpenDevice = TRUE;
	//EnableControls(TRUE);
	OnBnClickedGetParameterButton(); // ch:获取参数 | en:Get Parameter
}

// ch:关闭设备 | en:Close Device
int CCamera::CloseDevice()
{
	
	m_bStartGrabbing = FALSE;
	m_bOpenDevice = FALSE;

	if (m_pchSaveImageBuf)
	{
		free(m_pchSaveImageBuf);
		m_pchSaveImageBuf = NULL;
	}
	m_chSaveImageBufSize = 0;

	return MV_OK;
}
void CCamera::OnBnClickedCloseButton()
{
	CloseDevice();
	//EnableControls(TRUE);
}
//取流线程
int CCamera::GrabThreadProcess()
{

	MV_DISPLAY_FRAME_INFO stDisplayInfo = { 0 };
	int nRet = MV_OK;

	//MV_FRAME_OUT stOutFrame = { 0 };		//输出图像地址及图像信息  
	//memset(&stOutFrame, 0, sizeof(MV_FRAME_OUT));
	//nRet = m_pcMvCamera->MC_SetCommandValue();//相机拍照
	////nRet = m_pcMvCamera->doGrabImage(stImageInfo,strSaveMultiPth);//单次取流，每一次拍照，调用一次
	//const char* ii = "Image_w%d_h%d_fn%03d.bmp";
	//strcat(strSaveMultiPth.GetBuffer(), ii);
	while (m_bThreadState)
	{
		if (!m_bStartGrabbing)
		{
			Sleep(10);
			continue;
		}

		nRet = m_pcMvCamera->MC_GetImageBuffer(&stImageInfo, 1000);
		if (nRet == MV_OK)
		{
			//用于保存图片
			EnterCriticalSection(&m_hSaveImageMux);
			if (NULL == m_pchSaveImageBuf || stImageInfo.stFrameInfo.nFrameLen > m_chSaveImageBufSize)
			{
				if (m_pchSaveImageBuf)
				{
					free(m_pchSaveImageBuf);
					m_pchSaveImageBuf = NULL;
				}

				m_pchSaveImageBuf = (unsigned char*)malloc(sizeof(unsigned char) * stImageInfo.stFrameInfo.nFrameLen);
				if (m_pchSaveImageBuf == NULL)
				{
					LeaveCriticalSection(&m_hSaveImageMux);
					return 0;
				}
				m_chSaveImageBufSize = stImageInfo.stFrameInfo.nFrameLen;
			}
			memcpy(m_pchSaveImageBuf, stImageInfo.pBufAddr, stImageInfo.stFrameInfo.nFrameLen);
			memcpy(&m_stImageInfo, &(stImageInfo.stFrameInfo), sizeof(MV_FRAME_OUT_INFO_EX));
			LeaveCriticalSection(&m_hSaveImageMux);

			stDisplayInfo.hWnd = m_hwndDisplay;
			stDisplayInfo.pData = stImageInfo.pBufAddr;
			stDisplayInfo.nDataLen = stImageInfo.stFrameInfo.nFrameLen;
			stDisplayInfo.nWidth = stImageInfo.stFrameInfo.nWidth;
			stDisplayInfo.nHeight = stImageInfo.stFrameInfo.nHeight;
			stDisplayInfo.enPixelType = stImageInfo.stFrameInfo.enPixelType;
			m_pcMvCamera->MC_DisplayOneFrame(&stDisplayInfo);


			MV_CC_PIXEL_CONVERT_PARAM  stConvertParam = { 0 };
			memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
			//彩色相机
#ifdef COLOUR_CAMERA
			MvGvspPixelType enDstPixelType = PixelType_Gvsp_BGR8_Packed;
			unsigned int nChannelNum = 3;
#endif // COLOUR_CAMERA
#ifdef GRAY_CAMERA
			MvGvspPixelType enDstPixelType = PixelType_Gvsp_Mono8;
			unsigned int nChannelNum = 1;
#endif // GRAY_CAMERA
			//unsigned int nBufSize = 0;//缓存大小
			//MVCC_INTVALUE stIntvalue; //获取一帧数据的大小
			//memset(&stIntvalue, 0, sizeof(MVCC_INTVALUE));
			//int tempValue = m_pcMvCamera->MC_GetValue(&stIntvalue);
			//nBufSize = stIntvalue.nCurValue;

			unsigned char* pConvertData = NULL;
			pConvertData = (unsigned char*)malloc(stImageInfo.stFrameInfo.nWidth * stImageInfo.stFrameInfo.nHeight * nChannelNum);
			unsigned int nConvertDataSize = stImageInfo.stFrameInfo.nWidth * stImageInfo.stFrameInfo.nHeight * nChannelNum;
			memset(pConvertData, 0, sizeof(pConvertData));
			nRet = m_pcMvCamera->MC_GetOneFrameTimeout(pConvertData, nConvertDataSize, &stImageInfo.stFrameInfo, 1000);
			//void* handle = NULL;
			//stConvertParam.nWidth = stImageInfo.stFrameInfo.nWidth;                 // image width
			//stConvertParam.nHeight = stImageInfo.stFrameInfo.nHeight;               // image height
			//stConvertParam.pSrcData = stImageInfo.pBufAddr;                         // input data buffer
			//stConvertParam.nSrcDataLen = stImageInfo.stFrameInfo.nFrameLen;         // input data size
			//stConvertParam.enSrcPixelType = stImageInfo.stFrameInfo.enPixelType;    // input pixel format
			//stConvertParam.enDstPixelType = enDstPixelType;                         // output pixel format
			//stConvertParam.pDstBuffer = pConvertData;                               // output data buffer
			//stConvertParam.nDstBufferSize = nConvertDataSize;                       // output buffer size
		 //   MV_CC_ConvertPixelType(handle, &stConvertParam);

			bool bConvertRet = false;
			bConvertRet = Convert2Mat(&stImageInfo.stFrameInfo, pConvertData);
			
			cv::Mat mCameraImg = cv::Mat(stConvertParam.nHeight, stConvertParam.nWidth, CV_8UC3, stConvertParam.pDstBuffer);
			cv::imshow("asd", mCameraImg);
			cv::imwrite("..//Img//lllll.jpg", mCameraImg);
			cv::waitKey(0);
			if (nCameraNum == 1)
			{
				nRet = m_pcMvCamera->doGrabImage(stImageInfo, strSaveMultiPth);//单次取流，每一次拍照，调用一次

			}
			m_pcMvCamera->MC_FreeImageBuffer(&stImageInfo);
		}
		else
		{
			if (MV_TRIGGER_MODE_ON == m_nTriggerMode)
			{
				Sleep(5);
			}
		}
	}
	//strSaveMultiPth.ReleaseBuffer();

	return MV_OK;
}
// ch:像素排列由RGB转为BGR | en:Convert pixel arrangement from RGB to BGR
void CCamera::RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight)
{
	if (NULL == pRgbData)
	{
		return;
	}

	// red和blue数据互换
	for (unsigned int j = 0; j < nHeight; j++)
	{
		for (unsigned int i = 0; i < nWidth; i++)
		{
			unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
			pRgbData[j * (nWidth * 3) + i * 3] = pRgbData[j * (nWidth * 3) + i * 3 + 2];
			pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
		}
	}
}

// ch:帧数据转换为Mat格式图片并保存 | en:Convert data stream to Mat format then save image
bool CCamera::Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData)
{
	if (NULL == pstImageInfo || NULL == pData)
	{
		printf("NULL info or data.\n");
		return false;
	}

	try
	{
		// ch:保存Mat图片 | en:Save converted image in a local file
		cvSaveImage("Image_Mat.bmp", &(IplImage(srcImage)));
	}
	catch (cv::Exception& ex)
	{
		fprintf(stderr, "Exception in saving mat image: %s\n", ex.what());
	}

	srcImage.release();

	return true;
}

// ch:取流线程 | en:Grabbing thread
unsigned int __stdcall GrabThread(void* pUser)
{
	if (pUser)
	{
		CCamera* pCam = (CCamera*)pUser;

		pCam->GrabThreadProcess();

		return 0;
	}

	return -1;
}
void CCamera::OnBnClickedButton2()
{
	if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing || NULL == m_pcMvCamera)
	{
		return;
	}

	memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
	m_bThreadState = TRUE;
	unsigned int nThreadID = 0;
	m_phGrabThread = (void*)_beginthreadex(NULL, 0, GrabThread, this, 0, &nThreadID);
	if (NULL == m_phGrabThread)
	{
		m_bThreadState = FALSE;
		ShowErrorMsg(TEXT("Create thread fail"), 0);
		return;
	}

	int nRet = m_pcMvCamera->MC_StartGrabbing();
	if (MV_OK != nRet)
	{
		m_bThreadState = FALSE;
		ShowErrorMsg(TEXT("Start grabbing fail"), nRet);
		return;
	}
	m_bStartGrabbing = TRUE;
}


void CCamera::OnBnClickedStopGrabbingButton()
{
	if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing || NULL == m_pcMvCamera)
	{
		return;
	}

	m_bThreadState = FALSE;
	if (m_phGrabThread)
	{
		WaitForSingleObject(m_phGrabThread, INFINITE);
		CloseHandle(m_phGrabThread);
		m_phGrabThread = NULL;
	}


	m_bStartGrabbing = FALSE;
	//EnableControls(TRUE);
}

// ch:获取触发模式 | en:Get Trigger Mode
int  CCamera::GetTriggerMode()
{
	MVCC_ENUMVALUE stEnumValue = { 0 };

	int nRet = m_pcMvCamera->MC_GetEnumValue("TriggerMode", &stEnumValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	m_nTriggerMode = stEnumValue.nCurValue;

	return MV_OK;
}// ch:获取触发源 | en:Get Trigger Source
int CCamera::GetTriggerSource()
{
	MVCC_ENUMVALUE stEnumValue = { 0 };

	int nRet = m_pcMvCamera->MC_GetEnumValue("TriggerSource", &stEnumValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}



	return MV_OK;
}
// ch:获取曝光时间 | en:Get Exposure Time
int CCamera::GetExposureTime()
{
	MVCC_FLOATVALUE stFloatValue = { 0 };

	int nRet = m_pcMvCamera->MC_GetFloatValue("ExposureTime", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	m_dExposureEdit = stFloatValue.fCurValue;

	return MV_OK;
}
//获取参数
void CCamera::OnBnClickedGetParameterButton()
{
	int nRet = GetTriggerMode();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("Get Trigger Mode Fail"), nRet);
	}

	nRet = GetExposureTime();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("曝光时间获取失败"), nRet);
	}

	nRet = GetGain();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("增益获取失败"), nRet);
	}

	nRet = GetFrameRate();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("帧率获取失败"), nRet);
	}

	nRet = GetTriggerSource();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("触发源获取失败"), nRet);
	}

	UpdateData(FALSE);
}
// ch:设置曝光时间 | en:Set Exposure Time
int CCamera::SetExposureTime()
{
	// ch:调节这两个曝光模式，才能让曝光时间生效
	// en:Adjust these two exposure mode to allow exposure time effective
	int nRet = m_pcMvCamera->MC_SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	m_pcMvCamera->MC_SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

	return m_pcMvCamera->MC_SetFloatValue("ExposureTime", (float)m_dExposureEdit);
}
// ch:获取增益 | en:Get Gain
int CCamera::GetGain()
{
	MVCC_FLOATVALUE stFloatValue = { 0 };



	return MV_OK;
}
// ch:设置增益 | en:Set Gain
int CCamera::SetGain()
{
	// ch:设置增益前先把自动增益关闭，失败无需返回
	//en:Set Gain after Auto Gain is turned off, this failure does not need to return
	m_pcMvCamera->MC_SetEnumValue("GainAuto", 0);

	return m_pcMvCamera->MC_SetFloatValue("Gain", (float)m_dGainEdit);
}
// ch:获取帧率 | en:Get Frame Rate
int CCamera::GetFrameRate()
{
	MVCC_FLOATVALUE stFloatValue = { 0 };

	int nRet = m_pcMvCamera->MC_GetFloatValue("ResultingFrameRate", &stFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_dFrameRateEdit = stFloatValue.fCurValue;

	return MV_OK;
}
// ch:设置帧率 | en:Set Frame Rate
int CCamera::SetFrameRate()
{
	int nRet = m_pcMvCamera->MC_SetBoolValue("AcquisitionFrameRateEnable", true);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	return m_pcMvCamera->MC_SetFloatValue("AcquisitionFrameRate", (float)m_dFrameRateEdit);
}
//参数设置
void CCamera::OnBnClickedSetParameterButton()
{
	UpdateData(TRUE);

	bool bIsSetSucceed = true;
	int nRet = SetExposureTime();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		ShowErrorMsg(TEXT("曝光时间设置失败"), nRet);
	}
	nRet = SetGain();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		ShowErrorMsg(TEXT("增益设置失败"), nRet);
	}
	nRet = SetFrameRate();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		ShowErrorMsg(TEXT("帧率设置失败"), nRet);
	}

	if (true == bIsSetSucceed)
	{
		ShowErrorMsg(TEXT("参数设置成功！"), nRet);
	}
}


void CCamera::OnBnClickedMfcbutton1()
{
	CString strMsg;

	m_ctrlDeviceCombo.ResetContent();
	memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

	// ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
	int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
	if (MV_OK != nRet)
	{
		return;
	}

	// ch:将值加入到信息列表框中并显示出来 | en:Add value to the information list box and display
	for (unsigned int i = 0; i < m_stDevList.nDeviceNum; i++)
	{
		MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
		if (NULL == pDeviceInfo)
		{
			continue;
		}

		wchar_t* pUserName = NULL;
		if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
		{
			int nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
			int nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
			int nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
			int nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

			if (strcmp("", (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
			{
				DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, NULL, 0);
				pUserName = new wchar_t[dwLenUserName];
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
			}
			else
			{
				char strUserName[256] = { 0 };
				sprintf_s(strUserName, 256, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
					pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
					pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
				DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
				pUserName = new wchar_t[dwLenUserName];
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
			}
			strMsg.Format(_T("[%d]GigE:    %s  (%d.%d.%d.%d)"), i, pUserName, nIp1, nIp2, nIp3, nIp4);
		}
		else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
		{
			if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
			{
				DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, NULL, 0);
				pUserName = new wchar_t[dwLenUserName];
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
			}
			else
			{
				char strUserName[256] = { 0 };
				sprintf_s(strUserName, 256, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
					pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
					pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
				DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
				pUserName = new wchar_t[dwLenUserName];
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
			}
			strMsg.Format(_T("[%d]UsbV3:  %s"), i, pUserName);
		}
		else
		{
			ShowErrorMsg(TEXT("Unknown device enumerated"), 0);
		}
		m_ctrlDeviceCombo.AddString(strMsg);

		if (pUserName)
		{
			delete[] pUserName;
			pUserName = NULL;
		}
	}

	if (0 == m_stDevList.nDeviceNum)
	{
		ShowErrorMsg(TEXT("No device"), 0);
		return;
	}
	m_ctrlDeviceCombo.SetCurSel(0);// TODO: 在此添加控件通知处理程序代码
}


void CCamera::OnBnClickedOpenMfcbutton()
{
	if (TRUE == m_bOpenDevice || NULL != m_pcMvCamera)
	{
		return;
	}
	UpdateData(TRUE);

	int nIndex = m_nDeviceCombo;
	if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
	{
		ShowErrorMsg(TEXT("Please select device"), 0);
		return;
	}

	// ch:由设备信息创建设备实例 | en:Device instance created by device information
	if (NULL == m_stDevList.pDeviceInfo[nIndex])
	{
		ShowErrorMsg(TEXT("Device does not exist"), 0);
		return;
	}

	m_pcMvCamera = new CMvCamera;
	if (NULL == m_pcMvCamera)
	{
		return;
	}
	//int nRet = MV_CC_CreateHandleByGenTL(&handle, m_stDevList.pDeviceInfo[nIndex]);
	int nRet = m_pcMvCamera->MC_Open(m_stDevList.pDeviceInfo[nIndex]);
	if (MV_OK != nRet)
	{
		delete m_pcMvCamera;
		m_pcMvCamera = NULL;
		ShowErrorMsg(TEXT("Open Fail"), nRet);
		return;
	}

	// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
	if (m_stDevList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
	{
		unsigned int nPacketSize = 0;
		nRet = m_pcMvCamera->MC_GetOptimalPacketSize(&nPacketSize);
		if (nRet == MV_OK)
		{
			nRet = m_pcMvCamera->MC_SetIntValue("GevSCPSPacketSize", nPacketSize);
			if (nRet != MV_OK)
			{
				ShowErrorMsg(TEXT("Warning: Set Packet Size fail!"), nRet);
			}
		}
		else
		{
			ShowErrorMsg(TEXT("Warning: Get Packet Size fail!"), nRet);
		}
	}

	m_bOpenDevice = TRUE;
	//EnableControls(TRUE);
	OnBnClickedGetParameterButton(); // ch:获取参数 | en:Get Parameter
}


void CCamera::OnBnClickedCloseMfcbutton()
{
	CloseDevice();
	//EnableControls(TRUE);
}


void CCamera::OnBnClickedMfcbutton5()
{
	if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing || NULL == m_pcMvCamera)
	{
		return;
	}

	memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
	m_bThreadState = TRUE;
	unsigned int nThreadID = 0;
	m_phGrabThread = (void*)_beginthreadex(NULL, 0, GrabThread, this, 0, &nThreadID);
	if (NULL == m_phGrabThread)
	{
		m_bThreadState = FALSE;
		ShowErrorMsg(TEXT("Create thread fail"), 0);
		return;
	}

	int nRet = m_pcMvCamera->MC_StartGrabbing();
	if (MV_OK != nRet)
	{
		m_bThreadState = FALSE;
		ShowErrorMsg(TEXT("Start grabbing fail"), nRet);
		return;
	}
	m_bStartGrabbing = TRUE;
}


void CCamera::OnBnClickedStopGrabbingMfcbutton6()
{
	if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing || NULL == m_pcMvCamera)
	{
		return;
	}

	m_bThreadState = FALSE;
	if (m_phGrabThread)
	{
		WaitForSingleObject(m_phGrabThread, INFINITE);
		CloseHandle(m_phGrabThread);
		m_phGrabThread = NULL;
	}

	int nRet = m_pcMvCamera->MC_StopGrabbing();
	if (MV_OK != nRet)
	{
		ShowErrorMsg(TEXT("Stop grabbing fail"), nRet);
		return;
	}
	m_bStartGrabbing = FALSE;
	//EnableControls(TRUE);
}


void CCamera::OnBnClickedSinglebutton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CCamera::OnBnClickedGetParameterMfcbutton9()
{
	int nRet = GetTriggerMode();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("Get Trigger Mode Fail"), nRet);
	}

	nRet = GetExposureTime();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("曝光时间获取失败"), nRet);
	}

	nRet = GetGain();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("增益获取失败"), nRet);
	}

	nRet = GetFrameRate();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("帧率获取失败"), nRet);
	}

	nRet = GetTriggerSource();
	if (nRet != MV_OK)
	{
		ShowErrorMsg(TEXT("触发源获取失败"), nRet);
	}

	UpdateData(FALSE);
}


void CCamera::OnBnClickedSetParameterMfcbutton10()
{
	UpdateData(TRUE);

	bool bIsSetSucceed = true;
	int nRet = SetExposureTime();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		ShowErrorMsg(TEXT("曝光时间设置失败"), nRet);
	}
	nRet = SetGain();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		ShowErrorMsg(TEXT("增益设置失败"), nRet);
	}
	nRet = SetFrameRate();
	if (nRet != MV_OK)
	{
		bIsSetSucceed = false;
		ShowErrorMsg(TEXT("帧率设置失败"), nRet);
	}

	if (true == bIsSetSucceed)
	{
		ShowErrorMsg(TEXT("参数设置成功！"), nRet);
	}
}


void CCamera::OnEnChangeSinglesaveEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CCamera::OnEnChangeMulsaveEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

// ch:保存图片 | en:Save Image
int CCamera::SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType)
{
	MV_SAVE_IMG_TO_FILE_PARAM stSaveFileParam;
	memset(&stSaveFileParam, 0, sizeof(MV_SAVE_IMG_TO_FILE_PARAM));

	EnterCriticalSection(&m_hSaveImageMux);
	if (m_pchSaveImageBuf == NULL || m_stImageInfo.enPixelType == 0)
	{
		LeaveCriticalSection(&m_hSaveImageMux);
		return MV_E_NODATA;
	}

	stSaveFileParam.enImageType = enSaveImageType; // ch:需要保存的图像类型 | en:Image format to save
	stSaveFileParam.enPixelType = m_stImageInfo.enPixelType;  // ch:相机对应的像素格式 | en:Camera pixel type
	stSaveFileParam.nWidth = m_stImageInfo.nWidth;         // ch:相机对应的宽 | en:Width
	stSaveFileParam.nHeight = m_stImageInfo.nHeight;          // ch:相机对应的高 | en:Height
	stSaveFileParam.nDataLen = m_stImageInfo.nFrameLen;
	stSaveFileParam.pData = m_pchSaveImageBuf;
	stSaveFileParam.iMethodValue = 0;

	// ch:jpg图像质量范围为(50-99], png图像质量范围为[0-9] | en:jpg image nQuality range is (50-99], png image nQuality range is [0-9]
	if (MV_Image_Bmp == stSaveFileParam.enImageType)
	{
		//strcat(strSaveMultiPth.GetBuffer(), "Image_w%d_h%d_fn%03d.bmp");

		sprintf_s(stSaveFileParam.pImagePath, 256, strSaveSinglePth +"Image_w%d_h%d_fn%03d.bmp", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
		//strSaveMultiPth.ReleaseBuffer();
	}
	else if (MV_Image_Jpeg == stSaveFileParam.enImageType)
	{
		stSaveFileParam.nQuality = 80;
		//strcat(strSaveSinglePth.GetBuffer(), "Image_w%d_h%d_fn%03d.bmp");
		sprintf_s(stSaveFileParam.pImagePath, 256, strSaveSinglePth +"Image_w%d_h%d_fn%03d.bmp", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
		//strSaveMultiPth.ReleaseBuffer();
	}
	else if (MV_Image_Tif == stSaveFileParam.enImageType)
	{
		sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.tif", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
	}
	else if (MV_Image_Png == stSaveFileParam.enImageType)
	{
		stSaveFileParam.nQuality = 8;
		sprintf_s(stSaveFileParam.pImagePath, 256, "Image_w%d_h%d_fn%03d.png", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
	}

	int nRet = m_pcMvCamera->MC_SaveImageToFile(&stSaveFileParam);
	LeaveCriticalSection(&m_hSaveImageMux);

	return nRet;
}

void CCamera::OnBnClickedSinglemfcbutton7()
{
	int nRet = SaveImage(MV_Image_Jpeg);
	if (MV_OK != nRet)
	{
		ShowErrorMsg(TEXT("Save jpg fail"), nRet);
		return;
	}
	ShowErrorMsg(TEXT("Save jpg succeed"), nRet);
}


void CCamera::OnBnClickedMulmfcbutton8()
{
	//MV_FRAME_OUT stOutFrame = { 0 };		//输出图像地址及图像信息  
	//memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT));
	//int nRet = m_pcMvCamera->MC_SetCommandValue();//相机拍照
	//if (nRet != MV_OK)
	//{
	//	ShowErrorMsg(TEXT("TriggerSoftwarefail!"), nRet);
	//	return;
	//}
	//for (int i = 0; i < GrabNum; i++)
	//{
	//	nRet = m_pcMvCamera->doGrabImage(stImageInfo);//单次取流，每一次拍照，调用一次
	//	if (nRet != MV_OK)
	//	{
	//		printf("doGrabImage! nRet [0x%x]\n", nRet);
	//	}
	//}
	nCameraNum = 1;
}


void CCamera::OnBnClickedMulmfcbutton9()
{
	nCameraNum = 0;
}
