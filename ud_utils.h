/*****************************************************************************
* @description : �����쳣ֵ
*****************************************************************************/
#pragma once
#include<afxwin.h>
#include <cuda_runtime.h>  // cuda��
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
// \! ָ��device id��GPU�Ƿ����
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

// \! ǰ���� �����ࡢ�ָ�쳣��⡢Ŀ����ı�׼������һ������
// \! ��1��8U->32F;(2)����ֵ��������;(3)�����3ͨ���ģ�BGR->RGB;(4)ת��һά����
// \@param buffers: ���棬tensorRT�е�samples����
// \@param cv_images: ����ͼƬ��openCV��ʽ
// \@param params: �����ṹ��
// \@param inputName: onnx����������
PD_S32 normalization(const samplesCommon::BufferManager& buffers, std::vector<cv::Mat>& cv_images, const PARAMS_S& params, std::string inputName)
{
	// 1.����host�ڴ�
	//static_cast ֻ����������ת����������ת�����սϵͣ�һ�㲻�ᷢ��ʲô���⣬���ǲ������ڲ�ͬ����֮���ת��������PD_S32*תdouble *
	PD_FLOAT* hostDataBuffer = static_cast<PD_FLOAT*>(buffers.getHostBuffer(inputName));
	PD_S32 nums = cv_images.size();	// ���ͼƬ����-batchsize
	PD_S32 depth = cv_images[0].channels();	// ���channels
	PD_S32 height = cv_images[0].rows;	// ���height
	PD_S32 width = cv_images[0].cols;	// ���width

	if (depth == 1) {// ͨ����Ϊ1
		for (PD_S32 n = 0, index = 0; n < nums; n++)
		{
			cv_images[n].convertTo(cv_images[n], CV_32F, 1.0 / 255);
			cv_images[n] = (cv_images[n] - params.meanValue[0]) / params.stdValue[0];
			memcpy_s(hostDataBuffer + n * height * width, height * width * sizeof(PD_FLOAT), cv_images[n].data, height * width * sizeof(PD_FLOAT));
		}
	}
	else if (depth == 3) {// ͨ����Ϊ3
		std::vector<PD_FLOAT> mean = { 0.485, 0.456, 0.406 };
		std::vector<PD_FLOAT> std = { 0.229, 0.224, 0.225 };



		for (PD_S32 n = 0, index = 0; n < nums; n++)
		{
			//convertTo()��������ת���������Ͳ�ͬ��Mat�������Խ�����PD_FLOAT�͵�Matת����imwrite()�����ܹ����ܵ����͡�
			// 

			cv_images[n].convertTo(cv_images[n], CV_32FC3);


			// 
			//  1.0/255 �������ӣ������͵�1/255����ʾ���ŵı���
			//ͼ���Сû�б仯������������UINT8��Ϊ��FLOAT32λ

			//cv_images[n].convertTo(cv_images[n], CV_32FC3, 1.0 / 255, 0);
			//cv::normalize(resized_image, resized_image, 0, 1, cv::NormTypes::NORM_MINMAX, CV_32FC3);

			// ��׼��
			//cv_images[n] = Normalize(cv_images[n], mean, std);

			/*cv::imshow("asd", cv_images[n]);
			cv::waitKey(0);*/

			std::vector<cv::Mat> bgrChannels(3);
			//cv::split ���ѳ�������ͨ��ͼ��
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
				bgrChannels[d] = (bgrChannels[d] / 255.0 - params.meanValue[2 - d]) / params.stdValue[2 - d];	// ��ǰͼ��ͨ����RGB��ת��BGR

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
		LOG_F(INFO, "��֧�ֵ�ͼ������");
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


// \! �쳣������
// \@param buffers: TensorRT samples�ж���Ļ���
// \@param out_masks: �洢���mask��
// \@param engine_output_batch: ONNXģ�͵�batch
// \@param engine_output_height: ONNXģ�͵�height
// \@param engine_output_width: ONNXģ�͵�width
// \@param engine_output_name: ONNXģ�͵��������
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
	//// ��������
	float* output = static_cast<float*>(buffers.getHostBuffer(outputName));
	// ����out_mask
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
	//ת��Ϊ����
//std::cout << cv_images[0] << std::endl;
	anomaly_map.convertTo(anomaly_map1, CV_8UC1, 255, 0);
	anomaly_map.convertTo(anomaly_map, CV_8UC3, 255, 0);
	//    cv::Mat mask
	/*cv::imwrite("E:/Datasets/anomaly_map/" + std::to_string(ii) + ".jpg", anomaly_map);
	ii++;*/
	//��ͨ��ת��Ϊ3ͨ��
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
	// [3] ��˹�˲�  sigmaԽ��ԽƽԽģ��
	cv::GaussianBlur(anomaly_map, AnomalyGauss, cv::Size( 3,3 ), 0, 0);
	//cv::Mat mask = anomaly_map.clone();
	// ��ֵ�� https://blog.csdn.net/weixin_42296411/article/details/80901080
	cv::Mat bgr[3];
	cv::split(anomaly_map, bgr);
	cv::Mat redChannel = bgr[2];
	if (thresh == 0)
		thresh = 90;
	cv::threshold(anomaly_map1, mask, thresh, 255, cv::THRESH_BINARY);
	//cv::imshow("��ֵ", mask);
	//cv::waitKey(0);
	cv::cvtColor(mask, mask, cv::COLOR_GRAY2RGB);

	return ReturnImgs_S(anomaly_map, anomaly_map1, mask, combine, PD_OK);
}






// \! �Ƚϴ�С
bool PairCompare(const std::pair<PD_FLOAT, PD_S32>& lhs, const std::pair<PD_FLOAT, PD_S32>& rhs)
{
	return lhs.first > rhs.first;
}
// \�����������з�������N��÷����,�����С�������Ŀc���������СN
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