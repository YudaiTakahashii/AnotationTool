
#ifndef CODE_RECTANGLEPAINTER_HPP
#define CODE_RECTANGLEPAINTER_HPP

#include "ShapePainter.hpp"

class RectanglePainter : public ShapePainter {
public:
	RectanglePainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		const std::string& windowName, const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);
	~RectanglePainter();
	void mouseCallBack(int event, int x, int y, int flags) override;
};


#endif //CODE_RECTANGLEPAINTER_HPP
