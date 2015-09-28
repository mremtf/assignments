/*
 * Matthew England
 * 11/25/14
 * Assignment 4
 * Edge Detection
 */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <math.h>
 
using namespace cv;
using namespace std;
 
void arrowedLine(Mat& img, Point pt1, Point pt2, const Scalar& color,
int thickness=1, int line_type=8, int shift=0, double tipLength=0.1);
int getColorFromDegree (const float degree, int channel);

int main(int argc, char **argv )
{
	if (argc < 3) {
		std::cout << "Incorrect usage: ./edge_detector <input_image> <min_threshold>" << std::endl;
		return 0;
	}
	Mat src, src_gray;
	Mat grad;
	std::string input = "untouched image";
	std::string window_name = "Magnitude Image";
	std::string sobelx_name = "Sobel x";
	std::string sobely_name = "Sobel_y";
	std::string color_img = "Colored Angle";
	
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	
	int min_threshold = atoi(argv[2]);
	
	//int c;
	
	/// Load an image
	src = imread( argv[1] );
	
	if( !src.data )
	{ return -1; }
	
	
	
	imshow(input.c_str(), src);

	
	/// Convert it to gray
	cvtColor( src, src_gray, CV_RGB2GRAY );
	
	
    /// Create window
    namedWindow( window_name.c_str(), CV_WINDOW_AUTOSIZE );

    Mat out_img(src.rows,src.cols, CV_8UC3, Scalar::all(0));
    vector<Mat> channels;
    split(src, channels);
    /// Generate grad_x and grad_y
    std::vector<Mat> abs_grad_y_vec;
    std::vector<Mat> abs_grad_x_vec;
    for (int c = 0; c < channels.size(); ++c) {
    	
		Mat grad_x, grad_y;
		Mat abs_grad_x, abs_grad_y;
	
		/// Gradient X
		//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
		Sobel( channels[c], grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_REPLICATE );
		convertScaleAbs( grad_x, abs_grad_x, 1);
		namedWindow( sobelx_name.c_str(), CV_WINDOW_AUTOSIZE );
		imshow( sobelx_name.c_str(), abs_grad_x );
	
		/// Gradient Y
		//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
		Sobel( channels[c], grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_REPLICATE );
		convertScaleAbs( grad_y, abs_grad_y, 1);
		namedWindow( sobely_name.c_str(), CV_WINDOW_AUTOSIZE );
		imshow( sobely_name.c_str(), abs_grad_y );
	
		
		for (int i = 0; i < out_img.rows; ++i) {
			for (int j = 0; j < out_img.cols; ++j) {
	
				channels[c].at<uchar>(i,j) = sqrt(pow(abs_grad_x.at<uchar>(i,j),2) + pow(abs_grad_y.at<uchar>(i,j),2));

	
			}
		}
		abs_grad_y_vec.push_back(abs_grad_y);
		abs_grad_x_vec.push_back(abs_grad_x); 

    }
    merge(channels,out_img);

    imshow( window_name.c_str(), out_img );
    
    Mat colored_img(src.rows,src.cols, CV_8UC3, Scalar::all(255));

    const float degConversion = 180/3.14; 
    vector<Mat> color_channels;
    split(colored_img, color_channels);
    for (int ch = 0; ch < colored_img.channels(); ++ch) { // For each channel
		for (int r = 0; r < colored_img.rows; ++r) { // For each row in that channel
			for (int c = 0; c < colored_img.cols; ++c) { // For each col in that channel
				if (channels[ch].at<uchar>(r,c) > min_threshold) {
					if (abs_grad_x_vec[ch].at<uchar>(r,c) != 0) {
						float degree = (atan2(abs_grad_y_vec[ch].at<uchar>(r,c),abs_grad_x_vec[ch].at<uchar>(r,c)) * degConversion);
						if (degree < 0) {
							degree += 360;
						}
						int color = getColorFromDegree(degree,ch);
						color_channels[ch].at<uchar>(r,c) = color;
					}
					else {
						color_channels[ch].at<uchar>(r,c) = getColorFromDegree(0,ch);
					}	
				}
			}
		}
    }
    merge(color_channels,colored_img);
    namedWindow( color_img.c_str(), CV_WINDOW_AUTOSIZE );
    imshow(color_img.c_str(), colored_img);
    
    waitKey(0);
    return 0;
} 
/*
 * Converts ab angle into a HSV into a RGB color
 */
int getColorFromDegree (const float degree, int channel) {
	
	float H = degree;
	float V = .05;
	float S = 1.0;
	
	float a = 255 * V;
	float b = a * (1 - S);
	float c = (a-b) * (1 - abs(fmod(H/60,2) - 1));
	
	float R = a;
	float G = c + b;
	float B = b;
	
	if (H >= 60 && H < 120) {
		R = c + b;
		G = a;
		B = b;
	}
	else if (H >= 120 && H < 180) {
		R = b;
		G = a;
		B = c + b;
	}
	else if (H >= 180 && H < 240) {
		R = b;
		G = c + b;
		B = a;
	}
	else if (H >= 240 && H < 300) {
		R = c + b;
		G = b;
		B = a;
	}
	else if (H >= 300 && H < 360) {
		R = a;
		G = b;
		B = c + b;
	}
	
	if (channel == 0) { // BLUE
		return round(B);
	}
	else if (channel == 1) { // GREEN
		return round(G);
	}
	return round(R);
}
