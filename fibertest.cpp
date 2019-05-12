// g++ -g fibertest.cpp fibergrid.cpp -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -oft
#include <vector>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "fibergrid.h"
#include "fiberinit.h" // FIBER_COUNT
using namespace cv;
using namespace std;


int main(int argc, char* argv[]){
    const string cfgFileName = "minmax.tmp";
    FiberGrid fg;
    if( !fg.loadState(cfgFileName) ) {
        cerr << "failed to loadState() from config file "<< cfgFileName << endl;
    }

    const char win[] = "fibers";
    namedWindow(win);
    moveWindow(win,  10, 50);

    Mat img(410, 110, CV_8UC3);
    const int LPF = 4; // lines per fiber
    const int count = min(fibergrid::FIBER_COUNT, img.rows/LPF);
    const Scalar color = 255;

    for(;;){
        const std::vector<double>& dvals = fg.readNormalized();
        img = 0; // clear the image
        for(int i = 0; i < count; ++i){
	    const int y = i*LPF+5;
	    const int x = dvals[i]*100; // lines are 0 to 100 pixels wide
	    cv::line(img, Point(5, y), Point(5+x, y), color, 2);
        }
	imshow(win,img);

	const char ESCAPE = 27;
        char c = waitKey(100);
        if('q'==c || 'Q'==c || ESCAPE==c) {
            fg.saveState(cfgFileName);
            break;
        }
    }
}
