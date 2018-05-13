/*
imrang是否可以不针对hsv的挑选
如果有针对那就写转化*/
#include "stdafx.h" 
#include<opencv2/core.hpp>  
#include<opencv2/highgui.hpp>  
#include<opencv2/imgproc.hpp>  

#include<iostream>  
#include<string> 

using namespace cv;
using namespace std;



int main()
{
	vector<Point2f> corners(4);
	corners[0] = Point2f(0, 0);
	corners[1] = Point2f(72, 0);
	corners[2] = Point2f(0, 97);
	corners[3] = Point2f(72, 97);
	vector<Point2f> corners_trans(4);
	corners_trans[0] = Point2f(150, 250);
	corners_trans[1] = Point2f(771, 0);
	corners_trans[2] = Point2f(0, img_height - 1);
	corners_trans[3] = Point2f(650, img_height - 1);

	Mat transform = getPerspectiveTransform(corners, corners_trans);
	cout << transform << endl;
	vector<Point2f> ponits, points_trans;
	for (int i = 0; i<img_height; i++) {
		for (int j = 0; j<img_width; j++) {
			ponits.push_back(Point2f(j, i));
		}
	}

	perspectiveTransform(ponits, points_trans, transform);
	Mat img_trans = Mat::zeros(img_height*8, img_width*8, CV_8UC3);
	int count = 0;
	for (int i = 0; i<img_height; i++) {
		uchar* p = img.ptr<uchar>(i);
		for (int j = 0; j<img_width; j++) {
			int y = points_trans[count].y;
			int x = points_trans[count].x;
			if (y <= 0) continue;
			uchar* t = img_trans.ptr<uchar>(y);
			t[x * 3] = p[j * 3];

			t[x * 3 + 1] = p[j * 3 + 1];
			t[x * 3 + 2] = p[j * 3 + 2];
			count++;
		}
	}
	imwrite("boy_trans.png", img_trans);
	getchar();
	return 0;
}



int main()
{
	vector<Point2f> corners(4);
	corners[0] = Point2f(0, 0);								//左上
	corners[1] = Point2f(img_width - 1, 0);					//右上
	corners[2] = Point2f(0, img_height - 1);				//左下
	corners[3] = Point2f(img_width - 1, img_height - 1);	//右下
	vector<Point2f> corners_trans(4);
	corners_trans[0] = Point2f(0, 0);
	corners_trans[1] = Point2f(990, 0);
	corners_trans[2] = Point2f(420, 831);
	corners_trans[3] = Point2f(570, 831);

	Mat transform = getPerspectiveTransform(corners_trans, corners);
	cout << transform << endl;
	vector<Point2f> ponits, points_trans;
	for (int i = 0; i<img_height + 600; i++)
	{
		for (int j = 0; j<img_width + 500; j++)
		{
			ponits.push_back(Point2f(j, i));
		}
	}

	perspectiveTransform(ponits, points_trans, transform);
	Mat img_trans = Mat::zeros(img_height + 600, img_width + 500, CV_8UC3);
	int count = 0;
	for (int i = 0; i<img_height + 600; i++)
	{
		uchar* p = img_trans.ptr<uchar>(i);
		for (int j = 0; j<img_width + 500; j++)
		{
			int y = points_trans[count].y;
			int x = points_trans[count].x;
			if (x > 550 || x < 0 || y>272 || y < 0)
			{
				count++;
			}
			else
			{
				uchar* t = img.ptr<uchar>(y);
				p[j * 3] = t[x * 3];
				p[j * 3 + 1] = t[x * 3 + 1];
				p[j * 3 + 2] = t[x * 3 + 2];
				count++;
			}
		}
	}
	imwrite("boy_trans.png", img_trans);
	imshow("dst", img_trans);
	cvWaitKey(0);

	return 0;
}