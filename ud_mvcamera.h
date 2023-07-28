
#ifndef  _CMvCamera_H_
#define  _CMvCamera_H_
#include"MvCameraControl.h"
#include<string.h>
#include<iostream>
#ifndef MV_NULL
#define MV_NULL    0
#endif
#include <string>
using namespace std;
// MyCamera

class CMvCamera
{
public:

	CMvCamera();
	~CMvCamera();
	static int MC_GetSDKVersion();

	// ch:枚举设备 | en:Enumerate Device  静态变量
	static int EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST* pstDevList);
	// ch:打开设备 | en:Open Device
	int MC_Open(MV_CC_DEVICE_INFO* pstDeviceInfo);
	// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
	int MC_GetOptimalPacketSize(unsigned int* pOptimalPacketSize);
	// ch:获取和设置Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
	// en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
	int MC_GetIntValue(IN const char* strKey, OUT MVCC_INTVALUE_EX* pIntValue);
	int MC_SetIntValue(IN const char* strKey, IN int64_t nValue);
	// ch:开启抓图 | en:Start Grabbing
	int MC_StartGrabbing();
	// ch:获取和设置Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
   // en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
	int MC_GetBoolValue(IN const char* strKey, OUT bool* pbValue);
	int MC_SetBoolValue(IN const char* strKey, IN bool bValue);
	// ch:获取和设置Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
   // en:Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
	int MC_GetEnumValue(IN const char* strKey, OUT MVCC_ENUMVALUE* pEnumValue);
	int MC_SetEnumValue(IN const char* strKey, IN unsigned int nValue);
	int MC_SetEnumValueByString(IN const char* strKey, IN const char* sValue);

	// ch:获取和设置Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
  // en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
	int MC_GetFloatValue(IN const char* strKey, OUT MVCC_FLOATVALUE* pFloatValue);
	int MC_SetFloatValue(IN const char* strKey, IN float fValue);
	// ch:停止抓图 | en:Stop Grabbing
	int MC_StopGrabbing();

	// ch:主动获取一帧图像数据 | en:Get one frame initiatively
	int MC_GetImageBuffer(MV_FRAME_OUT* pFrame, int nMsec);

	// ch:释放图像缓存 | en:Free image buffer
	int MC_FreeImageBuffer(MV_FRAME_OUT* pFrame);
	// ch:显示一帧图像 | en:Display one frame image
	int MC_DisplayOneFrame(MV_DISPLAY_FRAME_INFO* pDisplayInfo);
	// ch:关闭设备 | en:Close Device
	int MC_Close();
	// ch:保存图片为文件 | en:Save the image as a file
	int MC_SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM* pstParam);
	int MC_GetValue(MVCC_INTVALUE* stIntvalue);
	int MC_ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM* stConvertParam);
	int MC_GetOneFrameTimeout(unsigned char* pData, unsigned int nPayloadSize, MV_FRAME_OUT_INFO_EX *stImageInfo,int time);
	// 批量保存图片
	int MC_SetCommandValue();
	int doGrabImage(MV_FRAME_OUT stOutFrame, CString strSaveMultiPth);
	int SaveImageToFile(void* handle, unsigned char* p_BufAddr, unsigned short n_Width, unsigned short n_Height, enum MvGvspPixelType en_PixelType, unsigned int n_FrameLen,CString strSaveMultiPth);


private:
	void* m_hDevHandle;

};
#endif //_MY_CAMERA_H_

