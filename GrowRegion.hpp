
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
	cv::Mat _grayIMG;
	cv::Vec3b _seedValue;
	int _transparencyBlue, _transparencyGreen, _transparencyRed;
	int _morphorogyNum = 6;
public:
	GrowRegion(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		const std::string& windowName,
		const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);
	~GrowRegion();
	void GrowRegionInternal(cv::Vec3b seed_val);
	void ReplacePixcelValueByRegionGrowing();
	void mouseCallBack(int event, int x, int y, int flags) override;
	bool isSameArea(int x, int y);
};


#endif //CODE_GROWREGION_HPP
#pragma once
