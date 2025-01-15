#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }


    // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    // Fill the matrix with pixel values from the image
    this->data = new int*[height];
    for(int i=0;i<height;i++)
        data[i] = new int[width];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            this->data[i][j] = image[i*width + j];
        }
    }

    this->set_pixel_amount();

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    // Initialize the image with a pre-existing data matrix by copying the values.
    // Don't forget to dynamically allocate memory for the matrix.
    this->width = w;
    this->height = h;
    this->set_pixel_amount();

    this->data = new int*[height];
    for(int i=0;i<height;i++)
        data[i] = new int[width];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            this->data[i][j] = inputData[i][j];
        }
    }
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // Just dynamically allocate the memory for the new matrix.
    this->data = new int*[height];
    for(int i=0;i<height;i++)
        data[i] = new int[width];

    this->set_pixel_amount();
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    // Copy constructor: dynamically allocate memory and
    // copy pixel values from another image.
    this->width = other.width;
    this->height = other.height;
    this->set_pixel_amount();

    this->data = new int*[height];
    for(int i=0;i<height;i++)
        data[i] = new int[width];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            this->data[i][j] = other.data[i][j];
        }
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // Destructor: deallocate memory for the matrix.
    if (this->data != nullptr){
        for (int i = 0; i < this->height; ++i) {
            if (this->data[i] != nullptr){
                delete[] data[i];
            }
        }
        delete[] data;
    }
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // Check if two images have the same dimensions and pixel values.
    // If they do, return true.

    if (this->width == other.width && this->height == other.height){
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                if (this->data[i][j] != other.data[i][j]){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // Add two images' pixel values and return a new image, clamping the results.
    int pixel_value;
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            pixel_value = (this->data[i][j] + other.data[i][j]);
            if (pixel_value < 0){
                result.data[i][j] = 0;
            } else if (pixel_value > 255){
                result.data[i][j] = 255;
            } else {
                result.data[i][j] = pixel_value;
            }
        }
    }
    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // Subtract pixel values of two images and return a new image, clamping the results.
    int pixel_value;
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            pixel_value = (this->data[i][j] - other.data[i][j]);
            if (pixel_value < 0){
                result.data[i][j] = 0;
            } else if (pixel_value > 255){
                result.data[i][j] = 255;
            } else {
                result.data[i][j] = pixel_value;
            }
        }
    }
    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
