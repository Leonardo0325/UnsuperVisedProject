
#ifndef UNSUPERVISEDEPLOY_H
#define UNSUPERVISEDEPLOY_H
// UnSuperviseDeployDlg.h: 头文件
//
#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include<afxwin.h>
#pragma once
#include<string>
#include"ud_trtdeploy.h"
#include"ud_trtcore.h"
#include"ud_preprocess.h"
#include"ud_imgchoose.h"
#include "ud_mvcamera.h"
#include"ud_camera.h"
#include <filesystem>
#include <algorithm>
#include <map>
#include <vector>
using std::map;
using std::vector;
using namespace std;
typedef unsigned int UNIT;
class CTrtCore;
#define WM_UPDATE_LOG (WM_USER+100)
//typedef 
typedef struct TAG_CTRLRECT
{
public:
	int nId;//控件ID
	double dScale[4];//缩放值
	TAG_CTRLRECT()
	{
		nId = 0;//未定义
		dScale[0] = 0;
		dScale[1] = 0;
		dScale[2] = 0;
		dScale[3] = 0;
	}
	TAG_CTRLRECT(const TAG_CTRLRECT& cr)
	{
		*this = cr;
	}
	bool operator () (const TAG_CTRLRECT& cr) {
		return (cr.nId == nId);
	}
}CTRLRECT;

class CCtrlScale
{
public:
	CCtrlScale(void);
	CCtrlScale(CWnd* pParentWnd);
	virtual ~CCtrlScale(void);

	void Init(CWnd* pParentWnd);
	void Scale(int cx, int cy);

private:
	CWnd* m_pParentWnd;
	CRect m_rect;
	vector<CTRLRECT> m_vecCtrl;//保存控件缩放信息
};
// CUnSuperviseDeployDlg 对话框
class CUnSuperviseDeployDlg : public CDialogEx
{
// 构造
public:
	CUnSuperviseDeployDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNSUPERVISEDEPLOY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
//自适应大小
private:
	CRect m_rect;
	map<int, vector<double>> tar;				//存储所有控件比例信息    key = 控件ID  value = 控件比例
	map<int, vector<double>>::iterator key;		//迭代器查找结果
	map<int, vector<double>>::iterator end;		//存储容器的最后一个元素

// 实现
protected:
	HICON m_hIcon;

	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	// 在头文件中声明消息映射函数
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void OpenPic();
	void ShowPic(CImage&imgLightImg);
	void ShowThresh(CImage&imgLightImg);
	void ShowAdd(CImage&imgLightImg);
	void ShowImg(CImage&imgLightImg,CWnd* pWnd);
	void MatToCImage(cv::Mat& mMatImg, CImage& cImage);
	int dlgModelChoose();
	void dlgAllDeployFunc();
	CString UD_OpenFile();
	void ReSize();//txwtech
	POINT old;//txwtech
	CRect m_DlgRect; //txwtech
	void repaint(UINT id, int last_Width, int now_Width, int last_Height, int now_Height);
	//显示多行
	void ShowMultilineText(CEdit& edit, CString& strInfo);
	void ShowString(CString name);
	//算法线程
	static UNIT _cdecl dlgDeployThread(LPVOID lparam);
	static UNIT _cdecl dlgALLDeployThread(LPVOID lparam);
	static UNIT _cdecl dlgTrainThread(LPVOID lparam);
	static UNIT _cdecl dlgCameraThread(LPVOID lparam);

private:
	CString m_pstrFilePath , m_pstrFileName;
	CString strText = _T("..\\ProcessImg\\");
	string m_sPicPath;
	CTrtDeploy * m_pcTrtDeploy = new CTrtDeploy();
	CPreProcess * m_pcPreProcess = new CPreProcess();
	CImgChoose * m_ImgChooseDlg = new CImgChoose();
	CCamera* m_CameraDlg = new CCamera();
	returnResult_S ImgResults;
	friend class CTrtCore;
	





private:
	int nLogIndex;
	CString m_cLogIndex;
	CString m_cLogtent;
	string ModelPth;
	CString LoguruText;
	CEdit m_editLoguru;
	CEdit m_editCtrl;
	CBrush m_brush;
	CString m_sPathName;  //文件夹路径
	CString cPreFolder;
	CSliderCtrl cThreshSlider;
	CEdit cThreshEdit;
	CEdit cWuEdit;
	CEdit cLouEdit;
	CEdit cRightEdit;
	CButton* pModelPadimRadio;
	CButton* pModelEffADRadio;
	int nThreshValue = 60;
	CCtrlScale m_scale;
	CString strValue;
	CImage m_LoadImg;
	CButton m_openButton;
	//CFileDialog FileDlg(TRUE);
	CFile f;
	CString FileName;
	//CString m_strFilePath = _T("../loguru.txt");
	//CfiletestDlg* g_CfiletestDlg = NULL;
	CUnSuperviseDeployDlg* g_CUnSuperviseDeployDl = NULL;
private:
	CSpinButtonCtrl m_Spin;
public:

	afx_msg void OnBnClickedOpenimg();
	afx_msg void OnBnClickedInit();
	afx_msg void OnBnClickedDeploy();
	afx_msg void OnBnClickedOpendic();
	afx_msg void OnEnChangeEditdict();
	afx_msg void OnBnClickedAlldeploy();
	afx_msg void OnBnClickedFirstimg();
	afx_msg void OnBnClickedOpencvradio();
	afx_msg void OnBnClickedLabelradio();
	afx_msg void OnBnClickedSavepreprocess();
	afx_msg void OnEnChangeProprecessedit();
	afx_msg void OnBnClickedPreprocess();
	afx_msg void OnEnChangeThreshedit();
	afx_msg void OnNMCustomdrawSliderthresh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditallnums();
	afx_msg void OnEnChangeEditwunums();
	afx_msg void OnEnChangeEditlounums();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedPreprocess2();
	afx_msg void OnEnChangeProprecessedit2();
	afx_msg void OnBnClickedModelchoose();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedChooseimg();
	afx_msg void OnBnClickedButtontrain();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStnClickedSrcimg();
	afx_msg void OnStnClickedStaticicon();
	afx_msg void OnEnChangeEditloguru();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedMfcbutton2();
	afx_msg void OnBnClickedFirstimgmfcbutton();
	afx_msg void OnBnClickedPreprocessmfcbutton();
	afx_msg void OnBnClickedChooseimgmfcbutton();
	afx_msg void OnBnClickedTrainmfcbutton();
	afx_msg void OnBnClickedOpenimgmfcbutton();
	afx_msg void OnBnClickedInitmfcbutton();
	afx_msg void OnBnClickedDeploymfcbutton();
	afx_msg void OnBnClickedOpendicmfcbutton();
	afx_msg void OnBnClickedAlldeploymfcbutton();
};
#pragma once

#endif