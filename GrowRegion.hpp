
#ifndef CODE_GROWREGION_HPP
#define CODE_GROWREGION_HPP

#include "ShapePainter.hpp"

class GrowRegion : public ShapePainter {
private:
	const int _MAX_PARAMETER = 50;
	int _parameter = 10;

	int _marker_size;
	std::vector<bool> _marker;
	bool _isRGB;

	Mat _grayIMG;
	Mat _tempIMG;

	int _transparencyBlue, _transparencyGreen, _transparencyRed;
public:
	GrowRegion(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		const std::string& windowName,
		const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);
	~GrowRegion();
	void GrowRegionInternal(unsigned char seed_val);
	void ReplacePixcelValueByRegionGrowing();
	void mouseCallBack(int event, int x, int y, int flags) override;
};


#endif //CODE_GROWREGION_HPP
#pragma once
