// ud_imgchoose.cpp: 实现文件
//

#include "pch.h"
#include "UnSuperviseDeploy.h"
#include "ud_imgchoose.h"
#include "afxdialogex.h"
#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem; // C++11的文件系统命名空间

// CImgChoose 对话框

IMPLEMENT_DYNAMIC(CImgChoose, CDialogEx)

BEGIN_MESSAGE_MAP(CImgChoose, CDialogEx)
	ON_EN_CHANGE(IDC_EDITOPENFOLD, &CImgChoose::OnEnChangeEditopenfold)
	ON_EN_CHANGE(IDC_EDITSAVEGOOD, &CImgChoose::OnEnChangeEditsavegood)
	ON_EN_CHANGE(IDC_EDITSAVEBAD, &CImgChoose::OnEnChangeEditsavebad)
	//ON_BN_CLICKED(IDC_OPENFILEBUTTON, &CImgChoose::OnBnClickedOpenfilebutton)
	ON_STN_CLICKED(IDC_DstImg, &CImgChoose::OnStnClickedDstimg)
	//ON_BN_CLICKED(IDC_PREVIOUSBUTTON, &CImgChoose::OnBnClickedPreviousbutton)
	//ON_BN_CLICKED(IDC_NEXTBUTTON, &CImgChoose::OnBnClickedNextbutton)
	//ON_BN_CLICKED(IDC_GOODBUTTON, &CImgChoose::OnBnClickedGoodbutton)
	//ON_BN_CLICKED(IDC_BADBUTTON, &CImgChoose::OnBnClickedBadbutton)
	ON_EN_CHANGE(IDC_EDITGOODNUM, &CImgChoose::OnEnChangeEditgoodnum)
	ON_EN_CHANGE(IDC_EDITBADNUM, &CImgChoose::OnEnChangeEditbadnum)
	ON_EN_CHANGE(IDC_EDITALLNUM, &CImgChoose::OnEnChangeEditallnum)
	ON_EN_CHANGE(IDC_EDITFOLDNUM, &CImgChoose::OnEnChangeEditfoldnum)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CImgChoose::OnBnClickedMfcbutton1)
	//ON_BN_CLICKED(IDC_MFCBUTTON3, &CImgChoose::OnBnClickedMfcbutton3)
	//ON_BN_CLICKED(IDC_MFCBUTTON4, &CImgChoose::OnBnClickedMfcbutton4)
	ON_BN_CLICKED(IDC_MFCBUTTON7, &CImgChoose::OnBnClickedMfcbutton7)
	ON_BN_CLICKED(IDC_MFCBUTTON6, &CImgChoose::OnBnClickedMfcbutton6)
	ON_BN_CLICKED(IDC_BUTTON1, &CImgChoose::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CImgChoose::OnBnClickedButton2)
END_MESSAGE_MAP()

CImgChoose::CImgChoose(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{


	//m_editOpenFold.SubclassDlgItem(IDC_EDITOPENFOLD, this);
	//m_editOpenFold.SetWindowText(strOpenFoldPth);

	//m_editSaveGood.SubclassDlgItem(IDC_EDITSAVEGOOD, this);
	//m_editSaveGood.SetWindowText(strSaveGoodPth);

	//m_editSaveBad.SubclassDlgItem(IDC_EDITSAVEBAD, this);
	//m_editSaveBad.SetWindowText(strSaveBadPth);

	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDC_BUTTON2));
}

CImgChoose::~CImgChoose()
{
}

void CImgChoose::DoDataExchange(CDataExchange* pDX)
{
	//初始化函数
	CDialogEx::DoDataExchange(pDX);
	m_editOpenFold.SubclassDlgItem(IDC_EDITOPENFOLD, this);
	m_editOpenFold.SetWindowText(strOpenFoldPth);

	m_editFoldNum.SubclassDlgItem(IDC_EDITFOLDNUM, this);
	m_editFoldNum.SetWindowText(to_string(0).c_str());

	m_editSaveGood.SubclassDlgItem(IDC_EDITSAVEGOOD, this);
	m_editSaveGood.SetWindowText(strSaveGoodPth);

	m_editSaveBad.SubclassDlgItem(IDC_EDITSAVEBAD, this);
	m_editSaveBad.SetWindowText(strSaveBadPth);

	m_editGoodNUm.SubclassDlgItem(IDC_EDITGOODNUM, this);
	m_editGoodNUm.SetWindowText(to_string(0).c_str());

	m_editBadNum.SubclassDlgItem(IDC_EDITBADNUM, this);
	m_editBadNum.SetWindowText(to_string(0).c_str());

	m_editAllNum.SubclassDlgItem(IDC_EDITALLNUM, this);
	m_editAllNum.SetWindowText(to_string(0).c_str());

	m_btnPrevious.SubclassDlgItem(IDC_BUTTON1, this);
	m_btnNext.SubclassDlgItem(IDC_BUTTON2, this);

	nSaveImgNum = 0;

	//上一张 可选/不选
	if (nImgIndex == 0)
	{
		m_btnPrevious.EnableWindow(0);
	}
}





// CImgChoose 消息处理程序




void CImgChoose::OnEnChangeEditopenfold()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CImgChoose::OnEnChangeEditsavegood()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CImgChoose::OnEnChangeEditsavebad()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CImgChoose::OnBnClickedOpenfilebutton()
{
	std::string folderPath = strOpenFoldPth.GetBuffer();
	if (isDirectoryEmpty(folderPath))
	{
		AfxMessageBox("文件夹为空,请确保抠图完成");
	}
	else
	{
		CImage cImage;
		CWnd* pOpenWnd = NULL;
		pOpenWnd = GetDlgItem(IDC_DstImg);//获取控件句柄
		cv::String pattern = folderPath; // 定义图像文件的模式（这里假设图像为.jpg格式）
		//std::vector<cv::String> imageFiles;
		for (const auto &i : stdfs::directory_iterator(stdfs::path(pattern))) {
			imageFiles.push_back(i.path().generic_string());
		}
		m_editFoldNum.SetWindowText(to_string(imageFiles.size()).c_str());

		dlgImage = cv::imread(imageFiles[0]); // 读取第一张图像
		m_pcTools->MatToCImage(dlgImage, cImage);
		m_pcTools->ShowImg(cImage, pOpenWnd);
		nImgIndex = 0;

	}
}

//判断文件夹是否为空
bool CImgChoose::isDirectoryEmpty(const std::string& folderPath)
{
	if (!fs::is_directory(folderPath))
	{
		std::cout << "指定路径不是文件夹" << std::endl;
		return false;
	}

	fs::directory_iterator it(folderPath);
	return it == fs::directory_iterator(); // 如果迭代器等于结束迭代器，则文件夹为空
}


void CImgChoose::OnStnClickedDstimg()
{
	// TODO: 在此添加控件通知处理程序代码
}



void CImgChoose::OnEnChangeEditgoodnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CImgChoose::OnEnChangeEditbadnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CImgChoose::OnEnChangeEditallnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CImgChoose::OnEnChangeEditfoldnum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CImgChoose::OnBnClickedMfcbutton1()
{
	std::string folderPath = strOpenFoldPth.GetBuffer();
	if (isDirectoryEmpty(folderPath))
	{
		AfxMessageBox("文件夹为空,请确保抠图完成");
	}
	else
	{
		CImage cImage;
		CWnd* pOpenWnd = NULL;
		pOpenWnd = GetDlgItem(IDC_DstImg);//获取控件句柄
		cv::String pattern = folderPath; // 定义图像文件的模式（这里假设图像为.jpg格式）
		//std::vector<cv::String> imageFiles;
		for (const auto& i : stdfs::directory_iterator(stdfs::path(pattern))) {
			imageFiles.push_back(i.path().generic_string());
		}
		m_editFoldNum.SetWindowText(to_string(imageFiles.size()).c_str());

		dlgImage = cv::imread(imageFiles[0]); // 读取第一张图像
		m_pcTools->MatToCImage(dlgImage, cImage);
		m_pcTools->ShowImg(cImage, pOpenWnd);
		nImgIndex = 0;

	}
}


void CImgChoose::OnBnClickedMfcbutton3()
{
	nImgIndex--;
	CWnd* pPreviousWnd = NULL;
	pPreviousWnd = GetDlgItem(IDC_DstImg);//获取控件句柄
	CImage cImage;
	dlgImage = cv::imread(imageFiles[nImgIndex]); // 读取第一张图像
	m_pcTools->MatToCImage(dlgImage, cImage);
	m_pcTools->ShowImg(cImage, pPreviousWnd);
	if (nImgIndex == 0)
	{
		m_btnPrevious.EnableWindow(0);
	}
}


void CImgChoose::OnBnClickedMfcbutton4()
{
	nImgIndex++;
	CWnd* pNextWnd = NULL;
	pNextWnd = GetDlgItem(IDC_DstImg);//获取控件句柄

	CImage cImage;
	dlgImage = cv::imread(imageFiles[nImgIndex]); // 读取第一张图像
	m_pcTools->MatToCImage(dlgImage, cImage);
	m_pcTools->ShowImg(cImage, pNextWnd);
	if (nImgIndex != 0) m_btnPrevious.EnableWindow(1);
	else if (nImgIndex == imageFiles.size() - 1) m_btnNext.EnableWindow(0);
}


void CImgChoose::OnBnClickedMfcbutton7()
{
	static int nGoodIndex = 0;
	std::string ImgPath = strSaveGoodPth.GetBuffer();
	if (!dlgImage.empty()) cv::imwrite(ImgPath + to_string(nGoodIndex) + ".jpg", dlgImage);
	nGoodIndex++;
	nSaveImgNum++;
	m_editGoodNUm.SetWindowText(to_string(nGoodIndex).c_str());
	m_editAllNum.SetWindowText(to_string(nSaveImgNum).c_str());
}


void CImgChoose::OnBnClickedMfcbutton6()
{
	static int nBadIndex = 0;
	std::string ImgPath = strSaveBadPth.GetBuffer();
	if (!dlgImage.empty()) cv::imwrite(ImgPath + to_string(nBadIndex) + ".jpg", dlgImage);
	nBadIndex++;
	nSaveImgNum++;
	m_editBadNum.SetWindowText(to_string(nBadIndex).c_str());
	m_editAllNum.SetWindowText(to_string(nSaveImgNum).c_str());
}


void CImgChoose::OnBnClickedButton1()
{
	nImgIndex--;
	CWnd* pPreviousWnd = NULL;
	pPreviousWnd = GetDlgItem(IDC_DstImg);//获取控件句柄
	CImage cImage;
	dlgImage = cv::imread(imageFiles[nImgIndex]); // 读取第一张图像
	m_pcTools->MatToCImage(dlgImage, cImage);
	m_pcTools->ShowImg(cImage, pPreviousWnd);
	if (nImgIndex == 0)
	{
		m_btnPrevious.EnableWindow(0);
	}
}


void CImgChoose::OnBnClickedButton2()
{
	nImgIndex++;
	CWnd* pNextWnd = NULL;
	pNextWnd = GetDlgItem(IDC_DstImg);//获取控件句柄

	CImage cImage;
	dlgImage = cv::imread(imageFiles[nImgIndex]); // 读取第一张图像
	m_pcTools->MatToCImage(dlgImage, cImage);
	m_pcTools->ShowImg(cImage, pNextWnd);
	if (nImgIndex != 0) m_btnPrevious.EnableWindow(1);
	else if (nImgIndex == imageFiles.size() - 1) m_btnNext.EnableWindow(0);
}
