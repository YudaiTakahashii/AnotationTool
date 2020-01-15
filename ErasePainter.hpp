
#ifndef CODE_ERASEPAINTER_HPP
#define CODE_ERASEPAINTER_HPP

#include "ShapePainter.hpp"

class ErasePainter : public ShapePainter {
public:

	ErasePainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		const std::string& windowName,
		const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);

	~ErasePainter();

	void mouseCallBack(int event, int x, int y, int flags) override;
	void ReplacePixcelValue2Original();
	void ReplacePixcelValue2OriginalForGray();
	void ReplacePixcelValue2OriginalForColor();
private:
	int _radius = 5;
	Mat _tempTargetIMG;
	bool _isRGB;
	unsigned char _markerForErasing;
};


#endif //CODE_ERASEPAINTER_HPP
#pragma once
