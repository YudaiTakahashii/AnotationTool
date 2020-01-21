#include "CaluculateIOU.hpp"

double CaluculateIOU(const std::string& DrawedImgPath, const std::string& TeacherImgPath)
{
	std::cout << 1 << std::endl;
	cv::waitKey();
	cv::Mat TeacherImg = cv::imread(TeacherImgPath, 0);
	cv::Mat DrawedImg = cv::imread(DrawedImgPath, 0);

	if (TeacherImg.data == NULL) {
		std::cout << "not exist : " << TeacherImgPath << std::endl;
		return 10;
	}
	if (DrawedImg.data == NULL) {
		std::cout << "not exist : " << DrawedImgPath << std::endl;
		return 10;
	}

	if (TeacherImg.size() != DrawedImg.size()) {
		std::cout << "size of " << TeacherImgPath << "is differnt from size of " << DrawedImgPath << std::endl;
	}
	
	//ƒ}ƒXƒN‰æ‘œ‚ð‚Q’l‰»
	cv::threshold(DrawedImg, DrawedImg, TRESH, MAX_VAL, cv::THRESH_BINARY);
	cv::threshold(TeacherImg, TeacherImg, TRESH, MAX_VAL, cv::THRESH_BINARY);
	
	//˜_—Ï—Ìˆæ, ˜_—˜a—Ìˆæ‚ðŠi”[‚·‚é
	cv::Mat AndMat, OrMat;
	cv::bitwise_and(DrawedImg, TeacherImg, AndMat);
	cv::bitwise_or(DrawedImg, TeacherImg, OrMat);
	
	double IOU;
	if (cv::countNonZero(OrMat) != 0) {
		IOU = double(cv::countNonZero(AndMat)) / cv::countNonZero(OrMat);
	}
	else {
		IOU = 0;
	}
	
	return IOU;
}