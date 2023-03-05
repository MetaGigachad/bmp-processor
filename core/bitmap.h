#pragma once

#include <string_view>
#include <tuple>
#include <istream>
#include <ostream>
#include <vector>

class Color {
public:
    double R;
    double G;
    double B;

    Color() {}
    Color(double red, double green, double blue) : R(red), G(green), B(blue) {}

    Color& Set(double red, double green, double blue) {
        R = red;
        G = green;
        B = blue;
        return *this;
    }

    bool operator==(const Color& other) const {
        return std::tie(R, G, B) == std::tie(other.R, other.G, other.B);
    }

    bool operator!=(const Color& other) const {
        return !operator==(other);
    }
};

class Bitmap {
public:
    struct BMPHeader {
        uint16_t signature;
        uint32_t file_size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t file_offset_to_pixel_array;
    } __attribute__((packed));

    struct DIBHeader {
        uint32_t dib_header_size;
        uint32_t image_width;
        uint32_t image_height;
        uint16_t planes;
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t image_size;
        uint32_t x_pixels_per_meter;
        uint32_t y_pixels_per_meter;
        uint32_t colors_in_color_table;
        uint32_t important_color_count;
    } __attribute__((packed));

    void Load(std::istream& stream);
    void LoadFromBMP(std::string_view file_name);

    void Export(std::ostream& stream) const;
    void ExportAsBMP(std::string_view file_path) const;

    uint32_t GetWidth() {
        return width_;
    }
    uint32_t GetHeight() {
        return height_;
    }

    Color& GetPixel(uint32_t x, uint32_t y) {
        return data_[base_width_ * y + x];
    }

    Color& GetClosestPixel(int64_t x, int64_t y);

    void Crop(uint32_t new_width, uint32_t new_height);

    bool operator==(const Bitmap& other) const;

    ~Bitmap() {
        delete[] data_;
    }

protected:
    BMPHeader bmp_header_;
    DIBHeader dib_header_;

    uint32_t base_width_;
    uint32_t base_height_;
    Color* data_ = nullptr;

    uint32_t width_;
    uint32_t height_;
};
