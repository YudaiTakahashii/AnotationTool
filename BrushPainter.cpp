#include "BrushPainter.hpp"

BrushPainter::BrushPainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory, 
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG, imgsHistory, windowName, lineColor, transparencyColor, thickness)
{
	// �O�̃g���b�N�o�[������΍폜
	if (cv::getWindowProperty(this->_trackbarName, WND_PROP_VISIBLE) != -1)
		destroyWindow(this->_trackbarName);
	// ���̑�����\���g���b�N�o�[���쐬����

	cv::namedWindow(this->_trackbarName);
	cv::resizeWindow(this->_trackbarName, 100, 0);
	cv::moveWindow(this->_trackbarName, 0, 0);
	
	cv::createTrackbar("Line thickness", this->_trackbarName, &this->_radius, 50, nullptr);

}

BrushPainter::~BrushPainter() {
	std::cout << "~BrushPainter" << std::endl;
}

void BrushPainter::mouseCallBack(int event, int x, int y, int flags) {

	cv::Mat tempTargetIMG = this->_targetIMG.clone();
	// ���{�^���������ꂽ�Ƃ�
	if (event == cv::EVENT_LBUTTONDOWN) {
		//// On the Clicking Flag.
		this->_isLClicking = true;
		// Record clicked point.
		this->_clickPoint = cv::Point2d(x, y);
	}
	// ���{�^���𗣂����Ƃ�
	if (event == cv::EVENT_LBUTTONUP) {
		this->_isLClicking = false;
		this->_imgsHistory.push_back(this->_targetIMG.clone());
	}
	// mouse���ړ����Ă���Ƃ�
	else if (event == cv::EVENT_MOUSEMOVE) {
		// ���������[�X���Ă��Ȃ���Ԃ�mouse���ړ����Ă���Ƃ�
		if (this->_isLClicking) {

			cv::line(this->_targetIMG, this->_clickPoint, Point2d(x, y), this->_transparencyColor, this->_radius);
			cv::Mat ImgForShow = this->_originalIMG.clone();
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
			cv::imshow(this->_windowName, ImgForShow);
			cv::moveWindow(this->_windowName, 300, 300);
			

			this->_clickPoint = Point2d(x, y);

		}
	}
}
