/* Matthew England
* Assignment 2
**/
#ifndef _CONNECTED_COMPONENTS_HPP_
#define _CONNECTED_COMPONENTS_HPP_
#include <map>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class ConnectedComponents {
	private:
		uchar background;
		cv::Mat image;
		std::vector<int> parent;
		int labels;
		std::map<int,std::vector<int> > objects;
	public:
		/*
		 * Sets the incoming image to attribute image
		 * Sets the background to to d
		 * Allocates the correct memory for parent vector
		 * Assigns the labels to start at 1
		 **/
		ConnectedComponents (cv::Mat& image, uchar b) {
			this->background = b;
			this->image = image;
			parent = std::vector<int>(image.rows * image.cols + 1,0);
			this->labels = 1;
		}
		void run ();
		~ConnectedComponents() {
		
		}
		void displayParents();
		std::map<int,std::vector<int> > getObjectMap();
		int getNumberOfIntermediateLabels(); 
	private:
		void labeling();
		int unite(int x, int y);
		int find(int x);
		int findCompression(int x);
		bool withinImage (int r, int c);
};
#endif
