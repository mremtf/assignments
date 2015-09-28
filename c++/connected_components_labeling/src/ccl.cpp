/* Matthew England
* Assignment 2
**/
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <vector>
#include <map>
#include <ctime>
#include <cstring>
#include "ConnectedComponents.hpp"
#include <fstream>

#define INPUT_IMAGE_IDX_WO_INVERT 1
#define OUTPUT_IMAGE_IDX_WO_INVERT 2

#define INPUT_IMAGE_IDX_W_INVERT 2
#define OUTPUT_IMAGE_IDX_W_INVERT 3


struct ObjectStats {	
	// ID of Object
	int id;
	//Parent Label of Object
	int parent_label;
	//Number of pixels per parent
	int area;
	// First Moment
	std::pair<int,int> centroid;
	// Second Moment
	double row_variance;
	double col_variance;
	double covariance;
};

void findObjectStatsWriteColorImage (std::map<int,std::vector<int> >& objects, std::vector<ObjectStats*>& stats,
		char* image_name, cv::Mat& binaryImage, int objectSizeThreshold); 

int main (int argc, char *argv[]) {
	/*
	 * Verifies that the user passes in atleast 3 parameters
	 **/
	if( argc < 3)
    {
		std::cout <<" Usage: ./ccl <-invert> <-size> <remove size>  input_image output_image" << std::endl;
     return -1;
    }
	/*
	 * Creates a random generator and sets my flags depending on usage of the program
	 **/
	srand(time(NULL));
	bool invert_set = false;
	bool remove_set = false;

	/*
	 * Creates a grayScale an binary Mat for storing the image during each converstion stage
	 * from gray to binary
	 **/
	cv:: Mat grayImage, binaryImage;
	
	int remove_size = 0;
	
	/*
	 * Handles the possible 
	 **/
	if (argc == 4 && strcmp(argv[1],"-invert") == 0) {
	    grayImage = cv::imread(argv[INPUT_IMAGE_IDX_W_INVERT], CV_LOAD_IMAGE_GRAYSCALE); // Read the file
		cv::threshold (grayImage,binaryImage,100,255,cv::THRESH_BINARY_INV);
		invert_set = true;	
	}
	else if (argc == 5 && strcmp(argv[1], "-size") == 0 && (remove_size = atoi(argv[2])) >= 0) {
		grayImage = cv::imread(argv[3], CV_LOAD_IMAGE_GRAYSCALE); // Read the file
		cv::threshold (grayImage,binaryImage,128,255,cv::THRESH_BINARY);
		remove_set = true;
	}
	else if (argc == 6 && strcmp(argv[2], "-size") == 0 && (remove_size = atoi(argv[3])) >= 0) {
		grayImage = cv::imread(argv[4], CV_LOAD_IMAGE_GRAYSCALE); // Read the file
		cv::threshold (grayImage,binaryImage,100,255,cv::THRESH_BINARY_INV);
		invert_set = true;
		remove_set = true;
	}
	else if (argc == 3){
	    grayImage = cv::imread(argv[INPUT_IMAGE_IDX_WO_INVERT], CV_LOAD_IMAGE_GRAYSCALE); // Read the file
		cv::threshold (grayImage,binaryImage,128,255,cv::THRESH_BINARY);
	}
	else {	
		std::cout << " Usage: ./ccl <-invert> <-size> <remove size>  input_image output_image" << std::endl;
		return -1;
	}

	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE ); // Create a window for display.
	cv::imshow( "Display window", binaryImage );                // Show our image inside it.

	while(cv::waitKey(0) != (int) 13); // Wait for a keystroke in the window

	/*
	 * Creates an object that will do the connected components on the passed image converted to binary
	 **/
	ConnectedComponents *connectedComponents = new ConnectedComponents(binaryImage,255);
	connectedComponents->run();
	
	/*
	 * The Objects extracted from the image: int being the parent label and the vector
	 * being the pixel associated with that parent label
	 **/
	std::map<int,std::vector<int> > objectsMap = connectedComponents->getObjectMap();

	/*
	 * Depending on the program mode choosen, write the output image and remove objects less than
	 * a minimum pixel threshold
	 **/
	std::vector<ObjectStats*> stats;
	if (invert_set && remove_set) {
		 findObjectStatsWriteColorImage(objectsMap,stats,argv[5], binaryImage, remove_size);	
	}
	else if (invert_set) {
		findObjectStatsWriteColorImage(objectsMap,stats,argv[OUTPUT_IMAGE_IDX_W_INVERT], binaryImage,remove_size);
	}
	else if (remove_set) {
		findObjectStatsWriteColorImage(objectsMap,stats,argv[4], binaryImage,remove_size);
	}
	else {
		findObjectStatsWriteColorImage(objectsMap,stats,argv[OUTPUT_IMAGE_IDX_WO_INVERT], binaryImage, remove_size);
	}

	/*
	 * Write the output statistics to a file for viewing later. 
	 * Writes the number of objects found, number of intermediate labels used, each objects area, centriod, and second moments
	 **/
	std::ofstream out;
	out.open("ObjectDetectionStats.txt");
	out << std::endl;
	out << "Number of Objects: " << stats.size() << std::endl;
	out << "Number of intermediate labels used: " 
		<< connectedComponents->getNumberOfIntermediateLabels() << std::endl << std::endl;
			
	std::vector<ObjectStats*>::iterator obj_itr;

	for (obj_itr = stats.begin(); obj_itr != stats.end(); ++obj_itr) {
		out << std::endl <<"Object ID: " << (*obj_itr)->id << std::endl;
		out << "Object Area: " << (*obj_itr)->area << std::endl;
		out << "First Moment-Centriod (" << (*obj_itr)->centroid.first << "," << (*obj_itr)->centroid.second << ")" << std::endl;
		out << "Second Moment:" <<std::endl<< "Row Variance: " << (*obj_itr)->row_variance << std::endl << "Column Variance: "
			<< (*obj_itr)->col_variance << std::endl << "Covariance: " << (*obj_itr)->covariance << std::endl << std::endl;
	}

	out << std::endl;
	out.close();

	delete connectedComponents;
	return 0;
}

/*
 * Handles the creation of the statistics of each extracted object and write images to the filesystem
 **/
void findObjectStatsWriteColorImage (std::map<int,std::vector<int>>& objects, std::vector<ObjectStats*>& stats,
		char* image_name, cv::Mat& binaryImage, int objectSizeThreshold = 0) {
	
	cv::Mat rgbImage(binaryImage.size(),CV_8UC3, cv::Scalar::all(255));


	std::map<int,std::vector<int> >::iterator map_itr;
	std::map<int,std::vector<int> >::iterator remove_itr;
	std::vector<int>::iterator vec_itr;
	std::vector<ObjectStats*>::iterator obj_stats_itr, obj_remove_itr;

	/*
	 * Finds the first moment of each object and stores that information
	 * into the passed in stats vector of type ObjectStats
	 **/
	map_itr = objects.begin();
	int i = 1;
	for (;map_itr != objects.end() ; ++map_itr, ++i) {

		const int areaOfObject = (*map_itr).second.size();
		ObjectStats *currObjStats = new ObjectStats;
		currObjStats->id = i;
		currObjStats->parent_label = (*map_itr).first;
		int rowCount = 0;
		int colCount = 0;

		vec_itr = map_itr->second.begin();
		for (; vec_itr != map_itr->second.end(); ++vec_itr) {
			rowCount += (*vec_itr)/rgbImage.cols;
			colCount += (*vec_itr) % rgbImage.cols;
		}

		const int centroid_row = rowCount/areaOfObject;
		const int centroid_col = colCount/areaOfObject;

		currObjStats->centroid = std::make_pair(centroid_row,centroid_col);
		currObjStats->area = areaOfObject;

		stats.push_back(currObjStats);

	}
	/*
	 * Finds the second moment of each object and stores that information
	 * into the passed in stats vector of type ObjectStats
	 **/

	map_itr = objects.begin();
	obj_stats_itr = stats.begin();

	for (; map_itr != objects.end() ; ++map_itr, ++obj_stats_itr) {
		 double variance_of_row = 0;
		 double variance_of_col = 0;
		 double covariance = 0;

		vec_itr = map_itr->second.begin();
		for (; vec_itr != map_itr->second.end(); ++vec_itr) {

			variance_of_row += pow((*vec_itr)/rgbImage.cols - (*obj_stats_itr)->centroid.first,2);
			variance_of_col += pow((*vec_itr) % rgbImage.cols - (*obj_stats_itr)->centroid.second,2);
			covariance += ((*vec_itr)/rgbImage.cols - (*obj_stats_itr)->centroid.first) * ((*vec_itr) % rgbImage.cols - (*obj_stats_itr)->centroid.second);
		}
		(*obj_stats_itr)->row_variance = (double) variance_of_row/(*obj_stats_itr)->area; 
		(*obj_stats_itr)->col_variance =  (double) variance_of_col/(*obj_stats_itr)->area; 
		(*obj_stats_itr)->covariance =  (double) covariance/(*obj_stats_itr)->area;
	}

	// Color the image for different objects
	map_itr = objects.begin();
	for (;map_itr != objects.end() ; ++map_itr) {
		//out << "Object id: " << map_itr->first << std::endl;
		cv::Vec3b color((rand() % 256),(rand() % 256),(rand() % 256));

		vec_itr = map_itr->second.begin();
		for (; vec_itr != map_itr->second.end(); ++vec_itr) {
			rgbImage.at<cv::Vec3b>((*vec_itr)/rgbImage.cols, (*vec_itr) % rgbImage.cols) = color;
		}
	}
	cv::imshow("Colored Image", rgbImage);
	while(cv::waitKey(0) != (int) 13); 
	cv::imwrite(image_name, rgbImage);

	/*
	 * Removes the objects from images that are less than the minimum threshold
	 **/
	rgbImage = cv::Mat(binaryImage.size(),CV_8UC3, cv::Scalar::all(255));
	obj_stats_itr = stats.begin();
	int count = 0;
	for (;obj_stats_itr != stats.end() ; ++obj_stats_itr) {
		if ((*obj_stats_itr)->area < objectSizeThreshold) {
			obj_remove_itr = obj_stats_itr;
			--obj_stats_itr;
			stats.erase(obj_remove_itr);
			count++;
			continue;
		}	
		cv::Vec3b color((rand() % 256),(rand() % 256),(rand() % 256));
		map_itr = objects.find((*obj_stats_itr)->parent_label);

		vec_itr = map_itr->second.begin();
		for (; vec_itr != map_itr->second.end(); ++vec_itr) {
			rgbImage.at<cv::Vec3b>((*vec_itr)/rgbImage.cols, (*vec_itr) % rgbImage.cols) = color;
		}
	}
	if (count) {
		cv::imshow("Removed Smaller Colored Objects", rgbImage);
		while(cv::waitKey(0) != (int) 13);
		cv::imwrite("Removed_Small_Colored_Objects.png", rgbImage);
	}
	
}
