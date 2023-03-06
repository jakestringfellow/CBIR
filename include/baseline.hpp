/* Jake Stringfellow
   CS 5330 Spring 2023
   Project 2: Content-based Image Retrieval
   baseline.hpp
   Header file for baseline image comparisons
*/

#ifndef BASELINE_HPP
#define BASELINE_HPP

#include <iostream>
#include <cstdio>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cstdlib>

    /*  Function for computing feature vectors for baseline comparisons
        Parameters: src, a mat object whose feature vector is to be calculated
        Returns: a vector containing 243 values representing the RGB values contained in the
                 9x9 box of pixels in the middle of the image
    */
    std::vector<float> baselineFeatureVector(cv::Mat src);

#endif
