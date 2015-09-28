/*
 * Matthew England
 * 11/25/14
 * Final
 * OCR Sudoku Solver
 */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cstdlib>
#include <math.h>
#include <utility>
#include <queue>
#include <climits>
#include <cfloat>


#define SUDOKU_IMAGE_IDX 1
#define N 9

void generateSquares(cv::Mat& croppedImage,std::vector<cv::Rect>& squaresSliced);
bool generateEdgesOfImage (cv::Mat& input, cv::Mat& edges);
void generateAndConvertImagesToNumbers(cv::Mat& input, std::vector<cv::Rect>& squares, int grid[N][N],
		std::map<int, std::vector<double> >& hogDatabase);

bool calcHistogramOfGradients(cv::Mat& cell, std::vector<double>& hog);
bool fillHogDatabse (std::map<int, std::vector<double> >& hogDatabase); 
int findTheNumber(std::map<int, std::vector<double> >& databaseOfHoG, std::vector<double>& histogram);
bool writeSolvedSudokuImage( int grid[N][N], std::vector<cv::Rect> squaresSliced, 
		std::map<int, std::vector<int> > hogDatabase);

void printGrid(int grid[N][N]);

#define HOG_BUCKETS 16
#define RAD2DEG(x) (180 *(x) / M_PI)
#define DEG2RAD(x) (M_PI *(x) / 180)

int grid_test[N][N] = {
		{3,4,0,8,2,6,0,7,1},
		{0,0,8,0,0,0,9,0,0},
		{7,6,0,0,9,0,0,4,3},
		{0,8,0,1,0,2,0,3,0},
		{0,3,0,0,0,0,0,9,0},
		{0,7,0,9,0,4,0,1,0},
		{8,2,0,0,4,0,0,5,9},
		{0,0,7,0,0,0,3,0,0},
		{4,1,0,3,8,9,0,6,2}
};

int pos = 0;
int bad_conversions = 0;

int main (int argc, char **argv) {
	
	if (argc < 2) {
		std::cout << "Incorrect usage: ./sudoku_ocr <sudoku_image>" << std::endl;
		return 0;
	}
	std::map<int, std::vector<double> > hogDatabase;

	fillHogDatabse(hogDatabase);
	
	std::cout << "DB SIZE " << hogDatabase.size() << std::endl;
	//return 0;
	/// Load an image
	cv::Mat srcImage, graySrcImage, binaryImage;
	srcImage = cv::imread( argv[SUDOKU_IMAGE_IDX] );
	
	if( !srcImage.data ) {
		std::cout << "image is damaged, or is incorrect format" << std::endl;
		return -1; 
	}
	/// Convert it to gray then binarize the image
	cv::cvtColor( srcImage, graySrcImage, CV_RGB2GRAY );
	cv::threshold( graySrcImage, binaryImage, 150, 255, 0 );
	//std::cout << binaryImage.channels() << std::endl;

	/*
	 * Preliminary Cropping of the outer edges not needed for finding sub squares
	 */
	std::string src_image = "Given Sudoku Image";
	cv::Rect edgeCropImage(9, 9, (binaryImage.cols - 18), (binaryImage.rows -18));
	cv::Mat subImageNoEdges(binaryImage, edgeCropImage);
	cv::Mat edges(subImageNoEdges.rows, subImageNoEdges.cols, CV_8UC1);
	generateEdgesOfImage(subImageNoEdges,edges);
	
    /// Display the unsolved sudoku image
    cv::namedWindow( src_image.c_str(), CV_WINDOW_AUTOSIZE );
    cv::Mat binaryEdge(edges.rows, edges.cols,CV_8UC1);
    cv::threshold(edges, binaryEdge, 20, 255, CV_THRESH_BINARY);
    cv::imshow(src_image.c_str(),srcImage);
    cv::waitKey(0);
    
    /*
     * Create rectangles that will be used to splice out sections 
     * on a binarized of the passed image
     */
	cv::imshow("BinaryEdgeDetection",binaryEdge);
	cv::waitKey(0);
    std::vector<cv::Rect> squaresSliced;
    generateSquares (binaryEdge,squaresSliced);
    
    /*
     * Converts the sub sqaures from the binary image passed in,
     * into digital numbers that a soduku solver can use.
     */
    int grid[N][N];
    generateAndConvertImagesToNumbers(subImageNoEdges,squaresSliced, grid, hogDatabase);
    std::cout << std::endl;
    double acc = (81 - bad_conversions) / 81.00;
    std::cout << "% Accuracy: " << acc << std::endl;
    std::cout << std::endl;
    printGrid(grid);
  
    
    
	return 0;
}

bool fillHogDatabse (std::map<int, std::vector<double> >& hogDatabase) {
	std::stringstream ss;
	cv::Mat srcImage, graySrcImage;
	std::map<int, std::vector<double> >::iterator map_itr;
	std::vector<double>::iterator vec_itr;
	int i = 0;
	for (i = 1; i < 20; ++i) {
		/*
		 * Reading the image files for the associated number
		 */
		ss << "../images_of_numbers/image_" << i << ".png";
		std::string filePath(ss.str());
		
		srcImage = cv::imread(filePath.c_str());
		//imshow("Inpput Image", srcImage);
		//cv::waitKey(0);
		//exit(0);
		if (!srcImage.data) {
			return false;
		}
		/*
		 * Allocate memory for 360 buckets and zero them
		 */
		cv::Mat binaryImage(srcImage.rows, srcImage.cols, CV_8UC1);
		cv::cvtColor( srcImage, graySrcImage, CV_RGB2GRAY );
		cv::threshold( graySrcImage, binaryImage, 150, 255, 0 ); 

		/*
		 * Converts the binaryImage that is 8U into an image that 32 bit float
		 */
		std::vector<double> hog(HOG_BUCKETS,0.0);		
		cv::Mat dst(binaryImage.rows,binaryImage.cols,CV_32F);
		binaryImage.convertTo(dst, CV_32F);
		
		/*
		 * Calculates the HistogramOfGradient
		 */
		calcHistogramOfGradients(dst,hog);		
		hogDatabase[i] = hog;
		ss.str("");
	}
	
	return true;
}

void generateAndConvertImagesToNumbers(cv::Mat& input, std::vector<cv::Rect>& squares, int grid[N][N],
		std::map<int, std::vector<double> >& hogDatabase) {
	
	const int max_rows  = 9;
	/*
	 * Converting from 8 bit to 32bit Float
	 */
	cv::Mat dst(input.rows,input.cols,CV_32F);
	input.convertTo(dst, CV_32F);
	
	for (int i = 0; i < 81; i++) {
		cv::Mat slicedSqaure(dst,squares.at(i));
		//cv::imshow("sliced square",slicedSqaure);
		//cv::waitKey(0);
		/*
		 * Generate HoG and find number with HoG
		 */
		std::vector<double> hog(HOG_BUCKETS,0.0);
		calcHistogramOfGradients(slicedSqaure,hog);
		const int number = findTheNumber(hogDatabase,hog);
		
		/*
		 * Store the row, col, and the number associated at that index
		 */
		grid[i/max_rows][i % max_rows] = number;
		//generatedNumbers[std::make_pair(i/max_rows,i % max_rows)] = number;
	}
}

bool calcHistogramOfGradients(cv::Mat& cell, std::vector<double>& hog) {
	/*
	 * Make sure the image is single banded, not multibanded
	 */
	if (cell.channels() != 1) {
		std::cout << "ERROR" << std::endl;
		return false;
	}
	const double bucketSize = 360.0 / HOG_BUCKETS;
	
	/*
	 * Calculate the sobel of x and y for the incoming image
	 */
	
	cv::Mat dy,dx;
	cv::Sobel(cell,dx, CV_32F, 1, 0, 3, 1, 0, cv::BORDER_REPLICATE); 
	cv::Sobel(cell,dy, CV_32F, 0, 1, 3, 1, 0, cv::BORDER_REPLICATE);
	
	/*
	 * Store the orientation into the histogram at that location
	 */
	int pixelAdded = 0;
	int pixelNotAdded = 0;
	for (int r = 0 ; r < dy.rows; ++r) {
		for (int c = 0 ; c < dx.cols; ++c)
		{
			const float dxVal = dx.at<float>(r,c);
			const float dyVal = dy.at<float>(r,c);
			
			const double magnitude = sqrt( dxVal * dxVal + dyVal * dyVal);
			if (magnitude > 0) {
				//std::cout << "dxVal: " << dxVal << " dyVal " << dyVal << std::endl;
				const double orientation = RAD2DEG(atan2( dyVal , dxVal )) + 180;
				int bucket = floor(orientation/bucketSize);
				//std::cout << "Ori:" << orientation << std::endl;
				//std::cout << "Bucket: " << bucket << std::endl;
				hog[bucket] += magnitude;
				pixelAdded++;
				
			}
			else {
				pixelNotAdded++;
			}
			
		}
	}
	/*std::cout << "pixelAdded " << pixelAdded << std::endl;
	std::cout << "pixelNotAdded " << pixelNotAdded << std::endl;
	std::cout << "sumOfPixel" << pixelNotAdded + pixelAdded << std::endl;
	std::cout << "hog size" << hog.size() << std::endl;*/
	return true;
}

int findTheNumber(std::map<int, std::vector<double> >& databaseOfHoG, std::vector<double>& histogram) {

	double totalSumDBHog = 0.0;
	double totalSumHog = 0.0;
	std::vector<double> hogToComparePDF;
	double minimum_divergence = FLT_MAX;
	int found_number = INT_MAX;
	
	/*
	 * Calculate the PDF for the incoming HoG
	 */
	std::vector<double>::iterator vec_itr, vec_itr2;
	std::vector<double>::iterator pdf_vec_itr, pdf_vec_itr_cp;
	
	vec_itr = histogram.begin();
	
	for (;vec_itr != histogram.end(); ++vec_itr) {
		totalSumHog += *vec_itr;
	}
	
	if (totalSumHog < 2) {
		pos++;
		return 0; // Empty Sqaure Handling
	}
	/*
	 * Compute PDF from histogram
	 */
	vec_itr = histogram.begin();
	for (;vec_itr != histogram.end(); ++vec_itr) {
		hogToComparePDF.push_back(*vec_itr/totalSumHog);
	}
	
	/*
	 * Loop through the entire databaseOfHoG
	 */
	std::map<int, std::vector<double> >::iterator map_itr;
	map_itr = databaseOfHoG.begin();
	
	for (;map_itr != databaseOfHoG.end(); ++map_itr) {
		
		/*
		 * Calculate the total sum of a hog from databseOfHog
		 */
		totalSumDBHog = 0.0;
		vec_itr2 = map_itr->second.begin();
		for (;vec_itr2 != map_itr->second.end(); ++vec_itr2) {
			totalSumDBHog += *vec_itr2;
		}
		
		/*
		 * Compute the PDF of a hog from databseOfHog
		 */
		std::vector<double> dbHogToComparePdf;
		vec_itr2 = map_itr->second.begin();
		for (;vec_itr2 != map_itr->second.end(); ++vec_itr2) {
			dbHogToComparePdf.push_back(*vec_itr2/totalSumDBHog);
		}

		if (dbHogToComparePdf.size() != hogToComparePDF.size()) {
			std::cout << "Not the same size PDFs" << std::endl;
			std::cout << dbHogToComparePdf.size() << "!=" << hogToComparePDF.size() << std::endl;
			exit(0);
		}
		
		double diff = 0;
		pdf_vec_itr = dbHogToComparePdf.begin();
		pdf_vec_itr_cp = hogToComparePDF.begin();
		
		/*
		 * Implements the Kullback-Leibler Divergence Formula:
		 * Formaula Sum of P(i) * ln(P(i)/Q(i))
		 */
		for (;pdf_vec_itr !=  dbHogToComparePdf.end(); ++pdf_vec_itr, ++pdf_vec_itr_cp) {

			if ((*pdf_vec_itr_cp > 0 && *pdf_vec_itr > 0) || (*pdf_vec_itr_cp < 0 && *pdf_vec_itr < 0)) {
				diff += *pdf_vec_itr * log(*pdf_vec_itr/ *pdf_vec_itr_cp);
			}
		}
		
		/*
		 * The lowest diff is the number the PDF belongs with
		 */
		if (diff < minimum_divergence) {
			minimum_divergence = diff;
			if (map_itr->first > 9) {
				found_number = map_itr->first - 9;
			}
			else {
				found_number = map_itr->first;
			}
		}
		
	}
	
	if (found_number != grid_test[pos/9][pos % 9]) {
		std::cout << "Difference " <<  found_number << "!=" << grid_test[pos/9][pos % 9] << std::endl;
		bad_conversions++;
	}
	pos++;
	
	return found_number;
}

bool generateEdgesOfImage (cv::Mat& cell, cv::Mat& edges) {
	
	/*
	 * Make sure the image is single banded, not multibanded
	 */
	if (cell.channels() != 1) {
		return false;
	}
	
	/*
	 * Calculate the sobel of x and y for the incoming image
	 */
	cv::Mat dy,dx, absDx, absDy;
	cv::Sobel(cell,dx, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_REPLICATE); 
	cv::Sobel(cell,dy, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_REPLICATE);
	
	cv::convertScaleAbs( dx, absDx);
	cv::convertScaleAbs( dy, absDy);
	
	/*
	 * Store the orientation into the histogram at that location
	 */
	for (int r = 0 ; r < absDy.rows; ++r) {
		for (int c = 0 ; c < absDx.cols; ++c)
		{
			uchar dxVal = absDx.at<uchar>(r,c);
			uchar dyVal = absDy.at<uchar>(r,c);			
			edges.at<uchar>(r,c) = sqrt( dxVal * dxVal + dyVal * dyVal);
			
		}
	}
	cv::imshow("EdgeDetection",edges);
	cv::waitKey(0);
    return true;
}

/*
 * Locates the squares in the image by taking the binary image that had edges outlined and uses them to cut
 * out rectangles for latter use in image extraction
 * 
 */
void generateSquares(cv::Mat& croppedImage, std::vector<cv::Rect>& squaresSliced) {
	int row = 0, col = 0; 
	int height = 0, width = 0;
	int sub_row, sub_col;
	int margin = 10;
	
	for (row = 0; row < croppedImage.rows; row += height + margin) {
		
		for (col = 0; col < croppedImage.cols; col += width + margin) { 
			width = 0;
			height = 0;
			for (sub_row = row; croppedImage.at<uchar>(sub_row,0) != 255 && sub_row < croppedImage.rows; ++sub_row){
				height++;
			}
			for (sub_col = col; croppedImage.at<uchar>(0,sub_col) != 255 && sub_col < croppedImage.cols; ++sub_col) {
				width++;
			}
			
			cv::Rect slicer(col + 3,row,width - 3,height - 3);
			//cv::Mat sliced(croppedImage,slicer);
		    //cv::imshow( "Sliced", sliced );
		    //cv::waitKey(0);
			//std::cout << "Rect" << slicer.x << " " << slicer.y << " " << slicer.width << " "<< slicer.height << std::endl;
			squaresSliced.push_back(slicer);
		}
	}
}

/* A utility function to print grid  */
void printGrid(int grid[N][N])
{
    for (int row = 0; row < N; row++)
    {
       for (int col = 0; col < N; col++)
             printf("%2d", grid[row][col]);
        printf("\n");
    }
}

