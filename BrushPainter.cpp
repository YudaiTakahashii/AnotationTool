#include "BrushPainter.hpp"

BrushPainter::BrushPainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG, imgsHistory, windowName, lineColor, transparencyColor, thickness),
	_trackbarWindowName("BrushTrackbar") {
	// 線の太さを表すトラックバーを作成する

	cv::namedWindow(this->_trackbarWindowName);
	cv::resizeWindow(this->_trackbarWindowName, 100, 0);
	cv::moveWindow(this->_trackbarWindowName, 300, 300);
	
	cv::createTrackbar("Line thickness", this->_trackbarWindowName, &this->_radius, 15, nullptr);

}

BrushPainter::~BrushPainter() {
	cv::destroyWindow(this->_trackbarWindowName);
	std::cout << "~BrushPainter" << std::endl;

}

void BrushPainter::mouseCallBack(int event, int x, int y, int flags) {

	cv::Mat tempTargetIMG = this->_targetIMG.clone();
	// 左ボタンが押されたとき
	if (event == cv::EVENT_LBUTTONDOWN) {
		//// On the Clicking Flag.
		this->_isLClicking = true;
		// Record clicked point.
		this->_clickPoint = cv::Point2d(x, y);
	}
	// 左ボタンを離したとき
	if (event == cv::EVENT_LBUTTONUP) {
		this->_isLClicking = false;
		this->_imgsHistory.push_back(this->_targetIMG.clone());
	}
	// mouseが移動しているとき
	else if (event == cv::EVENT_MOUSEMOVE) {
		// 左をリリースしていない状態でmouseが移動しているとき
		if (this->_isLClicking) {
			std::cout << x << y << ":" << this->_clickPoint.x << this->_clickPoint.y << std::endl;

			cv::line(this->_targetIMG, this->_clickPoint, Point2d(x, y), this->_transparencyColor, this->_radius, cv::LINE_AA);
			cv::Mat ImgForShow = this->_originalIMG.clone();
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
			cv::imshow(this->_windowName, ImgForShow);
			

			this->_clickPoint = Point2d(x, y);

		}
	}
}
