#ifndef UD_TOOLS_H
#define UD_TOOLS_H
#pragma once


#include<string>

#include <filesystem>
#include <algorithm>
#include <map>
#include <vector>
#include<opencv2/opencv.hpp>
using std::map;
using std::vector;
using namespace std;
class CTools
{
public:
	void ShowImg(CImage& imgLightImg, CWnd* pWnd);
	//void OpenPic();
	void MatToCImage(cv::Mat& mMatImg, CImage& cImage);
};

#endif