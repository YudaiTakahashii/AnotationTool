#include "MeanShitFilter.hpp"

MeanShitFilter::MeanShitFilter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG, imgsHistory, windowName, lineColor, transparencyColor, thickness) {

	// 前のトラックバーがあれば削除
	if (cv::getWindowProperty(this->_trackbarName, WND_PROP_VISIBLE) != -1)
		destroyWindow(this->_trackbarName);

	//トラックバーの生成
	cv::namedWindow(this->_trackbarName);
	cv::resizeWindow(this->_trackbarName, 100, 0);
	cv::moveWindow(this->_trackbarName, 0, 0);
	cv::createTrackbar("Space Window", this->_trackbarName, &this->_spaceWindowSize, this->_MAX_PARAMETER, nullptr);
	cv::createTrackbar("Color Window", this->_trackbarName, &this->_colorWindowSize, this->_MAX_PARAMETER, nullptr);
}

MeanShitFilter::~MeanShitFilter() {
	std::cout << "~MeanShitFilter" << std::endl;
}

void MeanShitFilter::mouseCallBack(int event, int x, int y, int flags) {

	// 左ボタンが押された時
	if (event == cv::EVENT_LBUTTONDOWN) {
		std::cout << "Mean Shit Filter is working..." << std::endl;
		ExcuteMeanShitFilter(this->_targetIMG);
		ExcuteMeanShitFilter(this->_originalIMG);


		cv::imshow(this->_windowName, this->_targetIMG);
		cv::moveWindow(this->_windowName, 300, 300);

		this->_imgsHistory.push_back(this->_targetIMG.clone());
	}
}

void MeanShitFilter::ExcuteMeanShitFilter(cv::Mat image)
{
	int level = 2;
	cv::Mat src_img = image.clone();

	cv::pyrMeanShiftFiltering(src_img, image, this->_spaceWindowSize, this->_colorWindowSize, level,
		 TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS, 5, 1));
}
