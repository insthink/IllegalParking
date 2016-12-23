#include <iostream>
#include "Detect.h"
#include "stdlib.h"


using namespace std;

int main(){
	cv::Mat inImg = cv::imread("t2.png");
	if(!inImg.data) throw "Image has no data";

	Detect d;
	d.imageProcess(inImg, 999);
	system("pause");	
}