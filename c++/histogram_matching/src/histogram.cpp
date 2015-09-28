/* Matthew England
* Assignment 3
**/

//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace cv;
/**
* @function main
*/

void displayColorHistogram (Mat& src, vector<Mat>& bgr_planes, float* range,const float* histRange,int histSize);
void displayGrayHistogram (Mat& src,float* range, const float* histRange,int histSize);
void logTheImage (Mat& img);
void equalization(Mat& img); 

int main( int argc, char** argv )
{
	if (argc < 2) {
		cout << "Incorrect usage! ./histogram input_image <num_of_bins>" << std::endl;
		return 0;
	}
	int histSize = 256;
	int bins = 1;
	if (argc == 3) {
		bins = atoi(argv[2]); 
	}
	Mat src, dst;
	/// Load image
	//src = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	src = imread( argv[1], CV_LOAD_IMAGE_UNCHANGED);
	//Mat src_copy(src);
	resize(src, src, Size(src.cols/4, src.rows/4)); // resized to half size
	imshow("Input Image", src);
	waitKey(0);
	bool isGrayScale = false;

	//int histSize = atoi(argv[2]);
	//float range[] = {0,256};
	
	float *range = (float*) malloc(sizeof (float) * 2);
	range[0] = 0;
	range[1] = histSize/bins;
	
	const float* histRange = {range};
	Mat b_hist, g_hist, r_hist;

	if( src.empty() ){ 
		return -1; 
	}
	if (src.channels() == 1) {
		std::cout << "GRAYSCALE" << std::endl;
		isGrayScale = true;
	}
	
	if (isGrayScale) {
		displayGrayHistogram(src,range,histRange,histSize);
	}
	else {
		/// Separate the image in 3 places ( B, G and R )
		vector<Mat> bgr_planes;
		split( src, bgr_planes );
		displayColorHistogram(src,bgr_planes,range,histRange,histSize);
	}
	src = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	resize(src, src, Size(src.cols/4, src.rows/4)); // resized to half size
	/*namedWindow( "Before Log", CV_WINDOW_AUTOSIZE );
	imshow( "Before Log", src );
	waitKey(0);*/
	logTheImage(src);
	
	free(range);
	/*
	 * PART 2 showing the soruce image vs the equalized image
	 * 
	 */
	src = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	resize(src, src, Size(src.cols/4, src.rows/4)); // resized to half size
	equalization(src);
	
	return 0;
}

void equalization(Mat& img) {
	Mat dst;
	//Display the original image and histogram equalized image
	equalizeHist( img, dst );
	
	int histSize = 256;
	float range[] = {0,256};
	const float* histRange = {range};
	/// Display results
	namedWindow( "Part 2) Source window", CV_WINDOW_AUTOSIZE );
	imshow( "Part 2) Source Image", img );
	waitKey(0);
	displayGrayHistogram(img,range,histRange,histSize);
	namedWindow( "Part 2) equalized window", CV_WINDOW_AUTOSIZE );
	imshow( "Part 2) Equalized Image", dst );
	/// Wait until user exits the program
	waitKey(0);
	displayGrayHistogram(dst,range,histRange,histSize);
}

void logTheImage (Mat& img) {
	
	Mat outputImage( img.rows, img.cols, CV_8UC1, cv::Scalar::all(0) );
	
	for (int row = 0; row < outputImage.rows; ++row) {
		for (int col = 0; col < outputImage.cols; ++col) {
			if (img.at<uchar>(row,col) != 0) {
				outputImage.at<uchar>(row,col) = log10(img.at<uchar>(row,col)) * 100;
			}
			else {
				outputImage.at<uchar>(row,col) = 0;
			}
			//outputImage.at<uchar>(row,col) = img.at<uchar>(row,col);
		}
	}
	
	namedWindow("Part 1 c) Log of Image", CV_WINDOW_AUTOSIZE );
	imshow("Part 1 c) Log of Image", outputImage );
	waitKey(0);
	
}

void displayGrayHistogram (Mat& src,float* range, const float* histRange,int histSize) {
	
	Mat hist;
	calcHist( &src, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false );
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );
	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	/// Draw for each channel
	for( int i = 1; i < histSize; i++ ) {
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
		Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
		Scalar( 255, 0, 0), 2, 8, 0 );
	}
	/// Display
	namedWindow("GRAYSCALE Histogram", CV_WINDOW_AUTOSIZE );
	imshow("GRAYSCALE Histogram", histImage );
	waitKey(0);
}

void displayColorHistogram (Mat& src, vector<Mat>& bgr_planes,float* range, const float* histRange,int histSize) {
	Mat b_hist, g_hist, r_hist;
	/// Compute the histograms:
	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, true, false );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, true, false );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, true, false );

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );
	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	/// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
		Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
		Scalar( 255, 0, 0), 2, 8, 0 );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
		Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
		Scalar( 0, 255, 0), 2, 8, 0 );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
		Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
		Scalar( 0, 0, 255), 2, 8, 0 );
	}
	/// Display
	namedWindow("COLOR Histogram", CV_WINDOW_AUTOSIZE );
	imshow("COLOR Histogram", histImage );
	waitKey(0);

}


