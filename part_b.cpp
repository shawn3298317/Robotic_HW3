#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
	if ( argc != 2 ) // if no argument, exit
		exit(1);

	cv::Mat sourceImage, modImage;
	sourceImage = cv::imread(argv[1], 0); // Load a gray scale picture.
	if (!sourceImage.data)
		exit(1); //can't open, then exit

	const char ESC_KEY = 27;
	cv::String windowName = "Source";

	cv::namedWindow( windowName, cv::WINDOW_AUTOSIZE); //open up a window
	cv::imshow( windowName, sourceImage ); //show the image in window

	//char key = (char)cv::waitKey(0); //show it forever
	//if( key == ESC_KEY )
	//	cv::destroyWindow( windowName );

	cv::namedWindow( "GaussianBlur", cv::WINDOW_AUTOSIZE );
	modImage = sourceImage.clone();

	//modifications
	cv::GaussianBlur( modImage, modImage, cv::Size( 3, 3 ), 0, 0 ); //blurry
	cv::threshold(modImage, modImage, 128, 255, cv::THRESH_BINARY); //black or white

	cv::erode(modImage, modImage, cv::Mat()); //what does this do?
	cv::dilate(modImage, modImage, cv::Mat()); //what does this do?
	
	CvMoments m = cv::moments( modImage, true );
	//centroid	
	double x_c = m.m10 / m.m00;
	double y_c = m.m01 / m.m00;
	//central moments m.mu02
	//principle angle
	cv::circle( modImage, Point( x_c, y_c), 100, Scalar( 255, 0, 0) );
	double th = 0.5 * atan2( 2*m.mu11, m.mu20 - m.mu02 );

	cout<< "Centroid = ("<< x_c << "," << y_c <<")"<< endl;
	cout<< "Principle Angle = " << th << " degree" << endl;
	cout<< "Img size = " << modImage.size() << endl;

	Point x0, x1;
	//x0 = Point( modImage.size().width - x_c, modImage.size().height - y_c );
	x0 = Point( 0, tan(th) * 0 + y_c - tan(th)*x_c );
	x1 = Point( x_c, y_c );
	cout<<"x0: "<< x0<<endl;
	cout<<"x1: "<< x1<<endl;
	//x0 = Point( 0, y_c );
	//x1 = Point( x_c, y_c );
	//line( modImage, x0, x1, Scalar( 255, 0, 0), 1, CV_AA ); // red
	//CV_AA antialiased line

	cv::imshow( "GaussianBlur", modImage );
	cv::waitKey();

	/*RNG rng(12345);

	std::vector< std::vector< cv::Point > > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours( modImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	/// Draw contours
	Mat drawing = Mat::zeros( modImage.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
		{
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
		}
	/// Show in a window
	cout<<"Numer of contours: "<<contours.size();
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );
	waitKey();*/
	
	return 0;
}