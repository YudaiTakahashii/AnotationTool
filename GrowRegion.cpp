#include "GrowRegion.hpp"

GrowRegion::GrowRegion(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG, imgsHistory, windowName, lineColor, transparencyColor, thickness) {

	// �O�̃g���b�N�o�[������΍폜
	if (cv::getWindowProperty(this->_trackbarName, WND_PROP_VISIBLE) != -1)
		destroyWindow(this->_trackbarName);

	//�g���b�N�o�[�̐���
	cv::namedWindow(this->_trackbarName);
	cv::resizeWindow(this->_trackbarName, 100, 0);
	cv::moveWindow(this->_trackbarName, 0, 0);
	cv::createTrackbar("parameter", this->_trackbarName, &this->_parameter, this->_MAX_PARAMETER, nullptr);

	//pixcel�P�ʂł̃A�N�Z�X������̂�, Scalar��RGB�̗v�f���Ƃɕۑ�����
	this->_transparencyBlue = this->_transparencyColor[0];
	this->_transparencyGreen = this->_transparencyColor[1];
	this->_transparencyRed = this->_transparencyColor[2];


	//�O���[�X�P�[���ɕϊ������摜��ۑ�
	if (this->_targetIMG.type() != 0) {
		cvtColor(this->_targetIMG, this->_grayIMG, cv::COLOR_RGB2GRAY);
		this->_isRGB = true;
	}
	else {
		this->_grayIMG = this->_targetIMG.clone();
		this->_isRGB = false;

	}

	////�̈�g���@�̑ΏۂƂȂ�̈�Ƀ}�[�J�[����ϐ���������
	
	this->_marker_size = this->_targetIMG.rows * this->_targetIMG.cols;
	this->_marker.reserve(this->_marker_size);
	for (int i = 0; i < this->_marker_size; i++) {
		this->_marker.push_back(false);
	}
}

GrowRegion::~GrowRegion() {

	std::cout << "~GrowRegion" << std::endl;

}

void GrowRegion::mouseCallBack(int event, int x, int y, int flags) {

	// ���{�^���������ꂽ��
	if (event == cv::EVENT_LBUTTONDOWN) {
		//�}�[�J�[�̏�����
		for (int i = 0; i < this->_marker_size; i++) this->_marker[i] = false;

		this->_clickPoint = cv::Point2d(x, y);
		this->_seedValue = this->_targetIMG.at<cv::Vec3b>(this->_clickPoint);

		//Region_Growing����
		GrowRegionInternal(this->_seedValue);
		//��f�l�̒u������
		ReplacePixcelValueByRegionGrowing();

		cv::Mat ImgForShow = this->_originalIMG.clone();
		cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
		cv::imshow(this->_windowName, ImgForShow);
		cv::moveWindow(this->_windowName, 300, 300);

		this->_imgsHistory.push_back(this->_targetIMG.clone());
	}

	if (event == cv::EVENT_MBUTTONDOWN) {
		cv::Scalar lower = cv::Scalar
		(this->_transparencyBlue - 1, this->_transparencyGreen - 1, this->_transparencyRed - 1);
		cv::Scalar upper = this->_transparencyColor;

		cv::Mat srcMask = cv::Mat::zeros(this->_originalIMG.size(), this->_originalIMG.type());
		cv::Mat dstMask = srcMask.clone();
		cv::Mat colorMask(this->_originalIMG.size(), this->_originalIMG.type(), this->_transparencyColor);

		// �}�X�N�摜�𐶐�
		cv::inRange(this->_targetIMG, lower, upper, srcMask);

		// �����t�H���W�[���Z���s��
		cv::morphologyEx(srcMask, dstMask, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1, -1), this->_morphorogyNum);

		// ���ʂ��o�͉摜�ɔ��f
		colorMask.copyTo(this->_targetIMG, dstMask);

		// �\���p�ƕۑ��p�̏���
		cv::Mat ImgForShow = this->_originalIMG.clone();
		cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
		cv::imshow(this->_windowName, ImgForShow);
		cv::moveWindow(this->_windowName, 300, 300);

		this->_imgsHistory.push_back(this->_targetIMG.clone());

	}
}


void GrowRegion::GrowRegionInternal(cv::Vec3b SeedValue) {
	std::vector<Point> stack;
	stack.push_back(this->_clickPoint);


	//region-growing����
	//�X�^�b�N����ɂȂ�܂ŌJ��Ԃ�
	while (!stack.empty()) {
		//���ړ_�����o��
		Point p = stack.back();
		stack.pop_back();

		if (!this->_marker[p.y * this->_grayIMG.cols + p.x]) {
			this->_marker[p.y * this->_grayIMG.cols + p.x] = true;

			//���ډ�f�̉E�ׂ𒲂ׂ�
			if (p.x + 1 < this->_targetIMG.cols && this->isSameArea(p.x+1, p.y))
				stack.push_back(Point(p.x + 1, p.y));

			//���ډ�f�̏�𒲂ׂ�
			if (p.y + 1 < this->_targetIMG.rows && this->isSameArea(p.x , p.y + 1))
				stack.push_back(Point(p.x, p.y + 1));

			//���ډ�f�̍��ׂ𒲂ׂ�
			if (p.x - 1 >= 0 && this->isSameArea(p.x -1, p.y))
				stack.push_back(Point(p.x - 1, p.y));

			//���ډ�f�̉��𒲂ׂ�
			if (p.y - 1 >= 0 && this->isSameArea(p.x, p.y-1))
				stack.push_back(Point(p.x, p.y - 1));
		}
	}

}


bool GrowRegion::isSameArea(int x, int y)
{
	//std::cout << x << " : " << y << std::endl;
	if (x < 0 && x >= this->_targetIMG.cols)
		return false;
	if (y < 0 && y >= this->_targetIMG.cols)
		return false;

	if (abs(this->_targetIMG.at<cv::Vec3b>(y, x)[0] - this->_seedValue[0]) < this->_parameter
		&& abs(this->_targetIMG.at<cv::Vec3b>(y, x)[1] - this->_seedValue[1]) < this->_parameter
		&& abs(this->_targetIMG.at<cv::Vec3b>(y, x)[2] - this->_seedValue[2]) < this->_parameter)
		return true;
	else
		return false;
}

void GrowRegion::ReplacePixcelValueByRegionGrowing() 
{
	int count = 0;
	if (this->_isRGB) {
		for (int y = 0; y < this->_targetIMG.rows; y++) {
			cv::Vec3b* p = this->_targetIMG.ptr<cv::Vec3b>(y);
			for (int x = 0; x < this->_targetIMG.cols; x++) {
				if (this->_marker[this->_targetIMG.cols * y + x]) {
					p[x][0] = this->_transparencyBlue;
					p[x][1] = this->_transparencyGreen;
					p[x][2] = this->_transparencyRed;
				}
			}
		}
	}
	else {
		for (int y = 0; y < this->_targetIMG.rows; y++) {
			unsigned char* p = this->_targetIMG.ptr<unsigned char>(y);
			for (int x = 0; x < this->_targetIMG.cols; x++) {
				if (this->_marker[this->_targetIMG.cols * y + x]) {
					p[x] = (this->_transparencyBlue + this->_transparencyGreen + this->_transparencyRed) / 3.0;
				}
			}
		}
	}
}

















//void GrowRegion::GrowRegionInternal(unsigned char SeedValue) {
//	int count = 0;
//	std::vector<Point> stack;
//	stack.push_back(this->_clickPoint);
//
//
//	//region-growing����
//	//�X�^�b�N����ɂȂ�܂ŌJ��Ԃ�
//	while (!stack.empty()) {
//		//���ړ_�����o��
//		Point p = stack.back();
//		stack.pop_back();
//
//		if (!this->_marker[p.y * this->_grayIMG.cols + p.x]) {
//			this->_marker[p.y * this->_grayIMG.cols + p.x] = true;
//			count++;
//
//			//���ډ�f�̉E�ׂ𒲂ׂ�
//			if (p.x + 1 < this->_grayIMG.cols &&
//				abs(this->_grayIMG.data[p.y * this->_grayIMG.cols + p.x + 1] - SeedValue) < this->_parameter)
//				stack.push_back(Point(p.x + 1, p.y));
//
//			//���ډ�f�̏�𒲂ׂ�
//			if (p.y + 1 < this->_grayIMG.rows &&
//				abs(this->_grayIMG.data[(p.y + 1) * this->_grayIMG.cols + p.x] - SeedValue) < this->_parameter)
//				stack.push_back(Point(p.x, p.y + 1));
//
//			//���ډ�f�̍��ׂ𒲂ׂ�
//			if (p.x - 1 >= 0 && abs(this->_grayIMG.data[p.y * this->_grayIMG.cols + p.x - 1] - SeedValue) < this->_parameter)
//				stack.push_back(Point(p.x - 1, p.y));
//
//			//���ډ�f�̉��𒲂ׂ�
//			if (p.y - 1 >= 0 && abs(this->_grayIMG.data[(p.y - 1) * this->_grayIMG.cols + p.x] - SeedValue) < this->_parameter)
//				stack.push_back(Point(p.x, p.y - 1));
//		}
//	}
//}
