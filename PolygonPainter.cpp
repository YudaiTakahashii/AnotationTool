//
// Created by I Koku on 11/25/2019.
//

#include "PolygonPainter.hpp"


PolygonPainter::PolygonPainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG, imgsHistory,windowName, lineColor, transparencyColor, thickness) {
}

void PolygonPainter::mouseCallBack(int event, int x, int y, int flags) {
	if (event == cv::EVENT_LBUTTONDOWN) {
		this->_points.emplace_back(x, y);
		cv::Mat tempTargetIMG;
		cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, tempTargetIMG);
		polylines(tempTargetIMG, this->_points, false, this->_lineColor, this->_thickness, cv::LINE_AA);
		cv::imshow(this->_windowName, tempTargetIMG);
		
	}
	else if (event == cv::EVENT_MBUTTONDOWN) {
		if (!this->_points.empty()) {
			cv::fillConvexPoly(this->_targetIMG, this->_points, this->_transparencyColor, cv::LINE_AA);
			cv::Mat ImgForShow = this->_originalIMG.clone();
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
			cv::imshow(this->_windowName, ImgForShow);
			this->_imgsHistory.push_back(this->_targetIMG.clone());
			this->_points.clear();
		}
	}
}
