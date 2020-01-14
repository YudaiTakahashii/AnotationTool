#ifndef CODE_GRABCUT_HPP
#define CODE_GRABCUT_HPP

#include "ShapePainter.hpp"

class GrabCut : public ShapePainter {
public:
	GrabCut(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		const std::string& windowName, const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);
	~GrabCut();
	void mouseCallBack(int event, int x, int y, int flags) override;
	cv::Mat ExcuteGrabCut(cv::Rect2d rectangle);

private:
	const int _MAX_PARAMETER = 50;
	int _parameter = 10;
	std::string _parameterWindowName;
};


#endif //CODE_GRABCUT_HPP
#pragma once