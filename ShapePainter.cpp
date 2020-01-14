

#include "ShapePainter.hpp"


ShapePainter::ShapePainter(cv::Mat& targetIMG, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
	std::string windowName, cv::Scalar lineColor, cv::Scalar_<int>& transparencyColor, int thickness) :
	_targetIMG(targetIMG), _windowName(std::move(windowName)), _clickPoint(-1, -1), _isLClicking(false),
	_originalIMG(originalIMG), _imgsHistory(imgsHistory),
	_lineColor(std::move(lineColor)), _transparencyColor(transparencyColor), _thickness(thickness) {
}

const cv::Mat& ShapePainter::getTargetImg() const {
	return _targetIMG;
}

void ShapePainter::setTargetImg(const cv::Mat& targetImg) {
	_targetIMG = targetImg;
}

const cv::Point2d& ShapePainter::getPoint() const {
	return _clickPoint;
}

void ShapePainter::setPoint(const cv::Point2d& point) {
	ShapePainter::_clickPoint = point;
}

bool ShapePainter::isIsLClicking() const {
	return _isLClicking;
}

void ShapePainter::setIsLClicking(bool isLClicking) {
	_isLClicking = isLClicking;
}

void ShapePainter::Undo() {
	if (!this->_imgsHistory.empty()) {
		this->_imgsHistory.pop_back();
		if (!this->_imgsHistory.empty()) {
			std::cout << "replaced" << std::endl;
			this->_targetIMG = this->_imgsHistory.back().clone();
		}
		else {
			this->_targetIMG = this->_originalIMG.clone();
		}
		cv::Mat ImgForShow;
		cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
		cv::imshow(this->_windowName, ImgForShow);
		std::cout << "Undo" << std::endl;
	}
	else {
		std::cout << "Cannot Undo : This is a original image." << std::endl;
	}
}
ShapePainter::~ShapePainter() {
}