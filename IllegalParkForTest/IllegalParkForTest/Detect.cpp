#include "Detect.h"
#include <iostream>

/************************************************************************/
/* 初始化对象、析构函数                                                   */
/************************************************************************/
Detect::Detect(){
	// map初始化
	mapCar temp;
	temp.insert(make_pair("empty", 0));
	for(int i = 0; i < presetNum; i++){
		_mapPreset.insert(make_pair(i, temp));
	}

	// 方差线初始化
	_iniPath = "E:\\wuxi.ini";
	readConfigFromIni(_iniPath);

	// 车牌识别初始化
	_isCarPlateInited = initCarPlate();
	
}

Detect::~Detect(){}

/************************************************************************/
/* 车牌识别初始化函数                                                     */
/************************************************************************/
bool Detect::initCarPlate(void){
	int b;
	// 通道号
	int nchannel = 1;	
	// 预判断
	b = LPR_SetImageFormat(FALSE,FALSE,ImageFormatBGR,FALSE,45,400,TRUE,FALSE,TRUE,nchannel);	
	if(!b) return FALSE;
	if(LPR_InitEx(nchannel)==FALSE) return FALSE;

	b = LPR_SetPlateType(FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,nchannel); 
	if(!b) return FALSE;

	_nRecoNum = 0;
	// 设置车牌识别区域，默认是全图识别
	_rcRange.left = 0; _rcRange.right = 0; _rcRange.top = 0; _rcRange.bottom = 0;

	return TRUE;
}

/************************************************************************/
/* 从INI文件中读取配置信息（方差线）                                       */
/************************************************************************/
void Detect::readConfigFromIni(const char *openPath){
	char buf[256];
	char *name[presetNum*4] = { // 点的数量随预置位的改变而变
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
/* 方差计算函数                                                          */
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
/* 每一帧图片处理函数                                                     */
/************************************************************************/
void Detect::imageProcess(const cv::Mat &inImg, long currentFrameN){
	// 预置位遍历
	for(int i = 0; i < presetNum; i++){
		// 全局检测每个预置位方差，如果对应阈值位无车辆停靠，则置为空；
		cv::Mat grayImg;
		cv::cvtColor(inImg, grayImg, cv::COLOR_BGR2GRAY);
		double var = calVariance(_allLine[i].p1, _allLine[i].p2, &IplImage(grayImg));
		mapCar tempCar;
		if(var < 0.1){
			tempCar.insert(make_pair("empty", 0));
			_mapPreset[i] = tempCar;
		}

		// 对相应预置位的车辆信息进行访问
		mapCar::iterator pCar = _mapPreset[i].begin();
		if(pCar->first == "empty"){
			cout<<i<<": empty before"<<endl;
			// 判断有车：1.gotopreset 2.进行识别 3.加入map 4.保存抓拍图片至内存
			if(var > 0.2){
				cout<<i<<": go to preset"<<endl;
				if(_isCarPlateInited){
					// 暂时方案，将该图片抓拍保存下来处理；之后可以试试从相机中直接读取buff;
					char *firstSnap = "E:\\实验室\\轧线与违停算法相关\\机场违停\\IllegalParkForTest\\IllegalParkForTest\\firstSnap.jpg";
					cv::imwrite(firstSnap, inImg);
					int b = LPR_FileEx(firstSnap, NULL, _plateResult, _nRecoNum, &_rcRange, 1);
					cout<<"Total car num in global range: "<<_nRecoNum<<endl;
					if(_nRecoNum){
						// 筛选出在预置位范围内的车牌
						for(int j = 0; j < _nRecoNum; j++){
							cv::Point centerPt;
							centerPt.x = (_plateResult[j].rcLocation.left + _plateResult[j].rcLocation.right) / 2;
							centerPt.y = (_plateResult[j].rcLocation.top + _plateResult[j].rcLocation.bottom) / 2;
							if(_isValid(_allLine[i], centerPt)){
								// 记录有效车牌及当前帧，并加入map中
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
	_printMap(_mapPreset); // 打印map数据结构
}

/************************************************************************/
/* 私有函数：判断车牌中心点是否在方差线的有效范围内                          */
/************************************************************************/
bool Detect::_isValid(Line inLine, cv::Point inPt){
	long factorX = (inLine.p1.x - inPt.x) * (inLine.p2.x - inPt.x);
	long factorY = (inLine.p1.y - inPt.y) * (inLine.p2.y - inPt.y);
	if(factorX < 0 && factorY < 0) return TRUE;
	return FALSE;
}

/************************************************************************/
/* 私有函数：用来打印map数据结构                                           */
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