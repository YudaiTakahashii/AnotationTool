//
// Created by I Koku on 11/25/2019.
//


#ifndef CODE_POLYGONPAINTER_HPP
#define CODE_POLYGONPAINTER_HPP

#include "MyOpenCV.h"
#include "ShapePainter.hpp"

class PolygonPainter : public ShapePainter {
public:

	PolygonPainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		const std::string& windowName, const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);

	void mouseCallBack(int event, int x, int y, int flags) override;
private:
	std::vector<cv::Point> _points;
};


#endif //CODE_POLYGONPAINTER_HPP

