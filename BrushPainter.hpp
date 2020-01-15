

#ifndef CODE_BRUSHPAINTER_HPP
#define CODE_BRUSHPAINTER_HPP

#include "MyOpenCV.h"
#include "ShapePainter.hpp"

class BrushPainter : public ShapePainter {
public:

	BrushPainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		const std::string& windowName, 
		const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);

	~BrushPainter();

	void mouseCallBack(int event, int x, int y, int flags) override;
private:
	int _radius = 10;
};


#endif //CODE_BRUSHPAINTER_HPP

