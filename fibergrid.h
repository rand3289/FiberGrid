#ifndef INCLUDED_FIBERGRID_H
#define INCLUDED_FIBERGRID_H

#include <opencv2/opencv.hpp>
#include "ocvdef.h"
#include <vector>


// VideoCapture::get(), VideoCapture::set() these:
// CV_CAP_PROP_FRAME_WIDTH
// CV_CAP_PROP_FRAME_HEIGHT
// CV_CAP_PROP_FPS
// CV_CAP_PROP_FORMAT
// CV_CAP_PROP_MODE

// CV_CAP_PROP_BRIGHTNESS
// CV_CAP_PROP_CONTRAST
// CV_CAP_PROP_SATURATION
// CV_CAP_PROP_HUE
// CV_CAP_PROP_GAIN
// CV_CAP_PROP_EXPOSURE

// CV_CAP_PROP_AUTO_EXPOSURE
// CV_CAP_PROP_FOCUS
// CV_CAP_PROP_AUTOFOCUS

class Video {
    cv::VideoCapture cam;
    cv::Mat data;
public:
    cv::Mat& getImage(){ cam.read(data); return data; }
    int open(int devIndex) {
	bool ret = cam.open(devIndex);
	if(ret){
	    cam.set( CV_CAP_PROP_FRAME_WIDTH,  640 ); // TODO: these should come from calibration (fiberinit.h)
	    cam.set( CV_CAP_PROP_FRAME_HEIGHT, 360 );
	    cam.set( CV_CAP_PROP_FPS,          330 ); // fibergrid::FPS
	}
	return ret;
    }
};


struct iPoint { // TODO: rename to "Fiber" if more info is added (color channel ???)
	int x = 0;
	int y = 0;
	iPoint(): x(0), y(0) {}
	iPoint(const int X, const int Y): x(X), y(Y) {}
};


// Analyzes images from a camera attached to a grid of optic fibers connected to mechanical sensors
// To increase dynamic range, an area of an image sensor will be summed up for each fiber
// At first I expect the light sources to be a single color R/G/B.
// Algorithm can use this fact and threshold that channel wrt the other two to reduce the effects of daylight
// Later light sources could be of different colors to increase fiber density on the image sensor
class FiberGrid {
    Video video;
    std::vector<iPoint>  fibers;      // detected centers
    std::vector<unsigned int> mins;   // min value observed at values[i]  Used to determine the dynamic range.
    std::vector<unsigned int> maxs;   // max value observed at values[i]
    std::vector<unsigned int> values; // fiber brightness values.  Returned via read()
    std::vector<double> normalized;   // brightness values normalized to [0.0,1.0]  Returned via readNormalized()
public:
    FiberGrid();
    const std::vector<unsigned int>& read();
    const std::vector<double>&       readNormalized();
    bool saveState(const std::string filename); // save mins and maxs to file to be loaded later
    bool loadState(const std::string filename);
};


#endif // INCLUDED_FIBERGRID_H
