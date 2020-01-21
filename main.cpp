#include <iostream>
#include<string>
#include<ctime>
#include<fstream>
#include "IMGLabelPaster.hpp"
#include "CaluculateIOU.hpp"
const int method_num = 5;
const int img_index = 14;
const std::string USER_NAME = "takahashi3";
const std::string OUTPUT_DIR = "dst\\" + std::to_string(img_index) + "\\" + USER_NAME + "\\";
const std::string TEXTFILE = "dst\\" + std::to_string(img_index) + "\\" + USER_NAME + ".csv";
const std::string GetMethod(int method_num);
void WriteToFile(std::vector<double> IOUs, std::vector<double> Times);

int main() {

	// 画像のPathを指定
	std::string srcImgPath = "src\\Input\\" + std::to_string(img_index) + ".jpg";
	std::string labelImgPath = "src\\label\\" + std::to_string(img_index) + ".png";

	std::vector<double> IOUs;
	std::vector<double> Times;

	
	for (int i = 0; i < method_num; i++) {
		
		// 正解画像を表示
		cv::Mat LabelImg = imread(labelImgPath);
		std::string FileName = "Answer";
		cv::imshow(FileName, LabelImg);
		cv::moveWindow(FileName, 1000, 0);

		const std::string mode_name = GetMethod(i);
		std::cout << std::endl << std::endl;
		std::cout << "使用するモード : " << GetMethod(i) << std::endl;
		std::cout << "準備ができたらEnterを押してください" << std::endl;
		cv::waitKey();
		// 出力を表示
		std::string dstImgPath = OUTPUT_DIR + mode_name + ".jpg";

		std::clock_t start = clock();
		IMGLabelPaster imgLabelPaster = IMGLabelPaster(srcImgPath, dstImgPath, labelImgPath);
		imgLabelPaster.start();
		std::clock_t end = clock();


		const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
		std::cout << ("time :  %lf[s]\n", time) << std::endl;

		
		double IOU = CaluculateIOU(dstImgPath, labelImgPath);

		std::cout << "IOU : " << IOU << std::endl;

		IOUs.push_back(IOU);
		Times.push_back(time);

	}

	WriteToFile(IOUs, Times);
	return 0;
}



const std::string GetMethod(int method_num)
{
	switch (method_num) {
		case 0:		return "Brush_Only";
		case 1:		return "Polygon_Only";
		case 2:		return "GrowRegion_AND_Brush";
		case 3:		return "GrowRegion_And_Brush_And\MeanShit";
		case 4:		return "GrubCut_AND_Brush";

		default:	return "error";
	}
}


void WriteToFile(std::vector<double> IOUs, std::vector<double> Times)
{

	std::ofstream writing_file;
	writing_file.open(TEXTFILE, std::ios::out);

	std::cout << "writing " << TEXTFILE << "..." << std::endl;

	writing_file << "Method, Time, IOU" << std::endl;
	for (int i = 0; i < IOUs.size(); i++)
	{
		writing_file << GetMethod(i) << ',';
		writing_file << Times[i] << ',';
		writing_file << IOUs[i] << std::endl;
	}
}
