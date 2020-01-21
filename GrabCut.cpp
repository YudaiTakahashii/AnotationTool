#include "GrabCut.hpp"

GrabCut::GrabCut(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG, imgsHistory, windowName, lineColor, transparencyColor, thickness) {

	// 前のトラックバーがあれば削除
	if (cv::getWindowProperty(this->_trackbarName, WND_PROP_VISIBLE) != -1)
		destroyWindow(this->_trackbarName);

	
}

void GrabCut::mouseCallBack(int event, int x, int y, int flags) {

	// 左ボタンが押された時
	if (event == cv::EVENT_LBUTTONDOWN) {
		// On the Clicking Flag.
		this->_isLClicking = true;
		// Record clicked point.
		this->_clickPoint = cv::Point2d(x, y);


	}// mouse が移動している間
	else if (event == cv::EVENT_MOUSEMOVE) {
		// もし mouse がまだ押されて、レリーズしていない状態で、移動すると。
		if (this->_isLClicking) {
			cv::Rect2d rect = cv::Rect2d(this->_clickPoint, cv::Point2d(x, y));
			cv::Mat tempTargetIMG;
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, tempTargetIMG);
			cv::rectangle(tempTargetIMG, rect, this->_lineColor, this->_thickness);
			cv::imshow(this->_windowName, tempTargetIMG);
			cv::moveWindow(this->_windowName, 300, 300);
		}

	}
	else if (event == cv::EVENT_LBUTTONUP) {
		if (this->_isLClicking) {
			this->_isLClicking = false;
			std::vector<cv::Point> rectPoints;
			rectPoints.emplace_back(this->_clickPoint.x, this->_clickPoint.y);
			rectPoints.emplace_back(x, this->_clickPoint.y);
			rectPoints.emplace_back(x, y);
			rectPoints.emplace_back(this->_clickPoint.x, y);
			ExcuteGrabCut(cv::Rect2d(this->_clickPoint, cv::Point2d(x, y)));


			cv::Mat ImgForShow = this->_originalIMG.clone();
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
			cv::imshow(this->_windowName, ImgForShow);
			cv::moveWindow(this->_windowName, 300, 300);

			this->_imgsHistory.push_back(this->_targetIMG.clone());
			
		}
	}
}

void GrabCut::ExcuteGrabCut(cv::Rect2d rectangle)
{
	cv::Mat result; // segmentation result (4 possible values)
	cv::Mat bgModel, fgModel; // 背景GMMモデルと前景GMMモデル

	if (rectangle.x == 0 || rectangle.y == 0) {
		std::cout << "領域が小さすぎます" << std::endl;
	}
	cv::grabCut(this->_targetIMG,    // input image
		result,   // segmentation result
		rectangle,// rectangle containing foreground 
		bgModel, fgModel, // models
		3,        // number of iterations
		cv::GC_INIT_WITH_RECT); // use rectangle
								// Get the pixels marked as likely foreground

	// 2つの配列を比較
	// 
	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
	// Generate output image
	cv::Mat foreground(this->_originalIMG.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat MaskImage(this->_originalIMG.size(), CV_8UC3, this->_transparencyColor);
	// resultが非ゼロの部分のみを，imageからforegroundにコピー
	MaskImage.copyTo(this->_targetIMG, result); // bg pixels not copied(

									  // draw rectangle on original image

}
GrabCut::~GrabCut() {
	std::cout << "~GrabCut" << std::endl;
}
