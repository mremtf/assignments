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

float difference (std::vector<float>& a, std::vector<float>& b);
float differenceMat (Mat& a, Mat& b);

int main( int argc, char** argv ) {
	if (argc < 2) {
		std::cout << "Incorrect Usage of Program ./histMatch <target> <image2> <image3> <imageN>" << std::endl;
		return 0;
	}
	vector<Mat> images;
	for (int i = 1; i < argc; ++i) {
		Mat img = imread(argv[i], CV_LOAD_IMAGE_GRAYSCALE);
		if (img.data) {
			resize(img, img, Size(img.cols/4, img.rows/4));
			equalizeHist( img, img );
			images.push_back(img);
		}
	}
	if (images.size() < 2) {
		std::cout << "Need one more image to run histogram comparison" << std::endl;
		return 0;
	}
	/*
	 * FILL THE HISTOGRAMS
	 */
	vector<Mat> histograms(images.size());
	vector<std::vector<float> > cdfs; 
	int histSize = 256;
	float range[] = {0,256};
	
	const float* histRange = {range};
	//std::cout << histograms.size() << std::endl;
	for (int i = 0; i < histograms.size(); ++i) {
		//std::cout << "HERE" << std::endl;
		calcHist( &images[i], 1, 0, Mat(), histograms[i], 1, &histSize, &histRange, true, true );
		normalize(histograms[i], histograms[i], 0, histograms[i].rows, NORM_MINMAX, -1, Mat() );
		
		/*
		 * Calculates the CDF of every image passed
		 */
		Mat curr = histograms[i];
		vector<float> cdf;
		cdf.push_back(curr.at<float>(0, 0)); 
		
		for (int i=1; i < curr.rows; i++) {
			//float val = curr.at<float>(i, 0);
		    //cout << curr.at<float>(i, 0) << " "<< cdf[i - 1] << " " << val << std::endl;
		    cdf.push_back(curr.at<float>(i, 0) + cdf[i - 1]);
		    /*cout.precision(15);
		    cout << cdf[i] << endl;*/
		}
		cdfs.push_back(cdf);
	}
	
	namedWindow("Target", CV_WINDOW_AUTOSIZE);
	imshow("Target", images[0]);
	waitKey(0);
	std::cout << cdfs.size() << std::endl;
	for (int i = 1; i < cdfs.size(); ++i) {
		float diff = difference(cdfs[0],cdfs[i]);
		std::cout << diff << std::endl;
		if (diff < 10600) {
			namedWindow("Found Match", CV_WINDOW_AUTOSIZE);
			imshow("Found Match", images[i]);
			waitKey(0);
		}
	}
	
	return 0;
	
}
/*
 * HELPS IN FINDING IF AN IMAGE IS THE SAME WITH SIMILAR HISTOGRAMS
 */
float difference (std::vector<float>& a, std::vector<float>& b) {
	if (a.size() != b.size()) {
		return -1;
	}
	float diff = 0;
	/*for (int i = 0; i < a.size(); ++i) {
		diff += abs(a[i] - b[i]);
	}*/
	diff = abs(a[a.size() - 1] - b[b.size() - 1]);
	return diff;
}