#include <iostream>
#include<string>
#include<ctime>
#include "IMGLabelPaster.hpp"
#include "CaluculateIOU.hpp"
const int method_num = 4;
const int img_index = 0;
int main() {

	// ‰æ‘œ‚ÌPath‚ğw’è
	std::string srcImgPath = "src\\Input\\" + std::to_string(img_index) + ".jpg";
	std::string labelImgPath = "src\\label\\" + std::to_string(img_index) + ".png";

	// ³‰ğ‰æ‘œ‚ğ•\¦
	cv::Mat LabelImg = imread(labelImgPath);
	std::string FileName = "Answer";
	cv::imshow(FileName, LabelImg);
	cv::moveWindow(FileName, 1000, 0);
	

	for (int i = 0; i < method_num; i++) {
		
		
		std::string dstImgPath = "dst\\hoge\\" + std::to_string(method_num) + ".jpg";
		std::clock_t start = clock();
		IMGLabelPaster imgLabelPaster = IMGLabelPaster(srcImgPath, dstImgPath, labelImgPath);
		imgLabelPaster.start();
		std::clock_t end = clock();


		const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
		std::cout << ("time %lf[s]\n", time) << std::endl;

		
		std::cout << "IOU : " << CaluculateIOU(dstImgPath, labelImgPath) << std::endl;
	}


	return 0;
}

