#include "opencv2/highgui/highgui.hpp"
#include <highgui.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include <iostream>


using namespace cv;
using namespace std;

 

int thresh = 90;

int main(int argc, char* argv[])
{
	VideoCapture cap("/home/Downloads/task_marker_22_seconds.avi"); 
	if (!cap.isOpened())  
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 

	

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); 
	
	
	while (1)
	{
		
		Mat frame, canny_output;
		
		bool bSuccess = cap.read(frame); 
	
		imshow("MyVideo", frame);

		cvtColor(frame, frame, CV_RGB2GRAY);
		blur(frame, frame, Size(5, 5));
		frame = frame < 128;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		Canny(frame, canny_output, thresh, thresh * 2, 3);
		cv::dilate(canny_output, canny_output, cv::Mat(), cv::Point(-1, -1));
		findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

		int biggestContourIdx = -1;
		float biggestContourArea = 0;
		vector<Point> approxShape;

		for (size_t i = 0; i < contours.size(); i++){
			
			approxPolyDP(contours[i], approxShape, arcLength(Mat(contours[i]), true)*0.4, true);
		drawContours(drawing, contours, i, Scalar(0, 255, 255), CV_FILLED);   
			

			float ctArea = cv::contourArea(contours[i]);
			if (ctArea > biggestContourArea )
			{
				biggestContourArea = ctArea;
				biggestContourIdx = i;
			}
		}

		cv::RotatedRect boundingBox = cv::minAreaRect(contours[biggestContourIdx]);
		 
                 for( int i = 0; i< contours.size(); i++ )
            {
                Scalar color = Scalar( 255,0,0 );
      
                 Point2f corners[4]; boundingBox.points( corners );
       
                 if(norm(corners[0]-corners[1])>norm(corners[1]-corners[2]))
                  cv::line( drawing, corners[0], corners[1], Scalar(255,0,0), 4, 8 );
                 else
                  cv::line( drawing, corners[1], corners[2], Scalar(255,0,0), 4, 8 );
                }

		
		
               	
		Rect roi;
		roi.width = boundingBox.size.width;
		cout << roi.width;
		cout << "\n";
		
		if (!bSuccess) 
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		imshow("MyVideo1", drawing); 

	 

		if (waitKey(30) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;}
