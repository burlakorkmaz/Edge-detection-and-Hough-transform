// Name Surname: Burla Nur Korkmaz
// Student ID: 2008484

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;

int main()
{

	Mat edges, img_gray, img_gray_clone, img_clone, img_gray_circle, img; // Inputs and outputs images
	std::vector<Vec2f> lines; // will hold the results of the Houghlines 
	std::vector<Vec3f> circles; // will hold the results of the HoughCircles

	//HoughLines parameters
	const double theta = CV_PI / 180;
	const int srn = 0;
	const int stn = 0;
	const double min_theta = 0;
	const double max_theta = CV_PI / 2;

	//HoughCircles parameters
	double dp = 1;
	double param1 = 100;
	double param2 = 30;

	// Loads an image
	img = imread("road.jpg", IMREAD_COLOR);

	// Convert img into grayscale
	cvtColor(img, img_gray, COLOR_BGR2GRAY);

	// Clone img_gray to be used in HoughCircles
	img_gray_circle = img_gray.clone();

	// Use median blur on img_gray_circle to be used in HoughCircles
	medianBlur(img_gray_circle, img_gray_circle, 5);

	// Create a window
	namedWindow("output", WINDOW_NORMAL);
	resizeWindow("output", 800, 800);

	//Create trackbar for canny treshold
	int treshold_canny = 323;
	createTrackbar("cannyTresh", "output", &treshold_canny, 500);

	// Create trackbars for HoughLines
	int treshold_hough = 132;
	createTrackbar("houghTresh", "output", &treshold_hough, 500);
	int rho_trackbar = 100;
	createTrackbar("rho*100", "output", &rho_trackbar, 500); // Trackbars do not take double, so i create integer rho_trackbar value to put in trackbar,  
	double rho = 1;                                          // then convert the rho_trackbar to double, divide it into 100 to be used in HoughLines (line 68) 

	// Create trackbars for HoughCircles
	int minRadius = 37;
	createTrackbar("minRadius", "output", &minRadius, 100);
	int maxRadius = 39;
	createTrackbar("maxRadius", "output", &maxRadius, 100);
	int minD = 100;
	createTrackbar("minD", "output", &minD, 10000); // Trackbars do not take double, so i create integer minD value to put in trackbar,
	double minDist = 1;                             // then convert the minD to double, divide it into 100 to be used in HoughCircles (line 89)

	while (true) // While loop for constant update
	{
		img_clone = img.clone(); // img_clone to keep original image safe
		img_gray_clone = img_gray.clone(); // img_gray_clone to keep original img_gray safe
		// treshold_canny parameter can be changed via trackbar, high treshold is defined as treshold_canny*3 - kernel size parameter is fixed
		Canny(img_gray_clone, edges, treshold_canny, treshold_canny * 3, 3); // Canny edge detector

		rho = (double)rho_trackbar / 100; // (line 51)
		if (rho == 0) // Prevent rho from being zero
		{
			rho = 1;
		}
		// treshold_hough and rho parameters can be changed via trackbar -  theta, srn, stn, min_theta, max_theta parameters are fixed
		HoughLines(edges, lines, rho, theta, treshold_hough, srn, stn, min_theta, max_theta);  // HoughLines line detector

		// Draw the lines i got from HoughLines to display
		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(img_clone, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
		}

		minDist = (double)minD / 100; // (line 60)
		if (minDist == 0) // Prevent minDist from being zero
		{
			minDist = 1;
		}
		// minDist, minRadius and maxRadius parameters can be changed via trackbar - dp, param1 and param2 parameters are fixed
		HoughCircles(img_gray_circle, circles, HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius); // HoughCircles circle detector

		// Draw the circles i got from HoughCircles to display
		for (size_t i = 0; i < circles.size(); i++)
		{
			Vec3i c = circles[i];
			Point center = Point(c[0], c[1]);
			// circle center
			circle(img_clone, center, 1, Scalar(0, 255, 0), 3, LINE_AA);
			// circle outline
			int radius = c[2];
			circle(img_clone, center, radius, Scalar(0, 255, 0), FILLED, LINE_AA);
		}

		imshow("output", img_clone);
		//imshow("Source image", img);

		// Wait until user press some key for 50ms
		int iKey = waitKey(50);

		//if user press 'ESC' key
		if (iKey == 27)
		{
			break;
		}
	}
}