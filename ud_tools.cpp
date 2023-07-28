#include "pch.h"
#include "ud_tools.h"
void CTools::ShowImg(CImage& imgLightImg, CWnd* pWnd)
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

	// 小心内存泄漏！！！
	//ReleaseDC(pDc);//释放指针空间
	imgLightImg.Destroy();
}
void CTools::MatToCImage(cv::Mat& mMatImg, CImage& cImage)
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
