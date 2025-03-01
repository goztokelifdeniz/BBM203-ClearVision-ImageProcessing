# ClearVision - Image Processing Project

## Overview
ClearVision is a personal project focused on image processing techniques. The goal is to develop an application capable of performing advanced image filtering, arithmetic operations, and secret message embedding/extraction in grayscale images.

## Features
This project is implemented using **C++11** and includes the following functionalities:

### Image Operations
- **Addition (`+`)**: Combines two grayscale images.
- **Subtraction (`-`)**: Computes the difference between two grayscale images.
- **Equality Check (`==`)**: Compares two images pixel by pixel.

### Image Filters
- **Mean Filter**: Reduces noise by averaging surrounding pixel values.
- **Gaussian Filter**: Applies Gaussian smoothing to preserve edges while reducing noise.
- **Unsharp Masking**: Enhances image sharpness by emphasizing edges.

### Secret Image Handling
- **Triangular Matrix Storage**: Stores images in upper and lower triangular matrices.
- **Reconstruction**: Rebuilds an image from its stored components.
- **Save and Load**: Reads and writes secret images to files.

### Steganography (Secret Message Embedding & Extraction)
- **Embed Message**: Hides a text message in the least significant bits (LSBs) of image pixels.
- **Extract Message**: Retrieves the hidden message from an image.

## Dependencies
- **stb_image.h**: Used for loading and saving grayscale images.
- **C++11 Standard**: The project must be compiled using `-std=c++11`.

## Compilation & Execution
### Compilation
Compile using `g++`:
```bash
$ g++ -g -std=c++11 -o clearvision main.cpp SecretImage.cpp GrayscaleImage.cpp Filter.cpp Crypto.cpp 
```

## File Structure
```bash
project_folder/
│── Crypto.cpp
│── Crypto.h
│── Filter.cpp
│── Filter.h
│── GrayscaleImage.cpp
│── GrayscaleImage.h
│── SecretImage.cpp
│── SecretImage.h
│── main.cpp
│── README.md
│── Makefile / CMakeLists.txt
```
