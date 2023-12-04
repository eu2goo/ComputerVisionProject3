
#include <iostream>
#include "opencv.hpp"
#include <cmath>
using namespace std;
using namespace cv;


struct MouseParams
{
	Mat img;
	vector<Point2f> in, out;
};


/*
int main() {
	Mat origin, gray, result, edge;
	VideoCapture cap;

	vector<Vec4i> lines;

	origin = imread("cleanRoadDrain.jpeg");
	if (!origin.data) exit;
	result = origin.clone();
	cvtColor(origin, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(5, 5), 5, 5, BORDER_DEFAULT);
	Canny(gray, edge, 50, 200, 3);
	HoughLinesP(edge, lines, 1, CV_PI / 180, 50, 500, 600);

	for (int i = 0; i < lines.size();i++) {
		Vec4i l = lines[i];
		line(result, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, 8);
	}

	imshow("result", edge);

	waitKey(0);

	return 0;
}
*/

static void onMouse(int event, int x, int y, int, void* param)
{
	int count=0;
	int plus=0;
	float rho, theta, a, b, x0, y0;
	Point p1, p2;
	float rho1, theta1, a1, b1, x10, y10;
	Point p11, p12;
	float rho2, theta2, a2, b2, x20, y20;
	Point p21, p22;
	float rho3, theta3, a3, b3, x30, y30;
	Point p31, p32;
	MouseParams* mp = (MouseParams*)param;
	Mat img = mp->img;
	if (event == EVENT_LBUTTONDOWN) // left button
	{
		Mat result;

		mp->in.push_back(Point2f(x, y));
		if (mp->in.size() == 4)
		{
			Mat gray, edge;
			

			Mat homo_mat = getPerspectiveTransform(mp->in, mp->out);

			warpPerspective(img, result, homo_mat, Size(img.cols, img.rows));
			cvtColor(result, gray, COLOR_BGR2GRAY);

			Canny(gray, edge, 50, 200, 3);



			while (true) {

				vector<Vec2f> lines;
				vector<Vec2f> lines1;
				vector<Vec2f> lines2;
				vector<Vec2f> lines3;
				vector<Vec2f> lines4;

				HoughLines(edge, lines, 1, CV_PI / 180, 210+plus, 0, 0, 10 * (CV_PI / 180), 80 * (CV_PI / 180));
				HoughLines(edge, lines1, 1, CV_PI / 180, 210+plus, 0, 0, 100 * (CV_PI / 180), 170 * (CV_PI / 180));
				HoughLines(edge, lines2, 1, CV_PI / 180, 210+plus, 0, 0, 190 * (CV_PI / 180), 260 * (CV_PI / 180));
				HoughLines(edge, lines3, 1, CV_PI / 180, 210+plus, 0, 0, 280 * (CV_PI / 180), 350 * (CV_PI / 180));


				for (int i = 0; i < lines.size();i++) {
					rho = lines[i][0];
					theta = lines[i][1];
					a = cos(theta);
					b = sin(theta);
					x0 = a * rho;
					y0 = b * rho;
					p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
					p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));

					count++;
					//line(result, p1, p2, Scalar(0, 0, 255), 1, 8);
				}

				for (int i = 0; i < lines1.size();i++) {
					rho1 = lines1[i][0];
					theta1 = lines1[i][1];
					a1 = cos(theta1);
					b1 = sin(theta1);
					x10 = a1 * rho1;
					y10 = b1 * rho1;
					p11 = Point(cvRound(x10 + 1000 * (-b1)), cvRound(y10 + 1000 * a1));
					p12 = Point(cvRound(x10 - 1000 * (-b1)), cvRound(y10 - 1000 * a1));

					count++;
					//line(result, p11, p12, Scalar(0, 0, 255), 1, 8);
				}

				for (int i = 0; i < lines2.size();i++) {
					rho2 = lines2[i][0];
					theta2 = lines2[i][1];
					a2 = cos(theta2);
					b2 = sin(theta2);
					x20 = a2 * rho2;
					y20 = b2 * rho2;
					p21 = Point(cvRound(x20 + 1000 * (-b2)), cvRound(y20 + 1000 * a2));
					p22 = Point(cvRound(x20 - 1000 * (-b2)), cvRound(y20 - 1000 * a2));

					count++;
					//line(result, p21, p22, Scalar(0, 0, 255), 1, 8);
				}

				for (int i = 0; i < lines3.size();i++) {
					rho3 = lines3[i][0];
					theta3 = lines3[i][1];
					a3 = cos(theta3);
					b3 = sin(theta3);
					x30 = a3 * rho3;
					y30 = b3 * rho3;
					p31 = Point(cvRound(x30 + 1000 * (-b3)), cvRound(y30 + 1000 * a3));
					p32 = Point(cvRound(x30 - 1000 * (-b3)), cvRound(y30 - 1000 * a3));

					count++;
					//line(result, p31, p32, Scalar(0, 0, 255), 3, 8);
				}


				if (count >= 20 && count <= 100) {
					putText(result, "Clean please", Point(result.cols/2 - 100, result.rows/2), FONT_HERSHEY_SIMPLEX, 3, Scalar(255, 0, 0), 3);
					break;
				}
				else if(count>100){
					count = 0;
					plus += 50;
				}
				else {
					break;
				}
			}

			imshow("output", result);
		}
		else
		{
			result = img.clone();
			for (size_t i = 0; i < mp->in.size(); i++)
			{
				circle(result, mp->in[i], 3, Scalar(0, 0, 255), 5);
			}
			imshow("input", result);
		}
	}
	//Reset positions
	if (event == EVENT_RBUTTONDOWN)
	{
		mp->in.clear();
		imshow("input", img);
	}
}


int main()
{

	Mat imput = imread("dirtyRoadDrain1.jpeg");


	imshow("input", imput);

	MouseParams mp;

	mp.out.push_back(Point2f(0, 0));
	mp.out.push_back(Point2f(imput.cols, 0));
	mp.out.push_back(Point2f(imput.cols, imput.rows));
	mp.out.push_back(Point2f(0, imput.rows));

	mp.img = imput;

	setMouseCallback("input", onMouse, (void*)&mp);

	waitKey();
	return 0;
}

