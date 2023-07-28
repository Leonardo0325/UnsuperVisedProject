
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

	// ch:ö���豸 | en:Enumerate Device  ��̬����
	static int EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST* pstDevList);
	// ch:���豸 | en:Open Device
	int MC_Open(MV_CC_DEVICE_INFO* pstDeviceInfo);
	// ch:̽��������Ѱ���С(ֻ��GigE�����Ч) | en:Detection network optimal package size(It only works for the GigE camera)
	int MC_GetOptimalPacketSize(unsigned int* pOptimalPacketSize);
	// ch:��ȡ������Int�Ͳ������� Width��Height����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
	// en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
	int MC_GetIntValue(IN const char* strKey, OUT MVCC_INTVALUE_EX* pIntValue);
	int MC_SetIntValue(IN const char* strKey, IN int64_t nValue);
	// ch:����ץͼ | en:Start Grabbing
	int MC_StartGrabbing();
	// ch:��ȡ������Bool�Ͳ������� ReverseX����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
   // en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
	int MC_GetBoolValue(IN const char* strKey, OUT bool* pbValue);
	int MC_SetBoolValue(IN const char* strKey, IN bool bValue);
	// ch:��ȡ������Enum�Ͳ������� PixelFormat����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
   // en:Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
	int MC_GetEnumValue(IN const char* strKey, OUT MVCC_ENUMVALUE* pEnumValue);
	int MC_SetEnumValue(IN const char* strKey, IN unsigned int nValue);
	int MC_SetEnumValueByString(IN const char* strKey, IN const char* sValue);

	// ch:��ȡ������Float�Ͳ������� ExposureTime��Gain����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
  // en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
	int MC_GetFloatValue(IN const char* strKey, OUT MVCC_FLOATVALUE* pFloatValue);
	int MC_SetFloatValue(IN const char* strKey, IN float fValue);
	// ch:ֹͣץͼ | en:Stop Grabbing
	int MC_StopGrabbing();

	// ch:������ȡһ֡ͼ������ | en:Get one frame initiatively
	int MC_GetImageBuffer(MV_FRAME_OUT* pFrame, int nMsec);

	// ch:�ͷ�ͼ�񻺴� | en:Free image buffer
	int MC_FreeImageBuffer(MV_FRAME_OUT* pFrame);
	// ch:��ʾһ֡ͼ�� | en:Display one frame image
	int MC_DisplayOneFrame(MV_DISPLAY_FRAME_INFO* pDisplayInfo);
	// ch:�ر��豸 | en:Close Device
	int MC_Close();
	// ch:����ͼƬΪ�ļ� | en:Save the image as a file
	int MC_SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM* pstParam);
	int MC_GetValue(MVCC_INTVALUE* stIntvalue);
	int MC_ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM* stConvertParam);
	int MC_GetOneFrameTimeout(unsigned char* pData, unsigned int nPayloadSize, MV_FRAME_OUT_INFO_EX *stImageInfo,int time);
	// ��������ͼƬ
	int MC_SetCommandValue();
	int doGrabImage(MV_FRAME_OUT stOutFrame, CString strSaveMultiPth);
	int SaveImageToFile(void* handle, unsigned char* p_BufAddr, unsigned short n_Width, unsigned short n_Height, enum MvGvspPixelType en_PixelType, unsigned int n_FrameLen,CString strSaveMultiPth);


private:
	void* m_hDevHandle;

};
#endif //_MY_CAMERA_H_

