// Calibration program for fibergrid
// creates a C++ include file with coordinates of fibers found in the device image
// This program requires OpenCV library to work
// to link with OpenCV 3.x binaries:
// g++ fibercal.cpp -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -ofc
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <iomanip> // format stream printing
#include <vector>
#include <algorithm>
using namespace std;
using namespace cv;


namespace mycv {

struct Range {
    int start;    // start of the hill
    int end;      // end of the hill
    int y;        // row in the matrix
    int val;      // height of the hill
    int islandID; // Range is an island slice one row (pixel) wide.  Island can spawn multiple rows.
    Range(int start_, int end_, int Y, int value): start(start_), end(end_), y(Y), val(value), islandID(-1) { }
    bool less(const Range& rhs) const { return end < rhs.start-1; }
    bool operator<(const Range& rhs) const { return end < rhs.start-1; }
};


template <typename T>
struct MatIter { // Matrix Iterator - iterates over a single line (dimention of the matrix) on the x axis
	int x, y;
	T& mat; // deleting the Mat invalidates the iterator
	MatIter(T& matrix, int X, int Y): x(X), y(Y), mat(matrix) {}
	MatIter(const MatIter& rhs): x(rhs.x), y(rhs.y), mat(rhs.mat) {}
	MatIter& operator=(const MatIter& rhs){ x = rhs.x; y = rhs.y; mat = rhs.mat; return *this; }
	MatIter& operator++() { ++x; return *this; }
	bool operator==(const MatIter& rhs) const { return rhs.x==x; } // && rhs.y==y && rhs.mat==mat
	bool operator!=(const MatIter& rhs) const { return rhs.x!=x; } // || rhs.y!=y || rhs.mat!=mat
	int operator*() const { return mat.template at<unsigned char>(y, x); }
};


template <typename T>
ostream& operator<<(ostream& os, const MatIter<T>& it){
	return os << "{" << it.x << "," << it.y << "}";
}
ostream& operator<<(ostream& os, const Range& r) {
	return os << r.val << '[' << r.start << '-' << r.end << ',' << r.y << ']' << r.islandID << ';';
}
ostream& operator<<(ostream& os, std::vector<Range>& row){
	for(auto i: row){
		os << i << ' ';
	}
	return os;
}


pair<int,int> findCenter(vector<Range>& cluster){
	int x = 0;
	int y = 0;
	int cnt = 0;
	for(Range& r: cluster){
		x+=(r.start+r.end)/2;
		y+=r.y;
		++cnt;
	}
	return cnt ? make_pair<int,int>(x/cnt,y/cnt) : make_pair<int,int>(-1,-1);
}


// O(n) = n
template <typename T>
void findPeaks(const MatIter<T>& beginLine, const MatIter<T>& endLine, vector<Range>& peaks) {
	const int THRESHOLD = 1; // how high should the island be to be inserted into peaks?
	if(beginLine == endLine) { return; }
	MatIter<T> begin = beginLine;
	MatIter<T> prev  = beginLine;
	MatIter<T> it    = beginLine;
	++it;
	if(endLine==it){ return; } // image has one column???
	bool uphill = *it > *prev; // starting uphill or downhill?
	int val = uphill ? *it : *prev;

	for(; it != endLine; prev = it, ++it){
		if( (*it >= *prev) && !uphill){ // reached a valley or a plateau. plaeau will start a new island
			if (val > THRESHOLD && begin.x != prev.x){ // && more than one pixel wide
				peaks.push_back(Range(begin.x, prev.x, begin.y, val) );
			}
			begin = prev; // hills share a valley (not setting begin = it)
			val = *it;
			uphill = true;  // start climing up the next hill
		} else if ( (*it <= *prev) && uphill){ // top of the hill
			val = *prev; // save the highest value on the hump
			uphill = false;
		}
	}

	if(begin!=it){ // we have reached the last pixel, make sure it's not just one pixel
	    int height = max(val,*prev);
	    if(height > THRESHOLD){
	        peaks.push_back(Range(begin.x, prev.x, begin.y, height) );
	    }
	}
}


// O(n) = n*log2(n)
void alignPeaks(vector<Range>& row1, vector<Range>& row2, vector<vector<Range> >& peaks){
	for(auto& r2: row2){ // as we are scanning down the image, iterate over the lower of the two rows
	    int& id2 = r2.islandID;
	    for(auto r1 = std::lower_bound(row1.begin(),row1.end(), r2);  r1 != row1.end();  ++r1 ){ // log2(n)
//		if(r1->end < r2.start-1 ){ continue; }
		if(r2.end < r1->start-1 ){ break; }
		const int id1 = r1->islandID;
		if(id2 < 0){ // this range is not yet affiliated with an island
		    id2 = id1;
		    peaks[id2].push_back(r2);
		} else if(id1 != id2){ // this range already belongs to another island - join the two islands
		    for(auto& range : peaks[id1]){ range.islandID = id2; }
		    std::move(peaks[id1].begin(), peaks[id1].end(), back_inserter(peaks[id2]) );
		    peaks[id1].clear();
		}
	    }
	    if(id2 < 0){ // start a new island
		id2 = peaks.size();
		peaks.emplace( peaks.end() ); // new vector
		peaks[id2].push_back(r2);
	    }
        }
}

/*
// WORKS!!! slow O(n) = n^2
void alignPeaks2(vector<Range>& row1, vector<Range>& row2, vector<vector<Range> >& peaks){
	for(auto& r2: row2){ // as we are scanning down the image, iterate over the lower of the two rows
	    int& id2 = r2.islandID;
	    for(auto& r1: row1){
		if(r1.end < r2.start-1 ){ continue; }
		if(r2.end < r1.start-1 ){ break; }
		const int id1 = r1.islandID;
		if(id2 < 0){ // this range is not yet affiliated with an island
		    id2 = id1;
		    peaks[id2].push_back(r2);
		} else if(id1 != id2){ // this range already belongs to another island - join the two islands
		    for(auto& range : peaks[id1]){ range.islandID = id2; }
		    std::move(peaks[id1].begin(), peaks[id1].end(), back_inserter(peaks[id2]) );
		    peaks[id1].clear();
		}
	    }
	    if(id2 < 0){ // start a new island
		id2 = peaks.size();
		peaks.emplace( peaks.end() ); // new vector
		peaks[id2].push_back(r2);
	    }
        }
}


// DOES NOT WORK!!!
void alignPeaks3(vector<Range>& row1, vector<Range>& row2, vector<vector<Range> >& peaks){
	auto r1 = row1.begin();
	auto r2 = row2.begin(); // we are scanning down 2 rows of the image at the same time

        for( ; r1 != row1.end(); ++r1 ){
	    for( ; r2 != row2.end(); ++r2 ){
		if(r1->end < r2->start-1) { break; }
		if(r2->end < r1->start-1) { continue;    }  // otherwise r1 & r2 are aligned (overlap)
		const int id1 = r1->islandID; // this range's island id does not change unless islands are joined
	        int& id2 = r2->islandID;

		if(id2 < 0){                  // this range is not yet affiliated with an island
		    id2 = id1;
		    peaks[id2].push_back(*r2);
		} else if(id1 != id2){        // this range already belongs to another island - join the two islands
		    for(auto& range : peaks[id1]){ range.islandID = id2; }
		    std::move(peaks[id1].begin(), peaks[id1].end(), back_inserter(peaks[id2]) );
		    peaks[id1].clear();
		}                             // else they are already parts of the same island
	    } // for r1
        } // for r2

	for(auto& ro2: row2){ // all ranges that are not part of the island get their own island
	    if(ro2.islandID < 0){            // start a new island - range in row 2 does not have an islandID
		ro2.islandID = peaks.size(); // how many islands are there already?
		peaks.emplace_back();        // new vector (new island)
		peaks.back().push_back(ro2); // new Range in the island
	    }
	}
}
*/

// Peak finding algorithm: find peaks in 1D then find adjacent ranges in 2D
// caller discards islands with 0 peaks
template <typename T>
void findPeaks2D(T& img, vector< vector<Range> >& clusters) {
	vector<Range> row;  // peaks from a single row
	vector<Range> prev; // peaks from a previous row
	for(int y = 0; y < img.rows; ++y){
		findPeaks( MatIter<T>(img, 0, y), MatIter<T>(img, img.cols, y), row);
		alignPeaks(prev, row, clusters); // arrange peaks into islands
		prev.swap(row);
		row.clear();
//		if(prev.size() > 0) { cout << y << '#' << prev.size() << ' '; }
	}
}

/*
void debugDrawFibers(vector<vector<mycv::Range> >& islands, cv::Mat& img){
    const cv::Scalar color = CV_RGB(255,255,255);
    for(auto& island: islands){
	for(auto& range: island){
		cv::line(img, cv::Point(range.start,range.y), cv::Point(range.end,range.y), color);
	}
    }
}
*/

int findFibers(cv::Mat& img, vector<cv::Point2f>& fibers) {
    vector<vector<mycv::Range> > islands;
    mycv::findPeaks2D(img,islands);
//    debugDrawFibers(islands, img);

    fibers.clear();
    for(vector<mycv::Range>& isl: islands){
	if(isl.size() > 0){
	    std::pair<int,int> center = mycv::findCenter(isl);
	    fibers.push_back(cv::Point2f(center.first,center.second));
	}
    }

    // find mininum distance between fibers.
    float minD = img.rows/3;
    for(size_t i=0; i<fibers.size(); ++i){
	for(size_t j=i+1; j < fibers.size(); ++j){
            float dx = fibers[i].x - fibers[j].x;
            float dy = fibers[i].y - fibers[j].y;
	    float d = sqrt(dx*dx + dy*dy); // hypotenuse
	    d = sqrt( (d*d) / 2 ); // triangle side length in equalateral triangle with hypotenuse d
            minD = min(minD,d);
        }
    }

//    cout << 'I' << islands.size() << "F" << fibers.size() << "M" << ( (int)minD<0 ? -1 : (int)minD) << ' ';
//    cout.flush();
    return minD;
}


} // namespace mycv


/******************************* main() related *******************************/

struct FiberInfo {
    int camID = 0;
    int minD = 0;
    std::vector<cv::Point2f> fibers;      // detected fiber centers
};


#include <time.h> // time(), localtime()
// save coordinates of the fibers in the video and other settings to a C++ header file to be #included by fibergrid library
void saveSettings(FiberInfo& info){
    string filename("fiberinit.h");
    cout << "Saving fibergrid calibration values to " << filename << endl;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    ofstream fo(filename);
    fo << "#ifndef INCLUDED_FIBERINIT_H" << endl;
    fo << "#define INCLUDED_FIBERINIT_H" << endl;
    fo << "// This file is auto generated by fibergrid calibration utility on " << setfill('0');       // prepare to print numbers as 01
    fo << 1900+tm.tm_year << "-" << setw(2) << tm.tm_mon+1 << "-" << setw(2) << tm.tm_mday << " ";     // yyyy-mm-dd
    fo << setw(2) << tm.tm_hour << ":" << setw(2) << tm.tm_min << ":" << setw(2) << tm.tm_sec << endl; // hh:mm:ss
    fo << "#include <vector>" << endl;
    fo << "#define FIBERGRID_DEBUG \"Comment this line to disable driver's debug video window.\"" << endl;
    fo << "namespace fibergrid {" << endl;
    fo << "    const int CAM_ID = " << info.camID << ";" << endl;
    fo << "    const int MIN_DISTANCE = " << info.minD << ";" << endl;
    fo << "    const int FIBER_COUNT = " << info.fibers.size() << ";" << endl;
    fo << "    const std::vector<int> FIBERS = {";  //    fo << "    const int FIBERS [] = {";
    int i = -1;
    for(auto f: info.fibers){
        fo << (++i ? "," : "");
        fo << endl;
        fo << "        " << (int) f.x << ", " << (int) f.y;
    }
    fo << endl;
    fo << "    }; // FIBERS" << endl;
    fo << "} // namespace fibergrid" << endl;
    fo << "#endif";
}


int getVideoSrcCount(cv::VideoCapture& cam){ // how many video sources are there in the current system?
    int i = 0;
    for( ; cam.open(i); ++i ) {
        cam.release(); 
    }
    return i;
}


struct VideoInfo{
	cv::VideoCapture& cam;
	const string& file;
	const int max; // max possible trackbar value
	VideoInfo(cv::VideoCapture& camera, string& fileName, int maxIndex): cam(camera), file(fileName), max(maxIndex) {}
};

void idChanged(int pos, void* userData){ // trackbar callback
	VideoInfo& inf = *static_cast<VideoInfo*>(userData);
	inf.cam.release();
	if( inf.max==pos  &&  inf.file.length() > 0){
	    inf.cam.open(inf.file);
	} else {
	    inf.cam.open(pos);
	}
}


// allow user to add or delete fibers by clicking on the video
void mouse(int event, int x, int y, int flags, void* userdata){
	if( EVENT_LBUTTONUP != event ) { return; }
	FiberInfo* info = static_cast<FiberInfo*> (userdata);
	std::vector<cv::Point2f>& fibers = info->fibers;

	auto found = std::find_if(fibers.begin(), fibers.end(), [=](const cv::Point2f& pt){ 
		const int dx = pt.x - x;
		const int dy = pt.y - y;
		return info->minD > sqrt(dx*dx+dy*dy); // find distance between points (hypotenuse)
	} );
	if(fibers.end() ==  found){
		fibers.emplace_back((float)x, (float)y);
	} else {
		fibers.erase(found);
	}
}


#include <chrono>
double getFps(){
	static int count = 0;
	static double fps = 0;
	static auto start = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	++count;
	if( now > start + std::chrono::seconds(1) ){
		fps = 1000.0 * (double)count / (double) std::chrono::duration_cast<std::chrono::milliseconds> (now - start).count();
		start = now;
		count = 0;
	}
	return fps;
}


int main(int argc, char* argv[]){
    cv::VideoCapture cam;
    const int camCount = getVideoSrcCount(cam);
    cout << "Found " << camCount << " video sources" << endl;

    FiberInfo info;
    info.camID = camCount; // for use with files, set the trackbar to the last value (non-existent device)

    string file(argc==2 ? argv[1] : "");
    if(argc == 2){
        if( cam.open(file) ) {
            cout << "Using file " << file << " provided on command line as the video source." << endl;
        } else {
            cerr << "File " << file << " not found!" << endl;
	    exit(1);
        }
    } else if(camCount > 0) {
	cam.open( --info.camID ); // convert device count to the largest existing zero based device index and open it
    }

    const char ctlWin[] = "fibergrid calibration utility";
    const char dataWin[] = "fibers";
    namedWindow(ctlWin);
    namedWindow(dataWin);
    moveWindow(ctlWin,  10, 50);
    moveWindow(dataWin, 400, 50);
    setMouseCallback(dataWin, mouse, (void*)&info);

    int calibrate = 0;
    int threshold = 100;
    VideoInfo videoInfo(cam, file, max(1, camCount) );

    createTrackbar( "Video source", ctlWin, &info.camID, videoInfo.max, idChanged, (void*)&videoInfo );
    createTrackbar( "Calibrate  Hold  Clear", ctlWin, &calibrate, 2); // types of calibration
    createTrackbar( "Fiber size",  ctlWin, &info.minD, 50);
    createTrackbar( "Detection threshold", ctlWin, &threshold, 250);

    const cv::Scalar colorR = CV_RGB(255,0,0); // const cv::Scalar color(0,0,255);
    const cv::Scalar colorG = CV_RGB(0,255,0);
    const cv::Scalar colorB = CV_RGB(0,0,255);

    cv::Mat img( 640, 200, CV_8U );
    cv::Mat imgGrey;
    cv::Mat imgThresh;
    cv::Mat imgTemp;
    cv::Mat imgZoom;
    cv::Mat imgOut;

    for(;;){
	if( cam.isOpened() ){
	    if( cam.read(imgTemp) ){ // file can run out of frames and device can get disconnected
		img = imgTemp;
        	cv::cvtColor(img, imgGrey, CV_BGR2GRAY);
	    } else {
		cam.release();
	    }
        } else {
	    cv::putText(img, "STATIC", cv::Point(3,30), 0, 1.0, colorR, 2);
	}

	cv::threshold(imgGrey, imgThresh, threshold+2, 255, CV_THRESH_BINARY);

	if(calibrate == 0){ // TODO: compute finbers without color conversion & thresholding
            info.minD = mycv::findFibers(imgThresh, info.fibers);
	    cv::setTrackbarPos("Fiber size", ctlWin, info.minD);
	} else if (calibrate == 2){ // 0=calibrate, 1=hold, 2=clear
	    info.fibers.clear();
	}

	imgOut = img.clone(); // unfortunately, if video stops, the only way to get a clean video frame is to copy
	const unsigned int dim2 = info.minD/2;
	for(auto& fiber: info.fibers){
	    cv::rectangle(imgOut, cv::Point(fiber.x-dim2, fiber.y-dim2), cv::Point(fiber.x+dim2, fiber.y+dim2), colorG); // draw bounds
	}

        stringstream ss;
	ss << "click to add/remove fibers, s to save, q to quit.  fibers=" << info.fibers.size() << " fps=" << std::setprecision(3) << getFps();
	const cv::Point xy(5,img.rows-5);
	cv::putText(imgOut, ss.str(), xy, 0, 0.4, colorB);
        imshow(dataWin, imgOut);

	// zoom a portion of an image to see the actual pixels lit up by fibers
	const cv::Rect roi( (3*img.cols)/8, (3*img.rows)/8, img.cols/4, img.rows/4);
	Mat zoomed = imgThresh(roi); // select a region of the image in the center
	cv::resize(zoomed, imgZoom, cv::Size(), 2.0, 2.0); // zoom 2x
	imshow(ctlWin, imgZoom);

        char c = waitKey(1);
	const char ESCAPE = 27;
        if('q'==c || 'Q'==c || ESCAPE==c) { break; }
        if('s'==c || 'S'==c ) { saveSettings(info); }
    }
}
