#include "CaluculateIOU.hpp"

double CaluculateIOU(const std::string& DrawedImgPath, const std::string& TeacherImgPath)
{
	cv::Mat TeacherImg = cv::imread(TeacherImgPath, 0);
	cv::Mat DrawedImg = cv::imread(DrawedImgPath, 0);

	std::cout << DrawedImg.type() << std::endl;
	std::cout << DrawedImg.size() << std::endl;
	std::cout << TeacherImg.size() << std::endl;
	std::cout << TeacherImg.type() << std::endl;

	
	//�}�X�N�摜���Q�l��
	cv::threshold(DrawedImg, DrawedImg, TRESH, MAX_VAL, cv::THRESH_BINARY);
	cv::threshold(TeacherImg, TeacherImg, TRESH, MAX_VAL, cv::THRESH_BINARY);
	
	//�_���ϗ̈�, �_���a�̈���i�[����
	cv::Mat AndMat, OrMat;
	cv::bitwise_and(DrawedImg, TeacherImg, AndMat);
	cv::bitwise_or(DrawedImg, TeacherImg, OrMat);
	
	std::cout << cv::countNonZero(AndMat) << std::endl;
	std::cout << cv::countNonZero(OrMat) << std::endl;
	double IOU;
	if (cv::countNonZero(OrMat) != 0) {
		IOU = double(cv::countNonZero(AndMat)) / cv::countNonZero(OrMat);
	}
	else {
		IOU = 0;
	}
	
	return IOU;
}