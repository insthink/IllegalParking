
#include "opencv2/opencv.hpp"
#include <map>
#include <string>
#include <vector>
#include "Shlwapi.h"//获取硬件信息，绑定用
#include "LPKernelEx.h"

#define presetNum 2
#define maxCarNum 10

using namespace std;

typedef map<string, long> mapCar;
struct Line{
	cv::Point p1, p2;
};

class Detect{
public:
	Detect();
	~Detect();
	void readConfigFromIni(const char *openPath);
	bool initCarPlate(void);
	double calVariance(cv::Point p1, cv::Point p2, IplImage* image);
	void imageProcess(const cv::Mat &inImg, long currentFrameN);

private:
	bool _isValid(Line inLine, cv::Point inPt);
	void _printMap(map<int, mapCar> mapPreset);

private:
	const char *_iniPath;
	map<int, mapCar> _mapPreset;
	Line _allLine[presetNum];

	TH_PlateResult _plateResult[maxCarNum];
	int _nRecoNum;
	TH_RECT _rcRange;
	bool _isCarPlateInited;
};