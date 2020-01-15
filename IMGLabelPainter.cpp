

#include <string>
#include <fstream>

#include "IMGLabelPaster.hpp"
#include "RectanglePainter.hpp"
#include "CirclePainter.hpp"
#include "PolygonPainter.hpp"
#include "BrushPainter.hpp"
#include "ErasePainter.hpp"
#include "GrowRegion.hpp"
#include "GrabCut.hpp"
#include "MeanShitFilter.hpp"


//cv::Scalar_<int> IMGLabelPaster::_transColor = cv::Scalar_<int>(100, 100, 150);
int IMGLabelPaster::tempBlue = 100;
int IMGLabelPaster::tempGreen = 100;
int IMGLabelPaster::tempRed = 100;
IMGLabelPaster::IMGLabelPaster(const std::string& srcImgPath, const std::string& dstImgPath, const std::string& labelImgPath) :
	_drawSelection(IMGLabelPaster::None), _terminalSignal(false), _windowName("LebelPaster"), 
	_shapePainterPointer(nullptr), _srcImgPath(srcImgPath), _dstImgPath(dstImgPath),  _labelImgPath(labelImgPath),
	_transColor(IMGLabelPaster::tempBlue, IMGLabelPaster::tempGreen, IMGLabelPaster::tempRed) {}

// Reade image into _targetIMG. Return True if succeed, otherwise return false.
bool IMGLabelPaster::_readIMG() {
	std::cout << "loading... :" << this->_srcImgPath << std::endl;
	std::cout << "loading... :" << this->_dstImgPath << std::endl;
	this->_targetIMG = cv::imread(this->_srcImgPath);
	this->_originalIMG = this->_targetIMG.clone();
	this->_labelIMG = cv::imread(this->_labelImgPath);
	
	return !this->_targetIMG.empty();
}

// Main Process.
void IMGLabelPaster::start() {
	if (this->_readIMG()) {
		// if reading succeed, process to construct GUI.
		cv::namedWindow(this->_windowName);
		cv::imshow(this->_windowName, this->_targetIMG);
		// Build function buttons.
		this->_buildGUI();
		// Configure mouse callback method
		cv::setMouseCallback(this->_windowName, IMGLabelPaster::_mouseCallBack, this);

		// User Interaction Loop
		while (!this->_terminalSignal) {
		
			int key = cv::waitKey(5);
			
			if (key == 'q') {
				this->_terminalSignal = true;
				std::cout << "b" << std::endl;

			}
			else if (key == 'z') {
				_shapePainterPointer->Undo();
				this->_targetIMG = _shapePainterPointer->getTargetImg();
				
			}
			// If the window is closed, terminate loop.
			if (cv::getWindowProperty(this->_windowName, cv::WND_PROP_VISIBLE) == 0) {
				this->_terminalSignal = true;
			}
		}
		std::cout << "a" << std::endl;
		cv::waitKey(0);
		
		//cv::destroyWindow(this->_windowName);
		cv::destroyAllWindows();

		std::cout << "b" << std::endl;
		cv::waitKey(0);

		std::cout << "program is terminated" << std::endl;

		
	}
	else {
		// Reading failed.
		std::cout << "No image" << std::endl;
	}
}


// Build function buttons.
void IMGLabelPaster::_buildGUI() {
	createButton("Rectangle", &IMGLabelPaster::_onRectangleButtonChanged, this,
		cv::QT_RADIOBOX, false);
	createButton("Circle", &IMGLabelPaster::_onCircleButtonChanged, this,
		cv::QT_RADIOBOX, false);
	createButton("Polygon", &IMGLabelPaster::_onPolygonButtonChanged, this,
		cv::QT_RADIOBOX, false);
	createButton("Brush", &IMGLabelPaster::_onBrushButtonChanged, this,
		cv::QT_RADIOBOX, false);
	createButton("Eraser", &IMGLabelPaster::_onEraserButtonChanged, this,
		cv::QT_RADIOBOX, false);
	createButton("ReginGrowing", &IMGLabelPaster::_onRegionGrowingButtonChanged, this,
		cv::QT_RADIOBOX, false);
	createButton("GrabCut", &IMGLabelPaster::_onGrabCutButtonChanged, this,
		cv::QT_RADIOBOX, false);
	createButton("MeanShit", &IMGLabelPaster::_onMeanShiftButtonChanged, this,
		cv::QT_RADIOBOX, false);
	createButton("Save", &IMGLabelPaster::_onSaveButtonChanged, this,
		cv::QT_RADIOBOX, false);


	cv::createTrackbar("Blue", this->_windowName, &IMGLabelPaster::tempBlue, 255, [](int pos, void* userdata) {
		auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);
		imgLabelPasterPointer->_transColor[0] = pos;
		}, this);
	
	cv::createTrackbar("Green", this->_windowName, &IMGLabelPaster::tempGreen, 255, [](int pos, void* userdata) {
		auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);
		imgLabelPasterPointer->_transColor[1] = pos;
		}, this);

	cv::createTrackbar("Red", this->_windowName, &IMGLabelPaster::tempRed, 255, [](int pos, void* userdata) {
		auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);
		imgLabelPasterPointer->_transColor[2] = pos;
		}, this);


	//cv::createTrackbar("Green", NULL, nullptr, 150, [](int pos, void* userdata) {
	//	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);
	//	imgLabelPasterPointer->_transColor[0] = pos});
	//cv::createTrackbar("Red", NULL, nullptr, 150, [](int pos, void* userdata) {IMGLabelPaster::_transColor[2] = pos; });
}

// CallBack method for "Rectangle" radio Button.
//state	current state of the button. It could be -1 for a push button, 0 or 1 for a check/radio box button.
//userdata	The optional parameter
void IMGLabelPaster::_onRectangleButtonChanged(int state, void* userdata) {

	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);
	if (state == 1) {
		// if the "Rectangle" radio Button is on, change the status of _drawSelection
		imgLabelPasterPointer->_drawSelection = IMGLabelPaster::Rectangle;
		if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
			delete(imgLabelPasterPointer->_shapePainterPointer);
		}
		// let the _shapePanterPointer point to a new RectanglePanter
		imgLabelPasterPointer->_shapePainterPointer = new RectanglePainter(
			imgLabelPasterPointer->_targetIMG, imgLabelPasterPointer->_originalIMG,
			imgLabelPasterPointer->_imgs_history, imgLabelPasterPointer->_windowName,
			cv::Scalar(255, 0, 0), 3,
			imgLabelPasterPointer->_transColor);
	}
}

// CallBack method for "Circle" radio Button.
void IMGLabelPaster::_onCircleButtonChanged(int state, void* userdata) {

	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);

	if (state == 1) {
		// if the "Circle" radio Button is on, change the status of _drawSelection
		imgLabelPasterPointer->_drawSelection = IMGLabelPaster::Circle;
		if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
			delete(imgLabelPasterPointer->_shapePainterPointer);
		}
		// let the _shapePanterPointer point to a new CirclePanter
		imgLabelPasterPointer->_shapePainterPointer = new CirclePainter(
			imgLabelPasterPointer->_targetIMG, imgLabelPasterPointer->_originalIMG,
			imgLabelPasterPointer->_imgs_history, imgLabelPasterPointer->_windowName,
			cv::Scalar(255, 0, 0), 3,
			imgLabelPasterPointer->_transColor);
	}
}

// CallBack method for "Polygon" radio Button.
void IMGLabelPaster::_onPolygonButtonChanged(int state, void* userdata) {

	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);


	if (state == 1) {
		// if the "Polygon" radio Button is on, change the status of _drawSelection
		imgLabelPasterPointer->_drawSelection = IMGLabelPaster::Polygon;
		if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
			delete(imgLabelPasterPointer->_shapePainterPointer);
		}

		// let the _shapePainterPointer point to a new PolygonPainter
		imgLabelPasterPointer->_shapePainterPointer = new PolygonPainter(
			imgLabelPasterPointer->_targetIMG,
			imgLabelPasterPointer->_originalIMG,
			imgLabelPasterPointer->_imgs_history,
			imgLabelPasterPointer->_windowName,
			cv::Scalar(255, 0, 0), 3,
			imgLabelPasterPointer->_transColor);
	}
}
// CallBack method for "Brush" radio Button.
//state	current state of the button. It could be -1 for a push button, 0 or 1 for a check/radio box button.
//userdata	The optional parameter
void IMGLabelPaster::_onBrushButtonChanged(int state, void* userdata) {


	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);


	if (state == 1) {
		// if the "Brush" radio Button is on, change the status of _drawSelection
		imgLabelPasterPointer->_drawSelection = IMGLabelPaster::Brush;
		if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
			delete(imgLabelPasterPointer->_shapePainterPointer);
		}
		// let the _shapePanterPointer point to a new BrushPainter
		imgLabelPasterPointer->_shapePainterPointer = new BrushPainter(
			imgLabelPasterPointer->_targetIMG, imgLabelPasterPointer->_originalIMG,
			imgLabelPasterPointer->_imgs_history, imgLabelPasterPointer->_windowName,
			cv::Scalar(255, 0, 0), 3,
			imgLabelPasterPointer->_transColor);
	}

}

// CallBack method for "Eraser" radio Button.
void IMGLabelPaster::_onEraserButtonChanged(int state, void* userdata) {

	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);


	if (state == 1) {
		// if the "Eraser" radio Button is on, change the status of _drawSelection
		imgLabelPasterPointer->_drawSelection = IMGLabelPaster::Eraser;
		if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
			delete(imgLabelPasterPointer->_shapePainterPointer);
		}
		// let the _shapePanterPointer point to a new BrushPainter
		imgLabelPasterPointer->_shapePainterPointer = new ErasePainter(
			imgLabelPasterPointer->_targetIMG, imgLabelPasterPointer->_originalIMG,
			imgLabelPasterPointer->_imgs_history, imgLabelPasterPointer->_windowName,
			cv::Scalar(255, 0, 0), 3,
			imgLabelPasterPointer->_transColor);
	}

}

// CallBack method for "Region Growing" radio Button.
void IMGLabelPaster::_onRegionGrowingButtonChanged(int state, void* userdata) {

	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);


	if (state == 1) {
		// if the "Eraser" radio Button is on, change the status of _drawSelection
		imgLabelPasterPointer->_drawSelection = IMGLabelPaster::Region_Growing;
		if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
			delete(imgLabelPasterPointer->_shapePainterPointer);
		}
		// let the _shapePanterPointer point to a new GrowRegion
		imgLabelPasterPointer->_shapePainterPointer = new GrowRegion(
			imgLabelPasterPointer->_targetIMG, imgLabelPasterPointer->_originalIMG,
			imgLabelPasterPointer->_imgs_history,
			imgLabelPasterPointer->_windowName,
			cv::Scalar(255, 0, 0), 3,
			imgLabelPasterPointer->_transColor);
	}
}


void IMGLabelPaster::_onGrabCutButtonChanged(int state, void* userdata) {

	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);
	if (state == 1) {
		// if the "GrabCut" radio Button is on, change the status of _drawSelection
		imgLabelPasterPointer->_drawSelection = IMGLabelPaster::Grab_Cut;
		if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
			delete(imgLabelPasterPointer->_shapePainterPointer);
		}
		// let the _shapePanterPointer point to a new GrabCut
		imgLabelPasterPointer->_shapePainterPointer = new GrabCut(
			imgLabelPasterPointer->_targetIMG, imgLabelPasterPointer->_originalIMG,
			imgLabelPasterPointer->_imgs_history,
			imgLabelPasterPointer->_windowName,
			cv::Scalar(255, 0, 0), 3,
			imgLabelPasterPointer->_transColor);
	}
}

void IMGLabelPaster::_onMeanShiftButtonChanged(int state, void* userdata) {

	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);
	if (state == 1) {
		// if the "GrabCut" radio Button is on, change the status of _drawSelection
		imgLabelPasterPointer->_drawSelection = IMGLabelPaster::MeanShift;
		if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
			delete(imgLabelPasterPointer->_shapePainterPointer);
		}
		// let the _shapePanterPointer point to a new GrabCut
		imgLabelPasterPointer->_shapePainterPointer = new MeanShitFilter(
			imgLabelPasterPointer->_targetIMG, imgLabelPasterPointer->_originalIMG,
			imgLabelPasterPointer->_imgs_history,
			imgLabelPasterPointer->_windowName,
			cv::Scalar(255, 0, 0), 3,
			imgLabelPasterPointer->_transColor);
	}
}

void IMGLabelPaster::_onSaveButtonChanged(int state, void* userdata) {
	auto* imgLabelPasterPointer = static_cast<IMGLabelPaster*>(userdata);
	/*if (imgLabelPasterPointer->_shapePainterPointer != nullptr) {
		delete(imgLabelPasterPointer->_shapePainterPointer);
	}*/
	imgLabelPasterPointer->_maskImgSave();
}



// Mouse Callback Method
void IMGLabelPaster::_mouseCallBack(int event, int x, int y, int flags, void* userdata) {
	auto* imgLabelPasterPoint = static_cast<IMGLabelPaster*>(userdata);
	if (imgLabelPasterPoint->_drawSelection != None) {
		imgLabelPasterPoint->_shapePainterPointer->mouseCallBack(event, x, y, flags);
	}
}

IMGLabelPaster::~IMGLabelPaster() {
	if (this->_shapePainterPointer != nullptr) {
		delete(this->_shapePainterPointer);
	}
}

bool file_exists(const std::string& str)
{
	std::ifstream fs(str);
	return fs.is_open();
}


void IMGLabelPaster::_maskImgSave() {
	cv::Mat ImgForSave = this->_originalIMG.clone();

	for (int y = 0; y < this->_originalIMG.rows; y++) {
		// ƒ|ƒCƒ“ƒ^‚ÌŽæ“¾
		cv::Vec3b* PtrTarget = this->_targetIMG.ptr<cv::Vec3b>(y);
		cv::Vec3b* PtrOriginal = this->_originalIMG.ptr<cv::Vec3b>(y);
		cv::Vec3b* PtrOutput = ImgForSave.ptr<cv::Vec3b>(y);

		for (int x = 0; x < this->_originalIMG.cols; x++) {
			if (PtrTarget[x] == PtrOriginal[x]) {
				PtrOutput[x] = cv::Vec3b(0, 0, 0);
			}
			else {
				PtrOutput[x] = PtrTarget[x];
			}
		}
	}
	

	cv::Mat ImgForSaveColor, ImgForSaveGray;

	if (ImgForSave.type() == 0) {
		ImgForSaveGray = ImgForSave.clone();
		cv::cvtColor(ImgForSave, ImgForSaveColor, cv::COLOR_GRAY2BGR);
	}
	else {
		ImgForSaveColor = ImgForSave.clone();
		cv::cvtColor(ImgForSave, ImgForSaveGray, cv::COLOR_BGR2GRAY);
	}

	const std::string window_name = "Mask Image";
	
	/*
	std::string filename;
	int i = 0; 
	cv::imshow(window_name, ImgForSaveColor);
	cv::moveWindow(window_name, 0, 0);

	do {
		filename = this->_dstImgPath + std::to_string(i) + ".png";
		std::cout << "name : " << filename << std::endl;
		i++;
	} while (file_exists(filename));
	*/
	cv::imwrite(this->_dstImgPath, ImgForSaveColor);

	std::cout << "saved mask image" << std::endl;
	std::cout << "name : " << this->_dstImgPath << std::endl;
	std::cout << "if you want to quit, press q" << std::endl;
}


