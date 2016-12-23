
#ifndef __LPRINTERFACE__
#define __LPRINTERFACE__

//������ɫ
#define LC_UNKNOWN  0  //δ֪
#define LC_BLUE     1  //��ɫ
#define LC_YELLOW   2  //��ɫ 
#define LC_WHITE    3  //��ɫ
#define LC_BLACK    4  //��ɫ
#define LC_GREEN    5  //��ɫ

//��������
#define LT_UNKNOWN  0   //δ֪����
#define LT_BLUE     1   //����С����
#define LT_BLACK    2   //����С����
#define LT_YELLOW   3   //���Ż���
#define LT_YELLOW2  4   //˫�Ż��ƣ���β�ƣ�ũ�ó���
#define LT_POLICE   5   //��������
#define LT_ARMPOL   6   //�侯����
#define LT_INDIVI   7   //���Ի�����
#define LT_ARMY     8   //���ž�����
#define LT_ARMY2    9   //˫�ž�����
#define LT_EMBASSY  10  //ʹ�ݳ���
#define LT_HONGKONG 11  //��۽����й���½����
#define LT_TRACTOR  12  //ũ�ó���
#define LT_COACH	13  //��������
#define LT_MACAO	14  //���Ž����й���½����
#define LT_ARMPOL2   15 //˫���侯����

//�˶�����
#define DIRECTION_LEFT	1  //��
#define DIRECTION_RIGHT	2  //��
#define DIRECTION_UP	3  //��
#define DIRECTION_DOWN	4  //��

//ͼ���ʽ��TH_SetImageFormat������cImageFormat������
#define ImageFormatRGB		0			//RGBRGBRGB...
#define ImageFormatBGR		1			//BGRBGRBGR...
#define ImageFormatYUV422	2			//YYYY...UU...VV..	(YV16)
#define ImageFormatYUV420COMPASS 3		//YYYY...UV...		(NV12)
#define ImageFormatYUV420	4			//YYYY...U...V...	(YU12)
#define ImageFormatUYVY	    5			//UYVYUYVYUYVY...	(UYVY)
#define ImageFormatNV21		6			//YYYY...VU...		(NV21)
#define ImageFormatYV12		7			//YYYY...V...U		(YV12)
#define ImageFormatYUYV	    8			//YUYVYUYVYUYV..	(YUYV)

//������ɫ
#define LGRAY_DARK	0	//��
#define LGRAY_LIGHT	1	//ǳ

#define LCOLOUR_WHITE	0	//��	
#define LCOLOUR_SILVER	1	//��(��)
#define LCOLOUR_YELLOW	2	//��
#define LCOLOUR_PINK	3	//��
#define LCOLOUR_RED		4	//��
#define LCOLOUR_GREEN	5	//��
#define LCOLOUR_BLUE	6	//��
#define LCOLOUR_BROWN	7	//�� 
#define LCOLOUR_BLACK	8	//��

typedef struct TH_RECT
{
	int left;
	int top;
	int right;
	int bottom;
}TH_RECT;

typedef struct TH_TimeVal
{
	long    tv_sec;         /* �� */
	long    tv_usec;        /* ΢�� */
	long	tv_frameStamp;	/* ֡��� */
}TH_TimeVal;

typedef struct TH_PlateResult 
{
	char license[16];   // ���ƺ���
	char color[8];      // ������ɫ
	int nColor;			// ������ɫ���
	int nType;			// ��������
	int nConfidence;	// ���ƿ��Ŷ�
	int nBright;		// ��������
	int nDirection;		// �˶�����0 unknown, 1 left, 2 right, 3 up , 4 down	
	TH_RECT rcLocation; //����λ��
	int nTime;          //ʶ������ʱ��
	unsigned char nCarBright;		//��������
	unsigned char nCarColor;		//������ɫ
	char reserved[100];				// ����
}TH_PlateResult;

typedef struct TH_PlateResultImage
{
	char license[16];				// ���ƺ���
	char color[8];					// ������ɫ
	int nColor;						// ������ɫ���
	int nType;						// ��������
	int nConfidence;				// ���ƿ��Ŷ�
	int nBright;					// ��������
	int nDirection;					// �˶�����0 unknown, 1 left, 2 right, 3 up , 4 down	
	int nTime;						// ʶ������ʱ��
	unsigned char nCarBright;		// ��������
	unsigned char nCarColor;		// ������ɫ

	unsigned char* pImageRGB24;     // ץ��ʶ�𵽳��Ƶ�ͼ��,RGB24λ��ʽ
	int nImageWidth;				// ץ��ʶ�𵽳��Ƶ�ͼ��Ŀ��
	int nImageHeight;				// ץ��ʶ�𵽳��Ƶ�ͼ��ĸ߶�

	TH_RECT		rcLocation;			// ץ�ĳ������ڵ�λ��
	TH_TimeVal  tv_time;			// ץ�ĳ��Ƶ�ʱ��
}TH_PlateResultImage;

#ifdef __cplusplus
extern "C" {
#endif

	/**************************************
	LPR_SetParameters����ʶ�����
	������bMovingImage[in]            ʶ���˶���ֹͼ��
	nPlatesNum[in]              ���ʶ��ĳ��Ƹ���
	bFlipVertical[in]           �Ƿ����µߵ�ͼ���ʶ��
	nColorOrder[in]             �Ƿ�ͼ���ʽ
	bVertCompress[in]           �Ƿ�ֱ����ѹ��һ��ʶ��
	nImageplateThr[in]          ���ƶ�λ��ֵ
	nImageRecogThr[in]          ����ʶ����ֵ
	nMinPlateWidth[in]          ��С���ƿ��
	nMaxPlateWidth[in]          ����ƿ��
	LocalProvince[in]           Ĭ��ʡ��
	bDwordAligned[in]           �Ƿ����ֽڶ���
	bInputHalfHeightImage[in]   �Ƿ����볡ͼ��
	bOutputSingleFrame[in]      �Ƿ�ֻ���һ��ʶ����
	bYellow2[in]                �Ƿ�ʶ��˫�����
	bIndivi[in]                 �Ƿ�ʶ����Ի�����
	bArmPol[in]                 �Ƿ�ʶ�����
	bArmy2[in]                  �Ƿ�ʶ��˫�����
	bTractor[in]                �Ƿ�ʶ��ũ�ó��� 
	bNight[in]                  �Ƿ���ҹ��ģʽ	
	nChannel[in]                ͨ����
	***************************************/
	BOOL WINAPI LPR_SetParameters(BOOL bMovingImage,
		int nPlatesNum,
		BOOL bFlipVertical, 
		int nColorOrder,
		BOOL bVertCompress,
		int nImageplateThr, int nImageRecogThr, 
		int nMinPlateWidth, int nMaxPlateWidth, 
		char *LocalProvince,
		BOOL bDwordAligned,
		BOOL bInputHalfHeightImage,    												   
		BOOL bOutputSingleFrame,
		BOOL bYellow2, BOOL bIndivi, BOOL bArmPol, BOOL bArmy2, BOOL bTractor, 
		BOOL bNight, 	
		int nChannel=1);

	/**************************************
	LPR_SetImageFormat����ͼ���ʽ�������ڵ���LPR_InitEx֮ǰ����
	������bMovingImage[in]            ʶ���˶���ֹͼ��
	bFlipVertical[in]           �Ƿ����µߵ�ͼ���ʶ��
	nColorOrder[in]             ͼ���ʽ
	bVertCompress[in]           �Ƿ�ֱ����ѹ��һ��ʶ��
	nMinPlateWidth[in]          ��С���ƿ��
	nMaxPlateWidth[in]          ����ƿ��
	bDwordAligned[in]           �Ƿ����ֽڶ���
	bInputHalfHeightImage[in]   �Ƿ����볡ͼ��
	bOutputSingleFrame[in]      �Ƿ�ֻ���һ��ʶ���� 	
	nChannel[in]                ͨ����
	***************************************/
	BOOL WINAPI LPR_SetImageFormat(BOOL bMovingImage,		
		BOOL bFlipVertical, 
		int nColorOrder,
		BOOL bVertCompress, 
		int nMinPlateWidth, 
		int nMaxPlateWidth, 
		BOOL bDwordAligned,
		BOOL bInputHalfHeightImage,    												   
		BOOL bOutputSingleFrame,	
		int nChannel=1);

	/**************************************
	LPR_SetPlateType����ʶ�������͡������ڵ���LPR_InitEx֮������
	������bYellow2[in]                �Ƿ�ʶ��˫�����
	bIndivi[in]                 �Ƿ�ʶ����Ի�����
	bArmPol[in]                 �Ƿ�ʶ���������
	bArmy2[in]                  �Ƿ�ʶ��˫�����
	bTractor[in]                �Ƿ�ʶ��ũ�ó��� 
	bOnlyDyellow[in]			  ֻ�Ǳ�˫�����
	bEmbassy[in]				  �Ƿ�ʶ��ʹ�ݳ���
	bDarmpolice[in]			  �Ƿ�ʶ��˫���侯����
	bOnlyLocation[in]			  ֻ��λ����
	nChannel[in]                ͨ����
	***************************************/
	BOOL WINAPI LPR_SetPlateType(BOOL bYellow2, 
		BOOL bIndivi,
		BOOL bArmPol, 
		BOOL bArmy2,
		BOOL bTractor,
		BOOL bOnlyDyellow,
		BOOL bEmbassy,
		BOOL bDarmpolice,
		BOOL bOnlyLocation,
		int  nChannel=1);

	/**************************************
	LPR_SetSpecialParameters����ҹ��ģʽ��ʶ����ֵ��ʡ��Ĭ��ֵ��ʶ���Ƹ����������ڵ���LPR_InitEx֮������
	������bNight[in]                  �Ƿ���ҹ��ģʽ
	nImageplateThr[in]          ���ƶ�λ��ֵ
	nImageRecogThr[in]          ����ʶ����ֵ
	nPlatesNum[in]              ���ʶ��ĳ��Ƹ���
	LocalProvince[in]           Ĭ��ʡ��	  	
	nChannel[in]                ͨ����
	***************************************/
	BOOL WINAPI LPR_SetSpecialParameters(BOOL bNight,		
		int nImageplateThr, 
		int nImageRecogThr,
		int nPlatesNum,
		char *LocalProvince,
		int nChannel=1);

	/**************************************
	��Ƶ��ʶ��ʱ�����õ�ǰͼ��֡��ʱ�䣬������Ƶ��ʶ��ʱ���������ʶ����ʱ��Ӧ��ͼ��֡��ʱ��
	������tv_time[in]			������Ƶ��ʶ��ʱʱ��
	***************************************/
	BOOL WINAPI LPR_SetTimeVal(TH_TimeVal& tv_time, int nChannel=1);
	
	/**************************************
	��Ƶ��ʶ��ʱ�����ó�����ʶ�������ڵ�����ʱ�䡣
	�ӿ����ã����������ʱ��ͣ�����ýӿڻ���LPR_GetReliableResult�ӿڣ���ʱ����ʶ������������
	          ���õ�����ʱ��������ε��ظ���ʶ�������Ӷ�����һ������γ���ʶ�����������
	Ӧ�ó��������� 1.�г���һֱͣ��������ϣ����������ͬһ������ʶ������Ӧ��
	������size_t duration_secondes[in]			������ʶ�������ڵ�����ʱ�䣨��λ:�룩
	***************************************/
	BOOL WINAPI LPR_SetCarDurationTime(size_t duration_secondes, int nChannel=1);


	/**************************************
	��ȡ���ܹ�֧�ֵ�ͨ����
	���������ص�ǰ���ܹ�֧�ֵ�ͨ����
	***************************************/
	int WINAPI LPR_GetTotalChannelNum();

	/**************************************
	LPR_InitEx��ʼ�����Ŀ�
	������nChannel[in] ͨ����
	***************************************/
	BOOL WINAPI LPR_InitEx(int  nChannel=1);

	/**************************************
	LPR_UninitExж�غ��Ŀ�
	������nChannel[in] ͨ����
	***************************************/
	void WINAPI LPR_UninitEx(int  nChannel=1);

	/**************************************
	LPR_RGB888Exʶ��������Ƶ����ʵʱ���泵��ʶ�������
	������pImg[in]       ָ���ڴ���ͼ���ָ�룬��ʽΪRGB888,YUV420,YUV422����ʽ��LPR_SetImageFormat������ָ��
	nWidth[in]     ͼ��Ŀ�ȣ�������Ϊ��λ
	nHeight[in]    ͼ��ĸ߶ȣ�������Ϊ��λ 
	pResult[in]    ʶ�����ṹ��
	nRecogNum[out] ʵ��ʶ�𵽵ĳ��Ƹ���
	prcRange[in]   ����ʶ��ķ�Χ����Ϊ(0,0,0,0)����ͼƬ��ʶ��������Ϊ��λ
	nChannel[in]   ͨ����
	***************************************/
	BOOL WINAPI LPR_RGB888Ex(unsigned char *pImg, int nWidth, int  nHeight, TH_PlateResult* pResult, int &nRecogNum, TH_RECT *prcRange, int  nChannel=1);

	/**************************************
	LPR_MemeryPictureExʶ���ڴ�ͼƬ�������������ʱ����Ϊ400���룬��������ʱ�����ڵ��ã��ӿ��ڲ�������400�����ٽ���ʶ��
	������pImg[in]       ָ�򵥷�ͼ���ڴ��е�ָ�룬��ʽΪRGB888,YUV420,YUV422����ʽ��LPR_SetImageFormat������ָ��
	nWidth[in]     ͼ��Ŀ�ȣ�������Ϊ��λ
	nHeight[in]    ͼ��ĸ߶ȣ�������Ϊ��λ 
	pResult[in]    ʶ�����ṹ��
	nRecogNum[out] ʵ��ʶ�𵽵ĳ��Ƹ���
	prcRange[in]   ����ʶ��ķ�Χ����Ϊ(0,0,0,0)����ͼƬ��ʶ��������Ϊ��λ
	nChannel[in]   ͨ����
	***************************************/
	BOOL WINAPI LPR_MemeryPictureEx(unsigned char *pImg, int nWidth, int  nHeight, TH_PlateResult* pResult, int &nRecogNum, TH_RECT *prcRange, int  nChannel=1);
	/**************************************
	LPR_GetReliableResultʶ��������Ƶ���󣬻�ȡ�ȶ���ʶ������
	LPR_RGB888Ex�ӿ��ڲ������ͬһ������֡��ʶ�����������һ����ѵ�ʶ������������һ����������ץ��ͼƬ�Լ�ץ��ʱ�̡�
	�����ѵ�ʶ������ͨ�����ӿ�����ȡ��
	���ӿڿ�����Ϊ����ץ��ʹ��
	ʹ�÷���:�˺�����Ҫ��LPR_RGB888Ex����֮�����
	������
	pResult[in]    ʶ�����ṹ��
	nRecogNum[out] ʵ��ʶ�𵽵ĳ��Ƹ���
	nChannel[in]   ͨ����
	***************************************/
	BOOL WINAPI LPR_GetReliableResult(TH_PlateResultImage* pResult, int& nRecogNum, int nChannel);
	
	/**************************************
	����ץ�ĵ�ͼƬ
	������
	ptrSaveName[in]    ��Ҫ������ļ��������磺"d:\\pic.jpg"����֧�ֵ�ͼƬ��ʽ����bmp,jpg�ȳ���ͼƬ��ʽ���ɱ�����ļ�������չ��ָ����
	pImageRGB24[in]    ͼ������ݲ��֣�ͼ����RGB24��ʽ
	nImageWidth[in]    ͼ��Ŀ��
	nImageHeight[in]   ͼ��ĸ߶�
	***************************************/	
	BOOL WINAPI LPR_SaveResultPic( char* ptrSaveName, unsigned char* pImageRGB24, int nImageWidth, int nImageHeight );

	/************************* *************
	LPR_GetImageBufʶ��������Ƶ��ʱ��ȡʶ�𵽳��Ƶ�֡�ڴ�
	������pImageBuf[in,out]   ����һ��BYTE����ָ�룬����Ҫ�����ڴ棻���ͼ���ָ��
	nWidth[out]         ͼ��Ŀ�ȣ�������Ϊ��λ
	nHeight[out]        ͼ��ĸ߶ȣ�������Ϊ��λ
	nSize[out]          ͼ��Ĵ�С�����ֽ�Ϊ��λ
	nChannel[in]       ͨ����
	˵������LPR_SetImageFormat�Ĳ���bOutputSingleFrame��ΪTRUE���ҵ��ú���LPR_RGB888Exʶ��������Ƶ��ʱ��
	�ô˺�����ȡʶ�𵽳��Ƶ�֡�ڴ档
	***************************************/
	BOOL WINAPI LPR_GetImageBuf(unsigned char *&pImageBuf, int &nWidth, int &nHeight, int &nSize, int  nChannel=1);

	/**************************************
	LPR_FileExʶ��ͼƬ�ļ�
	������lpszFileName[in]     ͼ���·��
	lpszPlateFile[in]    ���Ƶı���·������������泵��ͼƬ�˲�����NULL
	pResult[in]          ʶ�����ṹ��
	RecogNum[out]        ʵ��ʶ�𵽵ĳ��Ƹ���
	prcRange[in]         ����ʶ��ķ�Χ����Ϊ(0,0,0,0)����ͼƬ��ʶ��������Ϊ��λ
	nChannel[in]         ͨ����
	***************************************/
	BOOL WINAPI LPR_FileEx(char* lpszFileName, char *lpszPlateFile, TH_PlateResult* pResult, int &nRecogNum, TH_RECT *prcRange,int nChannel=1);

	/************************************************************************/
	/*                          Ӧ�ó�����μ���ģ��                        */
	/************************************************************************/

	/**************************************
	vz_CryptDogUserDataWrite������ܹ�д���û���Ȩ���ݣ�������ݳ���128λ��
	                          �ɹ�����д������ݳ��ȣ�ʧ�ܷ���-1��
	������userdata[in]   �û�����buffer��������ݳ���128λ
	userdata_len[in]     �û�����buffer�ĳ��ȣ���󳤶�128λ
	����ֵ�� ����û�����д��ɹ�������0�����д��ʧ�ܻ��߲����쳣������-1
	***************************************/
	int WINAPI vz_CryptDogUserDataWrite(char* userdata,int userdata_len);

	/**************************************
	vz_CryptDogUserDataRead���Ӽ��ܹ����û���Ȩ���ݡ��ɹ������ض�ȡ���ݳ��ȣ�ʧ�ܣ�����-1
	������userdata_buffer[in]   ����û���Ȩ���ݵ�buffer������ǰԤ�ȷ�������128�ֽڵĴ洢�ռ�
		  bufferSize[in]        userdata_buffer�ĳ��ȣ�����128�ֽڳ�
	����ֵ�� ��ȡ�ɹ������ض�ȡ���ݳ��ȣ���ȡʧ�ܣ�����-1
	***************************************/
	int WINAPI vz_CryptDogUserDataRead(char* userdata_buffer,int bufferSize);

	/**************************************
	vz_CryptDogSerialRead����ȡ���ܹ������кţ�ÿֻ���ܹ����к�Ψһ�����ݳ���Ϊ8�ֽڡ�
	                       �ɹ������ض�ȡ���ݳ��ȣ�ʧ�ܣ�����-1��
	������serial_buffer[in]   ������кŵ�buffer������ǰԤ�ȷ�������8�ֽڵĴ洢�ռ�
		  buffersize[in]       serial_buffer�ĳ��ȣ�����8�ֽڳ�
	����ֵ�� ��ȡ�ɹ������ض�ȡ���ݳ��ȣ���ȡʧ�ܣ�����-1
	***************************************/
	int WINAPI vz_CryptDogSerialRead(char* serial_buffer, int buffersize);
#ifdef __cplusplus
}	// extern "C"
#endif

#endif


