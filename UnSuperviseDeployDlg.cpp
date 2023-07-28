
// UnSuperviseDeployDlg.cpp: 实现文件
//
	
#include<afxwin.h>
#include "pch.h"
#include "framework.h"
#include "UnSuperviseDeploy.h"
#include "UnSuperviseDeployDlg.h"
#include "ud_trtcore.h"
#include "afxdialogex.h"
#include<opencv2/opencv.hpp>
#include"ud_trtcore.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
namespace stdfs = std::experimental::filesystem;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CUnSuperviseDeployDlg 对话框



CUnSuperviseDeployDlg::CUnSuperviseDeployDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNSUPERVISEDEPLOY_DIALOG, pParent)
{
	//ModifyStyle(WS_MAXIMIZEBOX, 0);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUnSuperviseDeployDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUnSuperviseDeployDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_OpenImg, &CUnSuperviseDeployDlg::OnBnClickedOpenimg)
	//ON_BN_CLICKED(IDC_Init, &CUnSuperviseDeployDlg::OnBnClickedInit)
	//ON_BN_CLICKED(IDC_Deploy, &CUnSuperviseDeployDlg::OnBnClickedDeploy)

	//ON_BN_CLICKED(IDC_OpenDic, &CUnSuperviseDeployDlg::OnBnClickedOpendic)
	ON_EN_CHANGE(IDC_EDITDict, &CUnSuperviseDeployDlg::OnEnChangeEditdict)
	//ON_BN_CLICKED(IDC_ALLDeploy, &CUnSuperviseDeployDlg::OnBnClickedAlldeploy)
	//ON_BN_CLICKED(IDC_FirstImg, &CUnSuperviseDeployDlg::OnBnClickedFirstimg)
	ON_BN_CLICKED(IDC_OpencvRADIO, &CUnSuperviseDeployDlg::OnBnClickedOpencvradio)
	ON_BN_CLICKED(IDC_LabelRADIO, &CUnSuperviseDeployDlg::OnBnClickedLabelradio)
	ON_BN_CLICKED(IDC_SAVEPREPROCESS, &CUnSuperviseDeployDlg::OnBnClickedSavepreprocess)
	ON_EN_CHANGE(IDC_PROPRECESSEDIT, &CUnSuperviseDeployDlg::OnEnChangeProprecessedit)
	//ON_BN_CLICKED(IDC_PREPROCESS, &CUnSuperviseDeployDlg::OnBnClickedPreprocess)
	ON_EN_CHANGE(IDC_THRESHEDIT, &CUnSuperviseDeployDlg::OnEnChangeThreshedit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERTHRESH, &CUnSuperviseDeployDlg::OnNMCustomdrawSliderthresh)
	ON_EN_CHANGE(IDC_EDITALLNUMS, &CUnSuperviseDeployDlg::OnEnChangeEditallnums)
	ON_EN_CHANGE(IDC_EDITWUNUMS, &CUnSuperviseDeployDlg::OnEnChangeEditwunums)
	ON_EN_CHANGE(IDC_EDITLOUNUMS, &CUnSuperviseDeployDlg::OnEnChangeEditlounums)
	ON_BN_CLICKED(IDC_PREPROCESS2, &CUnSuperviseDeployDlg::OnBnClickedPreprocess2)
	ON_EN_CHANGE(IDC_PROPRECESSEDIT2, &CUnSuperviseDeployDlg::OnEnChangeProprecessedit2)
	ON_WM_SIZE()

	ON_BN_CLICKED(IDC_ModelChoose, &CUnSuperviseDeployDlg::OnBnClickedModelchoose)
	ON_EN_CHANGE(IDC_EDIT1, &CUnSuperviseDeployDlg::OnEnChangeEdit1)
	//ON_BN_CLICKED(IDC_ChooseImg, &CUnSuperviseDeployDlg::OnBnClickedChooseimg)
	//ON_BN_CLICKED(IDC_BUTTONTRAIN, &CUnSuperviseDeployDlg::OnBnClickedButtontrain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CUnSuperviseDeployDlg::OnDeltaposSpin1)
	ON_STN_CLICKED(IDC_SrcImg, &CUnSuperviseDeployDlg::OnStnClickedSrcimg)
	ON_STN_CLICKED(IDC_STATICICON, &CUnSuperviseDeployDlg::OnStnClickedStaticicon)
	ON_EN_CHANGE(IDC_EDITLOGURU, &CUnSuperviseDeployDlg::OnEnChangeEditloguru)
	ON_BN_CLICKED(IDC_RADIO1, &CUnSuperviseDeployDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CUnSuperviseDeployDlg::OnBnClickedRadio2)
	//ON_MESSAGE(WM_UPDATE_LOG, &CUnSuperviseDeployDlg::OnBnClickedInit)// 将消息与消息处理函数关联起来
	//ON_BN_CLICKED(IDC_BUTTON1, &CUnSuperviseDeployDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CUnSuperviseDeployDlg::OnBnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_FirstImgMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedFirstimgmfcbutton)
	ON_BN_CLICKED(IDC_PREPROCESSMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedPreprocessmfcbutton)
	ON_BN_CLICKED(IDC_ChooseImgMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedChooseimgmfcbutton)
	ON_BN_CLICKED(IDC_TRAINMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedTrainmfcbutton)
	ON_BN_CLICKED(IDC_OpenImgMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedOpenimgmfcbutton)
	ON_BN_CLICKED(IDC_InitMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedInitmfcbutton)
	ON_BN_CLICKED(IDC_DeployMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedDeploymfcbutton)
	ON_BN_CLICKED(IDC_OpenDicMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedOpendicmfcbutton)
	ON_BN_CLICKED(IDC_ALLDeployMFCBUTTON, &CUnSuperviseDeployDlg::OnBnClickedAlldeploymfcbutton)
END_MESSAGE_MAP()


// CUnSuperviseDeployDlg 消息处理程序

BOOL CUnSuperviseDeployDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 设置窗口初始状态为普通
		// 修改窗口样式，禁止最大化

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetWindowText("InsightVision");
	//ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);
	g_CUnSuperviseDeployDl = this;
	// TODO: 在此添加额外的初始化代码
	cThreshSlider.SubclassDlgItem(IDC_SLIDERTHRESH, this);
	cThreshSlider.SetRange(0, 255);  // 设置Slider的范围
	cThreshEdit.SubclassDlgItem(IDC_THRESHEDIT, this);
	cThreshSlider.SetBuddy(&cThreshEdit);
	int nThreshValue = 60;
	cWuEdit.SubclassDlgItem(IDC_EDITWUNUMS, this);
	cLouEdit.SubclassDlgItem(IDC_EDITLOUNUMS, this);
	cRightEdit.SubclassDlgItem(IDC_EDITALLNUMS, this);
	m_editCtrl.SubclassDlgItem(IDC_PROPRECESSEDIT, this);
	m_editCtrl.SetWindowText(strText);
	m_editLoguru.SubclassDlgItem(IDC_EDITLOGURU, this);
	//CRect rect;
	//GetClientRect(&rect);     //取客户区大小    
	//old.x = rect.right - rect.left;  //Point old,用于记录窗口原始坐标
	//old.y = rect.bottom - rect.top;
		// 修改背景颜色
	CRect rect;
	CPaintDC dc(this);
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255)); //背景色就在这里~
	LoguruText = "--------------------------------------------------------";
	nLogIndex = 1;
	//CString filePath = _T("../luguru.txt");
	//CStdioFile file;
	//if (file.Open(filePath, CFile::modeRead))
	//{
	//	CString line;
	//	CString text;
	//	while (file.ReadString(line))
	//	{
	//		text += line + _Tr\n");
	//	}
	//	file.Close();

	//	// 将文本内容显示在编辑框中
	//	SetDlgItemText(IDC_EDIT_TEXT);
	//}
	HRESULT ret = m_LoadImg.Load("../Img/ICON.png"); //需要转换符'\'  从指定路径获取图片m_img
	if (FAILED(ret))  //判断获取图片是否成功，成功继续，不成功弹出提示框
	{
		AfxMessageBox(_T(" img Load  failed!"));
		return FALSE;
	}
	f.Open("TRT_Log.txt", CFile::modeReadWrite);
	f.Read(FileName.GetBufferSetLength(f.GetLength()), f.GetLength());
	f.Close();
	//GetDlgItem(IDC_EDITLOGURU)->SetWindowText(FileName);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUnSuperviseDeployDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUnSuperviseDeployDlg::OnPaint()
{
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATICICON);  //绑定Picture Control控件，用pWnd指针进行操作
	CDC* pc = pWnd->GetDC();  //获取指定窗口pWnd(Picture Control控件)的上下文环境，后面使用pc在该上下文环境中绘图
	CRect rctWindowSize;  //矩形类对象 rctWindowSize
	pWnd->GetClientRect(&rctWindowSize);  //获取Picture Control控件当前的客户区域，通常的窗口从上到下依次是标题栏，菜单栏，工具栏，客户区，最底下的是状态栏，客户区就是用来和应用程序交互的地方。
	m_LoadImg.StretchBlt(pc->m_hDC, 0, 0, rctWindowSize.Width(), rctWindowSize.Height(), SRCCOPY);//StretchBlt:将位图从源矩阵拷贝到目标矩阵，将该位图进行拉伸或压缩以适应目标矩阵的大小。 这里源目标指的是m_img,目标指的是pc->m_hDC，就是Picture Control控件。
	//用StretchBlt函数需要指出源目标m_img,目标矩阵的操作句柄，目标矩阵位置，目标矩形的宽和高。
	//pc->m_hDC：目标环境句柄， o:目标位置左上角x坐标，0：:目标位置左上角y坐标；rctWindowSize.Width()：目标矩形的宽度；rctWindowSize.Height()：目标矩形的高度；SRCCOPY：操作方式为：Copies the source bitmap to the destination bitmap. 
			//改背景颜色就是下面 这几部步：

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CDialogEx::OnPaint();

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUnSuperviseDeployDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//打开图片
void CUnSuperviseDeployDlg::OpenPic()
{
	CImage imgLightImg; //光条图片
	CFileDialog FileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*|"), this);
	FileDlg.DoModal();
	m_pstrFilePath = FileDlg.GetPathName();		//文件路径
	m_pstrFileName = FileDlg.GetFileName();	//文件名
	m_sPicPath = FileDlg.GetPathName();
	if (m_pstrFilePath == _T(""))
	{
		return;
	}

	imgLightImg.Load(m_pstrFilePath);
	float fImgWid, fImgHigh, fRecWid, fRecHigh, k, t;//跟控件的宽和高以及图片宽和高有关的参数
	//显示图片
	CRect rect;//控件矩形对象
	CRect rect1;
	//获取control控件大小
	fImgWid = imgLightImg.GetWidth();
	fImgHigh = imgLightImg.GetHeight();//获取图片的宽 高
	k = fImgHigh / fImgWid;//获得图片的宽高比

	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_SrcImg);//获取控件句柄
	pWnd->GetClientRect(&rect);//获取Picture Control控件的客户区
	fRecWid = rect.Width();
	fRecHigh = rect.Height();//获得控件的宽高比
	t = fRecHigh / fRecWid;//获得控件的宽高比
	if (k >= t)
	{

		rect.right = floor(rect.bottom / k);
		rect.left = (fRecWid - rect.right) / 2;
		rect.right = floor(rect.bottom / k) + (fRecWid - rect.right) / 2;
	}
	else
	{
		rect.bottom = floor(k * rect.right);
		rect.top = (fRecHigh - rect.bottom) / 2;
		rect.bottom = floor(k * rect.right) + (fRecHigh - rect.bottom) / 2;
	}
	//相关的计算为了让图片在绘图区居中按比例显示，如果图片很宽但是不高，就上下留有空白区；如果图片很高而不宽就左右留有空白区，并且保持两边空白区一样大

	CDC* pDc = NULL;
	pDc = pWnd->GetDC();//获取picture control的DC
	int nModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置指定设备环境中的位图拉伸模式

	GetDlgItem(IDC_SrcImg)->ShowWindow(FALSE);
	GetDlgItem(IDC_SrcImg)->ShowWindow(TRUE);
	imgLightImg.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//显示函数
	SetStretchBltMode(pDc->m_hDC, nModeOld);
	ReleaseDC(pDc);//释放指针空间


	
}

void CUnSuperviseDeployDlg::OnBnClickedOpenimg()
{
	//打开图片
	ShowString("打开单张图片");
	OpenPic();
}
void CUnSuperviseDeployDlg::MatToCImage(cv::Mat& mMatImg, CImage& cImage)
{
	if (0 == mMatImg.total())
	{
		return;
	}


	int nChannels = mMatImg.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = mMatImg.cols;
	int nHeight = mMatImg.rows;


	//重建cimage
	cImage.Destroy();
	cImage.Create(nWidth, nHeight, 8 * nChannels);


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cImage.GetBits();		//指向数据区的指针
	int nStep = cImage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	if (1 == nChannels)								//对于单通道的图像需要初始化调色板
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cImage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cImage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mMatImg.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}
void CUnSuperviseDeployDlg::ShowPic(CImage&imgLightImg)
{
	float fImgWid, fImgHigh, fRecWid, fRecHigh, k, t;//跟控件的宽和高以及图片宽和高有关的参数
	//显示图片
	CRect rect;//控件矩形对象
	CRect rect1;
	//获取control控件大小
	fImgWid = imgLightImg.GetWidth();
	fImgHigh = imgLightImg.GetHeight();//获取图片的宽 高
	k = fImgHigh / fImgWid;//获得图片的宽高比

	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_DstImg);//获取控件句柄
	pWnd->GetClientRect(&rect);//获取Picture Control控件的客户区
	fRecWid = rect.Width();
	fRecHigh = rect.Height();//获得控件的宽高比
	t = fRecHigh / fRecWid;//获得控件的宽高比
	if (k >= t)
	{

		rect.right = floor(rect.bottom / k);
		rect.left = (fRecWid - rect.right) / 2;
		rect.right = floor(rect.bottom / k) + (fRecWid - rect.right) / 2;
	}
	else
	{
		rect.bottom = floor(k * rect.right);
		rect.top = (fRecHigh - rect.bottom) / 2;
		rect.bottom = floor(k * rect.right) + (fRecHigh - rect.bottom) / 2;
	}
	//相关的计算为了让图片在绘图区居中按比例显示，如果图片很宽但是不高，就上下留有空白区；如果图片很高而不宽就左右留有空白区，并且保持两边空白区一样大

	CDC* pDc = NULL;
	pDc = pWnd->GetDC();//获取picture control的DC
	int nModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置指定设备环境中的位图拉伸模式

	GetDlgItem(IDC_DstImg)->ShowWindow(FALSE);
	GetDlgItem(IDC_DstImg)->ShowWindow(TRUE);
	imgLightImg.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//显示函数
	SetStretchBltMode(pDc->m_hDC, nModeOld);
	ReleaseDC(pDc);//释放指针空间
}
void CUnSuperviseDeployDlg::ShowImg(CImage&imgLightImg,CWnd* pWnd)
{
	float fImgWid, fImgHigh, fRecWid, fRecHigh, k, t;//跟控件的宽和高以及图片宽和高有关的参数
	//显示图片
	CRect rect;//控件矩形对象
	CRect rect1;
	//获取control控件大小
	fImgWid = imgLightImg.GetWidth();
	fImgHigh = imgLightImg.GetHeight();//获取图片的宽 高
	k = fImgHigh / fImgWid;//获得图片的宽高比

	pWnd->GetClientRect(&rect);//获取Picture Control控件的客户区
	fRecWid = rect.Width();
	fRecHigh = rect.Height();//获得控件的宽高比
	t = fRecHigh / fRecWid;//获得控件的宽高比
	if (k >= t)
	{

		rect.right = floor(rect.bottom / k);
		rect.left = (fRecWid - rect.right) / 2;
		rect.right = floor(rect.bottom / k) + (fRecWid - rect.right) / 2;
	}
	else
	{
		rect.bottom = floor(k * rect.right);
		rect.top = (fRecHigh - rect.bottom) / 2;
		rect.bottom = floor(k * rect.right) + (fRecHigh - rect.bottom) / 2;
	}
	//相关的计算为了让图片在绘图区居中按比例显示，如果图片很宽但是不高，就上下留有空白区；如果图片很高而不宽就左右留有空白区，并且保持两边空白区一样大

	CDC* pDc = NULL;
	pDc = pWnd->GetDC();//获取picture control的DC
	int nModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置指定设备环境中的位图拉伸模式

	imgLightImg.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//显示函数
	SetStretchBltMode(pDc->m_hDC, nModeOld);
	ReleaseDC(pDc);//释放指针空间
	imgLightImg.Destroy();
}
void CUnSuperviseDeployDlg::ShowThresh(CImage&imgLightImg)
{
	float fImgWid, fImgHigh, fRecWid, fRecHigh, k, t;//跟控件的宽和高以及图片宽和高有关的参数
	//显示图片
	CRect rect;//控件矩形对象
	CRect rect1;
	//获取control控件大小
	fImgWid = imgLightImg.GetWidth();
	fImgHigh = imgLightImg.GetHeight();//获取图片的宽 高
	k = fImgHigh / fImgWid;//获得图片的宽高比

	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_Thre);//获取控件句柄
	pWnd->GetClientRect(&rect);//获取Picture Control控件的客户区
	fRecWid = rect.Width();
	fRecHigh = rect.Height();//获得控件的宽高比
	t = fRecHigh / fRecWid;//获得控件的宽高比
	if (k >= t)
	{

		rect.right = floor(rect.bottom / k);
		rect.left = (fRecWid - rect.right) / 2;
		rect.right = floor(rect.bottom / k) + (fRecWid - rect.right) / 2;
	}
	else
	{
		rect.bottom = floor(k * rect.right);
		rect.top = (fRecHigh - rect.bottom) / 2;
		rect.bottom = floor(k * rect.right) + (fRecHigh - rect.bottom) / 2;
	}
	//相关的计算为了让图片在绘图区居中按比例显示，如果图片很宽但是不高，就上下留有空白区；如果图片很高而不宽就左右留有空白区，并且保持两边空白区一样大

	CDC* pDc = NULL;
	pDc = pWnd->GetDC();//获取picture control的DC
	int nModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置指定设备环境中的位图拉伸模式

	GetDlgItem(IDC_Thre)->ShowWindow(FALSE);
	GetDlgItem(IDC_Thre)->ShowWindow(TRUE);
	imgLightImg.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//显示函数
	SetStretchBltMode(pDc->m_hDC, nModeOld);
	ReleaseDC(pDc);//释放指针空间
}
void CUnSuperviseDeployDlg::ShowAdd(CImage&imgLightImg)
{
	float fImgWid, fImgHigh, fRecWid, fRecHigh, k, t;//跟控件的宽和高以及图片宽和高有关的参数
	//显示图片
	CRect rect;//控件矩形对象
	CRect rect1;
	//获取control控件大小
	fImgWid = imgLightImg.GetWidth();
	fImgHigh = imgLightImg.GetHeight();//获取图片的宽 高
	k = fImgHigh / fImgWid;//获得图片的宽高比

	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_Add);//获取控件句柄
	pWnd->GetClientRect(&rect);//获取Picture Control控件的客户区
	fRecWid = rect.Width();
	fRecHigh = rect.Height();//获得控件的宽高比
	t = fRecHigh / fRecWid;//获得控件的宽高比
	if (k >= t)
	{

		rect.right = floor(rect.bottom / k);
		rect.left = (fRecWid - rect.right) / 2;
		rect.right = floor(rect.bottom / k) + (fRecWid - rect.right) / 2;
	}
	else
	{
		rect.bottom = floor(k * rect.right);
		rect.top = (fRecHigh - rect.bottom) / 2;
		rect.bottom = floor(k * rect.right) + (fRecHigh - rect.bottom) / 2;
	}
	//相关的计算为了让图片在绘图区居中按比例显示，如果图片很宽但是不高，就上下留有空白区；如果图片很高而不宽就左右留有空白区，并且保持两边空白区一样大

	CDC* pDc = NULL;
	pDc = pWnd->GetDC();//获取picture control的DC
	int nModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置指定设备环境中的位图拉伸模式

	GetDlgItem(IDC_Add)->ShowWindow(FALSE);
	GetDlgItem(IDC_Add)->ShowWindow(TRUE);
	imgLightImg.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//显示函数
	SetStretchBltMode(pDc->m_hDC, nModeOld);
	ReleaseDC(pDc);//释放指针空间
}

UNIT _cdecl CUnSuperviseDeployDlg::dlgDeployThread(LPVOID lparam)
{
	CUnSuperviseDeployDlg* p = (CUnSuperviseDeployDlg*)lparam;
	int num = p->dlgModelChoose();
	p->m_pcTrtDeploy->Init(p->ModelPth);
	if (num == PD_OK) {
		p->ShowString("模型初始化成功！");
		p->MessageBox(_T("模型初始化成功！"), NULL, MB_OK);
	}

	return 0;
}


int CUnSuperviseDeployDlg::dlgModelChoose()
{
	pModelPadimRadio = (CButton*)GetDlgItem(IDC_RADIO1);
	pModelEffADRadio = (CButton*)GetDlgItem(IDC_RADIO2);
	if (pModelPadimRadio->GetCheck() == BST_CHECKED)
	{
		ModelPth = "..\\model\\LED-PaDim-SIM_384";
		return PD_OK;
	}
	else if (pModelEffADRadio->GetCheck() == BST_CHECKED)
	{
		ModelPth = "..\\model\\EffAD-SIM";
		return PD_OK;
	}
	else if (pModelPadimRadio->GetCheck() != BST_CHECKED && pModelEffADRadio->GetCheck() != BST_CHECKED)
	{
		MessageBox(_T("请选择模型!"), NULL, MB_ICONERROR);
		return PD_WRONG_FILE;
	}
}	

void CUnSuperviseDeployDlg::OnBnClickedInit()

{
	ShowString("模型初始化");
	CWinThread* p = AfxBeginThread(dlgDeployThread, this);
	/*dlgModelChoose();
	m_pcTrtDeploy->Init(ModelPth);
	MessageBox(_T("模型初始化成功！"), NULL, MB_OK);*/
}


void CUnSuperviseDeployDlg::OnBnClickedDeploy()
{
	ShowString("模型推理");
	//dlgModelChoose();
	ImgResults = m_pcTrtDeploy->run(m_sPicPath,nThreshValue,ModelPth);
	ShowString("模型推理成功！");
	cv::Mat img, imgThresh, imgAdd;
	img = ImgResults.Img;
	imgAdd = ImgResults.FinalImg;
	imgThresh = ImgResults.ThreshImg;

	CImage cDstImg,cAddImg, cImgThresh;
	MatToCImage(img, cDstImg);
	MatToCImage(imgAdd, cAddImg);
	MatToCImage(imgThresh, cImgThresh);
	CWnd* pDstWnd = NULL;
	pDstWnd = GetDlgItem(IDC_DstImg);//获取控件句柄
	CWnd* pAddWnd = NULL;
	pAddWnd = GetDlgItem(IDC_Add);//获取控件句柄
	CWnd* pThreshWnd = NULL;
	pThreshWnd = GetDlgItem(IDC_Thre);//获取控件句柄
	ShowImg(cDstImg, pDstWnd);
	ShowImg(cAddImg, pAddWnd);
	ShowImg(cImgThresh, pThreshWnd);


}

CString CUnSuperviseDeployDlg::UD_OpenFile()
{
	TCHAR            szFolderPath[255] = { 0 };
	CString          strFolderPath;
	BROWSEINFO       sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = "请选择缓存文件所在文件夹";
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	sInfo.lpfn = NULL;

	LPITEMIDLIST  lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		if (::SHGetPathFromIDListA(lpidlBrowse, szFolderPath))
		{
			strFolderPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
	return strFolderPath;
}
void CUnSuperviseDeployDlg::OnBnClickedOpendic()
{
	ShowString("打开文件夹");
	m_sPathName = UD_OpenFile();
	SetDlgItemText(IDC_EDITDict, m_sPathName); //Edit控件IDC_EDIT1
}


void CUnSuperviseDeployDlg::OnEnChangeEditdict()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//获取文件名
std::string getFileName(const std::string& filePath) {
	size_t lastSlashIndex = filePath.find_last_of("/\\");
	if (lastSlashIndex != std::string::npos) {
		return filePath.substr(lastSlashIndex + 1);
	}
	return filePath;
}

UNIT _cdecl CUnSuperviseDeployDlg::dlgALLDeployThread(LPVOID lparam)
{
	CUnSuperviseDeployDlg* p = (CUnSuperviseDeployDlg*)lparam;
	p->dlgAllDeployFunc();
	return 0;
}
void CUnSuperviseDeployDlg::dlgAllDeployFunc()
{
	std::string sSrcPth((LPCTSTR)m_sPathName);
	//cv::String sPthNames = sSrcPth + "/*.jpg";
	vector<cv::String> vResult;
	//string sFilePath = CT2A(sPthNames.GetBuffer());
	for (const auto& i : stdfs::directory_iterator(stdfs::path(sSrcPth))) {
		vResult.push_back(i.path().generic_string());
	}
	//cv::glob(sSrcPth, vResult,false); //从文件夹中读取图片路径名
	int nWuNums = 0;
	int nLouNums = 0;
	int nRightNums = 0;
	string sFoldName = getFileName(sSrcPth);
	CString strLowValue, strWuValue;
	for (size_t k = 0; k < vResult.size(); k++)     //读取扫描物体的所有结构光图片
	{
		cv::Mat SrcImg = cv::imread(vResult[k]);
		cv::Mat SrcClone = SrcImg.clone();
		ImgResults = m_pcTrtDeploy->run(vResult[k], nThreshValue, ModelPth);
		cv::Mat img, imgThresh, imgAdd, ImgSingle, imgMask;
		img = ImgResults.Img;
		imgAdd = ImgResults.FinalImg;
		imgThresh = ImgResults.ThreshImg;
		ImgSingle = ImgResults.Img1;
		CImage cSrcImg, cDstImg, cAddImg, cImgThresh;
		MatToCImage(SrcImg, cSrcImg);
		MatToCImage(img, cDstImg);
		MatToCImage(imgAdd, cAddImg);
		MatToCImage(imgThresh, cImgThresh);
		CWnd* pSrcWnd = NULL;
		pSrcWnd = GetDlgItem(IDC_SrcImg);//获取控件句柄
		CWnd* pDstWnd = NULL;
		pDstWnd = GetDlgItem(IDC_DstImg);//获取控件句柄
		CWnd* pAddWnd = NULL;
		pAddWnd = GetDlgItem(IDC_Add);//获取控件句柄
		CWnd* pThreshWnd = NULL;
		pThreshWnd = GetDlgItem(IDC_Thre);//获取控件句柄
		ShowImg(cSrcImg, pSrcWnd);
		ShowImg(cDstImg, pDstWnd);
		ShowImg(cAddImg, pAddWnd);
		ShowImg(cImgThresh, pThreshWnd);
		if (nThreshValue == 0) nThreshValue = 90;
		int value = nThreshValue;
		cv::threshold(ImgSingle, imgMask, nThreshValue, 255, cv::THRESH_BINARY);

		//std::vector<std::vector<cv::Point>> contours;
		//cv::findContours(imgMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		// 创建一个空白图像作为结果
		//cv::Mat resultImage = cv::Mat::zeros(ImgSingle.size(), CV_8UC3);
		// 将轮廓绘制到结果图像上
		//cv::drawContours(resultImage, contours, -1, cv::Scalar(255, 255, 0));
		// 将结果图像与原始图像相加
		//cv::add(imgAdd, resultImage, resultImage);

		double minValue, maxValue;
		cv::Point minLocation, maxLocation;
		cv::Mat mJugeThresh = ImgResults.Img1;
		// 获取矩阵的最小值和最大值以及它们的位置
		cv::minMaxLoc(mJugeThresh, &minValue, &maxValue, &minLocation, &maxLocation);
		if (sFoldName == "bad" && maxValue >= nThreshValue)
		{
			nRightNums++;
			std::string fileName = getFileName(vResult[k]);
			cv::imwrite("../result/bad/" + fileName, imgAdd);
			strValue.Format(_T("%d"), nRightNums);
			cRightEdit.SetWindowText(strValue);
		}
		else if (sFoldName == "bad" && maxValue < nThreshValue)
		{
			nLouNums++;
			std::string fileName = getFileName(vResult[k]);
			cv::imwrite("../result/LouJian/" + fileName, imgAdd);
			
			strValue.Format(_T("%d"), nLouNums);
			cLouEdit.SetWindowText(strLowValue);
		}

		else if (sFoldName == "good" && maxValue <= nThreshValue)
		{
			nRightNums++;
			std::string fileName = getFileName(vResult[k]);
			cv::imwrite("../result/good/" + fileName, imgAdd);
			strValue.Format(_T("%d"), nRightNums);
			cRightEdit.SetWindowText(strValue);
		}
		else if (sFoldName == "good" && maxValue > nThreshValue)
		{
			nWuNums++;
			std::string fileName = getFileName(vResult[k]);
			cv::imwrite("../result/WuJian/" + fileName, imgAdd);

			strWuValue.Format(_T("%d"), nWuNums);
			cWuEdit.SetWindowText(strWuValue);
		}

		//imgMask.release();
		//resultImage.release();
		//imgAdd.release();
		//contours.clear();


	}

	/*if (sFoldName == "bad")
	{
		nLouNums = vResult.size() - nRightNums;
		CString strValue;
		strValue.Format(_T("%d"), nLouNums);
		cLouEdit.SetWindowText(strValue);
		strValue.Format(_T("%d"), nRightNums);
		cRightEdit.SetWindowText(strValue);
		strValue.Format(_T("%d"), nWuNums);
		cWuEdit.SetWindowText(strValue);
	}
	else if (sFoldName == "good")
	{
		nWuNums = vResult.size() - nRightNums;
		CString strValue;
		strValue.Format(_T("%d"), nLouNums);
		cLouEdit.SetWindowText(strValue);
		strValue.Format(_T("%d"), nRightNums);
		cRightEdit.SetWindowText(strValue);
		strValue.Format(_T("%d"), nWuNums);
		cWuEdit.SetWindowText(strValue);
	}*/
}

void CUnSuperviseDeployDlg::OnBnClickedAlldeploy()
{
	ShowString("批量推理");
	UNIT uDeploy;
	(void*)_beginthreadex(NULL, 0, dlgALLDeployThread, this, 0, &uDeploy);
	}
	

void CUnSuperviseDeployDlg::OnBnClickedFirstimg()
{
	ShowString("打开图像");
	CButton * pOpencvRadio = (CButton*)GetDlgItem(IDC_OpencvRADIO);
	CButton * pLabelRadio = (CButton*)GetDlgItem(IDC_LabelRADIO);
	if (pOpencvRadio->GetCheck() == BST_CHECKED)
	{
		CFileDialog FileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*|"), this);
		FileDlg.DoModal();
		m_pstrFilePath = FileDlg.GetPathName();		//文件路径
		m_pstrFileName = FileDlg.GetFileName();	//文件名
		m_sPicPath = FileDlg.GetPathName();
		if (m_pstrFilePath == _T(""))
		{
			return;
		}
		std::string strFilePath(m_pstrFilePath);
		cv::Mat SrcImg = cv::imread(strFilePath);
		cv::namedWindow("SrcImage", cv::WINDOW_NORMAL);
		cv::imshow("SrcImage", SrcImg);
		cv::waitKey(0);
	}
	else if (pLabelRadio->GetCheck() == BST_CHECKED)
	{
		OpenPic();
	}
}
void CUnSuperviseDeployDlg::ShowMultilineText(CEdit& edit, CString& strInfo)
{
	int nLen = edit.GetWindowTextLength();	// 获取此前已显示的文本长度
	edit.SetSel(nLen, nLen); // SetSel到文本末尾
	edit.ReplaceSel(strInfo+ _T("\r\n")); // 显示要增加显示的文本
}
void CUnSuperviseDeployDlg::ShowString(CString name)
{
	// 获取当前时间
	std::time_t currentTime = std::time(nullptr);
	// 将时间转换本地时间
	std::tm* localTime = std::localtime(&currentTime);
	// 格式化时间为字符串
	char timeString[100];
	std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);
	m_cLogIndex.Format(_T("%s"), timeString);
	m_cLogtent = m_cLogIndex + LoguruText + name + LoguruText;
	ShowMultilineText(m_editLoguru, m_cLogtent);
	nLogIndex++;
}

void CUnSuperviseDeployDlg::OnBnClickedOpencvradio()
{
	ShowString("使用OpenCV打开图像");
}


void CUnSuperviseDeployDlg::OnBnClickedLabelradio()
{
	ShowString("使用MFC打开图像");
}


void CUnSuperviseDeployDlg::OnBnClickedSavepreprocess()
{
	
	BROWSEINFO browseInfo = { 0 };
	browseInfo.lpszTitle = _T("选择保存图像的文件夹");
	LPITEMIDLIST itemIdList = SHBrowseForFolder(&browseInfo);
	if (itemIdList != NULL)
	{
		// 获取文件夹路径
		TCHAR buffer[MAX_PATH];
		if (SHGetPathFromIDList(itemIdList, buffer))
		{
			cPreFolder = buffer;
		}
		// 释放选择的项的内存
		IMalloc* imalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(itemIdList);
			imalloc->Release();
		}
	}
	SetDlgItemText(IDC_PROPRECESSEDIT, cPreFolder);

}


void CUnSuperviseDeployDlg::OnEnChangeProprecessedit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnBnClickedPreprocess()
{
	ShowString("图像预处理");
	m_pcPreProcess->preAction(m_pstrFilePath, strText);
	
}


void CUnSuperviseDeployDlg::OnEnChangeThreshedit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnNMCustomdrawSliderthresh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	nThreshValue = cThreshSlider.GetPos();
	strValue.Format(_T("%d"), nThreshValue);
	cThreshEdit.SetWindowText(strValue);
	cv::Mat img  = ImgResults.Img1;
	cv::Mat mThreshMask;
	cv::threshold(img, mThreshMask, nThreshValue, 255, cv::THRESH_BINARY);
	CWnd* pThreshWnd = NULL;
	pThreshWnd = GetDlgItem(IDC_Thre);//获取控件句柄
	CImage cImgThresh;
	MatToCImage(mThreshMask, cImgThresh);
	if(nThreshValue!=0)
	//这里有个BUG
		ShowImg(cImgThresh, pThreshWnd);
	//delete pThreshWnd;
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

}


void CUnSuperviseDeployDlg::OnEnChangeEditallnums()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnEnChangeEditwunums()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnEnChangeEditlounums()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);


	m_scale.Scale(cx, cy);
	//if (nType != SIZE_MINIMIZED)
	//	ReSize();


}
void CUnSuperviseDeployDlg::ReSize()
{
	//// TODO: 在此处添加实现代码.
	//float FirstPos[2];
	//POINT Newp; //获取现在对话框的大小  
	//CRect recta;
	//GetClientRect(&recta);     //取客户区大小    
	//Newp.x = recta.right - recta.left;
	//Newp.y = recta.bottom - recta.top;
	//FirstPos[0] = (float)Newp.x / old.x;
	//FirstPos[1] = (float)Newp.y / old.y;
	//CRect Rect;
	//int woc;
	//CPoint OldTLPoint, TLPoint; //左上角  
	//CPoint OldBRPoint, BRPoint; //右下角  
	//HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	//while (hwndChild)
	//{
	//	woc = ::GetDlgCtrlID(hwndChild);//取得ID  
	//	GetDlgItem(woc)->GetWindowRect(Rect);
	//	ScreenToClient(Rect);w
	//	OldTLPoint = Rect.TopLeft();
	//	TLPoint.x = long(OldTLPoint.x*FirstPos[0]);
	//	TLPoint.y = long(OldTLPoint.y*FirstPos[1]);
	//	OldBRPoint = Rect.BottomRight();
	//	BRPoint.x = long(OldBRPoint.x *FirstPos[0]);
	//	BRPoint.y = long(OldBRPoint.y *FirstPos[1]);
	//	Rect.SetRect(TLPoint, BRPoint);
	//	GetDlgItem(woc)->MoveWindow(Rect, TRUE);
	//	hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	//}
	//old = Newp;



}

void CUnSuperviseDeployDlg::OnBnClickedPreprocess2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnEnChangeProprecessedit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnBnClickedModelchoose()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnBnClickedChooseimg()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowString("挑拣图像");
	m_ImgChooseDlg->DoModal();
}

UNIT _cdecl CUnSuperviseDeployDlg::dlgTrainThread(LPVOID lparam)
{
	CUnSuperviseDeployDlg* p = (CUnSuperviseDeployDlg*)lparam;
	system("..\\dist\\efficientad_2.exe");
	return 0;
}
void CUnSuperviseDeployDlg::OnBnClickedButtontrain()
{
	ShowString("开始训练");
	CWinThread* p = AfxBeginThread(dlgTrainThread, this);
}


CCtrlScale::CCtrlScale(void) :m_pParentWnd(NULL)
{
}

CCtrlScale::CCtrlScale(CWnd* pParentWnd) : m_pParentWnd(pParentWnd)
{
	Init(pParentWnd);
}

CCtrlScale::~CCtrlScale(void)
{

	m_vecCtrl.clear();
}

void CCtrlScale::Init(CWnd* pParentWnd)
{
	if (pParentWnd) {
		m_pParentWnd = pParentWnd;
		m_pParentWnd->GetClientRect(&m_rect); //获取对话框的大小
	}
}

void CCtrlScale::Scale(int cx, int cy)
{
	if (!m_pParentWnd) {
		return;
	}

	//判定是否为最小状态，是的话就不进行重绘
	if (cx == 0 || cy == 0) {
		return;
	}

	//获取控件得到大小以及原大小方便进行比例调整
	CWnd* pWnd;
	int nID = 0;
	//列出所有控件
	HWND hChild = ::GetWindow(m_pParentWnd->m_hWnd, GW_CHILD);
	while (hChild) {
		nID = ::GetDlgCtrlID(hChild);//获得控件ID
		pWnd = m_pParentWnd->GetDlgItem(nID);//获取控件指针
		if (pWnd) {
			CRect rect;//获取当前控件的大小
			pWnd->GetWindowRect(&rect);
			m_pParentWnd->ScreenToClient(&rect);//将控件大小转换为在窗口的区域坐标
			//防止控件失真
			m_pParentWnd->InvalidateRect(rect);

			CTRLRECT cr;
			cr.nId = nID;
			vector<CTRLRECT>::iterator it = find_if(m_vecCtrl.begin(), m_vecCtrl.end(), cr);
			if (it != m_vecCtrl.end()) {//如果保存的有该控件与窗口比值，直接乘上当前窗口大小
				rect.left = it->dScale[0] * cx;
				rect.right = it->dScale[1] * cx;
				rect.top = it->dScale[2] * cy;
				rect.bottom = it->dScale[3] * cy;
			}
			else {//没有找到控件的比值，则加入
				cr.dScale[0] = (double)rect.left / m_rect.Width();//注意类型转换，不然保存成long型就直接为0了
				cr.dScale[1] = (double)rect.right / m_rect.Width();
				cr.dScale[2] = (double)rect.top / m_rect.Height();
				cr.dScale[3] = (double)rect.bottom / m_rect.Height();
				m_vecCtrl.push_back(cr);
				// 调整控件大小
					rect.left = cr.dScale[0] * cx;
				rect.right = cr.dScale[1] * cx;
				rect.top = cr.dScale[2] * cy;
				rect.bottom = cr.dScale[3] * cy;
			}

			if (pWnd->IsKindOf(RUNTIME_CLASS(CComboBox))) {
				//解决ComboBox缩放后,无法下拉的问题
				pWnd->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height() + 200);
			}
			else {
				pWnd->MoveWindow(rect);//设置控件大小
			}
		}
		hChild = ::GetWindow(hChild, GW_HWNDNEXT);
	}
	m_pParentWnd->GetClientRect(&m_rect);//获取变化后的窗口大小
}

void CUnSuperviseDeployDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString ss;
	string str;
	str = CStringA(strValue);
	const char* str2;
	str2 = str.c_str();
	//以上是将CString转换为const char*
	size_t  len = strlen(str2) + 1;
	size_t converted = 0;
	wchar_t* WStr;
	WStr = (wchar_t*)malloc(len * sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, str2, _TRUNCATE);
	if (pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头  
	{
		double a;
		a = wcstod(WStr, NULL) + 1;
		ss.Format(_T("%.1f"), a);
	}
	else if (pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头  
	{
		double a;
		a = wcstod(WStr, NULL) - 1;
		ss.Format(_T("%.1f"), a);
	}
	
	UpdateData(false);
	CString str1(WStr);
	cThreshEdit.SetWindowText(str1);
	strValue = ss;
	ss.Format(_T("%d"), nThreshValue);
	cv::Mat img = ImgResults.Img1;
	cv::Mat mThreshMask;
	cv::threshold(img, mThreshMask, nThreshValue, 255, cv::THRESH_BINARY);
	CWnd* pThreshWnd = NULL;
	pThreshWnd = GetDlgItem(IDC_Thre);//获取控件句柄
	CImage cImgThresh;
	MatToCImage(mThreshMask, cImgThresh);
	if (nThreshValue != 0)
		//这里有个BUG
		ShowImg(cImgThresh, pThreshWnd);
	*pResult = 0;
}


void CUnSuperviseDeployDlg::OnStnClickedSrcimg()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnStnClickedStaticicon()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnEnChangeEditloguru()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CUnSuperviseDeployDlg::OnBnClickedRadio1()
{
	ShowString("选择模型MPG");
	ModelPth = "..\\model\\LED-PaDim-SIM_384";
}


void CUnSuperviseDeployDlg::OnBnClickedRadio2()
{
	ShowString("选择模型MEG");
	ModelPth = "..\\model\\EffAD-SIM";
}




void CUnSuperviseDeployDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_CameraDlg->DoModal();
}

UNIT _cdecl CUnSuperviseDeployDlg::dlgCameraThread(LPVOID lparam)
{
	CUnSuperviseDeployDlg* p = (CUnSuperviseDeployDlg*)lparam;
	p->m_CameraDlg->DoModal();
	return 0;
}
void CUnSuperviseDeployDlg::OnBnClickedMfcbutton2()
{
	ShowString("连接相机");
	CWinThread* p = AfxBeginThread(dlgCameraThread, this);
}


void CUnSuperviseDeployDlg::OnBnClickedFirstimgmfcbutton()
{
	ShowString("打开图像");
	CButton* pOpencvRadio = (CButton*)GetDlgItem(IDC_OpencvRADIO);
	CButton* pLabelRadio = (CButton*)GetDlgItem(IDC_LabelRADIO);
	if (pOpencvRadio->GetCheck() == BST_CHECKED)
	{
		CFileDialog FileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*|"), this);
		FileDlg.DoModal();
		m_pstrFilePath = FileDlg.GetPathName();		//文件路径
		m_pstrFileName = FileDlg.GetFileName();	//文件名
		m_sPicPath = FileDlg.GetPathName();
		if (m_pstrFilePath == _T(""))
		{
			return;
		}
		std::string strFilePath(m_pstrFilePath);
		cv::Mat SrcImg = cv::imread(strFilePath);
		cv::namedWindow("SrcImage", cv::WINDOW_NORMAL);
		cv::imshow("SrcImage", SrcImg);
		cv::waitKey(0);
	}
	else if (pLabelRadio->GetCheck() == BST_CHECKED)
	{
		OpenPic();
	}
}


void CUnSuperviseDeployDlg::OnBnClickedPreprocessmfcbutton()
{
	ShowString("图像预处理");
	m_pcPreProcess->preAction(m_pstrFilePath, strText);
}


void CUnSuperviseDeployDlg::OnBnClickedChooseimgmfcbutton()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowString("挑拣图像");
	m_ImgChooseDlg->DoModal();
}


void CUnSuperviseDeployDlg::OnBnClickedTrainmfcbutton()
{
	ShowString("开始训练");
	CWinThread* p = AfxBeginThread(dlgTrainThread, this);
}


void CUnSuperviseDeployDlg::OnBnClickedOpenimgmfcbutton()
{
	//打开图片
	ShowString("打开单张图片");
	OpenPic();
}


void CUnSuperviseDeployDlg::OnBnClickedInitmfcbutton()
{
	ShowString("模型初始化");
	CWinThread* p = AfxBeginThread(dlgDeployThread, this);
	/*dlgModelChoose();
	m_pcTrtDeploy->Init(ModelPth);
	MessageBox(_T("模型初始化成功！"), NULL, MB_OK);*/
}


void CUnSuperviseDeployDlg::OnBnClickedDeploymfcbutton()
{
	ShowString("模型推理");
	//dlgModelChoose();
	ImgResults = m_pcTrtDeploy->run(m_sPicPath, nThreshValue, ModelPth);
	ShowString("模型推理成功！");
	cv::Mat img, imgThresh, imgAdd;
	img = ImgResults.Img;
	imgAdd = ImgResults.FinalImg;
	imgThresh = ImgResults.ThreshImg;

	CImage cDstImg, cAddImg, cImgThresh;
	MatToCImage(img, cDstImg);
	MatToCImage(imgAdd, cAddImg);
	MatToCImage(imgThresh, cImgThresh);
	CWnd* pDstWnd = NULL;
	pDstWnd = GetDlgItem(IDC_DstImg);//获取控件句柄
	CWnd* pAddWnd = NULL;
	pAddWnd = GetDlgItem(IDC_Add);//获取控件句柄
	CWnd* pThreshWnd = NULL;
	pThreshWnd = GetDlgItem(IDC_Thre);//获取控件句柄
	ShowImg(cDstImg, pDstWnd);
	ShowImg(cAddImg, pAddWnd);
	ShowImg(cImgThresh, pThreshWnd);
}


void CUnSuperviseDeployDlg::OnBnClickedOpendicmfcbutton()
{
	ShowString("打开文件夹");
	m_sPathName = UD_OpenFile();
	SetDlgItemText(IDC_EDITDict, m_sPathName); //Edit控件IDC_EDIT1
}


void CUnSuperviseDeployDlg::OnBnClickedAlldeploymfcbutton()
{
	ShowString("批量推理");
	UNIT uDeploy;
	(void*)_beginthreadex(NULL, 0, dlgALLDeployThread, this, 0, &uDeploy);
}
