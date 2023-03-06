/* Jake Stringfellow
   CS 5330 Spring 2023
   Project 2: Content-Based Image Retrieval
   baseline.cpp
   Implements functionality necessary for baseline comparisons
*/

#include <cstdio>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include "baseline.hpp"

/* Function for computing feature vectors for baseline comparisons
   Parameters: src, a mat object whose feature vector is to be calculated
   Returns: a vector containing 243 values representing the RGB values contained in the 
	    9x9 box of pixels in the middle of the image
*/
std::vector<float> baselineFeatureVector(cv::Mat src) {

    std::vector<float> featureVector;
    // loop over the rows
    for (int i=(src.rows/2-4); i<(src.rows/2+5); i++) {
        // get the row pointer for row i
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);

        //     loop over the columns
        for (int j=(src.cols/2-4); j<(src.cols/2+5); j++) {
            // use row pointers to store the feature data
            featureVector.push_back(rptr[j][0]);
            featureVector.push_back(rptr[j][1]);
            featureVector.push_back(rptr[j][2]);

        }
    }

    return featureVector;
}
