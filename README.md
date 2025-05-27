# Parallelizing a Sequential Algorithm Using Pthreads

A C++ implementation of image filtering with both serial and parallel (pthreads) versions, designed to compare their performance on different hardware configurations.

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Filters](#filters)
- [Performance](#performance)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## Project Overview

This project implements image filtering algorithms in both serial and parallel fashions. It serves as a practical example of how multi-threading can be used to speed up image processing tasks. The implementation includes:

- A serial version for baseline performance
- A parallel version using POSIX threads (pthreads)
- Multiple filter kernels for different image processing effects
- Performance measurement and comparison capabilities

## Features

- **Dual Implementations**: Both serial and parallel versions for performance comparison
- **Multiple Filter Types**: Includes blur and edge detection filters
- **Scalable**: Can process images of various sizes
- **Performance Metrics**: Measures and reports execution time
- **Flexible**: Supports both grayscale and color images
- **Adjustable**: Optional image scaling for performance testing

## Requirements

- C++11 or later
- OpenCV (tested with version 4.5.0+)
- POSIX Threads (pthreads) for the parallel version
- CMake (for building)
- Make

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/Mohammad-Dawoud/Parallelizing-a-Sequential-Algorithm-Using-Pthreads.git
   cd Parallelizing-a-Sequential-Algorithm-Using-Pthreads/Source
   ```

2. Build the project:
   ```bash
   make
   ```

   This will compile both the serial (`apply_filter`) and parallel (`papply_filter`) executables.

## Usage

### Serial Version
```bash
./apply_filter <input_image> <mask_file> <output_image> [scale_factor]
```

### Parallel Version
```bash
./papply_filter <input_image> <mask_file> <output_image> <num_threads> [scale_factor]
```

#### Parameters:
- `input_image`: Path to the input image file
- `mask_file`: Path to the filter mask file (see Filters section)
- `output_image`: Path where the processed image will be saved
- `num_threads`: Number of threads to use (parallel version only)
- `scale_factor`: (Optional) Scale factor for the input image (e.g., 0.5 for half size)

### Example
```bash
# Apply blur filter using 4 threads
./papply_filter input.jpg Filters/Blur_Mask_1.txt output_blur.jpg 4

# Apply edge detection with image scaling
./papply_filter input.jpg Filters/Edge_Mask.txt output_edge.jpg 4 0.75
```

## Filters

The project includes several predefined filters in the `Filters/` directory:

- **Blur Masks**:
  - `Blur_Mask_1.txt`: Simple 3x3 Gaussian blur
  - `Blur_Mask_Sig16.txt`: Larger Gaussian blur with sigma=16
  
- **Edge Detection**:
  - `Edge_Mask.txt`: Basic edge detection
  - `Edge_Mask_2.txt`: Alternative edge detection kernel

You can create custom filters by creating a text file with 3x3 matrix values.

## Performance

The parallel implementation uses a row-based partitioning strategy where each thread processes a subset of the image rows. The performance gain depends on:

- Number of available CPU cores
- Image size
- Filter complexity
- System load

To compare performance:

```bash
# Run serial version
time ./apply_filter input.jpg Filters/Blur_Mask_1.txt output_serial.jpg

# Run parallel version with 4 threads
time ./papply_filter input.jpg Filters/Blur_Mask_1.txt output_parallel.jpg 4
```

## Project Structure

```
Parallelizing-a-Sequential-Algorithm-Using-Pthreads/
├── Source/
│   ├── Filters/           # Filter mask files
│   ├── Images/            # Sample images 
│   ├── Output/            # Directory for processed images
│   ├── sequential_filter_apply.cpp   # Serial implementation
│   ├── parallel_filter_apply.cpp  # Parallel implementation
│   └── Makefile          # Build configuration
└── README.md             # This file
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.


---

Created by Mohammad Saied Dawood - 2025
