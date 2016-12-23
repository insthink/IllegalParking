#include "Detect.h"
#include <iostream>

/************************************************************************/
/* ��ʼ��������������                                                   */
/************************************************************************/
Detect::Detect(){
	// map��ʼ��
	mapCar temp;
	temp.insert(make_pair("empty", 0));
	for(int i = 0; i < presetNum; i++){
		_mapPreset.insert(make_pair(i, temp));
	}

	// �����߳�ʼ��
	_iniPath = "E:\\wuxi.ini";
	readConfigFromIni(_iniPath);

	// ����ʶ���ʼ��
	_isCarPlateInited = initCarPlate();
	
}

Detect::~Detect(){}

/************************************************************************/
/* ����ʶ���ʼ������                                                     */
/************************************************************************/
bool Detect::initCarPlate(void){
	int b;
	// ͨ����
	int nchannel = 1;	
	// Ԥ�ж�
	b = LPR_SetImageFormat(FALSE,FALSE,ImageFormatBGR,FALSE,45,400,TRUE,FALSE,TRUE,nchannel);	
	if(!b) return FALSE;
	if(LPR_InitEx(nchannel)==FALSE) return FALSE;

	b = LPR_SetPlateType(FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,nchannel); 
	if(!b) return FALSE;

	_nRecoNum = 0;
	// ���ó���ʶ������Ĭ����ȫͼʶ��
	_rcRange.left = 0; _rcRange.right = 0; _rcRange.top = 0; _rcRange.bottom = 0;

	return TRUE;
}

/************************************************************************/
/* ��INI�ļ��ж�ȡ������Ϣ�������ߣ�                                       */
/************************************************************************/
void Detect::readConfigFromIni(const char *openPath){
	char buf[256];
	char *name[presetNum*4] = { // ���������Ԥ��λ�ĸı����
		"A0P1.X", "A0P1.Y",
		"A0P2.X", "A0P2.Y",
		"A1P1.X", "A1P1.Y",
		"A1P2.X", "A1P2.Y"
	};

	for(int i = 0; i < presetNum; i++){
		GetPrivateProfileString("VarianceLine", name[4*i], "0", buf, 256, openPath);
		_allLine[i].p1.x = atoi(buf);
		GetPrivateProfileString("VarianceLine", name[4*i+1], "0", buf, 256, openPath);
		_allLine[i].p1.y = atoi(buf);
		GetPrivateProfileString("VarianceLine", name[4*i+2], "0", buf, 256, openPath);
		_allLine[i].p2.x = atoi(buf);
		GetPrivateProfileString("VarianceLine", name[4*i+3], "0", buf, 256, openPath);
		_allLine[i].p2.y = atoi(buf);
	}
}

/************************************************************************/
/* ������㺯��                                                          */
/************************************************************************/
double Detect::calVariance(cv::Point p1, cv::Point p2, IplImage* image){
	float k=(float)(p2.y-p1.y)/(float)(p2.x-p1.x);
	float b=(float)(p1.y-k*p1.x);
	double fangcha=0;
	int w=image->width;
	int h=image->height;
	int step=image->widthStep;
	uchar *date=(uchar*)image->imageData;
	double sum=0;
	for(int i=MIN(p1.x,p2.x);i<MAX(p1.x,p2.x);i++)
	{	
		int fangc_temp=(int)(i*k)+b;
		sum+=date[(int)(fangc_temp*step+i)];
		//date[(int)(i+fangc_temp*step)]=155;
	}
	sum=sum/abs(p2.x-p1.x);
	for(int i=MIN(p1.x,p2.x);i<MAX(p1.x,p2.x);i++)
	{
		int fangc_temp=(int)(i*k)+b;
		fangcha+=abs(sum-date[(int)(fangc_temp*step+i)]);
	}
	return fangcha/(abs(p2.x-p1.x)*sum);	
}

/************************************************************************/
/* ÿһ֡ͼƬ������                                                     */
/************************************************************************/
void Detect::imageProcess(const cv::Mat &inImg, long currentFrameN){
	// Ԥ��λ����
	for(int i = 0; i < presetNum; i++){
		// ȫ�ּ��ÿ��Ԥ��λ��������Ӧ��ֵλ�޳���ͣ��������Ϊ�գ�
		cv::Mat grayImg;
		cv::cvtColor(inImg, grayImg, cv::COLOR_BGR2GRAY);
		double var = calVariance(_allLine[i].p1, _allLine[i].p2, &IplImage(grayImg));
		mapCar tempCar;
		if(var < 0.1){
			tempCar.insert(make_pair("empty", 0));
			_mapPreset[i] = tempCar;
		}

		// ����ӦԤ��λ�ĳ�����Ϣ���з���
		mapCar::iterator pCar = _mapPreset[i].begin();
		if(pCar->first == "empty"){
			cout<<i<<": empty before"<<endl;
			// �ж��г���1.gotopreset 2.����ʶ�� 3.����map 4.����ץ��ͼƬ���ڴ�
			if(var > 0.2){
				cout<<i<<": go to preset"<<endl;
				if(_isCarPlateInited){
					// ��ʱ����������ͼƬץ�ı�����������֮��������Դ������ֱ�Ӷ�ȡbuff;
					char *firstSnap = "E:\\ʵ����\\������Υͣ�㷨���\\����Υͣ\\IllegalParkForTest\\IllegalParkForTest\\firstSnap.jpg";
					cv::imwrite(firstSnap, inImg);
					int b = LPR_FileEx(firstSnap, NULL, _plateResult, _nRecoNum, &_rcRange, 1);
					cout<<"Total car num in global range: "<<_nRecoNum<<endl;
					if(_nRecoNum){
						// ɸѡ����Ԥ��λ��Χ�ڵĳ���
						for(int j = 0; j < _nRecoNum; j++){
							cv::Point centerPt;
							centerPt.x = (_plateResult[j].rcLocation.left + _plateResult[j].rcLocation.right) / 2;
							centerPt.y = (_plateResult[j].rcLocation.top + _plateResult[j].rcLocation.bottom) / 2;
							if(_isValid(_allLine[i], centerPt)){
								// ��¼��Ч���Ƽ���ǰ֡��������map��
								tempCar.insert(make_pair(_plateResult[j].license, currentFrameN));
								_mapPreset[i] = tempCar;
								//cout<<"plate: "<<_plateResult[j].license<<endl;
							}								
						}
					}
				}
			}
		}
	}
	_printMap(_mapPreset); // ��ӡmap���ݽṹ
}

/************************************************************************/
/* ˽�к������жϳ������ĵ��Ƿ��ڷ����ߵ���Ч��Χ��                          */
/************************************************************************/
bool Detect::_isValid(Line inLine, cv::Point inPt){
	long factorX = (inLine.p1.x - inPt.x) * (inLine.p2.x - inPt.x);
	long factorY = (inLine.p1.y - inPt.y) * (inLine.p2.y - inPt.y);
	if(factorX < 0 && factorY < 0) return TRUE;
	return FALSE;
}

/************************************************************************/
/* ˽�к�����������ӡmap���ݽṹ                                           */
/************************************************************************/
void Detect::_printMap(map<int, mapCar> mapPreset){
	map<int, mapCar>::iterator pPreset;
	mapCar::iterator pCar;
	for(pPreset = mapPreset.begin(); pPreset != mapPreset.end(); pPreset++){
		cout<<"preset: "<<pPreset->first<<endl;
		for(pCar = pPreset->second.begin(); pCar != pPreset->second.end(); pCar++){
			cout<<"carPlate: "<<pCar->first<<"\t"<<"cFrame: "<<pCar->second<<endl;
		}
	}
		
}