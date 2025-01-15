#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    // 2. Calculate the image dimensions.
    // 3. Determine the total bits required based on message length.
    // 4. Ensure the image has enough pixels; if not, throw an error.
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    // 6. Extract LSBs from the image pixels and return the result.

    secret_image.set_upper_tri_arr_size();
    secret_image.set_lower_tri_arr_size();

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage yeni = secret_image.reconstruct();

    // 2. Calculate the image dimensions.
    yeni.set_pixel_amount();
    // 3. Determine the total bits required based on message length.
    int total_bits_required = message_length * 7;

    // 4. Ensure the image has enough pixels; if not, throw an error.
    try {
        if (yeni.get_pixel_amount() < total_bits_required){
            throw std::exception();
        }
    } catch (const std::exception& e) {
        // std::cerr << "ERROR: Total bits required are greater than pixel amount of the image." << std::endl;
    }

    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    int starting_pixel = yeni.get_pixel_amount() - total_bits_required;
    int start_i = starting_pixel / secret_image.get_width();
    int start_j = starting_pixel % secret_image.get_width();

    // 6. Extract LSBs from the image pixels and return the result.
    for (int j = start_j; j < secret_image.get_width(); ++j) {
        std::bitset<8> current_bitset(yeni.get_pixel(start_i, j));
        LSB_array.push_back(current_bitset[0]);
    }
    for (int i = (start_i + 1); i < secret_image.get_height(); ++i) {
        for (int j = 0; j < secret_image.get_width(); ++j) {
            std::bitset<8> current_bitset((yeni.get_data()[start_i + 1][j]));
            LSB_array.push_back(current_bitset[0]);
        }
    }
    return LSB_array;
}

// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.
    try {
        // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
        if ((LSB_array.size() % 7) != 0){
            throw std::exception();
        }
        char *char_array = new char[(LSB_array.size()/7)];
        std::string the_character_in_binary;

        // 2. Convert each group of 7 bits into an ASCII character.
        for (int i = 0; i < (LSB_array.size() / 7); ++i) {
            the_character_in_binary = "";
            for (int j = 0; j < 7; ++j) {
                the_character_in_binary += std::to_string(LSB_array[(i*7 + j)]);
            }

            std::bitset<7> bitset(the_character_in_binary);
            char_array[i] = static_cast<char> (bitset.to_ulong());
        }

        for (int i = 0; i < (LSB_array.size()/7); ++i) {
            message += char_array[i];
        }

    } catch (const std::exception& e) {
        // std::cerr << "ERROR: LSB_array length is not valid for a message." << std::endl;
    }
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.
    for (int i = 0; i < message.size(); ++i) {
        std::bitset<7> current_bitset((static_cast<char>(message[i])));
        for (int j = 6; j >= 0; --j) {
            LSB_array.push_back(static_cast<int>(current_bitset[j]));
        }
    }

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // SecretImage secret_image(nullptr);

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.
    image.set_pixel_amount();
    try {
        if (image.get_pixel_amount() < LSB_array.size()){
            throw std::exception();
        }
        int starting_pixel = image.get_pixel_amount() - static_cast<int>(LSB_array.size());
        int start_i = starting_pixel / image.get_width();
        int start_j = starting_pixel % image.get_width();
        int lsb_index = 0;

        for (int i = start_i; i < image.get_height(); ++i) {
            if (i == start_i){
                for (int j = start_j; j < image.get_width(); ++j) {
                    std::bitset<8> pixel_bitset(image.get_pixel(i, j));
                    pixel_bitset[0] = LSB_array[lsb_index];
                    lsb_index += 1;
                    if (static_cast<int>(pixel_bitset.to_ulong()) > 255){
                        image.set_pixel(i, j, 255);
                    } else if (static_cast<int>(pixel_bitset.to_ulong()) < 0){
                        image.set_pixel(i, j, 0);
                    } else {
                        image.set_pixel(i, j, (static_cast<int>(pixel_bitset.to_ulong())));
                    }
                }
            } else {
                for (int j = 0; j < image.get_width(); ++j) {
                    std::bitset<8> pixel_bitset(image.get_pixel(i, j));
                    pixel_bitset[0] = LSB_array[lsb_index];
                    lsb_index += 1;
                    if ((static_cast<int>(pixel_bitset.to_ulong())) > 255){
                        image.set_pixel(i, j, 255);
                    } else if ((static_cast<int>(pixel_bitset.to_ulong())) < 0){
                        image.set_pixel(i, j, 0);
                    } else {
                        image.set_pixel(i, j, (static_cast<int>(pixel_bitset.to_ulong())));
                    }
                }
            }
        }

    } catch (const std::exception& e){
        // std::cerr << "ERROR: LSB_array size greater than pixel amount of the image." << std::endl;
    }

    SecretImage secret_image(image);

    // I don't know if I need these two but let's go
    secret_image.set_upper_tri_arr_size();
    secret_image.set_lower_tri_arr_size();

    return secret_image;
}