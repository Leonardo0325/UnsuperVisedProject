// CMvCamera.cpp: 实现文件
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

// ch:获取SDK版本号 | en:Get SDK Version
int CMvCamera::MC_GetSDKVersion()
{
    return MV_CC_GetSDKVersion();
}
// ch:枚举设备 | en:Enumerate Device
int CMvCamera::EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST* pstDevList)
{
    return MV_CC_EnumDevices(nTLayerType, pstDevList);
}
// ch:打开设备 | en : Open Device
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
// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
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
// ch:获取和设置Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 mv_cameraNode.xlsx 文件
// en:Get Int type parameters, such as Width and Height, for details please refer to mv_cameraNode.xlsx file under SDK installation directory
int CMvCamera::MC_GetIntValue(IN const char* strKey, OUT MVCC_INTVALUE_EX* pIntValue)
{
    return MV_CC_GetIntValueEx(m_hDevHandle, strKey, pIntValue);
}
int CMvCamera::MC_SetIntValue(IN const char* strKey, IN int64_t nValue)
{
    return MV_CC_SetIntValueEx(m_hDevHandle, strKey, nValue);
}
// ch:开启抓图 | en:Start Grabbing
int CMvCamera::MC_StartGrabbing()
{
    return MV_CC_StartGrabbing(m_hDevHandle);
}
// ch:获取和设置Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 mv_cameraNode.xlsx 文件
// en:Get Bool type parameters, such as ReverseX, for details please refer to mv_cameraNode.xlsx file under SDK installation directory
int CMvCamera::MC_GetBoolValue(IN const char* strKey, OUT bool* pbValue)
{
    return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}

int CMvCamera::MC_SetBoolValue(IN const char* strKey, IN bool bValue)
{
    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}
// ch:获取和设置Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 mv_cameraNode.xlsx 文件
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
// ch:获取和设置Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 mv_cameraNode.xlsx 文件
// en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to mv_cameraNode.xlsx file under SDK installation directory
int CMvCamera::MC_GetFloatValue(IN const char* strKey, OUT MVCC_FLOATVALUE* pFloatValue)
{
    return MV_CC_GetFloatValue(m_hDevHandle, strKey, pFloatValue);
}
int CMvCamera::MC_SetFloatValue(IN const char* strKey, IN float fValue)
{
    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}
// ch:设置显示窗口句柄 | en:Set Display Window Handle
int CMvCamera::MC_DisplayOneFrame(MV_DISPLAY_FRAME_INFO* pDisplayInfo)
{
    return MV_CC_DisplayOneFrame(m_hDevHandle, pDisplayInfo);
}
// ch:释放图像缓存 | en:Free image buffer
int CMvCamera::MC_FreeImageBuffer(MV_FRAME_OUT* pFrame)
{
    return MV_CC_FreeImageBuffer(m_hDevHandle, pFrame);
}
// ch:主动获取一帧图像数据 | en:Get one frame initiatively
int CMvCamera::MC_GetImageBuffer(MV_FRAME_OUT* pFrame, int nMsec)
{
    return MV_CC_GetImageBuffer(m_hDevHandle, pFrame, nMsec);

}
// ch:关闭设备 | en:Close Device
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


    // CMvCamera 消息处理程序
}
// ch:停止抓图 | en:Stop Grabbing
int CMvCamera::MC_StopGrabbing()
{
    return MV_CC_StopGrabbing(m_hDevHandle);
}
// ch:保存图片为文件 | en:Save the image as a file
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
//批量保存图片
int CMvCamera::MC_SetCommandValue()
{
    return MV_CC_SetCommandValue(m_hDevHandle, "TriggerSoftware");//相机拍照
}
int CMvCamera::doGrabImage(MV_FRAME_OUT stOutFrame,CString strSaveMultiPth)
{
    int nRet = MV_OK;
    double T1, T2, T3;
   // T1 = qMyMilliseconds();//计时1
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
    //源数据                 
    pstSaveFileParam.pData = p_BufAddr;                 //原始图像数据
    pstSaveFileParam.nDataLen = n_FrameLen;    			//原始图像数据长度
    pstSaveFileParam.enPixelType = en_PixelType;  			//原始图像数据的像素格式
    pstSaveFileParam.nWidth = n_Width;      				//图像宽
    pstSaveFileParam.nHeight = n_Height;      			//图像高 
    pstSaveFileParam.nJpgQuality = 80;							//JPEG图片编码质量,仅jpeg生效
    //目标数据
    pstSaveFileParam.enImageType = MV_Image_Bmp;            	//需要保存的图像类型，转换成JPEG格式
    pstSaveFileParam.nBufferSize = n_FrameLen * 3 + 2048;          //存储节点的大小
    unsigned char* pImage = (unsigned char*)malloc(n_FrameLen * 3 + 2048);//2048为图像头
    pstSaveFileParam.pImageBuffer = pImage;                   //输出数据缓冲区，存放转换之后的图片数据  
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
    //将转换之后图片数据保存成文件
    FILE* fp = fopen(chImageName, "wb");
    fwrite(pImage, 1, pstSaveFileParam.nImageLen, fp);
    fclose(fp);
    free(pImage);
    return MV_OK;
}