#ifndef CODE_MEANSHITFILTER_HPP
#define CODE_MEANSHITFILTER_HPP

#include "ShapePainter.hpp"

class MeanShitFilter : public ShapePainter {
public:
	MeanShitFilter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		const std::string& windowName, 
		const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);
	~MeanShitFilter();
	void mouseCallBack(int event, int x, int y, int flags) override;
	void ExcuteMeanShitFilter(cv::Mat Image);

private:
	const int _MAX_PARAMETER = 50;
	cv::Rect2d roi;
	int _spaceWindowSize = 30;
	int _colorWindowSize = 30;
};


#endif //CODE_MEANSHITFILTER_HPP
#pragma once