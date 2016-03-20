#include <mex.h>
#include <opencv2/opencv.hpp>
#include <string>


using namespace cv;
using namespace std;

const char *USAGE = "usage: output = kmeansMex(image [n][m], rect[1][4])\n";
int image_width;


int index_matrix(int, int);

void kmeansMex(char *filename, double *rect){
    mexPrintf("filepath: %s\n", filename);
    string path(filename);
    // Use the first input to load the image
    //Mat image = imread(mxArrayToString(prhs[0]));
    Mat image = imread("100_0109.png");
    if(image.empty())
            mexPrintf("error reading image!\n");
    mexPrintf("Image Dimensions: [%d][%d]\n", image.rows, image.cols);
    image_width = image.cols;
    // Use the second input to create a Rect object
    //double *rect = mxGetPr(prhs[1]);
    Rect bounding_box(rect[0]-1, rect[1]-1, rect[2], rect[3]);
    mexPrintf("(x, y, w, h): [%d][%d][%d][%d]\n", bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height);
                                                                                                                            
    // Create a feature matrix [w*h][3]
    Mat feature_matrix(bounding_box.width * bounding_box.height, 3, CV_32F);
    // Create the corresponding initial label [w*h][1]
    Mat label(bounding_box.width * bounding_box.height, 1, CV_8U);
    // Use the input rectangle and assign the initial labels of the points inside and outside with 1 and 0 respectively
    for(int col=0;col<image.cols;col++){
        for(int row=0;row<image.rows;row++){
            mexPrintf("[%d][%d] -> %d\n", row, col, index_matrix(row, col));
            if(col >= bounding_box.x && col < bounding_box.width && row >= bounding_box.y && row < bounding_box.height){
                label.at<int>(index_matrix(row, col)) = 1; 
                mexPrintf("wut\n");
            }else
                label.at<int>(index_matrix(row, col)) = 0;
        }
    }
    mexPrintf("can you see this?\n");
    mexPrintf("[%d][%d] : %u\n", bounding_box.x, bounding_box.y, label.at<int>(index_matrix(bounding_box.x, bounding_box.y)));
}
    
// Gateway function
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    // Verify input/output arguments
    if(nrhs != 2 || nlhs != 1)
        mexErrMsgIdAndTxt("kmeansMex:mexFunction", USAGE);
    // Verify first input is a character string
    if(!mxIsChar(prhs[0]))
        mexErrMsgIdAndTxt("kmeansMex:mexFunction", "first argument must be a string.");
    // Verify second input is an array[1][4]
    if(mxGetM(prhs[1]) != 1 || mxGetN(prhs[1]) != 4 || !mxIsDouble(prhs[1]))
        mexErrMsgIdAndTxt("kmeansMex:mexFunction", "second argument must be a matrix of shape [1][4].");

    char *image_filepath = mxArrayToString(prhs[0]);
    double *rect = mxGetPr(prhs[1]);
    
    mexPrintf("filepath: %s\n", image_filepath);
    kmeansMex(image_filepath, rect);

    //plhs[0] = stringToMxArray("hello world");
}

int index_matrix(int row, int col){
    int index = col*image_width + row;
    //mexPrintf("Accessing: %d\n", index);
    return col*image_width + row;
}

