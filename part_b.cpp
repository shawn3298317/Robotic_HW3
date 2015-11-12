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

	cv::String windowName = "Source";
	cv::imshow( windowName, sourceImage );

	modImage = sourceImage.clone();
	//modifications
	cv::fastNlMeansDenoising( modImage, modImage, 12, 7, 21 ); //removes noise
	cv::GaussianBlur( modImage, modImage, cv::Size( 3, 3 ), 0, 0 ); //blurry
	cv::threshold(modImage, modImage, 128, 255, cv::THRESH_BINARY); //black or white

	//find each contour in the picture
	std::vector< std::vector< cv::Point > > contours;
	std::vector<cv::Vec4i> hierarchy;
	findContours( modImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	//calculate moment for each contour
	vector<Moments> allMoments( contours.size() );
	for( size_t i = 0; i < contours.size(); i++ )
	{
		allMoments[i] = moments( contours[i], true );
	}

	//calculate centroid & angle for each contour
	vector<Point2d> allCentroids( contours.size() );
	vector<double> pAngle( contours.size() );
	for( size_t i = 0; i < contours.size(); i++ )
	{
		allCentroids[i] = Point2d( allMoments[i].m10 / allMoments[i].m00, allMoments[i].m01 / allMoments[i].m00 );
		pAngle[i] = 0.5 * atan2( 2*allMoments[i].mu11, allMoments[i].mu20 - allMoments[i].mu02 );
		cout<<allCentroids[i] << ' ' << pAngle[i] * (180 / 3.14) << endl; //prints the results
	}

	/// Draw circle for centroid and line for principle angle
	Mat drawing = Mat::zeros( modImage.size(), CV_8UC3 );
	for( size_t i = 0; i< contours.size(); i++ )
		{
		circle( sourceImage, allCentroids[i], 100, Scalar(255, 0, 0) );
		Point2d x0 = Point2d( 0, allCentroids[i].y - tan( pAngle[i] ) * allCentroids[i].x );
		Point2d x1 = Point2d( modImage.cols, allCentroids[i].y + tan(pAngle[i]) * (modImage.cols - allCentroids[i].x ));
		line( sourceImage, x0, x1, Scalar(255,0,0) );
		}

	cv::imshow( "Modified", sourceImage ); //show the image in window
	waitKey();
	
	return 0;
}