//
// Created by I Koku on 11/24/2019.
//

#ifndef CODE_IMGLABELPASTER_HPP
#define CODE_IMGLABELPASTER_HPP

#include "MyOpenCV.h"
#include "ShapePainter.hpp"


class IMGLabelPaster {
public:
	IMGLabelPaster(const std::string& imgPath);
	enum DrawSelection {
		None, Rectangle, Circle, Polygon, Brush, Eraser, Region_Growing, 
		Grab_Cut, MeanShift
	};
	// Main entrance.
	void start();
	
	virtual ~IMGLabelPaster();

private:
	// Store the selection that user chose.
	IMGLabelPaster::DrawSelection _drawSelection;
	// Interface of ShapePainter.
	ShapePainter* _shapePainterPointer;
	// Control the loop when to end.
	bool _terminalSignal;
	// Main target image.
	cv::Mat _targetIMG;
	cv::Mat _originalIMG;
	//Main window name for display.
	std::string _windowName;

	cv::Scalar_<int> _transColor;
	static int tempBlue, tempGreen, tempRed;

	// Reade image into _targetIMG. Return True if succeed, otherwise return false.
	bool _readIMG();
	// Configure GUI buttons
	void _buildGUI();
	std::string _imgPath;

	// Callback methods.
	static void _onRectangleButtonChanged(int state, void* userdata);
	static void _onCircleButtonChanged(int state, void* userdata);
	static void _onPolygonButtonChanged(int state, void* userdata);
	static void _onBrushButtonChanged(int state, void* userdata);
	static void _onEraserButtonChanged(int state, void* userdata);
	static void _onRegionGrowingButtonChanged(int state, void* userdata);
	static void _onGrabCutButtonChanged(int state, void* userdata);
	static void _onMeanShiftButtonChanged(int state, void* userdata);
	static void _onSaveButtonChanged(int state, void* userdata);
	static void _mouseCallBack(int event, int x, int y, int flags, void* userdata);


	// forSave
	void _maskImgSave();

private:
	std::vector<cv::Mat> _imgs_history;
};


#endif //CODE_IMGLABELPASTER_HPP
