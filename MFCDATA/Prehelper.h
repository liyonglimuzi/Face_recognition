//Ô¤´¦Àíº¯Êý

#pragma once
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"  
#include <cv.h>  
#include <vector>  
#include <utility> 
using namespace cv;
using namespace std;

class Prehelper
{
public:
	int pred_label;
public:
	void normalizeone(const char* dir, IplImage* standard);

	void CutImg(IplImage* src, CvRect rect, IplImage* res);

	vector<Rect> detectAndDraw(Mat& img, CascadeClassifier& cascade,
		CascadeClassifier& nestedCascade,
		double scale, bool tryflip, bool draw);

	pair<IplImage*, Rect> DetectandExtract(Mat& img, CascadeClassifier& cascade,
		CascadeClassifier& nestedCascade,
		double scale, bool tryflip);

	int read_img(const string& dir, vector<Mat> &images);
	void resizeandtogray(char* dirr, int K, vector<Mat>&images, vector<int>&labels, vector<Mat>&testimages, vector<int>&testlabels);
	vector<pair<string, Mat>>  read_img(const string& dir);

	Ptr<FaceRecognizer> Recognition(vector<Mat> &images, vector<int> &labels,
		vector<Mat> &testimages, vector<int> &testlabels);
	IplImage* Prehelper::CaptureandRecognize(Ptr<FaceRecognizer>model, CvCapture* capture);
	IplImage* Prehelper::StartFace(CvCapture* capture);
	int Prehelper::getData();
};
