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

typedef struct Point
{
	char x;
	char y;
}Point;

typedef struct Point_4
{
	int x;
	int y;
}Point_4;

int P_T[500][2];


/*计算丢弃B图re_num行后P图梯形的大小
re_num是要从B图最上方减少的行数，也就是无效行
function在使用时输入2
*/
void count_Size(int re_num,int function)
{
	int i = 0;
	int j = 0;
	int count = 0;
	int flag1 = 0;
	int flag2 = 0;
	int bad = 0;
	int max[2] = { 0,0 };
	int min[2] = { 100000,100000 };
	int temp[4][2];
	
	if (1 == function)
	{
		vector<Point2f> corners(4);		//P图 原图
		corners[0] = Point2f(0, 0);
		corners[1] = Point2f(330, 0);
		corners[2] = Point2f(140, 277);
		corners[3] = Point2f(190, 277);
		vector<Point2f> corners_trans(4);	//B图 转化图
		corners_trans[0] = Point2f(0, 0);
		corners_trans[1] = Point2f(97, 0);
		corners_trans[2] = Point2f(0, 72);
		corners_trans[3] = Point2f(97, 72);

		Mat transform = getPerspectiveTransform(corners, corners_trans);

		vector<Point2f> points, points_trans;
		for (int i = 0; i<=277; i++) 
		{
			for (int j = 0; j<=330; j++) 
			{
				points.push_back(Point2f(j, i));
			}
		}
	
		perspectiveTransform(points, points_trans, transform);
	}
	else if (2 == function)
	{
		vector<Point2f> corners(4);				//B图 原图
		corners[0] = Point2f(0, 0);
		corners[1] = Point2f(97, 0);
		corners[2] = Point2f(0, 72);
		corners[3] = Point2f(97, 72);
		vector<Point2f> corners_trans(4);		//P图 转化图
		corners_trans[0] = Point2f(0, 0);
		corners_trans[1] = Point2f(330, 0);
		corners_trans[2] = Point2f(140, 277);
		corners_trans[3] = Point2f(190, 277);

		Mat transform = getPerspectiveTransform(corners, corners_trans);
		vector<Point2f> points, points_trans;
		for (int i = 0; i <= 72; i++)
		{
			for (int j = 0; j <= 97; j++)
			{
				points.push_back(Point2f(j, i));
			}
		}
		perspectiveTransform(points, points_trans, transform);

		temp[0][0] = points_trans[re_num * 98].y;
		temp[0][1] = points_trans[re_num * 98].x;
		temp[1][0] = points_trans[re_num * 98 + 97].y;
		temp[1][1] = points_trans[re_num * 98 + 97].x;
		temp[2][0] = points_trans[72 * 98].y;
		temp[2][1] = points_trans[72 * 98].x;
		temp[3][0] = points_trans[72 * 98 + 97].y;
		temp[3][1] = points_trans[72 * 98 + 97].x;

		cout << "减去B图最上面的"<< re_num <<"行后对应于P图：" << endl;
		cout << "左上：" << points_trans[re_num * 98].y << "," << points_trans[re_num * 98].x << endl;
		cout << "右上：" << points_trans[re_num * 98 + 97].y << "," << points_trans[re_num * 98 + 97].x << endl;
		cout << "左下：" << points_trans[72 * 98].y << "," << points_trans[72 * 98].x << endl;
		cout << "右下：" << points_trans[72 * 98 + 97].y << "," << points_trans[72 * 98 + 97].x << endl;
		cout << "梯形所在矩形：" 
			<< points_trans[72 * 98].y - points_trans[re_num * 98].y 
			<< "," 
			<< points_trans[re_num * 98 + 97].x - points_trans[re_num * 98].x << endl;
		cout << "大小约为：" 
			<< (int)((points_trans[72 * 98].y - points_trans[re_num * 98].y)*(points_trans[re_num * 98 + 97].x - points_trans[re_num * 98].x)) << endl;
		cout << "梯形的新坐标："
			<< "(" << "0" << "," << "0" << ") "
			<< "(" << temp[1][0] - temp[0][0] << "," << temp[1][1] - temp[0][1] << ") "
			<< "(" << temp[2][0] - temp[0][0] << "," << temp[2][1] - temp[0][1] << ") "
			<< "(" << temp[3][0] - temp[0][0] << "," << temp[3][1] - temp[0][1] << ") "
			<< endl;
		cout << "B图新坐标："
			<< "(" << "0" << "," << "0" << ") "
			<< "(" << "0" << "," << "97" << ") "
			<< "(" << 72 - re_num << "," << "0" << ") "
			<< "(" << 72 - re_num << "," << "97" << ") "
			<< endl;
	}
}



/*完成透视变换*/
void P_Trans(vector<Point2f> corners, vector<Point2f> corners_trans,int row,int col,char filename[], char filename1[],int re)
{
	int min[2];
	int max[2];
	int i = 0;
	int j = 0;
	int flag = 0;
	int count = 0;

	Mat transform = getPerspectiveTransform(corners, corners_trans);
	//find_Matrix(corners, min, max);
	vector<Point2f> points, points_trans;
	for (int i = 0; i <= row; i++)
	{
		for (int j = 0; j <= col; j++)
		{
			points.push_back(Point2f(j, i));
		}
	}

	perspectiveTransform(points, points_trans, transform);

	for (i = 0; i<=row; i++)
	{
		flag = 0;
		for (j = 0; j<=col; j++)
		{
			if (0 == flag)
			{
				if (points_trans[i*(col+1)+j].x >= 0 && points_trans[i*(col + 1) + j].x <= points_trans[col].x && points_trans[i*(col + 1) + j].y >= re && points_trans[i*(col + 1) + j].y <= points_trans[(row)*(col + 1) +2].y)
				{
					P_T[i][0] = j - 1;
					flag++;
				}
			}
			else
			{
				if (!(points_trans[i*(col + 1) + j].x >= 0 && points_trans[i*(col + 1) + j].x <= points_trans[col].x) && points_trans[i*(col + 1) + j].y >= re && points_trans[i*(col + 1) + j].y <= points_trans[(row)*(col + 1) + 2].y)
				{
					P_T[i][1] = j;
					flag++;
					break;
				}
			}
		}
		if (flag == 0)
		{
			P_T[i][0] = -1;
			P_T[i][1] = col + 1;
		}
		else if (1 == flag)
		{
			P_T[i][1] = col + 1;
		}
	}

	FILE *fp=fopen(filename,"w");
	if(!fp)
	{
	    printf("Can't open file\n");
	    return ;
	}

	for (i = 0; i <= row; i++)
	{
		fprintf(fp,"{%3d,%3d} ", P_T[i][0], P_T[i][1]);
		if (i<row) fprintf(fp,",");
		fprintf(fp,"\n");
	}
	fclose(fp);
	fp = fopen(filename1, "w");
	if (!fp)
	{
		printf("Can't open file\n");
		return;
	}
	count = 0;
	for (int i = 0; i <= row; i++)
	{
		fprintf(fp, "{");
		for (int j = 0; j <= col; j++)
		{
			fprintf(fp, "{%3d,%3d} ", (int)points_trans[count].y, (int)points_trans[count].x);
	        if(j<col) fprintf(fp,",");
			count++;
			if(0==count%10) fprintf(fp,"\n");
		}
		fprintf(fp, "}");
		if(i<row) fprintf(fp,",");
	}
	fclose(fp);
}

/*输入4个向量坐标，找出行坐标和列坐标的取值范围*/
/*vector<Point2f> corners(4);中corners是一个vector<Point2f>类型的变量，（4）只是生成变量点的参数
不是指一个数组，而是指corners是一个对象并含有4个元素
&是引用，别名对象是一个int类型的数组
*/
void find_Matrix(vector<Point2f> v, int (&min)[2], int (&max)[2])
{
	int i = 0;

	min[0] = 10000;
	min[1] = 10000;
	max[0] = 0;
	max[1] = 0;

	for (i = 0; i < v.size(); i++)
	{
		if (v[i].y < min[0]) min[0] = v[i].y;
		if (v[i].y > max[0]) max[0] = v[i].y;
		if (v[i].x < min[1]) min[1] = v[i].x;
		if (v[i].x > max[1]) max[1] = v[i].x;
	}
}



int main()
{
	vector<Point2f> corners(4);		//P图 原图
	corners[0] = Point2f(0, 0);
	corners[1] = Point2f(177, 0);
	corners[2] = Point2f(64, 127);
	corners[3] = Point2f(114, 127);
	vector<Point2f> corners_trans(4);	//B图 转化图
	//corners_trans[0] = Point2f(0, 0);
	//corners_trans[1] = Point2f(97, 0);
	//corners_trans[2] = Point2f(0, 61);
	//corners_trans[3] = Point2f(97, 61);
	corners_trans[0] = Point2f(0, 11);
	corners_trans[1] = Point2f(97, 11);
	corners_trans[2] = Point2f(0, 72);
	corners_trans[3] = Point2f(97, 72);

	char filename[] = "E:\\github\\transform\\transform\\data\\try.txt";
	char filename1[] = "E:\\github\\transform\\transform\\data\\try1.txt";

	P_Trans(corners, corners_trans,127,177, filename,filename1,11);

	//count_Size(11, 2);


	//int i = 0;
	//int j = 0;
	//int count = 0;
	//int flag1 = 0;
	//int flag2 = 0;
	//int bad = 0;
	//int max[2] = { 0,0 };
	//int min[2] = { 100000,100000 };
	//Mat transform = getPerspectiveTransform(corners, corners_trans);
	//cout << transform << endl;
	//vector<Point2f> points, points_trans;
	//for (int i = 0; i <175; i++)
	//{
	//	for (int j = 0; j <205; j++)
	//	{
	//		points.push_back(Point2f(j, i));
	//	}
	//}

	//perspectiveTransform(points, points_trans, transform);

	//FILE *fp=fopen("E:\\github\\transform\\transform\\data\\BP_rivise_s.txt","w");
	//if(!fp)
	//{
	//    printf("Can't open file\n");
	//    return 0;
	//}
	//count = 0;
	//for (int i = 0; i <175; i++)
	//{
	//	fprintf(fp, "{");
	//	for (int j = 0; j <205; j++)
	//	{
	//		fprintf(fp, "{%3d,%3d} ", (int)points_trans[count].y, (int)points_trans[count].x);
	//        if(j<204) fprintf(fp,",");
	//		count++;
	//		if(0==count%10) fprintf(fp,"\n");
	//	}
	//	fprintf(fp, "}");
	//	if(i<174) fprintf(fp,",");
	//}
	//fclose(fp);

	//cout << count << endl;
	////cout << bad << endl;
	//cout << min[0] << endl;
	//cout << max[0] << endl;
	//cout << min[1] << endl;
	//cout << max[1] << endl;
	//cout << points_trans[H * 98 + L + 1 - 1].y<< points_trans[H * 98 + L + 1 - 1].x << endl;
	//getchar();
	return 0;
}














/*Point2f的列坐标在前，行坐标在后*/
//int main()
//{
//	int i = 0;
//	int j = 0;
//	int count = 0;
//	int flag1 = 0;
//	int flag2 = 0;
//	int bad = 0;
//	int max[2] = { 0,0 };
//	int min[2] = { 100000,100000 };
//
//	vector<Point2f> corners(4);
//	corners[0] = Point2f(0, 0);
//	corners[2] = Point2f(0, 72);
//	corners[1] = Point2f(97, 0);
//	corners[3] = Point2f(97, 72);
//	vector<Point2f> corners_trans(4);
//	corners_trans[0] = Point2f(0, 0);
//	corners_trans[2] = Point2f(140, 277);
//	corners_trans[1] = Point2f(330, 0);
//	corners_trans[3] = Point2f(190, 277);
//
//	Mat transform = getPerspectiveTransform(corners_trans, corners);
//	cout << transform << endl;
//	vector<Point2f> points, points_trans;
//	for (int i = 0; i<=277; i++) 
//	{
//		for (int j = 0; j<=330; j++) 
//		{
//			points_trans.push_back(Point2f(j, i));
//		}
//	}
//
//	perspectiveTransform(points_trans, points, transform);
//	count = 0;
//	for (int i = 0; i <= 277; i++)
//	{
//		for (int j = 0; j <= 330; j++)
//		{
//			if (points[count].x < 0)
//			{
//				points[count].x = 0;
//				flag1++;
//			}
//			if (points[count].x > 97)
//			{
//				points[count].x = 97;
//				flag1++;
//			}
//			if (points[count].y < 0)
//			{
//				points[count].y = 0;
//				flag2++;
//			}
//			if (points[count].y > 72)
//			{
//				points[count].x = 72;
//				flag2++;
//			}
//			if (flag1 != 0 && flag2 != 0)
//			{
//				if (i < min[0]) min[0] = i;
//				if (i > max[0]) max[0] = i;
//				if (j < min[1]) min[1] = j;
//				if (j > max[1]) max[1] = j;
//				bad++;
//			}
//			flag1 = 0;
//			flag2 = 0;
//			count++;
//		}
//	}
//	//FILE *fp=fopen("E:\\github\\transform\\transform\\data\\BP_rivise.txt","w");
//	//if(!fp)
//	//{
//	//    printf("Can't open file\n");
//	//    return 0;
//	//}
//	//count = 0;
//	//for (int i = 0; i <= 277; i++)
//	//{
//	//	fprintf(fp, "{");
//	//	for (int j = 0; j <= 330; j++)
//	//	{
//	//		fprintf(fp, "{%3d,%3d} ", (int)points[count].y, (int)points[count].x);
// //           if(j<330) fprintf(fp,",");
//	//		count++;
//	//		if(0==count%10) fprintf(fp,"\n");
//	//	}
//	//	fprintf(fp, "}");
//	//	if(i<277) fprintf(fp,",");
//	//}
//	//fclose(fp);
//
//	cout << count << endl;
//	cout << bad << endl;
//	cout << min[0] << endl;
//	cout << max[0] << endl;
//	cout << min[1] << endl;
//	cout << max[1] << endl;
//	getchar();
//	return 0;
//}

//     for(i=0;i<ROW;i++)
//         {
//             
//             for(j=0;j<COL;j++)
//             {
//                 
//             }
//             
//         }

//int main()
//{
//	vector<Point2f> corners(4);
//	corners[0] = Point2f(0, 0);								//左上
//	corners[1] = Point2f(img_width - 1, 0);					//右上
//	corners[2] = Point2f(0, img_height - 1);				//左下
//	corners[3] = Point2f(img_width - 1, img_height - 1);	//右下
//	vector<Point2f> corners_trans(4);
//	corners_trans[0] = Point2f(0, 0);
//	corners_trans[1] = Point2f(990, 0);
//	corners_trans[2] = Point2f(420, 831);
//	corners_trans[3] = Point2f(570, 831);
//
//	Mat transform = getPerspectiveTransform(corners_trans, corners);
//	cout << transform << endl;
//	vector<Point2f> ponits, points_trans;
//	for (int i = 0; i<img_height + 600; i++)
//	{
//		for (int j = 0; j<img_width + 500; j++)
//		{
//			ponits.push_back(Point2f(j, i));
//		}
//	}
//
//	perspectiveTransform(ponits, points_trans, transform);
//	Mat img_trans = Mat::zeros(img_height + 600, img_width + 500, CV_8UC3);
//	int count = 0;
//	for (int i = 0; i<img_height + 600; i++)
//	{
//		uchar* p = img_trans.ptr<uchar>(i);
//		for (int j = 0; j<img_width + 500; j++)
//		{
//			int y = points_trans[count].y;
//			int x = points_trans[count].x;
//			if (x > 550 || x < 0 || y>272 || y < 0)
//			{
//				count++;
//			}
//			else
//			{
//				uchar* t = img.ptr<uchar>(y);
//				p[j * 3] = t[x * 3];
//				p[j * 3 + 1] = t[x * 3 + 1];
//				p[j * 3 + 2] = t[x * 3 + 2];
//				count++;
//			}
//		}
//	}
//	imwrite("boy_trans.png", img_trans);
//	imshow("dst", img_trans);
//	cvWaitKey(0);
//
//	return 0;
//}