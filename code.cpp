#include "opencv2/highgui/highgui.hpp"
#include <highgui.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <array>

using namespace cv;
using namespace std;

int thresh = 90;

int main(int argc, char* argv[])
{
	VideoCapture cap("/home/Downloads/task_marker_22_seconds.avi"); // open the video camera no. 0

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called &quot;MyVideo&quot;
	
	
	while (1)
	{
		
		Mat frame, canny_output;
		
		bool bSuccess = cap.read(frame); // read a new frame from video
	
		imshow("MyVideo", frame);

		cvtColor(frame, frame, CV_RGB2GRAY);
		blur(frame, frame, Size(5, 5));
		frame = frame < 128;
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		Canny(frame, canny_output, thresh, thresh * 2, 3);
		cv::dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1, -1));
		findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

		int biggestContourIdx = -1;
		float biggestContourArea = 0;
		vector<Point> approxShape;

		for (size_t i = 0; i < contours.size(); i++){
			//approxPolyDP(contours[i], approxShape, arcLength(Mat(contours[i]), true)*0.04, true); tiiba eka 

			approxPolyDP(contours[i], approxShape, arcLength(Mat(contours[i]), true)*0.4, true);
		drawContours(drawing, contours, i, Scalar(0, 255, 255), CV_FILLED);   // fill BLUE
			//cv::Scalar color = cv::Scalar(0, 0, 0);
			//	drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, cv::Point() );

			float ctArea = cv::contourArea(contours[i]);
			if (ctArea < biggestContourArea + 10)
			{
				biggestContourArea = ctArea;
				biggestContourIdx = i;
			}
		}

		cv::RotatedRect boundingBox = cv::minAreaRect(contours[biggestContourIdx]);
		cv::Point2f corners[4];
		boundingBox.points(corners);
                 float d1 = (float)fabs(corners[0] - corners[1]);
                 float d2 = (float)fabs(corners[1] - pts[2]);
                 float d3 = (float)fabs(pts[2] - pts[3]);
                 float d4= (float)fabs(pts[3]-pts[0]);
                if(max(d1,d2,d3,d4)==d1)
		cv::line(drawing, corners[0], corners[1], cv::Scalar(255, 0, 0));
                 if(max(d1,d2,d3,d4)==d2)
		cv::line(drawing, corners[1], corners[2], cv::Scalar(255, 0, 0));
                 if(max(d1,d2,d3,d4)==d3)
		cv::line(drawing, corners[2], corners[3], cv::Scalar(255, 0, 0));
                 if(max(d1,d2,d3,d4)==d4)
		cv::line(drawing, corners[3], corners[0], cv::Scalar(255, 0, 0));	
		Rect roi;
		roi.width = boundingBox.size.width;
		cout << roi.width;
		cout << "\n";
		
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		imshow("MyVideo1";, drawing); //show the frame in &quot;MyVideo&quot; window


	 //show the frame in &quot;MyVideo&quot; window

		if (waitKey(30) == 27) //wait for &#39;esc&#39; key press for 30ms. If &#39;esc&#39; key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;}
