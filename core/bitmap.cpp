#include "bitmap.h"

#include <fstream>
#include <cmath>

#include "app_error.h"

void Bitmap::Load(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&bmp_header_), sizeof(bmp_header_));

    if (bmp_header_.signature != *reinterpret_cast<const int16_t*>("BM")) {
        throw AppError(AppError::FileSignatureError);
    }

    stream.read(reinterpret_cast<char*>(&dib_header_), sizeof(dib_header_));

    base_width_ = dib_header_.image_width;
    base_height_ = dib_header_.image_height;
    width_ = base_width_;
    height_ = base_height_;

    uint8_t row_padding = base_width_ * 3 % 4;
    if (row_padding != 0) {
        row_padding = 4 - row_padding;
    }

    if (data_ != nullptr) {
        delete[] data_;
    }
    data_ = new Color[base_width_ * base_height_];

    for (uint32_t y = 0; y < base_height_; ++y) {
        for (uint32_t x = 0; x < base_width_; ++x) {
            uint8_t blue = 0;
            uint8_t green = 0;
            uint8_t red = 0;

            stream.read(reinterpret_cast<char*>(&blue), 1);
            stream.read(reinterpret_cast<char*>(&green), 1);
            stream.read(reinterpret_cast<char*>(&red), 1);

            Color pixel;
            pixel.Set(red / 255.0, green / 255.0, blue / 255.0);

            data_[y * base_width_ + x] = pixel;
        }
        stream.ignore(row_padding);
    }
}

void Bitmap::LoadFromBMP(std::string_view file_name) {
    std::ifstream file(file_name.data(), std::ios_base::in | std::ios_base::binary);

    if (!file.is_open()) {
        throw AppError(AppError::InputFileIsNotOpen);
    }

    Load(file);
}

void Bitmap::Export(std::ostream& stream) const {
    stream.write(reinterpret_cast<const char*>(&bmp_header_), sizeof(bmp_header_));
    stream.write(reinterpret_cast<const char*>(&dib_header_), sizeof(dib_header_));

    uint8_t row_padding = width_ * 3 % 4;
    if (row_padding != 0) {
        row_padding = 4 - row_padding;
    }

    for (uint32_t y = 0; y < height_; ++y) {
        for (uint32_t x = 0; x < width_; ++x) {
            const Color& pixel = data_[y * base_width_ + x];
            uint8_t blue = std::round(pixel.B * 255);
            uint8_t green = std::round(pixel.G * 255);
            uint8_t red = std::round(pixel.R * 255);

            stream.write(reinterpret_cast<const char*>(&blue), 1);
            stream.write(reinterpret_cast<const char*>(&green), 1);
            stream.write(reinterpret_cast<const char*>(&red), 1);
        }

        for (uint8_t p = 0; p < row_padding; ++p) {
            stream.write(reinterpret_cast<const char*>(" "), 1);
        }
    }
}

void Bitmap::ExportAsBMP(std::string_view file_path) const {
    std::ofstream file(file_path.data(), std::ios_base::out | std::ios_base::binary);

    if (!file.is_open()) {
        throw AppError(AppError::OutputFileIsNotOpen);
    }

    Export(file);
}

bool Bitmap::operator==(const Bitmap& other) const {
    if (data_ == nullptr || other.data_ == nullptr) {
        return data_ == other.data_;
    }

    if (base_width_ != other.base_width_ || base_height_ != other.base_height_) {
        return false;
    }

    for (uint32_t i = 0; i < base_height_; ++i) {
        for (uint32_t j = 0; j < base_width_; ++j) {
            if (data_[base_width_ * i + j] != other.data_[base_width_ * i + j]) {
                return false;
            }
        }
    }

    return true;
}

void Bitmap::Crop(uint32_t new_width, uint32_t new_height) {
    if (width_ > new_width) {
        width_ = new_width;
        dib_header_.image_width = new_width;
    }
    if (height_ > new_height) {
        height_ = new_height;
        dib_header_.image_height = new_height;
    }
    dib_header_.image_size = (width_ * 3 + (4 - (width_ * 3) % 4) % 4) * height_;
    bmp_header_.file_size = 54 + dib_header_.image_size;
}

Color& Bitmap::GetClosestPixel(int64_t x, int64_t y) {
    return GetPixel(std::max<int64_t>(0, std::min<int64_t>(width_ - 1, x)),
                    std::max<int64_t>(0, std::min<int64_t>(height_ - 1, y)));
}
