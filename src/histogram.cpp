/* Jake Stringfellow
   CS 5330 Spring 2023
   Project 2: Content: Based Image Retrievel
   histogram.cpp
   Implements functionality necessary for histogram generation and comparison
*/

#include <cstdio>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include "histogram.hpp"
#include <opencv2/imgproc.hpp>

// Creates a color histogram for a given source image
int calc_RGB_histogram( cv::Mat &src, cv::Mat &hist, int bins ) {
    // Variable for the number of bins
    const int Hsize = bins;
    
    // Divisor is equal to the number of possible values divided by the number of bins
    const int divisor = 256 / Hsize;

    int i, j;

    // defines dimensions of the matrix
    int dim[3] = {Hsize, Hsize, Hsize}; //Hisize * Hsize * Hsize

    // allocate and initialize a histogram
    hist = cv::Mat::zeros( 3, dim, CV_32S );
    
    // Access the rgb values of the source image
    for (i=0; i<src.rows;i++) {
	cv::Vec3b *sptr = src.ptr<cv::Vec3b>(i);
	for(j=0;j<src.cols;j++){
 	    int r = sptr[j][2] / divisor; // R index
	    int g = sptr[j][1] / divisor; // G index
	    int b = sptr[j][0] / divisor; // B index
	
	    // Set the rgb values obtained from the calculation into the Histogram
	    // space coordinates
	    hist.at<int>(r,g,b)++; // increment
	}
    }

    return (0);
}

// Creates histograms for the top and bottom halves of a given source image
int calc_multi_histogram( cv::Mat &src, cv::Mat &histA, cv::Mat &histB, int bins ) {
    
    // Variable for the number of bins
    const int Hsize = bins;

    // Divisor is equal to the number of possible values divided by the number of bins
    const int divisor = 256 / Hsize;

    int i, j;

    // defines dimensions of the matrix
    int dim[3] = {Hsize, Hsize, Hsize}; //Hisize * Hsize * Hsize

    // allocate and initialize histograms
    histA = cv::Mat::zeros( 3, dim, CV_32S );
    histB = cv::Mat::zeros( 3, dim, CV_32S );

    // Access the rgb values of the top half of the source image
    for (i=0; i<(src.rows/2);i++) {
        cv::Vec3b *sptrA = src.ptr<cv::Vec3b>(i);
        for(j=0;j<src.cols;j++){
            int r = sptrA[j][2] / divisor; // R index
            int g = sptrA[j][1] / divisor; // G index
            int b = sptrA[j][0] / divisor; // B index

            // Set the rgb values obtained from the calculation into the Histogram
            // space coordinates for the top half histogram
            histA.at<int>(r,g,b)++; // increment
        }
    }
    // Access the rgb values of the bottom half of the source image
    for (i=(src.rows/2); i< src.rows; i++) {
	cv::Vec3b *sptrB = src.ptr<cv::Vec3b>(i);
        for(j=0;j<src.cols;j++){
            int r = sptrB[j][2] / divisor; // R index
            int g = sptrB[j][1] / divisor; // G index
            int b = sptrB[j][0] / divisor; // B index

            // Set the rgb values obtained from the calculation into the Histogram
            // space coordinates for the bottom half histogram
            histB.at<int>(r,g,b)++; // increment
        }
    }


    return (0);
}

// Creates histogram for center of a given source image
int calc_centered_histogram( cv::Mat &src, cv::Mat &hist, int bins ) {
    // Variable for the number of bins
    const int Hsize = bins;

    // Divisor is equal to the number of possible values divided by the number of bins
    const int divisor = 256 / Hsize;

    int i, j;

    // defines dimensions of the matrix
    int dim[3] = {Hsize, Hsize, Hsize}; //Hisize * Hsize * Hsize

    // allocate and initialize a histogram
    hist = cv::Mat::zeros( 3, dim, CV_32S );

    // Access the rgb values of the source image
    for (int i=(src.rows/2-104); i<(src.rows/2+105); i++) {
        cv::Vec3b *sptr = src.ptr<cv::Vec3b>(i);
        for (int j=(src.cols/2-104); j<(src.cols/2+105); j++) {
            int r = sptr[j][2] / divisor; // R index
            int g = sptr[j][1] / divisor; // G index
            int b = sptr[j][0] / divisor; // B index

            // Set the rgb values obtained from the calculation into the Histogram
            // space coordinates
            hist.at<int>(r,g,b)++; // increment
        }
    }

    return (0);
}

// Creates magnitude image for the center of a given source image
cv::Mat centered_magnitude(cv::Mat &src) {
    // Initialize variables, mag = magnitude image
    cv::Mat mag = cv::Mat::zeros( src.size(), CV_16SC1 );
    // sX = X-sobel, sY = Y-sobel
    cv::Mat sX = cv::Mat::zeros( src.size(), CV_16SC1 );
    cv::Sobel(src, sX, -1, 1, 0);
    cv::Mat sY = cv::Mat::zeros( src.size(), CV_16SC1 );
    cv::Sobel(src, sY, -1, 0, 1);

    // Access each pixel from the two Sobels
    for (int i=(sX.rows/2-104); i<(sX.rows/2+105); i++) {

	// Create a pointer to the X-sobel and Y-sobel pixels
	cv::Vec3b *rptrX = sX.ptr<cv::Vec3b>(i);
	cv::Vec3b *rptrY = sY.ptr<cv::Vec3b>(i);

        for (int j=(sX.cols/2-104); j<(sX.cols/2+105); j++) {
	    for(int c=0; c<3; c++) {
		// Set the magnitude pixel equal to the sqrt of the x-sobel^2 + y-sobel^2
		mag.at<cv::Vec3b>(i,j)[c] = sqrt(rptrX[j][c]*rptrX[j][c] + rptrY[j][c]*rptrY[j][c]);
	    }
	}
    }  
    return mag;  
}

// Creates a magnitude image for a given source image
cv::Mat magnitude(cv::Mat &src) {
// Initialize variables, mag = magnitude image
    cv::Mat mag = cv::Mat::zeros( src.size(), CV_16SC1 );
    // sX = X-sobel, sY = Y-sobel
    cv::Mat sX = cv::Mat::zeros( src.size(), CV_16SC1 );
    cv::Sobel(src, sX, -1, 1, 0);
    cv::Mat sY = cv::Mat::zeros( src.size(), CV_16SC1 );
    cv::Sobel(src, sY, -1, 0, 1);

    // Access each pixel from the two Sobels
    for(int i=0; i<sX.rows; i++) {

        // Create a pointer to the X-sobel and Y-sobel pixels
        cv::Vec3b *rptrX = sX.ptr<cv::Vec3b>(i);
        cv::Vec3b *rptrY = sY.ptr<cv::Vec3b>(i);

        for (int j=0; j< sX.cols; j++) {
            for(int c=0; c<3; c++) {
                // Set the magnitude pixel equal to the sqrt of the x-sobel^2 + y-sobel^2
                mag.at<cv::Vec3b>(i,j)[c] = sqrt(rptrX[j][c]*rptrX[j][c] + rptrY[j][c]*rptrY[j][c]);
            }
        }
    }
    return mag;
}

// Flattens 3D histogram into a vector
std::vector<float> flattenHistogram(cv::Mat &src) {
    std::vector<float> flatHist;

    // Use openCV's reshape to turn the 3D Mat into one row with many columns
    cv::Mat result = src.reshape(0,1);

    // Creates the capacity to contain the same number as elements as there are cols
    flatHist.reserve(result.cols);

    // Copy the data into the vector
    result.copyTo(flatHist);

    // Return the vector
    return flatHist;
}

// intersection between two flattened histograms as std::vector
float intersection_distance( std::vector<float> &ha, std::vector<float> &hb ) {
    double intersection = 0.0;

    // compute the sum of the histogram buckets
    // Assuming histogram has not been normalized yet
    double suma = 0.0;
    double sumb = 0.0;
    for(int i=0; i<ha.size(); i++) {
	suma += ha[i];
	sumb += hb[i];
    }
    // compute intersection
    for(int i=0;i<ha.size(); i++) {
	double af = ha[i] / suma;
	double bf = hb[i] / sumb;
        
        // Add the minimum of either af or bf
	intersection += af < bf ? af : bf; // shorthand tripartite assignment
    }
    // convert to a distance
    // Maximum sum of buckets is 1.0
    return (1.0 - intersection);
}
