#include <iostream>
#include "IMGLabelPaster.hpp"

int main() {
	//IMGLabelPaster imgLabelPaster = IMGLabelPaster("C:\\Users\\Ritter\\source\\repos\\User-interface_bymouse\\User-interface_bymouse\\lenna_std.bmp");
	std::cout << 1 << std::endl;

	IMGLabelPaster imgLabelPaster = IMGLabelPaster("Cancer.jpg");
	std::cout << 1 << std::endl;
	imgLabelPaster.start();

	return 0;
}

//#include<iostream>
//#include<opencv2/opencv.hpp>
//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc.hpp>
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//	// 画像の読み込み
//	Mat image;
//	image = cv::imread("lenna.jpg");
//
//	if (!image.data) // Check for invalid input
//	{
//		cout << "Could not open or find the image" << std::endl;
//		return -1;
//	}
//
//	// 前景かもしれない短形領域
//	cv::Rect rectangle(30, 50, 150, 500);
//
//	cv::Mat result; // segmentation result (4 possible values)
//	cv::Mat bgModel, fgModel; // 背景GMMモデルと前景GMMモデル
//
//	cv::grabCut(image,    // input image
//		result,   // segmentation result
//		rectangle,// rectangle containing foreground 
//		bgModel, fgModel, // models
//		3,        // number of iterations
//		cv::GC_INIT_WITH_RECT); // use rectangle
//								// Get the pixels marked as likely foreground
//	
//	// 2つの配列を比較
//	// 
//	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
//	// Generate output image
//	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
//	// resultが非ゼロの部分のみを，imageからforegroundにコピー
//	image.copyTo(foreground, result); // bg pixels not copied(
//
//									  // draw rectangle on original image
//	// 元画像にセグメンテーションする部位を表示
//	cv::rectangle(image, rectangle, cv::Scalar(0, 255, 255), 1);
//	cv::namedWindow("Image");
//	cv::imshow("Image", image);
//
//	// display result
//	cv::namedWindow("Segmented Image");
//	cv::imshow("Segmented Image", foreground);
//
//
//	waitKey();
//	return 0;
//
//}