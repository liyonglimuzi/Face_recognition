#include "stdafx.h"
#include "Prehelper.h"
#include "BrowseDir.h" 
#include "MFCDATADlg.h"
#include "StatDir.h"  
#include "ShowData.h"
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <cv.h> 
#include <contrib/contrib.hpp>
#include <imgproc/imgproc.hpp>
#include <objdetect/objdetect.hpp>
#include"Ado.h"
#define _STDC_WANT_LIB_EXET_ 1
#include<string.h>

using namespace cv;

CascadeClassifier cascade, nestedcascade;
bool tryflip = false;
double scale = 1.0;
string cascadeName = "D:/opencv/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
string nestedCascadeName = "D:/opencv/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
char dirmodel[256];
char *dir = "C:\\Users\\Administrator\\Desktop\\";

FILE *pfile = NULL;

#define CAM 2
#define PHO 1
//#define K 5

void Prehelper::normalizeone(const char* dir, IplImage* standard)//裁剪图片，使得大小一样
{
	StatDir statdir;
	if (!statdir.SetInitDir(dir))
	{
		AfxMessageBox(_T("路径不存在"), MB_ICONHAND);
		return;
	}
	vector<string>file_vec = statdir.BeginBrowseFilenames("*.*");
	int i;
	for (i = 0; i < file_vec.size(); i++)
	{
		IplImage* cur_img = cvLoadImage(file_vec[i].c_str(), CV_LOAD_IMAGE_GRAYSCALE);
		cvResize(cur_img, standard, CV_INTER_AREA);
		cvSaveImage(file_vec[i].c_str(), cur_img);

	}

}
void Prehelper::CutImg(IplImage *src, CvRect rect, IplImage *res)
{
	CvSize img_size;
	img_size.height = rect.height;
	img_size.width = rect.width;
	cvSetImageROI(src, rect);
	cvCopy(src, res);
	cvResetImageROI(res);
}
int Prehelper::read_img(const string& dir, vector<Mat> &images)//读入训练图片
{
	StatDir statdir;
	if (!statdir.SetInitDir(dir.c_str()))
	{
		//	cout << "Direct " << dir << "  not exist!" << endl;
		return 0;
	}
	int cls_id = dir[dir.length() - 1] - '0';
	vector<string>file_vec = statdir.BeginBrowseFilenames("*.*");
	int i, s = file_vec.size();
	for (i = 0; i<s; i++)
	{
		Mat graymat = imread(file_vec[i], 0);
		//graymat.reshape(1,1);//flatten to one row  
		images.push_back(graymat);
	}
	return s;

}
vector<pair<string, Mat>> Prehelper::read_img(const string& dir)//读入训练图片
{
	StatDir statdir;
	pair<string, Mat> pfi;
	typedef vector<pair<string, Mat>> Vp;
	Vp vp;
	if (!statdir.SetInitDir(dir.c_str()))
	{
		//cout << "Direct " << dir << "  not exist!" << endl;
		return vp;
	}
	int cls_id = dir[dir.length() - 1] - '0';
	vector<string>file_vec;

	file_vec = statdir.BeginBrowseFilenames("*.*");

	int i, s = file_vec.size();
	for (i = 0; i<s; i++)
	{
		pfi.first = file_vec.at(i);
		//cout << file_vec.at(i) << endl;
		//char imagename = file_vec[i];
		pfi.second = imread(pfi.first);


		vp.push_back(make_pair(pfi.first, pfi.second));

	}

	return vp;
}

vector<Rect>Prehelper::detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool tryflip, bool draw)//检测并人脸并且圈出来
{
	int i = 0;
	double t = 0;
	vector<Rect> faces, faces2;
	const static Scalar colors[] = { CV_RGB(0, 0, 255),
		CV_RGB(0, 128, 255),
		CV_RGB(0, 255, 255),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 128, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(255, 0, 0),
		CV_RGB(255, 0, 255) };
	Mat gray, smallImg(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);

	cvtColor(img, gray, CV_BGR2GRAY);
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	t = (double)cvGetTickCount();
	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		| CV_HAAR_FIND_BIGGEST_OBJECT
		//|CV_HAAR_DO_ROUGH_SEARCH  
		//|CV_HAAR_SCALE_IMAGE  
		,
		Size(30, 30));
	if (tryflip)
	{
		flip(smallImg, smallImg, 1);
		cascade.detectMultiScale(smallImg, faces2,
			1.1, 2, 0
			| CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH  
			//|CV_HAAR_SCALE_IMAGE  
			,
			Size(30, 30));
		for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++)
		{
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}
	t = (double)cvGetTickCount() - t;

	//strcpy_s(dirmodel, dir); strcat_s(dirmodel, "model.txt");
	//if (fopen_s(&pfile, dirmodel, "a"));
	//{
	//	fprintf_s(pfile, "detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000));
	//	fclose(pfile);
	//	pfile = NULL;
	//	//exit(1);
	//}
	//fwrite(, sizeof(model), 1, pfile);

	//printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
	if (draw)
	{
		for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++)
		{
			Mat smallImgROI;
			vector<Rect> nestedObjects;
			Point center;
			Scalar color = colors[i % 8];
			//			int radius;

			double aspect_ratio = (double)r->width / r->height;
			rectangle(img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
				cvPoint(cvRound((r->x + r->width - 1)*scale), cvRound((r->y + r->height - 1)*scale)),
				color, 3, 8, 0);
			if (nestedCascade.empty())
				continue;
			smallImgROI = smallImg(*r);
			nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
				1.1, 2, 0
				| CV_HAAR_FIND_BIGGEST_OBJECT
				//|CV_HAAR_DO_ROUGH_SEARCH  
				//|CV_HAAR_DO_CANNY_PRUNING  
				//|CV_HAAR_SCALE_IMAGE  
				,
				Size(30, 30));
			//draw eyes  
			//         for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )  
			//         {  
			//             center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);  
			//             center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);  
			//             radius = cvRound((nr->width + nr->height)*0.25*scale);  
			//             circle( img, center, radius, color, 3, 8, 0 );  
			//         }  
		}
		//cv::imshow("result", img);
		//cvWaitKey();
	}
	return faces;
}

pair<IplImage*, Rect> Prehelper::DetectandExtract(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool tryflip)
{
	vector<Rect> Rvec = detectAndDraw(img, cascade, nestedCascade, scale, tryflip, 0);
	int i, maxxsize = 0, id = -1, area;
	for (i = 0; i<Rvec.size(); i++)
	{
		area = Rvec[i].width*Rvec[i].height;
		if (maxxsize<area)
		{
			maxxsize = area;
			id = i;
		}
	}
	IplImage* transimg = cvCloneImage(&(IplImage)img);
	pair<IplImage*, Rect> p;
	if (id != -1)
	{
		CvSize imgsize;
		imgsize.height = Rvec[id].height;
		imgsize.width = Rvec[id].width;
		IplImage* res = cvCreateImage(imgsize, transimg->depth, transimg->nChannels);
		CutImg(transimg, Rvec[id], res);
		p.first = res; p.second = Rvec[id];

	}
	return p;
}
void Prehelper::resizeandtogray(char* dirr, int k, vector<Mat> &images, vector<int>&labels,
	vector<Mat>&testimages, vector<int>&testlabels)//调整大小和灰度化
{
	IplImage* standard = cvLoadImage("C:\\Users\\Administrator\\Desktop\\Face_recognition\\5\\xi (3).jpg", CV_LOAD_IMAGE_GRAYSCALE);

	string cur_dir;
	char id[5];
	int i, j;

	for (int i = 1; i <= k; i++)
	{
		cur_dir = dirr;

		cur_dir.append("\\");

		_itoa_s(i, id, 10);
		cur_dir.append(id);
		const char* dd = cur_dir.c_str();
		StatDir statdir;
		if (!statdir.SetInitDir(dd))
		{
			AfxMessageBox(_T("Dir not exist"));
			return;
		}
		//cout << "Processing samples in Class " << i << endl;
		vector<string>file_vec = statdir.BeginBrowseFilenames("*.*");
		for (j = 0; j<file_vec.size(); j++)
		{
			IplImage* cur_img = cvLoadImage(file_vec[j].c_str(), CV_LOAD_IMAGE_GRAYSCALE);
			cvResize(cur_img, standard, CV_INTER_AREA);
			Mat cur_mat = cvarrToMat(standard, true), des_mat;
			normalize(cur_mat, des_mat, 0, 255, NORM_MINMAX, CV_8UC1);
			cvSaveImage(file_vec[j].c_str(), cvCloneImage(&(IplImage)des_mat));
			if (j != file_vec.size() - 2)
			{
				images.push_back(des_mat);
				labels.push_back(i);
			}
			else
			{
				testimages.push_back(des_mat);
				testlabels.push_back(i);
			}
		}
		//cout << file_vec.size() << " images." << endl;
	}
}
Ptr<FaceRecognizer>Prehelper::Recognition
(vector<Mat> &images, vector<int>& labels,
vector<Mat> &testimages, vector<int> &testlabels)//训练出模型
{
	Ptr<FaceRecognizer> model = createEigenFaceRecognizer(15);//10 Principal components  
	//cout << "train" << endl;
	model->train(images, labels);
	int i, acc = 0, predict_l;
	for (i = 0; i<testimages.size(); i++)
	{
		predict_l = model->predict(testimages[i]);
		if (predict_l != testlabels[i])
		{
			//	cout << "An error in recognition: sample " << i + 1 << ", predict " <<
			//		predict_l << ", groundtruth " << testlabels[i] << endl;
			//	imshow("error 1", testimages[i]);
			//	waitKey();
		}
		else
			acc++;
	}
	//cout << "Recognition Rate: " << acc*1.0 / testimages.size() << endl;
	//cout << "Recognition Rate: " << acc << endl;
	return model;
}
void Init()//初始化
{

	if ((!cascade.load(cascadeName)) || (!nestedcascade.load(nestedCascadeName)))
	{
		AfxMessageBox(_T("error:不能装载cascade或者nestedcascade"));
		return;
	}
}
IplImage* Prehelper::CaptureandRecognize(Ptr<FaceRecognizer>model, CvCapture* capture)//打开摄像头并且识别
{
	CBrowseDir browsedir;

	Mat frame, frameCopy, image;
	Init();
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);
	char strlabel[10];
	IplImage* iplimage;
	int Num_Same = 0;
	int	Number = 0;
	CMFCDATADlg showdata;
	
	while (true)
	{
		iplimage = cvQueryFrame(capture);
		frame = iplimage;
		if (frame.empty())
			break;
		if (iplimage->origin == IPL_ORIGIN_TL)
			frame.copyTo(frameCopy);
		else
			flip(frame, frameCopy, 0);

		pair<IplImage*, Rect> p = DetectandExtract(frameCopy, cascade, nestedcascade, scale, tryflip);
		IplImage* ori_detected = p.first;
		Rect r = p.second;
		if (ori_detected &&strlen(ori_detected->imageData)>10)
		{
			IplImage* gray_detected = cvCreateImage(cvGetSize(ori_detected), ori_detected->depth, 1);
			cvCvtColor(ori_detected, gray_detected, CV_RGB2GRAY);
			IplImage* detected = cvLoadImage("C:\\Users\\Administrator\\Desktop\\Face_recognition\\4\\lijunlin (5).jpg", CV_LOAD_IMAGE_GRAYSCALE);
			cvResize(gray_detected, detected, 1);
			pred_label = model->predict(cvarrToMat(detected, true));
			//if (fopen_s(&pfile, dirmodel, "a"));
			//{
			//	fprintf_s(pfile, "这是第%d\n", pred_label);
			//	fclose(pfile);
			//	pfile = NULL;
			//	//exit(1);

			//}

			//cout << pred_label << endl;
			_itoa_s(pred_label, strlabel, 10);;
			cvRectangle(iplimage, cvPoint(r.x, r.y), cvPoint(r.x + r.width, r.y + r.height), cvScalar(50, 255, 50), 3);
			cvPutText(iplimage, strlabel, cvPoint(r.x + r.width, r.y - 20), &font, CV_RGB(255, 0, 0));//在图片中输出字符
			//cvShowImage("Show", iplImg);
			if (Number == pred_label)
			{
				Num_Same = Num_Same + 1;
			}

			Number = pred_label;

		}
		if ((Num_Same == 3) && iplimage)
		{
			Num_Same = 0;
			Number = 0;
			getData();
			break;


			return iplimage;
		}

		/*if (waitKey(10) >= 0)
		goto _cleanup_;*/
	}

	/*waitKey();

	_cleanup_:
	cvReleaseCapture(&capture);*/
	return iplimage;
}
IplImage* Prehelper::StartFace(CvCapture* capture)//为了主程序的简洁性，特意新建的一个类，重要是初始化各种变量
{
	
	CMFCDATADlg facedlg;
	Mat frame, framecope;
	string inputName;
	IplImage* iplImg;
	int mode;
	//CascadeClassifier cascade, nestedcascade;
	//double scale = 1.0;
	//if ((!cascade.load(cascadeName)) || (!nestedcascade.load(nestedCascadeName)))
	//{
	//	cerr << "Error:不能装载cascade或者nestedcascade" << endl;
	//	return -1;
	//}
	//char** pics = (char**)malloc(sizeof*pics);
	///************************************************************************/
	///*                                  detect face and save                                    */
	///************************************************************************/
	int i, j;
	//cout << "detect and save..." << endl;

	//char dir[256] = "C:\\Users\\Administrator\\Desktop\\";
	//CString cur;
	//string cur_dir;
	//=new char[256];
	vector<Mat> images, testimages;
	vector<int> labels, testlabels;
	char *diri = facedlg.GetEditString();//获取用户选择的路径
	char dir[256];
	strcpy_s(dir, sizeof(dir), diri);
	ShowData showdata;
	int K = showdata.Folder(dir);//在大文件夹中有几个小文夹
	

	
	/*;char id[K]
	for (i = 1; i <= K; i++)
	{
	cur_dir = dir;
	_itoa_s(i, id, 10);
	cur_dir.append("Face_recognition\\");
	cur_dir.append(id);
	}*/
	//	vector<pair<string, Mat>> imgs = prehelper.read_img(cur_dir);
	//	for (j = 0;j<imgs.size(); j++)
	//	{   //imshow(imgs[j].first, imgs[j].second);
	//		IplImage* res = prehelper.DetectandExtract(imgs[j].second, cascade,
	//			nestedcascade, scale, tryflip);
	//	
	//		if (res)
	//			cvSaveImage(imgs[j].first.c_str(),res);
	//		
	//		//res = NULL;
	//	}
	//}

	//togray, normalize and resize; load to images,labels,testimages,testlabels  

	resizeandtogray(dir, K, images, labels, testimages, testlabels);
	//recognition  
	Ptr<FaceRecognizer> model = Recognition(images, labels, testimages, testlabels);
	iplImg = CaptureandRecognize(model, capture);

	//system("pause");
	return iplImg;

}

int Prehelper::getData()//获取检测到的人脸的序号
{
	int	number = pred_label;;
	return number;
}