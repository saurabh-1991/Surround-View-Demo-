//============================================================================
// Name        : ImageRead.cpp
// Author      : Saurabh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>

#include "opencv4/opencv2/opencv.hpp"
#include "opencv4/opencv2/highgui/highgui.hpp"
#include "opencv4/opencv2/opencv_modules.hpp"
#include "opencv4/opencv2/core/core.hpp"
#include "opencv4/opencv2/imgproc.hpp"
#include "opencv4/opencv2/imgcodecs.hpp"


using namespace std;
using namespace cv;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	// Read the image file
	Mat MyImage,FrontView,RearView,LeftView,RightView,
	ResizedFrontView,ResizedRearView,ResizedLeftView,ResizedRightView,
	LeftRotateImage,RightRotateImage,TopViewCar,Resize_TopViewCar;
//	MyImage = imread("/home/saurabh/MI-EE-Architecture/SVS_Experiments/SorroundView.jpeg");
	TopViewCar = imread("/home/saurabh/MI-EE-Architecture/SVS_Experiments/SurroundViewImages/carTopView1.png");
	FrontView = imread("/home/saurabh/MI-EE-Architecture/SVS_Experiments/SurroundViewImages/FV.jpeg");
	RearView = imread("/home/saurabh/MI-EE-Architecture/SVS_Experiments/SurroundViewImages/RV.jpeg");
	LeftView = imread("/home/saurabh/MI-EE-Architecture/SVS_Experiments/SurroundViewImages/LeftView.jpeg");
	RightView = imread("/home/saurabh/MI-EE-Architecture/SVS_Experiments/SurroundViewImages/RightView.jpeg");

	/*------------Resize Camera Images  ---- ---------------------- */
	auto ResizeDim_Col = 640;
	auto ResizeDim_Rows = 240;
	resize(FrontView, ResizedFrontView, Size(ResizeDim_Col, ResizeDim_Rows), INTER_LINEAR);
	resize(RearView, ResizedRearView, Size(ResizeDim_Col, ResizeDim_Rows), INTER_LINEAR);
	resize(LeftView, ResizedLeftView, Size(ResizeDim_Col, ResizeDim_Rows), INTER_LINEAR);
	resize(RightView, ResizedRightView, Size(ResizeDim_Col, ResizeDim_Rows), INTER_LINEAR);
	resize(TopViewCar, Resize_TopViewCar, Size(300, 380), INTER_LINEAR);
	/*------------Rotate Camera images (Affine Transformation) ---------------------- */
	rotate(ResizedLeftView, LeftRotateImage, ROTATE_90_COUNTERCLOCKWISE);
	rotate(ResizedRightView, RightRotateImage, ROTATE_90_CLOCKWISE);
	/*------------create main canvass image ---------------------- */

	Mat Canvass_img(720,1280, CV_8UC3, Scalar(0, 0, 0));
	Mat ProjectedView(960,1280, CV_8UC3, Scalar(0, 0, 0));
//	Resize_TopViewCar.copyTo(ProjectedView(Rect(460,325,Resize_TopViewCar.cols, Resize_TopViewCar.rows)));
	cout << "CanvasImage Size =" << Canvass_img.size << endl;
	cout << "FrontView Size =" << ResizedFrontView.size << endl;
	cout << "TopViewCar Size =" << Resize_TopViewCar.size << endl;
	/*------------ Perspective transform  ----------------------- */
	vector<Point> frontView_Perspective_Points,frontView_src_Points,LeftView_src_Points,LeftView_Perspective_Points,
	RightView_Perspective_Points,RightView_src_Points,RearView_Perspective_Points,RearView_src_Points;
	frontView_Perspective_Points.push_back(Point(5,0));
	frontView_Perspective_Points.push_back(Point(460,320));
	frontView_Perspective_Points.push_back(Point(780,320));
	frontView_Perspective_Points.push_back(Point(1275,0));

	RearView_Perspective_Points.push_back(Point(460,700));
	RearView_Perspective_Points.push_back(Point(0,960));
	RearView_Perspective_Points.push_back(Point(1280,960));
	RearView_Perspective_Points.push_back(Point(780,700));

	LeftView_Perspective_Points.push_back(Point(0,0));
	LeftView_Perspective_Points.push_back(Point(0,960));
	LeftView_Perspective_Points.push_back(Point(460,700));
	LeftView_Perspective_Points.push_back(Point(460,320));

//	RightView_Perspective_Points.push_back(Point(1280,5));
//	RightView_Perspective_Points.push_back(Point(1280,960));
//	RightView_Perspective_Points.push_back(Point(760,700));
//	RightView_Perspective_Points.push_back(Point(760,320));

	RightView_Perspective_Points.push_back(Point(760,320)); //Anticlockwise
	RightView_Perspective_Points.push_back(Point(760,700));
	RightView_Perspective_Points.push_back(Point(1280,960));
	RightView_Perspective_Points.push_back(Point(1280,5));


	frontView_src_Points.push_back(Point(0,0));
	frontView_src_Points.push_back(Point(0,240));
	frontView_src_Points.push_back(Point(640,240));
	frontView_src_Points.push_back(Point(640,0));

	RearView_src_Points.push_back(Point(0,0));
	RearView_src_Points.push_back(Point(0,240));
	RearView_src_Points.push_back(Point(640,240));
	RearView_src_Points.push_back(Point(640,0));

	LeftView_src_Points.push_back(Point(0,0));    	//Rectangular extreme points of Rotated images
	LeftView_src_Points.push_back(Point(0,640));
	LeftView_src_Points.push_back(Point(240,640));
	LeftView_src_Points.push_back(Point(240,0));

	RightView_src_Points.push_back(Point(0,0));      //Rectangular extreme points of Rotated images
	RightView_src_Points.push_back(Point(0,640));
	RightView_src_Points.push_back(Point(240,640));
	RightView_src_Points.push_back(Point(240,0));

	/*------------------Calculate Homography---------------------- */
	Mat h_Front = findHomography(frontView_src_Points, frontView_Perspective_Points);
	Mat h_left = findHomography(LeftView_src_Points, LeftView_Perspective_Points);
	Mat h_right = findHomography(RightView_src_Points, RightView_Perspective_Points);
	Mat h_Rear = findHomography(RearView_src_Points, RearView_Perspective_Points);
	/*----------------  Output Image ------------------------------ */
	Mat im_out,cropped_image;
	warpPerspective(ResizedFrontView, im_out, h_Front, ProjectedView.size());
	/*---------------------------Cropping Logic --------------------- */
	Mat TempViewMask = Mat(960,1280,CV_8UC3,Scalar(0,0,0));
	Rect rectROI(0,0,1280,960);
	Point P1(5,0);
	Point P2(460,320);
	Point P3(780,320);
	Point P4(1275,0);

	vector< vector<Point> >  co_ordinates;
	co_ordinates.push_back(vector<Point>());
	co_ordinates[0].push_back(P1);
	co_ordinates[0].push_back(P2);
	co_ordinates[0].push_back(P3);
	co_ordinates[0].push_back(P4);

	drawContours( TempViewMask,co_ordinates,0, Scalar(255,255,255),FILLED, 8);

	Mat srcROI=im_out(rectROI);
	Mat dstROI=TempViewMask(rectROI);
	Mat TopView_Mask;

	srcROI.copyTo(TopView_Mask,TempViewMask);
	Resize_TopViewCar.copyTo(TopView_Mask(Rect(465,318,Resize_TopViewCar.cols, Resize_TopViewCar.rows)));
	/*---------------------------------------------------------------------------------------------------------
	 * Left Side View Perspective Transform and Stiching */
	im_out = Mat::zeros( im_out.size(), im_out.type());
	TempViewMask = Mat::zeros( TempViewMask.size(), TempViewMask.type());
	warpPerspective(LeftRotateImage, im_out, h_left, ProjectedView.size());
	Point Pl1(0,5);
	Point Pl2(0,955);
	Point Pl3(460,730);
	Point Pl4(460,320);

	vector< vector<Point> >  co_ordinates_left;
	co_ordinates_left.push_back(vector<Point>());
	co_ordinates_left[0].push_back(Pl1);
	co_ordinates_left[0].push_back(Pl2);
	co_ordinates_left[0].push_back(Pl3);
	co_ordinates_left[0].push_back(Pl4);
	drawContours( TempViewMask,co_ordinates_left,0, Scalar(255,255,255),FILLED, 8);
	srcROI.copyTo(TopView_Mask,TempViewMask);

	/*-------------------------------------------------------------------------------------------------------
	 * Right Side Perspective Transform*/
	im_out = Mat::zeros( im_out.size(), im_out.type());
	TempViewMask = Mat::zeros( TempViewMask.size(), TempViewMask.type());
	warpPerspective(RightRotateImage, im_out, h_right, ProjectedView.size());
//	Point Psr1(1280,0);
//	Point Psr2(1280,955);
//	Point Psr3(750,740);
//	Point Psr4(750,360);

//	Point Psr1(1280,0);
//	Point Psr2(750,360);
//	Point Psr3(750,740);
//	Point Psr4(1280,955);

	Point Psr1(750,360);
	Point Psr2(750,740);
	Point Psr3(1280,955);
	Point Psr4(1280,0);

	vector< vector<Point> >  co_ordinates_right;
	co_ordinates_right.push_back(vector<Point>());
	co_ordinates_right[0].push_back(Psr1);
	co_ordinates_right[0].push_back(Psr2);
	co_ordinates_right[0].push_back(Psr3);
	co_ordinates_right[0].push_back(Psr4);

	drawContours( TempViewMask,co_ordinates_right,0, Scalar(255,255,255),FILLED, 8);
	srcROI.copyTo(TopView_Mask,TempViewMask);
	/*-------------------------------------------------------------------------------------------------------
	 * Rear View Perspective Transform and Stiching */
	im_out = Mat::zeros( im_out.size(), im_out.type());
	TempViewMask = Mat::zeros( TempViewMask.size(), TempViewMask.type());
	warpPerspective(ResizedRearView, im_out, h_Rear, ProjectedView.size());
	Point Pr1(460,700);
	Point Pr2(0,960);
	Point Pr3(1280,960);
	Point Pr4(780,700);

	vector< vector<Point> >  co_ordinates_rear;
	co_ordinates_rear.push_back(vector<Point>());
	co_ordinates_rear[0].push_back(Pr1);
	co_ordinates_rear[0].push_back(Pr2);
	co_ordinates_rear[0].push_back(Pr3);
	co_ordinates_rear[0].push_back(Pr4);

	drawContours( TempViewMask,co_ordinates_rear,0, Scalar(255,255,255),FILLED, 8);
	srcROI.copyTo(TopView_Mask,TempViewMask);

	imshow("TopViewMask",TopView_Mask);
	/*------------Blending Image to Canvass Image ---------------------- */
	//FrontView.copyTo(Canvass_img(Rect(10,10,FrontView.cols, FrontView.rows)));
	ResizedFrontView.copyTo(Canvass_img(Rect(ResizeDim_Col * 0.5,0,ResizedFrontView.cols, ResizedFrontView.rows)));
	ResizedRearView.copyTo(Canvass_img(Rect(ResizeDim_Col * 0.5,ResizeDim_Rows * 2,ResizedFrontView.cols, ResizedFrontView.rows)));
	LeftRotateImage.copyTo(Canvass_img(Rect(0,50,LeftRotateImage.cols, LeftRotateImage.rows)));
	RightRotateImage.copyTo(Canvass_img(Rect(1010,50,RightRotateImage.cols, RightRotateImage.rows)));
//	Resize_TopViewCar.copyTo(ProjectedView(Rect(460,322,Resize_TopViewCar.cols, Resize_TopViewCar.rows)));
	imwrite("/home/saurabh/MI-EE-Architecture/SVS_Experiments/SurroundViewImages/SorroundViewOutput.jpg", TopView_Mask);
	waitKey(0);

   return 0;
}
