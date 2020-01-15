//
// Created by I Koku on 11/25/2019.
//

#ifndef CODE_CIRCLEPAINTER_HPP
#define CODE_CIRCLEPAINTER_HPP

#include "ShapePainter.hpp"

class CirclePainter : public ShapePainter {
public:
	CirclePainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
		const std::string& windowName, 
		const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);

	void mouseCallBack(int event, int x, int y, int flags) override;
};


#endif //CODE_CIRCLEPAINTER_HPP
