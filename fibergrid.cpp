#include "fibergrid.h"
#include "fiberinit.h"
#include <vector>
#include <iostream>
#include <istream>
#include <ostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


FiberGrid:: FiberGrid(){
	if(0==fibergrid::FIBER_COUNT){
		cerr << "ERROR: no fibers found!  Rebuild fiberinit.h using fibergrid calibration utiltity!" << endl;
		exit(0);
	}
	video.open(fibergrid::CAM_ID); // OpenCV will display a message in a console if this does not work.  Should we exit here???

	for(int i=0; i<fibergrid::FIBER_COUNT; ++i){ // rebuild fibers in a convenient format. In the future fibers might contain an RGB channel
		fibers.emplace_back(fibergrid::FIBERS[2*i], fibergrid::FIBERS[2*i+1] );
	}

	mins.resize(fibergrid::FIBER_COUNT);
	maxs.resize(fibergrid::FIBER_COUNT);
	values.resize(fibergrid::FIBER_COUNT);
	normalized.resize(fibergrid::FIBER_COUNT);
	for(unsigned int& m: mins){ m = 0xFFFFFFFF; }
}

template <typename T>
void debugDrawFibers(Mat& img, const std::vector<iPoint>& fibers, const std::vector<T>& vals){
	const cv::Scalar color = CV_RGB(0,255,0); // const cv::Scalar color(0,0,255);
	const int dim = fibergrid::MIN_DISTANCE;
	for(auto& fiber: fibers){
	    cv::rectangle(img, cv::Point(fiber.x-dim/2, fiber.y-dim/2), cv::Point(fiber.x+dim/2, fiber.y+dim/2), color); // add bounds
	}

	stringstream ss;
	const size_t count = min( (size_t)(img.rows/20), vals.size() );
	for(size_t i=0; i < count; ++i){ // max 9 fibers assuming img.rows >= 200 pixels
	    ss << vals[i];
	    cv::putText(img, ss.str(), cv::Point(3, 15+i*20), 0, 0.4, color);
	    ss.str("");
	}

        ss << "image depth=" << img.depth() << " channels=" << img.channels() << endl;
	cv::putText(img, ss.str(), cv::Point(100, img.rows-10), 0, 0.4, color);
    	cv::imshow("driver_view", img);
	cv::waitKey(1);
}


unsigned int sumPixels(cv::Mat& img, const iPoint& fiber, const int dim){
	const int dim2 = dim/2; // TODO: use fibergrid::MIN_DISTANCE/2  to get constexpr
	int sum = 0;
	for(int x = fiber.x-dim2; x < fiber.x+dim2; ++x){ // TODO: check image boundaries
		for(int y = fiber.y-dim2; y < fiber.y+dim2; ++y){
			sum += img.at<unsigned char>(y,x); // notice y,x not x,y
		}
	}
	return sum;
}


const vector<unsigned int>& FiberGrid::read() {
//    cv::Mat img = cv::imread("./bin/pattern.png", CV_LOAD_IMAGE_COLOR); // DEBUG without a camera
    cv::Mat& img = video.getImage();
    if(!img.data){
    	return values;
    }

    // TODO: Do not convert to grayscale if your sensors light up fibers in one color via red/green/blue LEDs.  Use only one BRG channel.
    // TODO: add individual fiber color setting to fibergrid init utility???  #define FIBERS_HAVE_INDIVIDUAL_COLORS flag.
    cv::Mat imgGrey(img.size(), CV_8UC1);
    cv::cvtColor(img, imgGrey, CV_BGR2GRAY);

    for(size_t i = 0; i < fibers.size(); ++i){ // fibergrid::FIBER_COUNT
	values[i] = sumPixels(imgGrey, fibers[i], fibergrid::MIN_DISTANCE);
    }
#ifdef FIBERGRID_DEBUG
    debugDrawFibers(img, fibers, values); // or normalized
#endif
    return values;
}


const vector<double>& FiberGrid::readNormalized(){
    auto& vals = read();

    for(size_t i=0; i < vals.size(); ++i){
	const unsigned int v = vals[i];
	mins[i] = min(mins[i], v);
	maxs[i] = max(maxs[i], v);
//	cout << v << "[" << mins[i] << "," << maxs[i] << "]=" << maxs[i]-mins[i] << "  ";
	normalized[i] = (double)( v-mins[i] ) / (double)( maxs[i]-mins[i]+1 ); // normalized to [0,1]
    }
    return normalized;
}


bool FiberGrid::saveState(const std::string filename){ // save mins and maxs to file to be loaded later
    ofstream file(filename);
    if( !file.is_open() ){ 
	cerr << "ERROR in FiberGrid::saveState().  Could not open output file " << filename << endl;
	return false;
    }
    file << "mins=";
    for(auto m: mins){ file << m << ','; }
    file << std::endl << "maxs=";
    for(auto m: maxs){ file << m << ','; }
    file << std::endl;
    bool good = file.good();
    file.close();
    return good;
}


bool FiberGrid::loadState(const std::string filename){
    ifstream file(filename);
    if( !file.is_open() ){
	cerr << "ERROR in FiberGrid::loadState().  Could not open input file " << filename << endl;
	return false;
    }

    string line, token;
    while( getline(file, line) ){
        stringstream ss(line);
        getline(ss, token, '='); // looking for mins= or maxs=

        if( string::npos != token.find("mins") ){
	    for(size_t i=0;  i<mins.size() && getline(ss,token,',');  ++i ){
                mins[i] = stoi(token);
            }
        } else if ( string::npos != token.find("maxs") ){
	    for(size_t i=0;  i<maxs.size() && getline(ss,token,',');  ++i ){
                maxs[i] = stoi(token);
            }
        }
    }
    return !file.bad();
}
