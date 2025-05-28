//Mohammad Saied Dawood
//ID: 202010256
//Date: 28/5/2025
//---------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace cv;
using namespace std::chrono;

void applyMask(Mat& src, Mat& dst, double kernel[][3]) {
    int rows = src.rows;
    int cols = src.cols;
    
    // Ensure the output image is allocated
    if (src.channels() == 1) {
        dst = Mat::zeros(rows, cols, CV_64F);
    } else {
        dst = Mat::zeros(rows, cols, CV_8UC3);
    }

    // Handle grayscale and color images
    if (src.channels() == 1) {
        // Grayscale image processing
        for(int i = 1; i < rows - 1; i++) {
            for(int j = 1; j < cols - 1; j++) {
                double sum = 0.0;
                for(int x = -1; x <= 1; x++) {
                    for(int y = -1; y <= 1; y++) {
                        sum += src.at<uchar>(i + x, j + y) * kernel[x + 1][y + 1];
                    }
                }
                // Clamp values to 0-255
                dst.at<double>(i, j) = max(0.0, min(255.0, sum));
            }
        }
    } else {
        // Color image processing (process each channel separately)
        vector<Mat> channels;
        split(src, channels);
        
        for(int c = 0; c < 3; c++) {
            for(int i = 1; i < rows - 1; i++) {
                for(int j = 1; j < cols - 1; j++) {
                    double sum = 0.0;
                    for(int x = -1; x <= 1; x++) {
                        for(int y = -1; y <= 1; y++) {
                            sum += channels[c].at<uchar>(i + x, j + y) * kernel[x + 1][y + 1];
                        }
                    }
                    // Clamp values to 0-255 and convert back to uchar
                    channels[c].at<uchar>(i, j) = max(0.0, min(255.0, sum));
                }
            }
        }
        merge(channels, dst);
    }
}

int main(int argc, char *argv[]) {
    if(argc < 4) {
        cout << "Usage: " << argv[0] << " <input_image> <mask_file> <output_image> [scale_factor]" << endl;
        return 1;
    }

    string inputImage = argv[1];
    string maskFile = argv[2];
    string outputImage = argv[3];
    double scale = 1.0;
    
    if (argc > 4) {
        scale = stod(argv[4]);
    }

    // Read input image
    Mat img = imread(inputImage, IMREAD_COLOR);
    if(img.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    // Resize if scale is not 1.0
    if (scale != 1.0) {
        resize(img, img, Size(), scale, scale, INTER_LINEAR);
    }

    // Convert to grayscale if needed
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Read kernel from file
    ifstream maskInput(maskFile);
    if(!maskInput.is_open()) {
        cout << "Error opening mask file" << endl;
        return 1;
    }

    double kernel[3][3];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            maskInput >> kernel[i][j];
        }
    }
    maskInput.close();

    // Apply filter
    Mat result;
    auto start = high_resolution_clock::now();
    
    applyMask(gray, result, kernel);
    
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    cout << "---------->Serial code elapsed time = " << elapsed.count() << " seconds" << endl;

    // Convert back to 8-bit for saving
    Mat output;
    result.convertTo(output, CV_8U);
    
    // Save the result
    imwrite(outputImage, output);
    
    return 0;
}