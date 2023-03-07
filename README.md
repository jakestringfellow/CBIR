# Content-Based Image Recognition

## Project Description: 
* *Please refer to the Project Report pdf to see some example output filenames (and the images they represent). Ability to display target image and 
   similar images within a GUI will be updated to this project later.*
* The Content-Based Image Retrieval project includes functionality to find and return the names of images from a given database most
  “similar” to an image given by a user. The user can choose between five different methods of computing features that will be used to match 
  images. All of the methods read in the image given by the user, read in the database given by the user, and return the number of similar images 
  the user wants in ascending order of similarity based on the feature computation method they chose. The project reads in the target image, database, 
  feature computation method, and number of similar images they wish to return. It computes the target image’s feature vector based on the given 
  computation method, then computes each of the database images’ feature vectors, then computes the distance between the two. Once the distance is 
  computed, the result is stored in a vector of “ImageData” structs, which contain the filename and the distance from the target image as attributes. 
  This vector is sorted in ascending order by the distance attribute and the filename’s of the first N ImageData structs is returned.
* The first method, baseline-matching uses a 9x9 square in the middle of the image as it’s feature vector. The 9x9 box of pixels is converted into a 
  vector containing 243 values that represent the RGB values in the center of the image, this target image’s vector is compared to the vectors of every
  other image in the database. The distance metric for this method is the sum of squared error, where the sum is taken from every element of the two
  feature vectors and then squared.
* The second method, histogram matching uses a single normalized 3-Dimensional color histogram as it’s feature vector. The histogram is created by    creating a mat object initialized to zero, then filled with values based on the RGB values of the target image and the given number of bins. The mat object storing the histogram is flattened into a vector containing all of the histograms values. This target image’s vector is compared to the vectors of every other image in the database. The distance metric for this method is histogram intersection, where the sum of the histogram bins is computed for each histogram vector, then an intersection value is added based on the contents of the vectors. The result is subtracted from 1.0, where exact copies will return a distance of zero and completely different images return up to 1.0.
* The third method, multi-histogram matching uses two normalized 3-Dimensional color histograms as it’s feature vector. The histograms are created the same way as the previous method, except these histograms each only account for half of the given image, the first and second histograms represent the top and bottom halves of the image respectively. The same histograms are computed for every image in the database. The distance metric for this method is also histogram intersection, however this time there is one distance value created for the intersection between the histograms representing the top halves of the target and database image, and a second distance value created for the intersection between the bottom halves of the images. Weighted averaging is used to combine the distances between the histograms, the two distances are added together and divided by 2, and the result represents the distance for the entire image.
* The fourth method, texture and color matching uses a whole image color histogram and a whole image texture histogram as it’s feature vector. The texture histograms for the target and database images are created by first creating a magnitude image of the given image by initializing a Mat type with one color channel to zero, then filling in the points based on values obtained by the X and Y sobel images created from the given image. Once the magnitude image is created, a texture histogram is made from it the same way a color histogram is created from the previous images. The color histograms for the target and database images are created the same way as the previous methods as well. The distance metric for this method is also histogram intersection, however this time there is one distance value created for the intersection between the texture histograms f the target and database image, and a second distance value created for the intersection between the color histograms of the images. Weighted averaging is used to combine the distances between the histograms, the two distances are added together and divided by 2, and the result represents the distance for the entire image.
* Compared to methods 2 and 3, this method places value on the contents of the image outside of color. For example a rock wall is featured in the most similar image, they do have a similar color scheme, but the presence of a rock wall has a significant impact on the texture histograms compared for the images, I imagine the large amount of edges in the 3rd and 4th image’s combined with their large amounts of whites, browns, and greens are what landed them in these spots. The results are images that share a relatively similar color scheme, but also similar amounts of edges (many) within their images. When a color comparison is used on pic.0535.jpg, the same top 2 results remain, but the 4th image is replaced with pic.0952.jpg, which shares many colors with the target image, but not a similar amount of edges.
* For the fifth method, I chose images of recycling bins as the type of image I wanted to execute CBIR on. There were four images of recycling bins present in the given database; pic.0287.jpg, pic.0288.jpg, pic.0291.jpg, and pic.0289.jpg. Since each image featured the blue recycling bin in the middle of the image, I decided to use a centered version of the texture and color method from the previous task, in a way similar to the 9x9 square from the baseline method, only this time the center square was 109x109 pixels. I chose this size, space, and method because the recycling bin images all had the recycling bin take up a significant amount of the middle space of their images, all were shades of the same color (blue), and all were similar smooth textures without many edges. The bin parameter of 4 was given to the histogram creation method, in order to reduce the range of colors to choose from in the middle of the image. The distance metric from the texture and color method was reused, where k1 = 0.4 for color and k2 = 0.6 for texture, in an effort to match recycling bins in different lighting (such as pic.0288.jpg).


## Going Further: 
* Adding a GUI to display the target image and resulting similar images will be the next step in this project.

                  
## Technologies used:  
* OpenCV - Open Source Computer Vision LIBRARY

## How to install and run: 
* 	Have the following files in the current directory: 
*		- makefile (update opencv libraries location ot your computer)
*		- main.cpp
*		- baseline.cpp
*		- histogram.cpp
*		- baseline.hpp
*		- histogram.hpp
*		- olympus (Database given in Project 2, replace with any database if needed)


* Enter "make similarImages" to the command-line
* Run the executable with: "./similarImages [your target file name here] [your database directory name here]
		  [feature computation method] [number of output images]"
* An example: 
*	    - "./similarImages olympus/pic.0291.jpg olympus texture-and-color-histograms 5"
*		- the above will return the top 5 images in olympus similar to pic.0291.jpg
*	- There are 5 different case-sensitive feature computation methods:
*		- "baseline" for baseline matching
*		- "histogram" for single histogram color matching
*		- "multi-histogram" for 1 top half and 1 bottom half histogram color matching
*		- "texture-and-color-histogram" for texture and color histogram matching
*		- "recycle-detector" for image retrieval specific to recycling bins,
		    uses texture and color histogram in the center of images



## Acknowledgements: 
* Histogram and histogram interception code were heavily based on the tutorial provided by Professor Bruce Maxwell. 
* Function for reading databases was heavily based on the code provided from Professor Bruce Maxwell within the assignment.
* Olympus database provided by Professor Bruce Maxwell in CS 5330 Pattern Recognition and Computer Vision
