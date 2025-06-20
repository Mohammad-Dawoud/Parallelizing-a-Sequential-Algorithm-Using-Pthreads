//Mohammad Saied Dawood
//ID: 202010256
//Date: 20/6/2025
//---------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace cv;
using namespace std::chrono;

int main(int argc, char *argv[]) {
    if (argc < 5) {
        cout << "Usage: " << argv[0]
             << " <input_image> <mask_file> <output_image> <num_threads> [scale_factor]"
             << endl;
        return 1;
    }

    // Parse arguments
    string inputImage  = argv[1];
    string maskFile    = argv[2];
    string outputImage = argv[3];
    int    numThreads  = stoi(argv[4]);
    double scale       = (argc > 5) ? stod(argv[5]) : 1.0;

    // Disable OpenCV internal threading for clean timing
    cv::setNumThreads(1);
    cv::setUseOptimized(false);

    // Load and optionally resize image
    Mat src = imread(inputImage, IMREAD_GRAYSCALE);
    if (src.empty()) {
        cerr << "Could not open or find the image" << endl;
        return -1;
    }
    if (scale != 1.0) {
        resize(src, src, Size(), scale, scale, INTER_LINEAR);
    }

    // Prepare destination image
    Mat dst = Mat::zeros(src.size(), src.type());

    // Read 3×3 kernel from file
    double kernel[3][3];
    ifstream maskInput(maskFile);
    if (!maskInput.is_open()) {
        cerr << "Error opening mask file" << endl;
        return 1;
    }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            maskInput >> kernel[i][j];
    maskInput.close();

    // Set OpenMP thread count
    omp_set_num_threads(numThreads);

    // Start chrono timer
    auto t0 = high_resolution_clock::now();

    // Parallel convolution: one loop over rows, thread-local kernel copy
    #pragma omp parallel
    {
        // Each thread gets its own copy of the 3×3 mask
        double k[3][3];
        memcpy(k, kernel, sizeof k);

        #pragma omp for schedule(static)
        for (int i = 1; i < src.rows - 1; ++i) {
            const uchar* prev = src.ptr<uchar>(i - 1);
            const uchar* curr = src.ptr<uchar>(i);
            const uchar* next = src.ptr<uchar>(i + 1);
            uchar*       out  = dst.ptr<uchar>(i);

            for (int j = 1; j < src.cols - 1; ++j) {
                // Unrolled 3×3 convolution
                double sum =
                    prev[j - 1]*k[0][0] + prev[j]*k[0][1] + prev[j + 1]*k[0][2] +
                    curr[j - 1]*k[1][0] + curr[j]*k[1][1] + curr[j + 1]*k[1][2] +
                    next[j - 1]*k[2][0] + next[j]*k[2][1] + next[j + 1]*k[2][2];

                // Clamp and store
                out[j] = static_cast<uchar>(sum < 0 ? 0 : (sum > 255 ? 255 : sum));
            }
        }
    }

    // Stop chrono timer
    auto t1 = high_resolution_clock::now();
    duration<double> diff = t1 - t0;
    cout << "----------------------------------------------------------" << endl;
    cout << "----------> OpenMP code with " << numThreads << " threads" 
         << " elapsed time = " << diff.count() << " seconds" << endl;
    cout << "----------------------------------------------------------" << endl;

    // Write result
    if (!imwrite(outputImage, dst)) {
        cerr << "Failed to write output image" << endl;
        return -1;
    }

    return 0;
}
