/*****************************************************************************
* @description : 处理异常值
*****************************************************************************/
#pragma once
#include<afxwin.h>
#include <cuda_runtime.h>  // cuda库
#include <cuda_runtime_api.h>
#include<algorithm>
#include<fstream>
#include "ud_basetype.h"
#include "ud_loguru.hpp" // https://github.com/emilk/loguru
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#define DIM_THIRD 248
#define DIM_DOUBLE 56
#define DIM_FIRST 56
#define PIC_SIZE 448*448
cv::Mat Normalize(cv::Mat& src, const std::vector<PD_FLOAT>& mean,
	const std::vector<PD_FLOAT>& std, bool to_rgb = false, bool inplace = true);
// \! 指定device id的GPU是否兼容
std::vector<std::vector<PD_FLOAT>>vOutputArrs;
bool IsCompatible(PD_S32 device)
{
	cudaError_t st = cudaSetDevice(device);
	if (st != cudaSuccess)
		return false;

	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, device);
	if (deviceProp.major < 3)
		return false;

	return true;
}

// \! 前处理 ：分类、分割、异常检测、目标检测的标准化、归一化处理
// \! （1）8U->32F;(2)减均值，除方差;(3)如果是3通道的，BGR->RGB;(4)转成一维数组
// \@param buffers: 缓存，tensorRT中的samples定义
// \@param cv_images: 输入图片，openCV格式
// \@param params: 参数结构体
// \@param inputName: onnx的输入名称
PD_S32 normalization(const samplesCommon::BufferManager& buffers, std::vector<cv::Mat>& cv_images, const PARAMS_S& params, std::string inputName)
{
	// 1.分配host内存
	//static_cast 只能用于良性转换，这样的转换风险较低，一般不会发生什么意外，但是不能用于不同类型之间的转化，例如PD_S32*转double *
	PD_FLOAT* hostDataBuffer = static_cast<PD_FLOAT*>(buffers.getHostBuffer(inputName));
	PD_S32 nums = cv_images.size();	// 获得图片数量-batchsize
	PD_S32 depth = cv_images[0].channels();	// 获得channels
	PD_S32 height = cv_images[0].rows;	// 获得height
	PD_S32 width = cv_images[0].cols;	// 获得width

	if (depth == 1) {// 通道数为1
		for (PD_S32 n = 0, index = 0; n < nums; n++)
		{
			cv_images[n].convertTo(cv_images[n], CV_32F, 1.0 / 255);
			cv_images[n] = (cv_images[n] - params.meanValue[0]) / params.stdValue[0];
			memcpy_s(hostDataBuffer + n * height * width, height * width * sizeof(PD_FLOAT), cv_images[n].data, height * width * sizeof(PD_FLOAT));
		}
	}
	else if (depth == 3) {// 通道数为3
		std::vector<PD_FLOAT> mean = { 0.485, 0.456, 0.406 };
		std::vector<PD_FLOAT> std = { 0.229, 0.224, 0.225 };



		for (PD_S32 n = 0, index = 0; n < nums; n++)
		{
			//convertTo()函数负责转换数据类型不同的Mat，即可以将类似PD_FLOAT型的Mat转换到imwrite()函数能够接受的类型。
			// 

			cv_images[n].convertTo(cv_images[n], CV_32FC3);


			// 
			//  1.0/255 比例因子，浮点型的1/255，表示缩放的比例
			//图像大小没有变化，但是类型又UINT8变为了FLOAT32位

			//cv_images[n].convertTo(cv_images[n], CV_32FC3, 1.0 / 255, 0);
			//cv::normalize(resized_image, resized_image, 0, 1, cv::NormTypes::NORM_MINMAX, CV_32FC3);

			// 标准化
			//cv_images[n] = Normalize(cv_images[n], mean, std);

			/*cv::imshow("asd", cv_images[n]);
			cv::waitKey(0);*/

			std::vector<cv::Mat> bgrChannels(3);
			//cv::split 分裂出三个单通道图像
			cv::split(cv_images[n], bgrChannels);
			//for (auto i = 0; i < bgrChannels.size(); i++)
			//{
			//	bgrChannels[i].convertTo(bgrChannels[i], CV_32FC1, 1.0 / params.stdValue[i], (0.0 - params.meanValue[i]) / params.stdValue[i]);
			//}
			//PD_S32 size = 0;
			//for (auto d = 0; d < bgrChannels.size(); d++)
			//{
			//	//size = i * bgrChannels[i].rows * bgrChannels[i].cols;
			//	memcpy_s(hostDataBuffer + height * width * (3 * n + (d)), height * width * sizeof(PD_FLOAT), bgrChannels[d].data, height * width * sizeof(PD_FLOAT));
			//}

			for (PD_S32 d = 0; d < 3; d++) {
				//std::cout << bgrChannels[d] << std::endl;
				bgrChannels[d] = (bgrChannels[d] / 255.0 - params.meanValue[2 - d]) / params.stdValue[2 - d];	// 当前图像通道是RGB，转成BGR

				//std::cout << bgrChannels[d] << std::endl;
				memcpy_s(hostDataBuffer + height * width * (3 * n + (2 - d)), height * width * sizeof(PD_FLOAT), bgrChannels[d].data, height * width * sizeof(PD_FLOAT));
			}
			//cv_images[n] = Normalize(cv_images[n], mean, std);

			//PD_S32 depth1 = cv_images[0].channels();
			//PD_S32 height1 = cv_images[0].rows;
			//PD_S32 width1 = cv_images[0].cols;
			//PD_S32 nc = cv_images[0].channels();
			//cv::Mat dst;
			//dst.create(cv_images[0].size(), cv_images[0].type());
			//for (PD_S32 row = 0; row < height1; row++) {
			//	for (PD_S32 col = 0; col < width1; col++) {
			//		/*PD_S32 b = image.at<cv::Vec3b>(row, col)[0];
			//		PD_S32 g = image.at<cv::Vec3b>(row, col)[1];
			//		PD_S32 r = image.at<cv::Vec3b>(row, col)[2];*/
			//		PD_S32 r = cv_images[0].at<cv::Vec3b>(row, col)[2];
			//		std::cout << r << std::endl;
			//		//PD_S32 g = image.at<uchar>(row, col);
			//		//PD_S32 r = image.at<uchar>(row, col);
			//		//std::cout << r << std::endl;
			//		/*dst.at<cv::Vec3b>(row, col)[0] = 255 - b;
			//		dst.at<cv::Vec3b>(row, col)[1] = 255 - g;
			//		dst.at<cv::Vec3b>(row, col)[2] = 255 - r;*/
			//	}
			//}
			// 
			// 
			//cv::imshow("asd", cv_images[n]);
			//cv::waitKey(0);
		}
	}
	else {
		LOG_F(INFO, "不支持的图像类型");
		return PD_WRONG_IMG;
	}
	return PD_OK;
}
template<class T>

PD_S32 length(T& arr)
{
	//cout << sizeof(arr[0]) << endl;
	//cout << sizeof(arr) << endl;
	return sizeof(arr) / sizeof(arr[0]);
}


// \! 异常检测后处理
// \@param buffers: TensorRT samples中定义的缓存
// \@param out_masks: 存储输出mask的
// \@param engine_output_batch: ONNX模型的batch
// \@param engine_output_height: ONNX模型的height
// \@param engine_output_width: ONNX模型的width
// \@param engine_output_name: ONNX模型的输出名称
struct ReturnImgs_S
{
	cv::Mat Img;
	cv::Mat Img1;
	cv::Mat ThreshImg;
	cv::Mat FinalImg;
	int nLY;
	ReturnImgs_S() = default;
	ReturnImgs_S(cv::Mat Img_, cv::Mat Img1_, cv::Mat ThreshImg_, cv::Mat FinalImg_, int nLY_) :Img(Img_), Img1(Img1_), ThreshImg(ThreshImg_), FinalImg(FinalImg_), nLY(nLY_) {};
};
ReturnImgs_S anomalyPost(const samplesCommon::BufferManager& buffers, cv::Mat& cv_images, std::vector<cv::Mat>& out_masks, const int engine_output_batchsize, const int engine_output_channel, const int engine_output_height, const int engine_output_width, const std::string outputName, int thresh)
{
	static int ii = 0;
	//// 拷贝数据
	float* output = static_cast<float*>(buffers.getHostBuffer(outputName));
	// 生成out_mask
	for (int i = 0; i < engine_output_batchsize; i++) {
		cv::Mat tmp = cv::Mat(
			//engine_output_channel,
			engine_output_height,
			engine_output_width,
			CV_32F,
			output + engine_output_height * engine_output_width * i
		).clone();
		//std::cout << "adsasdasdasdas" << engine_output_height << engine_output_width << std::endl;
		out_masks.push_back(tmp);
	}
	cv::Mat anomaly_map1;
	cv::Mat anomaly_map = cv::Mat(cv::Size(engine_output_height, engine_output_width),
		CV_32F, output);
	cv::Mat mask = anomaly_map.clone();
	//转换为整形
//std::cout << cv_images[0] << std::endl;
	anomaly_map.convertTo(anomaly_map1, CV_8UC1, 255, 0);
	anomaly_map.convertTo(anomaly_map, CV_8UC3, 255, 0);
	//    cv::Mat mask
	/*cv::imwrite("E:/Datasets/anomaly_map/" + std::to_string(ii) + ".jpg", anomaly_map);
	ii++;*/
	//单通道转化为3通道
	cv::applyColorMap(anomaly_map, anomaly_map, cv::COLORMAP_JET);

	//cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	//cv::erode(anomaly_map, anomaly_map, element);
	// 
	//    cv::imshow("asdssssssssssssssssssssss", anomaly_map);
	//    cv::waitKey(0);
	cv::Mat combine;
	//cv_images.convertTo(cv_images, CV_8UC1, 255, 0);
//    cv::imshow("asd", cv_images[0]);
//    cv::waitKey(0);
//    std::cout << cv_images[0].channels() << std::endl;
//    std::cout << anomaly_map.channels() << std::endl;
	cv::addWeighted(anomaly_map, 0.4, cv_images, 0.6, 0, combine, CV_8U);
	cv::Mat AnomalyGauss;
	// [3] 高斯滤波  sigma越大越平越模糊
	cv::GaussianBlur(anomaly_map, AnomalyGauss, cv::Size( 3,3 ), 0, 0);
	//cv::Mat mask = anomaly_map.clone();
	// 二值化 https://blog.csdn.net/weixin_42296411/article/details/80901080
	cv::Mat bgr[3];
	cv::split(anomaly_map, bgr);
	cv::Mat redChannel = bgr[2];
	if (thresh == 0)
		thresh = 90;
	cv::threshold(anomaly_map1, mask, thresh, 255, cv::THRESH_BINARY);
	//cv::imshow("阈值", mask);
	//cv::waitKey(0);
	cv::cvtColor(mask, mask, cv::COLOR_GRAY2RGB);

	return ReturnImgs_S(anomaly_map, anomaly_map1, mask, combine, PD_OK);
}






// \! 比较大小
bool PairCompare(const std::pair<PD_FLOAT, PD_S32>& lhs, const std::pair<PD_FLOAT, PD_S32>& rhs)
{
	return lhs.first > rhs.first;
}
// \！分类任务中返回最大的N类得分类别,输入大小是类别数目c个，输出大小N
std::vector<PD_S32> Argmax(const std::vector<PD_FLOAT>& v, PD_S32 N)
{
	std::vector<std::pair<PD_FLOAT, PD_S32>> pairs;
	for (size_t i = 0; i < v.size(); ++i)
		pairs.push_back(std::make_pair(v[i], i));

	std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);

	std::vector<PD_S32> result;
	for (PD_S32 i = 0; i < N; ++i)
		result.push_back(pairs[i].second);
	return result;
}