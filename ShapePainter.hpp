//
// Created by I Koku on 11/25/2019.
//

#ifndef CODE_SHAPEPAINTER_HPP
#define CODE_SHAPEPAINTER_HPP

#include "MyOpenCV.h"


class ShapePainter {
public:
	explicit ShapePainter(cv::Mat& targetIMG, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
		std::string windowName ,cv::Scalar lineColor, cv::Scalar_<int>& transparencyColor, int thickness);
	virtual void mouseCallBack(int event, int x, int y, int flags) = 0;

	const cv::Point2d& getPoint() const;
	void setPoint(const cv::Point2d& point);

	const cv::Mat& getTargetImg() const;
	void setTargetImg(const cv::Mat& targetImg);

	bool isIsLClicking() const;

	void setIsLClicking(bool isLClicking);

	void Undo();

	//cv::Mat getTargetIMG() {
	//	return this->_targetIMG;
	//}

	virtual ~ShapePainter();
	

protected:
	cv::Mat& _originalIMG;
	cv::Mat _targetIMG;
	std::string _windowName;
	const std::string _trackbarName = "Parameter";
	cv::Point2d _clickPoint;
	bool _isLClicking;
	cv::Scalar _lineColor;
	cv::Scalar_<int>& _transparencyColor;
	int _thickness;
	std::vector<cv::Mat>& _imgsHistory;
};


#endif //CODE_SHAPEPAINTER_HPP

