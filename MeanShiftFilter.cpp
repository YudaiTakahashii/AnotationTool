#include "MeanShitFilter.hpp"

MeanShitFilter::MeanShitFilter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) : _parameterWindowName("parameter"),
	ShapePainter(targetImg, originalIMG, imgsHistory, windowName, lineColor, transparencyColor, thickness) {

	//トラックバーの生成
	cv::namedWindow(this->_parameterWindowName);
	cv::resizeWindow(this->_parameterWindowName, 100, 0);
	cv::moveWindow(this->_parameterWindowName, 300, 300);
	cv::createTrackbar("Space Window", this->_parameterWindowName, &this->_spaceWindowSize, this->_MAX_PARAMETER, nullptr);
	cv::createTrackbar("Color Window", this->_parameterWindowName, &this->_colorWindowSize, this->_MAX_PARAMETER, nullptr);
}

MeanShitFilter::~MeanShitFilter() {
	cv::destroyWindow(this->_parameterWindowName);
	std::cout << "~MeanShitFilter" << std::endl;
}

void MeanShitFilter::mouseCallBack(int event, int x, int y, int flags) {

	// 左ボタンが押された時
	if (event == cv::EVENT_LBUTTONDOWN) {
		ExcuteMeanShitFilter(this->_targetIMG);
		ExcuteMeanShitFilter(this->_originalIMG);


		cv::namedWindow("target Image");
		cv::imshow("target Image", this->_targetIMG);

		cv::Mat ImgForShow = this->_originalIMG.clone();
		cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
		cv::imshow(this->_windowName, ImgForShow);
		this->_imgsHistory.push_back(this->_targetIMG.clone());
	}
}

void MeanShitFilter::ExcuteMeanShitFilter(cv::Mat image)
{
	int level = 2;
	
	// (3)分割結果画像出力用の画像領域を確保し，領域分割を実行
	cv::Mat src_img = image.clone();

	cv::pyrMeanShiftFiltering(src_img, image, this->_spaceWindowSize, this->_colorWindowSize, level,
		 TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS, 5, 1));

	//// (4)入力画像と分割結果画像の表示
	//cv::namedWindow("Source");
	//cv::namedWindow("MeanShift");
	//cv::imshow("Source", image);
	//cv::imshow("MeanShift", dst_img);


}
