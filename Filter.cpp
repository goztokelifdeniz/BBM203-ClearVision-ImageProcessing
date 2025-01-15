#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.

    GrayscaleImage reference(image);

    int current_pixel_value;
    int kernel_matrix_sum;
    int kernel_matrix_size = kernelSize * kernelSize;

    for (int row_index = 0; row_index < reference.get_height(); ++row_index) {    // THESE TWO FOR LOOPS ARE FOR DOING THE EFFECT
        for (int col_index = 0; col_index < reference.get_width(); ++col_index) { // TO EVERY SINGLE PIXEL OF THE IMAGE

            kernel_matrix_sum = 0;

            // KERNEL MATRIX SUM
            for (int i = -((kernelSize - 1) / 2); i <= ((kernelSize - 1) / 2); ++i) {      // THESE TWO FOR LOOPS ARE FOR REACHING
                for (int j = -((kernelSize - 1) / 2); j <= ((kernelSize - 1) / 2); ++j) {  // EVERY PIXEL OF THE KERNEL MATRIX

                    if ((row_index + i) < 0
                        || (row_index + i) >= reference.get_height()
                        || (col_index + j) < 0
                        || (col_index + j) >= reference.get_width()){

                        // pixel is out of the image bounds - black - 0
                        current_pixel_value = 0;
                        kernel_matrix_sum += current_pixel_value;
                    } else {
                        current_pixel_value = reference.get_data()[(row_index + i)][(col_index + j)];
                        kernel_matrix_sum += current_pixel_value;
                    }
                }
            }
            // KERNEL MATRIX MEAN VALUE
            image.get_data()[row_index][col_index] = (kernel_matrix_sum / kernel_matrix_size);
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    GrayscaleImage reference(image);

    double current_pixel_value;
    double gaussian_weighted_matrix_sum;
    double gaussian_weight_sum;
    double pixel_weight;

    for (int row_index = 0; row_index < reference.get_height(); ++row_index) {    // THESE TWO FOR LOOPS ARE FOR DOING THE EFFECT
        for (int col_index = 0; col_index < reference.get_width(); ++col_index) { // TO EVERY SINGLE PIXEL OF THE IMAGE

            gaussian_weight_sum = 0;
            gaussian_weighted_matrix_sum = 0;

            // GAUSSIAN MATRIX SUM
            for (int i = -((kernelSize - 1) / 2); i <= ((kernelSize - 1) / 2); ++i) {      // THESE TWO FOR LOOPS ARE FOR REACHING
                for (int j = -((kernelSize - 1) / 2); j <= ((kernelSize - 1) / 2); ++j) {  // EVERY PIXEL OF THE KERNEL MATRIX

                    pixel_weight = (exp(-((i*i + j*j)/(2*sigma*sigma)))) / (2*M_PI*sigma*sigma);
                    gaussian_weight_sum += pixel_weight;

                    if ((row_index + i) < 0
                        || (row_index + i) >= reference.get_height()
                        || (col_index + j) < 0
                        || (col_index + j) >= reference.get_width()){

                        // pixel is out of the image bounds - black - 0
                        current_pixel_value = 0;
                    } else {
                        current_pixel_value = reference.get_data()[(row_index + i)][(col_index + j)];
                    }
                    gaussian_weighted_matrix_sum += pixel_weight * current_pixel_value;
                }
            }
            // GAUSSIAN MATRIX MEAN VALUE
            image.get_data()[row_index][col_index] =
                    static_cast<int>(std::floor((gaussian_weighted_matrix_sum / gaussian_weight_sum)));
        }
    }
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].

    GrayscaleImage reference(image);
    apply_gaussian_smoothing(image, kernelSize, 1.0);
    double unsharp_mask_output;
    for (int i = 0; i < image.get_height(); ++i) {
        for (int j = 0; j < image.get_width(); ++j) {
            unsharp_mask_output = reference.get_pixel(i,j) + amount
                    * (reference.get_pixel(i,j) - image.get_pixel(i,j));
            if (unsharp_mask_output < 0){
                image.set_pixel(i, j, 0);
            } else if (unsharp_mask_output > 255){
                image.set_pixel(i, j, 255);
            } else {
                image.set_pixel(i, j, unsharp_mask_output);
            }
        }
    }
}
