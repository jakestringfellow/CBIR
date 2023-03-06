/* Jake Stringfellow
   Spring 2023 CS 5330
   Project 2: Content-Based Image Retrieval
   main.cpp
*/

#include <cstdio> // lots of standard C/C++ functions including printf, scanf
#include <cstring> // C/C++ functions for working with strings (char arrays)
#include <opencv2/opencv.hpp> // OpenCV main include file
#include <vector> // C++ Vectors
#include <string>
#include <cstdlib>
#include <dirent.h>
#include <cmath>
#include "baseline.hpp" // Contains functions for computing baseline comparisons
#include "histogram.hpp" // Contains functions for computing histogram comparisons

// Data type to store the image ID and distance from target image together
struct ImageData {
    // The image filename
    std::string ID;
    // The distance from the target image data
    float distance;
};

// Helper function to store each image of the database
// Taken from Professor Maxwell's sample file reading code
std::vector<std::string> readDatabase(std::string database) {
    std::vector<std::string> databaseImageNames;

    // Get the directory path
    char dirname[256];
    strcpy(dirname, database.c_str());

    char buffer[256];
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;
    int i;

    // Print the directory path
    printf("Processing directory %s\n", dirname );

    // open the directory
    dirp = opendir( dirname );
    if( dirp == NULL) {
	printf("Cannot open directory %s\n", dirname );
	exit(-1);
    }

    // loop over all the files in the image file listing
    while( (dp = readdir(dirp)) != NULL ) {
	
	// check if the file is an image
	if( strstr(dp->d_name, ".jpg") ||
	    strstr(dp->d_name, ".png") ||
	    strstr(dp->d_name, ".ppm") ||
	    strstr(dp->d_name, ".tif") ) {

	    //printf("processing image file: %s\n", dp->d_name);
	
	    // build the overall filename
	    strcpy(buffer, dirname);
	    strcpy(buffer, "/");
	    strcpy(buffer, dp->d_name);

	    std::string filename = dp->d_name;

	    databaseImageNames.push_back(filename);

	    //printf("full path name: %s\n", dp->d_name);
	}
    }
   
    return databaseImageNames;
}


// define main function
// argc is the number of command line arguments
// argv is an array of character arrays (command line arguments)
// argv[0] is the name of the executable function
int main( int argc, char *argv[]) {

    if( argc < 3 ) {
        printf("Include target image, database, computation method, and number of output images.\n" );
        exit(-1);
    }

    cv::Mat targetImage;

    // Name of target file
    std::string filename(argv[1]);
    
    // Name of the database
    std::string dbName(argv[2]);

    // Computation method
    std::string compMethod(argv[3]);



    // Number of output images N
    int N = atoi(argv[4]);
    

    // Read target image
    targetImage = cv::imread( filename ); // reads the image from the file

    // Feature vector for target
    std::vector<float> F_target;
    // Mat object for target histogram 
    cv::Mat H_target;
    // Mat object for second target histogram (for multi-histogram matching)
    cv::Mat H_targetB;
    // Vector for flattened target histogram
    std::vector<float> flatTarget;
    // Vector for second flattened target histogram
    std::vector<float> flatTargetB;
    // Compute feature vector from the target image F_target based on comp method
//    switch(method) {
//	case 'b' : 
//	    F_target = baselineFeatureVector(targetImage);
//	    break;
//    }
    if (compMethod == "baseline") {
	std::cout << "Creating target feature vector.." << std::endl;
	F_target = baselineFeatureVector(targetImage);
    }
    else if (compMethod == "histogram") {
	std::cout << "Creating target feature vector..." << std::endl;
	int success = calc_RGB_histogram( targetImage, H_target, 8 );
        // Flatten the target histogram into a vector
        flatTarget = flattenHistogram(H_target);
    }
    else if (compMethod == "multi-histogram") {
	std::cout << "Creating target feature vector...." << std::endl;
	int success = calc_multi_histogram( targetImage, H_target, H_targetB, 8);
        
	// Flatten the target histogram into a vector
        flatTarget = flattenHistogram(H_target);
        flatTargetB = flattenHistogram(H_targetB);
    }
    else if (compMethod == "texture-and-color-histogram") {
	std::cout << "Creating target feature vector....." << std::endl;
	// First create the magnitude image, which will serve as the second histogram spot
	cv::Mat mag = magnitude(targetImage);
	int successColor = calc_RGB_histogram( targetImage, H_target, 8 );
	int successTexture = calc_RGB_histogram( mag, H_targetB, 8 );

	// Flatten the target histogram into a vector
	flatTarget = flattenHistogram(H_target);
	flatTargetB = flattenHistogram(H_targetB); 
    }
    else if (compMethod == "recycle-detector") {
	std::cout << "Finding recycling bins" << std::endl;
	// First create the magnitude image, which will serve as teh second histogram spot
	cv::Mat mag = centered_magnitude(targetImage);
	int successColor = calc_centered_histogram( targetImage, H_target, 4 );
	int successTexture = calc_centered_histogram( mag, H_targetB, 4 );

        // FLatten the target histogram into a vector
	flatTarget = flattenHistogram(H_target);
	flatTargetB = flattenHistogram(H_targetB);
    }
 
    // Gather information from the database
    std::vector<std::string> databaseImageNames = readDatabase(dbName); 
    std::vector<float> distances;

    std::cout << "Creating feature vectors for database images..." << std::endl;

    // For each image i in the DB
    for (int i=0; i < databaseImageNames.size(); i++) {
        
	// Read image i
	std::string fullPathName = dbName + "/" + databaseImageNames[i];
	cv::Mat image = cv::imread( fullPathName );


	std::vector<float> F_dbImage;
	cv::Mat H_dbImage;
	cv::Mat H_dbImageB;

	// Compute feature vector from image i based on comp method	
	if (compMethod == "baseline") {
	    F_dbImage = baselineFeatureVector(image);
	   
	    float sum = 0;
            // Compute the distance from image i FV to F_target
            for (int i=0; i< F_dbImage.size(); i++) {
                sum += pow(F_target.at(i) - F_dbImage.at(i), 2);
            }
            // Store distance and image i ID
            distances.push_back(sum);
	}
 
	else if (compMethod == "histogram") {
	    // Calculate histogram of the current image
	    int success = calc_RGB_histogram( image, H_dbImage, 8 );
            // Flatten the image histogram into a vector
	    std::vector<float> flatImage = flattenHistogram(H_dbImage);
            // Calculate the intersection distance between the two vectors
	    float distance = intersection_distance( flatTarget, flatImage );
	    // Store the distance in the vector
	    distances.push_back(distance);  	
	    
	}
	
	else if (compMethod == "multi-histogram") {
	    // Calculate histograms for the top and bottom halves of the current image
	    int success = calc_multi_histogram( image, H_dbImage, H_dbImageB, 8);

	    // Flatten the histograms into vectors
	    std::vector<float> flatImageA = flattenHistogram(H_dbImage);
            std::vector<float> flatImageB = flattenHistogram(H_dbImageB);

	    // Calculate the intersection distance between the top vectors
	    float distanceA = intersection_distance(flatTarget, flatImageA);
	    // Calculate the intersection distance between the bottom vectors
	    float distanceB = intersection_distance(flatTargetB, flatImageB);
	    
	    // Use weighted averaging to combine the distances between the histograms
            float average = (distanceA + distanceB) / 2;  
	
	    distances.push_back(average);
	}
	else if (compMethod == "texture-and-color-histogram") {
	    // Create magnitude image for the database image
	    cv::Mat magnitudeDB = magnitude( image );
	    int successColor = calc_RGB_histogram( image, H_dbImage, 8 );
	    int successTexture = calc_RGB_histogram( magnitudeDB, H_dbImageB, 8 );

	    //Flatten the histograms into vectors
	    std::vector<float> flatImageColor = flattenHistogram(H_dbImage);
	    std::vector<float> flatImageTexture = flattenHistogram(H_dbImageB);

	    // Calculate the intersection distance between the top vectors
	    float distanceColor = intersection_distance(flatTarget, flatImageColor);
	    float distanceTexture = intersection_distance(flatTargetB, flatImageTexture);

	    // Use weighted averaging to combine the distances between the histograms
	    float average = (distanceColor + distanceTexture) / 2;

	    distances.push_back(average);
	}
	else if (compMethod == "recycle-detector") {
	    // Create magnitude image for the database image
	    cv::Mat magnitudeDB = centered_magnitude( image );

	    // Calculate histogram of the current image
            int successColor = calc_centered_histogram( image, H_dbImage, 4 );
	    int successTexture = calc_centered_histogram( magnitudeDB, H_dbImageB, 4 );

            // Flatten the image histogram into a vector
            std::vector<float> flatImageColor = flattenHistogram(H_dbImage);
	    std::vector<float> flatImageTexture = flattenHistogram(H_dbImageB);

            // Calculate the intersection distance between the two vectors
            float distanceColor = intersection_distance( flatTarget, flatImageColor );
	    float distanceTexture = intersection_distance( flatTargetB, flatImageTexture );

	    // Use weighted averaging to combine the distances between the histograms
	    float average = 0.4*(distanceColor) + 0.6*(distanceTexture);

            // Store the distance in the vector
            distances.push_back(average);
	}
    }
    
    // Create a vector storing the needed information from the database images, ID and distance
    std::vector<ImageData> DatabaseImageData;
    for (int i=0; i<distances.size(); i++) {
	ImageData temp;
	// Get the name of the file
	temp.ID = databaseImageNames.at(i);
	// Get the distance
	temp.distance = distances.at(i);
	// Store the info in the vector
	DatabaseImageData.push_back(temp);
    }
    // Sort the images based on their distance value
    std::sort(DatabaseImageData.begin(), DatabaseImageData.end(), [](const ImageData &i, const ImageData &j) { 
	return i.distance < j.distance; 
    });

    std::string Nstring = std::to_string(N);
    std::string result = "The " + Nstring + " most similar images in " + dbName + ": \n";
    std::cout << result << std::endl;
    // Checking to see the correct amount of images are in the vector
    //std::cout << DatabaseImageData.size() << std::endl;
    // Print out the top N closest images' names to the target
    for (int i=0; i<N; i++) {
	std::cout << DatabaseImageData.at(i).ID << std::endl;
    }


    printf("Terminating\n");

    // convention is to return 0 on successful completion
    return(0);
}
