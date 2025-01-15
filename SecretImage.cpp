#include "SecretImage.h"
#include <vector>

// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.

    this->height = image.get_height();
    this->width = image.get_width();

    set_upper_tri_arr_size();
    set_lower_tri_arr_size();

    this->upper_triangular = new int[((image.get_height() * (image.get_height() + 1)) / 2)];
    this->lower_triangular = new int[((image.get_height() * (image.get_height() - 1)) / 2)];

    for (int i = 0; i < image.get_height(); ++i) {
        for (int j = 0; j < image.get_width(); ++j) {
            if (j >= i){
                upper_triangular[(((image.get_height() * (image.get_height() + 1)) / 2)
                - (((image.get_height() - i) * (image.get_height() - i - 1)) / 2)
                - (image.get_height() - j - 1) - 1)
                ] = image.get_pixel(i, j);
            } else {
                lower_triangular[(((i*(i-1))/2) + j)] = image.get_pixel(i, j);
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    this->width = w;
    this->height = h;

    set_upper_tri_arr_size();
    set_lower_tri_arr_size();

    // I'm not sure if I need to create these but let's go I guess
    this->upper_triangular = new int[upper_tri_arr_size];
    this->lower_triangular = new int[lower_tri_arr_size];

    // WARNING: NOT KNOWING MIGHT CAUSE PROBLEMS LATER ON

    // it's either this way
    /*
    this->upper_triangular = upper;
    this->lower_triangular = lower;
    */

    // or this way
    for (int i = 0; i < upper_tri_arr_size; ++i) {
        this->upper_triangular[i] = upper[i];
    }
    for (int i = 0; i < lower_tri_arr_size; ++i) {
        this->lower_triangular[i] = lower[i];
    }

}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.

    // IDE said if-clause is unnecessary.
    // if (this->upper_triangular != nullptr) delete[] this->upper_triangular;

    delete[] upper_triangular;
    delete[] lower_triangular;

}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);


    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (j >= i){ // upper tri matrix
                image.set_pixel(i, j,
                                upper_triangular[(
                                        ((height * (height + 1)) / 2)
                                        - (((height - i) * (height - i - 1)) / 2)
                                        - (height - j - 1)
                                        - 1
                                )]);
            } else {
                image.set_pixel(i, j,
                                lower_triangular[(((i*(i-1))/2) + j)]);
            }
        }
    }

    // image.set_data() olmasını da beklerdim ama sanırım ona burada dokunmuyoruz

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // Update the lower and upper triangular matrices
    // based on the GrayscaleImage given as the parameter.

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (j >= i){ // upper tri matrix
                upper_triangular[(upper_tri_arr_size
                - (((height - i) * (height - i - 1)) / 2)
                - (height - j - 1)- 1)
                ] = image.get_data()[i][j];
            } else {
                lower_triangular[(((i*(i-1))/2) + j)] = image.get_data()[i][j];
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // 1. Write width and height on the first line, separated by a single space.
    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated. 
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.

    std::ofstream file(filename);

/*    if (!file.is_open()) {
        std::cerr << "Couldn't open file: " << filename << std::endl;
        return;
    }*/

    file << this->width << " " << this->height << "\n";

    for (int i = 0; i < upper_tri_arr_size; ++i) {
        if (i == (upper_tri_arr_size - 1)){
            file << upper_triangular[i] << "\n";
        } else {
            file << upper_triangular[i] << " ";
        }
    }

    for (int i = 0; i < lower_tri_arr_size; ++i) {
        if (i == (lower_tri_arr_size - 1)){
            file << lower_triangular[i];
        } else {
            file << lower_triangular[i] << " ";
        }
    }

    file.close();

}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // 1. Open the file and read width and height from the first line, separated by a space.
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    // 3. Allocate memory for both arrays.
    // 4. Read the upper_triangular array from the second line, space-separated.
    // 5. Read the lower_triangular array from the third line, space-separated.
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    SecretImage secret_image(0, 0, nullptr, nullptr);
    std::ifstream file(filename);
    std::string line;

    for (int i = 0; i < 3; ++i) {
        std::getline(file, line);

        std::stringstream ss(line);

        // Variable to hold each word
        std::string word;

        // Vector to store the words
        std::vector<std::string> words;

        // Extract words from the sentence
        while (ss >> word) {
            // Add the word to the vector
            words.push_back(word);
        }

        switch (i) {
            case 0: {
                secret_image.set_height(std::stoi(words[0]));
                secret_image.set_width(std::stoi(words[1]));

                secret_image.set_upper_tri_arr_size();
                secret_image.set_lower_tri_arr_size();

                secret_image.set_upper_triangular(new int[secret_image.get_upper_tri_arr_size()]);
                secret_image.set_lower_triangular(new int[secret_image.get_lower_tri_arr_size()]);
                break;
            }
            case 1:
                for (int j = 0; j < words.size(); ++j) {
                    secret_image.upper_triangular[j] = std::stoi(words[j]);
                }
                break;
            case 2:
                for (int j = 0; j < words.size(); ++j) {
                    secret_image.lower_triangular[j] = std::stoi(words[j]);
                }
                break;
/*            default:
                std::cerr << "Error: Unexpected value of i: " << i << std::endl;
                break;*/
        }
    }
    file.close();
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}

void SecretImage::set_upper_tri_arr_size() {
    this->upper_tri_arr_size = ((this->height * (this->height + 1)) / 2);
}

void SecretImage::set_lower_tri_arr_size() {
    this->lower_tri_arr_size = ((this->height * (this->height - 1)) / 2);
}

int SecretImage::get_upper_tri_arr_size() const {
    return this->upper_tri_arr_size;
}

int SecretImage::get_lower_tri_arr_size() const {
    return this->lower_tri_arr_size;
}

void SecretImage::set_upper_triangular(int *upper) {
    this->upper_triangular = upper;
}

void SecretImage::set_lower_triangular(int *lower) {
    this->lower_triangular = lower;
}

void SecretImage::set_height(int value) {
    this->height = value;
}

void SecretImage::set_width(int value) {
    this->width = value;
}

