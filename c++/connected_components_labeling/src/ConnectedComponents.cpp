/* Matthew England
* Assignment 2
**/
#include "ConnectedComponents.hpp"

#include <iostream>

void ConnectedComponents::run() {
	//this->displayParents();
	//exit(0);
	this->labeling();
}
/*
 * Implements the two-pass eight connected components algorithm over a cv::Mat image
 * Using a Union Find and Decision Tree
 **/
void ConnectedComponents::labeling() {
	/*
	 *  
	 **/
	const unsigned int max_rows = this->image.rows;
	const unsigned int max_cols = this->image.cols;
	unsigned int current_pixel_idx = 0; // current pixel thus used for indexing inside of parent	
	//Start one pixel in to avoid overflow on first pass

	for (int row = 0; row < max_rows; ++row) {
		for (int col = 0; col < max_cols; ++col) {
			current_pixel_idx = row * max_cols + col;
			// current_pixel_idx is background color so skip this pixel
			if (image.at<uchar>(row,col) == this->background) {
				continue;
			}

			// Decision Tree for 8 connected Components
			if (this->withinImage(row - 1, col) && this->parent[(row -1) * max_cols + col]) { // B Check
				 this->parent[current_pixel_idx] = this->parent[(row - 1) * max_cols + col]; // copy b into current location
		
			}
			else if (this->withinImage(row - 1, col + 1) && this->parent[(row - 1) * max_cols + (col + 1)]) { 
				if ( this->withinImage(row - 1, col - 1) && parent[(row - 1) * max_cols + (col - 1)]) {
					//union of c,a
					this->parent[current_pixel_idx] = this->unite(this->parent[(row - 1) * max_cols + (col + 1)], this->parent[(row - 1) * max_cols + (col - 1)]); 
				}
				else if (this->withinImage(row, col - 1) && this->parent[row * max_cols + (col - 1)]){
					this->parent[current_pixel_idx] =  this->unite(parent[(row - 1) * max_cols + (col + 1)], this->parent[row * max_cols + (col - 1)]); 
					//union of c,d
				}
				else {
					this->parent[current_pixel_idx] = this->parent[(row - 1) * max_cols + (col + 1)]; // take c's label
				}
			}
			else if (this->withinImage(row - 1, col -1) && this->parent[(row - 1) * max_cols + (col - 1)]) {
				this->parent[current_pixel_idx] = this->parent[(row - 1) * max_cols + (col - 1)]; // take a's label
			}
			else if (this->withinImage(row, col -1) && parent[row * max_cols + (col - 1)]) {
				this->parent[current_pixel_idx] = this->parent[row * max_cols + (col - 1)]; //take d's label
			}
			else {
				this->parent[current_pixel_idx] = this->labels++; //create a new label
					
			}	
		}
	}
	/*
	 * Pass Two fixes any issues in the parent array that is holding the labels
	 **/
	for (int row  = 0; row < max_rows; ++row) {
		for (int col = 0; col < max_cols; ++col) { 
			
			if (image.at<uchar>(row,col) == 0) {
				int parent = findCompression(row * max_cols + col);
				this->objects[parent].push_back(row * max_cols + col);
			}

		}
	}
}

/*
 * Returns the objects extacted from the images, the key is the label and the vector as the pixel associated with that label
 **/
std::map<int,std::vector<int> > ConnectedComponents::getObjectMap() {
	return this->objects;
}

/*
 * Returns the number of used intermediate labels
 **/
int ConnectedComponents::getNumberOfIntermediateLabels () {
	return this->labels;
}

/*
 * Union Operation of two array indexes together
 **/
int ConnectedComponents::unite (int x, int y) {
	int j = find(x);
	int k = find(y);
	if (j != k) {
		this->parent[k] = j;
	}
	return std::min(j,k);
}

/*
 * Finds the root of the passed index x 
 **/
int ConnectedComponents::find (int x) {
	int j = x;

	while (this->parent[j] != 0) {
		if (this->parent[j] < 0) {
			//std::cout << "Negative: " << this->parent[j]  << std::endl;
		}
		//std::cout << "STUCK IN FIND with (" <<  j << "'" << x <<")" << std::endl;
		j = parent[j];
	}
	return j;
}

/*
 * Compresses the path inside the parent label array for each incoming index
 **/
int ConnectedComponents::findCompression(int x) {
	int j = x;

	while (this->parent[j]  !=  0) {
    	j  =  this->parent[j];
	}
	this->parent[x] = j;  
	return j;
} 
void ConnectedComponents::displayParents () {
	const int max_rows = image.rows;
	const int max_cols = image.cols;
	for (int row  = 0; row < max_rows; ++row) {
		for (int col = 0; col < max_cols; ++col) { 
						std::cout << this->parent[row * max_cols + col] << " ";
		}
		std::cout << std::endl;
	}

}

/*
 * Checks to make sure we are within the bounds of our image 
 **/
bool ConnectedComponents::withinImage (int r, int c) {
	if (r >= 0 && c >= 0 && c < this->image.cols &&  r < this->image.rows) {
		return true;
	}
	return false;
	
}
