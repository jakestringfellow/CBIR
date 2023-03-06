/* Jake Stringfellow
   CS 5330 Spring 2023
   Project 2: Content: Based Image Retrievel
   histogram.hpp
   Header file for functionality necessary for histogram generation and comparison
*/

#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP

#include <cstdio>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include "histogram.hpp"
#include <opencv2/imgproc.hpp>

/* Calculates RGB histogram from a source Mat type
   Parameters: src:  image the histogram will be based on
	       hist: mat object where the resulting histogram will be stored
	       size: integer representing the number of "bins" the histogram will have 
   Returns:    int 0 if successful
*/
int calc_RGB_histogram( cv::Mat &src, cv::Mat &hist, int bins );

/* Variation of calc_RGB_histogram(), Calculates RGB histogram from the center of a source Mat type
   Parameters: src:  image the histogram will be based on
               hist: mat object where the resulting histogram will be stored
               size: integer representing the number of "bins" the histogram will have 
   Returns:    int 0 if successful
*/
int calc_centered_histogram( cv::Mat &src, cv::Mat &hist, int bins );

/* Calculates magnitude image based on the given image's X and Y sobel images
   Parameters: src: image the magnitude image will be based on
   Returns:    The resulting magnitude image in Mat form 
*/
cv::Mat magnitude(cv::Mat &src);

/* Variation of magnitude(), Calculates magnitude image based on the center of a given image's X and Y sobel images
   Parameters: src: image the magnitude image will be based on
   Returns:    The resulting magnitude image in Mat form 
*/
cv::Mat centered_magnitude(cv::Mat &src);

/* Flattens 3-Dimensional histogram into one long vector
   Parameters: src: Histogram in mat type
   Returns:    std::vector<float>, the same histogram flattened into vectors
*/
std::vector<float> flattenHistogram(cv::Mat &src);

/* Calculates distance between two flattened histogram vectors
   Parameters: ha: "Histogram A", the first histogram given
	       hb: "Histogram B", the second histogram given
   Returns:    The floating point value of the distance between the two histograms
	       Number is between 0 and 1
*/
float intersection_distance( std::vector<float> &ha, std::vector<float> &hb );

/* Calculates RGB histograms from a source Mat type for the top and bottom halves 
   of the image
   Parameters: src:  image the histogram will be based on
               histA: mat object where the resulting histogram for the top half of
		      the image will be stored
               histB: mat object where the resulting histogram for the bottom half of
		      the image will be stored
	       size: integer representing the number of "bins" the histogram will have 
   Returns:    int 0 if successful
*/
int calc_multi_histogram( cv::Mat &src, cv::Mat &histA, cv::Mat &histB, int bins);

#endif
