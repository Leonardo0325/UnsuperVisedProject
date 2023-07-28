#include "pch.h"
#include "ud_tools.h"
void CTools::ShowImg(CImage& imgLightImg, CWnd* pWnd)
{
	float fImgWid, fImgHigh, fRecWid, fRecHigh, k, t;//���ؼ��Ŀ�͸��Լ�ͼƬ��͸��йصĲ���
	//��ʾͼƬ
	CRect rect;//�ؼ����ζ���
	CRect rect1;
	//��ȡcontrol�ؼ���С
	fImgWid = imgLightImg.GetWidth();
	fImgHigh = imgLightImg.GetHeight();//��ȡͼƬ�Ŀ� ��
	k = fImgHigh / fImgWid;//���ͼƬ�Ŀ�߱�

	pWnd->GetClientRect(&rect);//��ȡPicture Control�ؼ��Ŀͻ���
	fRecWid = rect.Width();
	fRecHigh = rect.Height();//��ÿؼ��Ŀ�߱�
	t = fRecHigh / fRecWid;//��ÿؼ��Ŀ�߱�
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
	//��صļ���Ϊ����ͼƬ�ڻ�ͼ�����а�������ʾ�����ͼƬ�ܿ��ǲ��ߣ����������пհ��������ͼƬ�ܸ߶�������������пհ��������ұ������߿հ���һ����

	CDC* pDc = NULL;
	pDc = pWnd->GetDC();//��ȡpicture control��DC
	int nModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//����ָ���豸�����е�λͼ����ģʽ

	imgLightImg.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//��ʾ����
	SetStretchBltMode(pDc->m_hDC, nModeOld);

	// С���ڴ�й©������
	//ReleaseDC(pDc);//�ͷ�ָ��ռ�
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


	//�ؽ�cimage
	cImage.Destroy();
	cImage.Create(nWidth, nHeight, 8 * nChannels);


	//��������


	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cImage.GetBits();		//ָ����������ָ��
	int nStep = cImage.GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	if (1 == nChannels)								//���ڵ�ͨ����ͼ����Ҫ��ʼ����ɫ��
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
