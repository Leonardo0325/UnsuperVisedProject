// CMvCamera.cpp: ʵ���ļ�
//

#include "pch.h"
#include "ud_mvcamera.h"



CMvCamera::CMvCamera()
{
    m_hDevHandle = MV_NULL;
}

CMvCamera::~CMvCamera()
{
    if (m_hDevHandle)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = MV_NULL;
    }
}

// ch:��ȡSDK�汾�� | en:Get SDK Version
int CMvCamera::MC_GetSDKVersion()
{
    return MV_CC_GetSDKVersion();
}
// ch:ö���豸 | en:Enumerate Device
int CMvCamera::EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST* pstDevList)
{
    return MV_CC_EnumDevices(nTLayerType, pstDevList);
}
// ch:���豸 | en : Open Device
int CMvCamera::MC_Open(MV_CC_DEVICE_INFO* pstDeviceInfo)
{
    if (MV_NULL == pstDeviceInfo)
    {
        return MV_E_PARAMETER;
    }

    if (m_hDevHandle)
    {
        return MV_E_CALLORDER;
    }

    int nRet = MV_CC_CreateHandle(&m_hDevHandle, pstDeviceInfo);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    nRet = MV_CC_OpenDevice(m_hDevHandle);
    if (MV_OK != nRet)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = MV_NULL;
    }

    return nRet;
}
// ch:̽��������Ѱ���С(ֻ��GigE�����Ч) | en:Detection network optimal package size(It only works for the GigE camera)
int CMvCamera::MC_GetOptimalPacketSize(unsigned int* pOptimalPacketSize)
{
    if (MV_NULL == pOptimalPacketSize)
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_CC_GetOptimalPacketSize(m_hDevHandle);
    if (nRet < MV_OK)
    {
        return nRet;
    }

    *pOptimalPacketSize = (unsigned int)nRet;

    return MV_OK;
}
// ch:��ȡ������Int�Ͳ������� Width��Height����ϸ���ݲο�SDK��װĿ¼�µ� mv_cameraNode.xlsx �ļ�
// en:Get Int type parameters, such as Width and Height, for details please refer to mv_cameraNode.xlsx file under SDK installation directory
int CMvCamera::MC_GetIntValue(IN const char* strKey, OUT MVCC_INTVALUE_EX* pIntValue)
{
    return MV_CC_GetIntValueEx(m_hDevHandle, strKey, pIntValue);
}
int CMvCamera::MC_SetIntValue(IN const char* strKey, IN int64_t nValue)
{
    return MV_CC_SetIntValueEx(m_hDevHandle, strKey, nValue);
}
// ch:����ץͼ | en:Start Grabbing
int CMvCamera::MC_StartGrabbing()
{
    return MV_CC_StartGrabbing(m_hDevHandle);
}
// ch:��ȡ������Bool�Ͳ������� ReverseX����ϸ���ݲο�SDK��װĿ¼�µ� mv_cameraNode.xlsx �ļ�
// en:Get Bool type parameters, such as ReverseX, for details please refer to mv_cameraNode.xlsx file under SDK installation directory
int CMvCamera::MC_GetBoolValue(IN const char* strKey, OUT bool* pbValue)
{
    return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}

int CMvCamera::MC_SetBoolValue(IN const char* strKey, IN bool bValue)
{
    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}
// ch:��ȡ������Enum�Ͳ������� PixelFormat����ϸ���ݲο�SDK��װĿ¼�µ� mv_cameraNode.xlsx �ļ�
// en:Get Enum type parameters, such as PixelFormat, for details please refer to mv_cameraNode.xlsx file under SDK installation directory
int CMvCamera::MC_GetEnumValue(IN const char* strKey, OUT MVCC_ENUMVALUE* pEnumValue)
{
    return MV_CC_GetEnumValue(m_hDevHandle, strKey, pEnumValue);
}
int CMvCamera::MC_SetEnumValue(IN const char* strKey, IN unsigned int nValue)
{
    return MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
}
int CMvCamera::MC_SetEnumValueByString(IN const char* strKey, IN const char* sValue)
{
    return MV_CC_SetEnumValueByString(m_hDevHandle, strKey, sValue);
}
// ch:��ȡ������Float�Ͳ������� ExposureTime��Gain����ϸ���ݲο�SDK��װĿ¼�µ� mv_cameraNode.xlsx �ļ�
// en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to mv_cameraNode.xlsx file under SDK installation directory
int CMvCamera::MC_GetFloatValue(IN const char* strKey, OUT MVCC_FLOATVALUE* pFloatValue)
{
    return MV_CC_GetFloatValue(m_hDevHandle, strKey, pFloatValue);
}
int CMvCamera::MC_SetFloatValue(IN const char* strKey, IN float fValue)
{
    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}
// ch:������ʾ���ھ�� | en:Set Display Window Handle
int CMvCamera::MC_DisplayOneFrame(MV_DISPLAY_FRAME_INFO* pDisplayInfo)
{
    return MV_CC_DisplayOneFrame(m_hDevHandle, pDisplayInfo);
}
// ch:�ͷ�ͼ�񻺴� | en:Free image buffer
int CMvCamera::MC_FreeImageBuffer(MV_FRAME_OUT* pFrame)
{
    return MV_CC_FreeImageBuffer(m_hDevHandle, pFrame);
}
// ch:������ȡһ֡ͼ������ | en:Get one frame initiatively
int CMvCamera::MC_GetImageBuffer(MV_FRAME_OUT* pFrame, int nMsec)
{
    return MV_CC_GetImageBuffer(m_hDevHandle, pFrame, nMsec);

}
// ch:�ر��豸 | en:Close Device
int CMvCamera::MC_Close()
{
    if (MV_NULL == m_hDevHandle)
    {
        return MV_E_HANDLE;
    }

    MV_CC_CloseDevice(m_hDevHandle);

    int nRet = MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = MV_NULL;

    return nRet;


    // CMvCamera ��Ϣ�������
}
// ch:ֹͣץͼ | en:Stop Grabbing
int CMvCamera::MC_StopGrabbing()
{
    return MV_CC_StopGrabbing(m_hDevHandle);
}
// ch:����ͼƬΪ�ļ� | en:Save the image as a file
int CMvCamera::MC_SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM* pstSaveFileParam)
{
    return MV_CC_SaveImageToFile(m_hDevHandle, pstSaveFileParam);
}
int CMvCamera::MC_GetValue(MVCC_INTVALUE *stIntvalue)
{
    return MV_CC_GetIntValue(m_hDevHandle, "PayloadSize", stIntvalue);
}
int CMvCamera::MC_ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM * stConvertParam)
{
    return MV_CC_ConvertPixelType(m_hDevHandle, stConvertParam);
}
int CMvCamera::MC_GetOneFrameTimeout(unsigned char* pData, unsigned int nPayloadSize, MV_FRAME_OUT_INFO_EX* stImageInfo, int time)
{
    return MV_CC_GetOneFrameTimeout(m_hDevHandle, pData, nPayloadSize, stImageInfo, time);
}
//��������ͼƬ
int CMvCamera::MC_SetCommandValue()
{
    return MV_CC_SetCommandValue(m_hDevHandle, "TriggerSoftware");//�������
}
int CMvCamera::doGrabImage(MV_FRAME_OUT stOutFrame,CString strSaveMultiPth)
{
    int nRet = MV_OK;
    double T1, T2, T3;
   // T1 = qMyMilliseconds();//��ʱ1
    //nRet = MV_CC_GetImageBuffer(m_hDevHandle, &stOutFrame, 20000);
    if (nRet == MV_OK)
    {
        SaveImageToFile(m_hDevHandle, stOutFrame.pBufAddr, stOutFrame.stFrameInfo.nWidth, stOutFrame.stFrameInfo.nHeight, stOutFrame.stFrameInfo.enPixelType, stOutFrame.stFrameInfo.nFrameLen, strSaveMultiPth);
        nRet = MV_CC_FreeImageBuffer(m_hDevHandle, &stOutFrame);
        if (nRet != MV_OK)
        {
            return nRet;
        }
        return MV_OK;
    }
    return MV_OK;
    }
int SaveNum = 0;
int CMvCamera::SaveImageToFile(void* handle, unsigned char* p_BufAddr, unsigned short n_Width, unsigned short n_Height, enum MvGvspPixelType en_PixelType, unsigned int n_FrameLen,CString strSaveMultiPth1)
{
    int nRet = MV_OK;
    MV_SAVE_IMAGE_PARAM_EX pstSaveFileParam;
    memset(&pstSaveFileParam, 0, sizeof(MV_SAVE_IMAGE_PARAM_EX));
    //Դ����                 
    pstSaveFileParam.pData = p_BufAddr;                 //ԭʼͼ������
    pstSaveFileParam.nDataLen = n_FrameLen;    			//ԭʼͼ�����ݳ���
    pstSaveFileParam.enPixelType = en_PixelType;  			//ԭʼͼ�����ݵ����ظ�ʽ
    pstSaveFileParam.nWidth = n_Width;      				//ͼ���
    pstSaveFileParam.nHeight = n_Height;      			//ͼ��� 
    pstSaveFileParam.nJpgQuality = 80;							//JPEGͼƬ��������,��jpeg��Ч
    //Ŀ������
    pstSaveFileParam.enImageType = MV_Image_Bmp;            	//��Ҫ�����ͼ�����ͣ�ת����JPEG��ʽ
    pstSaveFileParam.nBufferSize = n_FrameLen * 3 + 2048;          //�洢�ڵ�Ĵ�С
    unsigned char* pImage = (unsigned char*)malloc(n_FrameLen * 3 + 2048);//2048Ϊͼ��ͷ
    pstSaveFileParam.pImageBuffer = pImage;                   //������ݻ����������ת��֮���ͼƬ����  
    nRet = MV_CC_SaveImageEx2(handle, &pstSaveFileParam);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    char chImageName[256] = { 0 };
    if (MV_Image_Bmp == pstSaveFileParam.enImageType)
    {
        SaveNum++;
       // strcat(strSaveMultiPth1.GetBuffer(), "Image_w%d_h%d_fn%03d.bmp");
        sprintf_s(chImageName, 256, strSaveMultiPth1+ "Image_w%d_h%d_fn%03d.bmp", SaveNum, pstSaveFileParam.nWidth, pstSaveFileParam.nHeight);
    }
    if (MV_Image_Jpeg == pstSaveFileParam.enImageType)
    {   
        SaveNum++;
        //strcat(strSaveMultiPth1.GetBuffer(), "Image_w%d_h%d_fn%03d.bmp");
        sprintf_s(chImageName, 256, strSaveMultiPth1+ "Image_w%d_h%d_fn%03d.jpg", SaveNum, pstSaveFileParam.nWidth, pstSaveFileParam.nHeight);
    }
    //��ת��֮��ͼƬ���ݱ�����ļ�
    FILE* fp = fopen(chImageName, "wb");
    fwrite(pImage, 1, pstSaveFileParam.nImageLen, fp);
    fclose(fp);
    free(pImage);
    return MV_OK;
}