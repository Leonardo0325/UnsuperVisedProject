#pragma once
#include"ud_mvcamera.h"
#include<queue>
#include<opencv2/opencv.hpp>
// ud_camera 对话框

class CCamera : public CDialogEx
{
	DECLARE_DYNAMIC(CCamera)

public:
	CCamera(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCamera();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// ch:去除自定义的像素格式 | en:Remove custom pixel formats
	//bool RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat);
private:
	//MVS
	CComboBox						m_ctrlDeviceCombo;//枚举到的设备
	CComboBox                       m_ctrlFileNameCombo;   //选择的文件名
	int								m_nDeviceCombo;//在线的设备数
	MV_FRAME_OUT_INFO_EX            m_stImageInfo;
	void* m_phGrabThread;              // ch:取流线程句柄 | en:Grab thread handle
	BOOL                    m_bThreadState;
	MV_CC_DEVICE_INFO_LIST			m_stDevList;   //设备列表

	CRITICAL_SECTION        m_hSaveImageMux;
	unsigned char* m_pchSaveImageBuf;
	unsigned int            m_chSaveImageBufSize;
	//void EnableControls(BOOL bIsCameraReady);

	
	BOOL                    m_bOpenDevice;                        // ch:是否打开设备 | en:Whether to open device
	BOOL                    m_bStartGrabbing;                     // ch:是否开始抓图 | en:Whether to start grabbing
	BOOL                    m_bLightSaveSingle;                   //光条单张保存
	BOOL                    m_bLightSaveMultiple;                 //光条多张保存
	int                     m_nTriggerMode;                       // ch:触发模式 | en:Trigger Mode
	int                     m_nTriggerSource;                     // ch:触发源 | en:Trigger Source
	int                     GetTriggerMode();                     //  获取触发模式
	int CloseDevice();                   // ch:关闭设备 | en:Close Device
	//int SaveCalibImage(MV_SAVE_IAMGE_TYPE enSaveImageType);						 //保存图片
	//int SaveLightImage(MV_SAVE_IAMGE_TYPE enSaveImageType);
	//int SaveLightImage1(MV_SAVE_IAMGE_TYPE enSaveImageType);
	CMvCamera* m_pcMvCamera;               // MvCamera封装了常用接口 | en:ClslMV_CAMERA_C packed commonly used interface
	HWND                    m_hwndDisplay;                        // ch:显示句柄 | en:Display Handle
	int SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType);
	//最开始时的窗口初始化 
	void DisplayWindowInitial();
	void ShowErrorMsg(CString csMessage, int nErrorNum);   //显示错误信息
	int GetExposureTime();               // ch:设置曝光时间 | en:Set Exposure Time
	int SetExposureTime();
	int GetGain();                       // ch:设置增益 | en:Set Gain
	int SetGain();
	int GetFrameRate();                  // ch:设置帧率 | en:Set Frame Rate
	int SetFrameRate();
	int GetTriggerSource();              // ch:设置触发源 | en:Set Trigger Source
	//int SetTriggerSource();

	double                  m_dExposureEdit;
	double                  m_dGainEdit;
	double                  m_dFrameRateEdit;
	MV_FRAME_OUT stImageInfo = { 0 };
	void* handle;
	CEdit m_editSaveSingle;
	CEdit m_editSaveMulti;
	CString strSaveSinglePth = _T("..\\Img\\CameraImg\\Single\\");
	CString strSaveMultiPth = _T("..\\Img\\CameraImg\\Multi\\");
	int nCameraNum;
	queue<cv::Mat> qCameraImg;
public:
	int GrabThreadProcess();                  //初始化
	//int GrabThreadSave();					  //取流+保存图片
	void RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight);
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char* pData);

	enum CONVERT_TYPE
	{
		OpenCV_Mat = 0,    // ch:Mat图像格式 | en:Mat format
		OpenCV_IplImage = 1,    // ch:IplImage图像格式 | en:IplImage format
	};

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOpenButton();
	afx_msg void OnBnClickedCloseButton();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedStopGrabbingButton();
	afx_msg void OnBnClickedGetParameterButton();
	afx_msg void OnBnClickedSetParameterButton();
	afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnBnClickedOpenMfcbutton();
	afx_msg void OnBnClickedCloseMfcbutton();
	afx_msg void OnBnClickedMfcbutton5();
	afx_msg void OnBnClickedStopGrabbingMfcbutton6();
	afx_msg void OnBnClickedSinglebutton();
	afx_msg void OnBnClickedGetParameterMfcbutton9();
	afx_msg void OnBnClickedSetParameterMfcbutton10();
	afx_msg void OnEnChangeSinglesaveEdit();
	afx_msg void OnEnChangeMulsaveEdit();
	afx_msg void OnBnClickedSinglemfcbutton7();
	afx_msg void OnBnClickedMulmfcbutton8();
	afx_msg void OnBnClickedMulmfcbutton9();
};
