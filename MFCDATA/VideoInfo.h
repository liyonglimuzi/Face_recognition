#pragma once
#include "opencv2/opencv.hpp"
class VideoInfo
{
public:
	CvCapture* m_capture;
	IplImage* m_iplimage;
	int m_FrameWidth;
	int m_FrameHeigth;

public:
	VideoInfo();
	~VideoInfo();

};


