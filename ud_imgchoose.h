#ifndef UD_IMGCHOOSE_H
#define UD_IMGCHOOSE_H


#pragma once

#include<string>
#include<opencv2/opencv.hpp>
#include "framework.h"
#include <experimental/filesystem>
#include <cstdlib>
#include"ud_tools.h"
namespace stdfs = std::experimental::filesystem;
// CImgChoose 对话框

class CImgChoose : public CDialogEx
{
	DECLARE_DYNAMIC(CImgChoose)

public:
	CImgChoose(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImgChoose();
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	//virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	HACCEL m_hAccelTable;
private:
	CTools* m_pcTools = new CTools();
	CString strOpenFoldPth = _T("..\\ProcessImg\\");
	CString strSaveGoodPth = _T("..\\dist\\DataSets\\LED\\train\\good\\");
	CString strSaveBadPth = _T("..\\dist\\DataSets\\LED\\test\\bad\\");
	CEdit m_editOpenFold;
	CEdit m_editSaveGood;
	CEdit m_editSaveBad;
	CEdit m_editGoodNUm;
	CEdit m_editBadNum;
	CEdit m_editAllNum;
	CEdit m_editFoldNum;
	std::vector<cv::String> imageFiles;
	int nImgIndex;
	//CMFCButton* m_btnPrevious = new CMFCButton;
	//CMFCButton* m_btnNext = new CMFCButton;
	CButton m_btnPrevious;
	CButton m_btnNext;
	cv::Mat dlgImage;
	int nSaveImgNum;
public:
	afx_msg void OnEnChangeEditopenfold();
	afx_msg void OnEnChangeEditsavegood();
	afx_msg void OnEnChangeEditsavebad();
	afx_msg void OnBnClickedOpenfilebutton();
public:
	bool isDirectoryEmpty(const std::string& folderPath);
	afx_msg void OnStnClickedDstimg();
	afx_msg void OnBnClickedPreviousbutton();
	afx_msg void OnBnClickedNextbutton();
	afx_msg void OnBnClickedGoodbutton();
	afx_msg void OnBnClickedBadbutton();
	afx_msg void OnEnChangeEditgoodnum();
	afx_msg void OnEnChangeEditbadnum();
	afx_msg void OnEnChangeEditallnum();
	afx_msg void OnEnChangeEditfoldnum();
	afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnBnClickedMfcbutton3();
	afx_msg void OnBnClickedMfcbutton4();
	afx_msg void OnBnClickedMfcbutton7();
	afx_msg void OnBnClickedMfcbutton6();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
#endif // !UD_IMGCHOOSE_H