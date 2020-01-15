#include "RectanglePainter.hpp"

RectanglePainter::RectanglePainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG, imgsHistory, windowName, lineColor, transparencyColor, thickness) {
}

RectanglePainter:: ~RectanglePainter() {
	std::cout << "~Rectangle" << std::endl;
}
void RectanglePainter::mouseCallBack(int event, int x, int y, int flags) {

	
	// 左ボタンが押された時
	if (event == cv::EVENT_LBUTTONDOWN) {
		// On the Clicking Flag.
		this->_isLClicking = true;
		// Record clicked point.
		this->_clickPoint = cv::Point2d(x, y);


	}// mouse が移動している間
	else if (event == cv::EVENT_MOUSEMOVE) {
		// もし mouse がまだ押されて、レリーズしていない状態で、移動すると。
		if (this->_isLClicking) {
			cv::Rect2d rect = cv::Rect2d(this->_clickPoint, cv::Point2d(x, y));
			cv::Mat tempTargetIMG;
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, tempTargetIMG);
			cv::rectangle(tempTargetIMG, rect, this->_lineColor, this->_thickness);
			cv::imshow(this->_windowName, tempTargetIMG);
		}

	}
	else if (event == cv::EVENT_LBUTTONUP) {
		if (this->_isLClicking) {
			this->_isLClicking = false;
			std::vector<cv::Point> rectPoints;
			rectPoints.emplace_back(this->_clickPoint.x, this->_clickPoint.y);
			rectPoints.emplace_back(x, this->_clickPoint.y);
			rectPoints.emplace_back(x, y);
			rectPoints.emplace_back(this->_clickPoint.x, y);
			cv::fillConvexPoly(this->_targetIMG, rectPoints, this->_transparencyColor);
			cv::Mat ImgForShow = this->_originalIMG.clone();
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
			cv::imshow(this->_windowName, ImgForShow);
			this->_imgsHistory.push_back(this->_targetIMG.clone());
		}
	}
}