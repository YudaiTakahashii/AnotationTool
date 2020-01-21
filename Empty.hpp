
#ifndef CODE_EMPTY_HPP
#define CODE_EMPTY_HPP

#include "MyOpenCV.h"
#include "ShapePainter.hpp"

class Empty : public ShapePainter {
public:

	Empty(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
		const std::string& windowName,
		const cv::Scalar& lineColor, int thickness,
		cv::Scalar_<int>& transparencyColor);


	void mouseCallBack(int event, int x, int y, int flags) override;

};


#endif //CODE_EMPTY_HPP

