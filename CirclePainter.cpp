
#include "CirclePainter.hpp"

CirclePainter::CirclePainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG, imgsHistory, windowName, lineColor, transparencyColor, thickness) {
	
	// 前のトラックバーがあれば削除
	if (cv::getWindowProperty(this->_trackbarName, WND_PROP_VISIBLE) != -1)
		destroyWindow(this->_trackbarName);
}

void CirclePainter::mouseCallBack(int event, int x, int y, int flags) {
	if (event == cv::EVENT_LBUTTONDOWN) {
		this->_isLClicking = true;
		this->_clickPoint = cv::Point2d(x, y);

	}
	else if (event == cv::EVENT_MOUSEMOVE) {
		if (this->_isLClicking) {
			cv::Point2d circleCenter = this->_clickPoint;
			double radius = sqrt(pow(x - circleCenter.x, 2) + pow(y - circleCenter.y, 2));
			cv::Mat tempTargetIMG = this->_originalIMG.clone();
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, tempTargetIMG);
			cv::circle(tempTargetIMG, circleCenter, static_cast<int>(radius), this->_lineColor, this->_thickness);
			cv::imshow(this->_windowName, tempTargetIMG);
			cv::moveWindow(this->_windowName, 300, 300);

		}

	}
	else if (event == cv::EVENT_LBUTTONUP) {
		this->_isLClicking = false;
		cv::Point2d circleCenter = this->_clickPoint;
		double radius = sqrt(pow(x - circleCenter.x, 2) + pow(y - circleCenter.y, 2));
		cv::circle(this->_targetIMG, circleCenter, static_cast<int>(radius), this->_transparencyColor, cv::FILLED
		
		
		);

		cv::Mat ImgForShow = this->_originalIMG.clone();
		cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
		cv::imshow(this->_windowName, ImgForShow);
		cv::moveWindow(this->_windowName, 300, 300);
		this->_imgsHistory.push_back(this->_targetIMG.clone());
	}
}