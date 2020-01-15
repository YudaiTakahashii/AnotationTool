#include "ErasePainter.hpp"

ErasePainter::ErasePainter(cv::Mat& targetImg, cv::Mat& originalIMG, std::vector<cv::Mat>& imgsHistory,
	const std::string& windowName, const cv::Scalar& lineColor,
	int thickness, cv::Scalar_<int>& transparencyColor) :
	ShapePainter(targetImg, originalIMG,imgsHistory, windowName, lineColor, transparencyColor, thickness)
{
	// �O�̃g���b�N�o�[������΍폜
	if (cv::getWindowProperty(this->_trackbarName, WND_PROP_VISIBLE) != -1)
		destroyWindow(this->_trackbarName);

	// ���̑�����\���g���b�N�o�[���쐬����
	cv::namedWindow(this->_trackbarName);
	cv::resizeWindow(this->_trackbarName, 100, 0);
	cv::moveWindow(this->_trackbarName, 300, 300);
	cv::createTrackbar("Line thickness", this->_trackbarName, &this->_radius, 15, nullptr);

	//�摜���O���[�X�P�[���œ����Ă��Ă���Ƃ��́CBGR�ɕϊ�
	if (this->_targetIMG.type() == 0) {
		cvtColor(this->_targetIMG, this->_tempTargetIMG, COLOR_GRAY2BGR);
		this->_isRGB = false;
	}
	else {
		this->_tempTargetIMG = this->_targetIMG.clone();
		this->_isRGB = true;
	}


	//B�̉�f�l�ŁC�摜���Ŏg���Ă��Ȃ��l�������S���̃}�[�J�Ƃ���
	this->_markerForErasing = 255;
	std::vector<unsigned char> used_b_val;
	for (int y = 0; y < this->_tempTargetIMG.rows; y++) {
		Vec3b* p = this->_tempTargetIMG.ptr<Vec3b>(y);
		for (int x = 0; x < this->_tempTargetIMG.cols; x++) {
			auto it = find(used_b_val.begin(), used_b_val.end(), p[x][0]);
			if (it != used_b_val.end())
				used_b_val.push_back(p[x][0]);
		}
	}
	while (1) {
		auto it = find(used_b_val.begin(), used_b_val.end(), this->_markerForErasing);
		if (it != used_b_val.end()) this->_markerForErasing--;
		else break;
	}

	std::cout << "marker val :" << int(this->_markerForErasing) << std::endl;
}

ErasePainter::~ErasePainter() {
}

void ErasePainter::mouseCallBack(int event, int x, int y, int flags) {

	cv::Mat tempTargetIMG = this->_targetIMG.clone();
	// ���{�^���������ꂽ�Ƃ�
	if (event == cv::EVENT_LBUTTONDOWN) {
		//// On the Clicking Flag.
		this->_isLClicking = true;
		// Record clicked point.
		this->_clickPoint = cv::Point2d(x, y);
	}

	// mouse���ړ����Ă���Ƃ�
	else if (event == cv::EVENT_MOUSEMOVE) {
		// ���������[�X���Ă��Ȃ���Ԃ�mouse���ړ����Ă���Ƃ�
		if (this->_isLClicking) {
			//void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
			cv::line(this->_tempTargetIMG, this->_clickPoint, Point2d(x, y), cv::Scalar(this->_markerForErasing, 0, 0), this->_radius, cv::LINE_AA);

			this->ReplacePixcelValue2Original();
			cv::Mat ImgForShow = this->_originalIMG.clone();
			cv::addWeighted(this->_originalIMG, 0.3, this->_targetIMG, 0.7, 0, ImgForShow);
			cv::imshow(this->_windowName, ImgForShow);
			this->_clickPoint = Point2d(x, y);
		}
	}
	// ���{�^���𗣂����Ƃ�
	if (event == cv::EVENT_LBUTTONUP) {
		this->_isLClicking = false;
	}
}


//�}�[�J�������Ă��镔���̉�l�l��,�I���W�i���̉摜�̉�f�l�ɒu��������
void ErasePainter::ReplacePixcelValue2Original() {

	if (this->_isRGB)		this->ReplacePixcelValue2OriginalForColor();
	else					this->ReplacePixcelValue2OriginalForGray();
}

void ErasePainter::ReplacePixcelValue2OriginalForColor()
{

	for (int y = 0; y < this->_targetIMG.rows; y++) {
		cv::Vec3b* PtrForTargetIMG = this->_targetIMG.ptr<cv::Vec3b>(y);
		cv::Vec3b* PtrForOriginalIMG = this->_originalIMG.ptr<cv::Vec3b>(y);
		cv::Vec3b* PtrForTempTargetIMG = this->_tempTargetIMG.ptr<cv::Vec3b>(y);
		//��f��B�l���}�[�J�[�ƈ�v����ꍇ�C�I���W�i���̉�f�l�ɒu��������
		for (int x = 0; x < this->_targetIMG.cols; x++) {
			if (PtrForTempTargetIMG[x][0] == this->_markerForErasing) {
				PtrForTargetIMG[x][0] = PtrForOriginalIMG[x][0];
				PtrForTargetIMG[x][1] = PtrForOriginalIMG[x][1];
				PtrForTargetIMG[x][2] = PtrForOriginalIMG[x][2];
			}
		}
	}
}

void ErasePainter::ReplacePixcelValue2OriginalForGray()
{
	std::cout << this->_markerForErasing << std::endl;

	for (int y = 0; y < this->_targetIMG.rows; y++) {
		unsigned char* PtrForTargetIMG = this->_targetIMG.ptr<unsigned char>(y);
		unsigned char* PtrForOriginalIMG = this->_originalIMG.ptr<unsigned char>(y);
		cv::Vec3b* PtrForTempTargetIMG = this->_tempTargetIMG.ptr<cv::Vec3b>(y);
		//��f��B�l���}�[�J�[�ƈ�v����ꍇ�C�I���W�i���̉�f�l�ɒu��������
		for (int x = 0; x < this->_targetIMG.cols; x++) {
			if (PtrForTempTargetIMG[x][0] == this->_markerForErasing) {
				std::cout << this->_markerForErasing << std::endl;
				PtrForTargetIMG[x] = PtrForOriginalIMG[x];
			}
		}
	}
}