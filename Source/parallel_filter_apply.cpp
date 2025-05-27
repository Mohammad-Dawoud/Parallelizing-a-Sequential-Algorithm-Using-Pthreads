//Mohammad Saied Dawood
//ID: 202010256
//Date: 28/5/2025
//---------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <chrono>

using namespace std;
using namespace cv;
using namespace std::chrono;

// Global variables for thread communication
Mat g_src;
Mat g_dst;
double g_kernel[3][3];
int g_threadNum;

struct ThreadData {
    int start_row;
    int end_row;
};

void* applyMaskParallel(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int start_row = data->start_row;
    int end_row = data->end_row;
    
    // Process assigned rows
    for(int i = start_row; i < end_row; i++) {
        for(int j = 0; j < g_src.cols; j++) {
            if (i == 0 || i == g_src.rows - 1 || j == 0 || j == g_src.cols - 1) {
                // Handle border pixels
                g_dst.at<uchar>(i, j) = g_src.at<uchar>(i, j);
                continue;
            }

            double sum = 0.0;
            for(int x = -1; x <= 1; x++) {
                for(int y = -1; y <= 1; y++) {
                    sum += g_src.at<uchar>(i + x, j + y) * g_kernel[x + 1][y + 1];
                }
            }
            // Clamp values to 0-255
            g_dst.at<uchar>(i, j) = max(0.0, min(255.0, sum));
        }
    }
    return nullptr;
}

int main(int argc, char *argv[]) {
    if(argc < 5) {
        cout << "Usage: " << argv[0] << " <input_image> <mask_file> <output_image> <num_threads> [scale_factor]" << endl;
        return 1;
    }

    string inputImage = argv[1];
    string maskFile = argv[2];
    string outputImage = argv[3];
    int numThreads = stoi(argv[4]);
    double scale = 1.0;
    
    if (argc > 5) {
        scale = stod(argv[5]);
    }

    // Read input image
    g_src = imread(inputImage, IMREAD_GRAYSCALE);
    if(g_src.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    // Resize if scale is not 1.0
    if (scale != 1.0) {
        resize(g_src, g_src, Size(), scale, scale, INTER_LINEAR);
    }

    // Initialize output matrix
    g_dst = Mat::zeros(g_src.size(), g_src.type());

    // Read kernel from file
    ifstream maskInput(maskFile);
    if(!maskInput.is_open()) {
        cout << "Error opening mask file" << endl;
        return 1;
    }

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            maskInput >> g_kernel[i][j];
        }
    }
    maskInput.close();

    // Create thread data
    pthread_t* threads = new pthread_t[numThreads];
    ThreadData* threadData = new ThreadData[numThreads];
    int rowsPerThread = g_src.rows / numThreads;

    auto start = high_resolution_clock::now();

    // Create threads
    for(int i = 0; i < numThreads; i++) {
        threadData[i].start_row = i * rowsPerThread;
        threadData[i].end_row = (i == numThreads - 1) ? g_src.rows : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], nullptr, applyMaskParallel, &threadData[i]);
    }

    // Wait for all threads to complete
    for(int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], nullptr);
    }

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    cout << "---------->Parallel code with " << numThreads << " threads" 
         << " elapsed time = " << elapsed.count() << " seconds" << endl;

    // Save the result
    imwrite(outputImage, g_dst);

    // Clean up
    delete[] threads;
    delete[] threadData;

    return 0;
}