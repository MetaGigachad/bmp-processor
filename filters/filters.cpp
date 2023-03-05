#include "filters.h"

#include <cmath>

#include "utils.h"
#include "app_error.h"

BaseFilter* CropFilter::Create(const FilterInfo& info) {
    const auto& params = info.GetParams();

    if (params.size() != 2) {
        throw AppError(AppError::CropFilterParamsError);
    }

    uint32_t new_width = SVToType<uint32_t>(params[0]);
    uint32_t new_height = SVToType<uint32_t>(params[1]);

    return new CropFilter(new_width, new_height);
}

void CropFilter::Apply(Bitmap& image) const {
    image.Crop(new_width_, new_height_);
}

BaseFilter* GrayscaleFilter::Create(const FilterInfo& info) {
    const auto& params = info.GetParams();

    if (!params.empty()) {
        throw AppError(AppError::GrayscaleFilterParamsError);
    }

    return new GrayscaleFilter();
}

void GrayscaleFilter::Apply(Bitmap& image) const {
    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            Color& pixel = image.GetPixel(x, y);
            double new_val = 0.299 * pixel.R + 0.587 * pixel.G + 0.114 * pixel.B;
            pixel.Set(new_val, new_val, new_val);
        }
    }
}

BaseFilter* NegativeFilter::Create(const FilterInfo& info) {
    const auto& params = info.GetParams();

    if (!params.empty()) {
        throw AppError(AppError::NegativeFilterParamsError);
    }

    return new NegativeFilter();
}

void NegativeFilter::Apply(Bitmap& image) const {
    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            Color& pixel = image.GetPixel(x, y);
            pixel.Set(1 - pixel.R, 1 - pixel.G, 1 - pixel.B);
        }
    }
}

BaseFilter* SharpeningFilter::Create(const FilterInfo& info) {
    const auto& params = info.GetParams();

    if (!params.empty()) {
        throw AppError(AppError::SharpeningFilterParamsError);
    }

    return new SharpeningFilter();
}

void SharpeningFilter::Apply(Bitmap& image) const {
    std::vector<std::vector<Color>> new_colors(image.GetHeight(), std::vector<Color>(image.GetHeight()));

    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            Color& up = image.GetClosestPixel(x, y - 1);
            Color& down = image.GetClosestPixel(x, y + 1);
            Color& left = image.GetClosestPixel(x - 1, y);
            Color& right = image.GetClosestPixel(x + 1, y);
            Color& center = image.GetPixel(x, y);

            double red = up.R * -1 + left.R * -1 + center.R * 5 + right.R * -1 + down.R * -1;
            double green = up.G * -1 + left.G * -1 + center.G * 5 + right.G * -1 + down.G * -1;
            double blue = up.B * -1 + left.B * -1 + center.B * 5 + right.B * -1 + down.B * -1;

            red = std::max(0.0, std::min(1.0, red));
            green = std::max(0.0, std::min(1.0, green));
            blue = std::max(0.0, std::min(1.0, blue));

            Color new_color;
            new_color.Set(red, green, blue);

            new_colors[y][x] = new_color;
        }
    }

    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            image.GetPixel(x, y) = new_colors[y][x];
        }
    }
}

BaseFilter* EdgeDetectionFilter::Create(const FilterInfo& info) {
    const auto& params = info.GetParams();

    if (params.size() != 1) {
        throw AppError(AppError::EdgeDetectionFilterParamsError);
    }

    double threshold = SVToType<double>(params[0]);

    return new EdgeDetectionFilter(threshold);
}

void EdgeDetectionFilter::Apply(Bitmap& image) const {
    GrayscaleFilter gs_filter;
    gs_filter.Apply(image);

    std::vector<std::vector<bool>> new_colors(image.GetHeight(), std::vector<bool>(image.GetHeight()));

    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            Color& up = image.GetClosestPixel(x, y - 1);
            Color& down = image.GetClosestPixel(x, y + 1);
            Color& left = image.GetClosestPixel(x - 1, y);
            Color& right = image.GetClosestPixel(x + 1, y);
            Color& center = image.GetPixel(x, y);

            double val = up.R * -1 + left.R * -1 + center.R * 4 + right.R * -1 + down.R * -1;

            val = std::max(0.0, std::min(1.0, val));

            new_colors[y][x] = val > threshold_;
        }
    }

    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            image.GetPixel(x, y) = new_colors[y][x] ? Color(1, 1, 1) : Color(0, 0, 0);
        }
    }
}

GaussianBlurFilter::GaussianBlurFilter(double sigma) : sigma_(sigma), radius_(std::round(3 * sigma)) {}

BaseFilter* GaussianBlurFilter::Create(const FilterInfo& info) {
    const auto& params = info.GetParams();

    if (params.size() != 1) {
        throw AppError(AppError::GaussianBlurFilterParamsError);
    }

    double sigma = SVToType<double>(params[0]);

    return new GaussianBlurFilter(sigma);
}

double GaussianBlurFilter::GaussFunc(int32_t i) const {
    return 1 / std::sqrt(2 * std::numbers::pi) / sigma_ * std::exp(-i * i / (2 * sigma_ * sigma_));
}

Color GaussianBlurFilter::CalcNewPixelHorizontal(Bitmap& image, uint32_t x, uint32_t y) const {
    Color result(0, 0, 0);

    for (int32_t i = -radius_; i <= radius_; ++i) {
        Color& pixel = image.GetClosestPixel(x + i, y);
        double expr = GaussFunc(i);
        result.R += pixel.R * expr;
        result.G += pixel.G * expr;
        result.B += pixel.B * expr;
    }

    result.R = std::max(0.0, std::min(1.0, result.R));
    result.G = std::max(0.0, std::min(1.0, result.G));
    result.B = std::max(0.0, std::min(1.0, result.B));

    return result;
}

Color GaussianBlurFilter::CalcNewPixelVertical(Bitmap& image, uint32_t x, uint32_t y) const {
    Color result(0, 0, 0);

    for (int32_t i = -radius_; i <= radius_; ++i) {
        Color& pixel = image.GetClosestPixel(x, y + i);
        double expr = GaussFunc(i);
        result.R += pixel.R * expr;
        result.G += pixel.G * expr;
        result.B += pixel.B * expr;
    }

    result.R = std::max(0.0, std::min(1.0, result.R));
    result.G = std::max(0.0, std::min(1.0, result.G));
    result.B = std::max(0.0, std::min(1.0, result.B));

    return result;
}

void GaussianBlurFilter::Apply(Bitmap& image) const {
    std::vector<std::vector<Color>> new_colors(image.GetHeight(), std::vector<Color>(image.GetHeight()));

    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            new_colors[y][x] = CalcNewPixelHorizontal(image, x, y);
        }
    }

    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            image.GetPixel(x, y) = new_colors[y][x];
        }
    }

    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            new_colors[y][x] = CalcNewPixelVertical(image, x, y);
        }
    }

    for (uint32_t y = 0; y < image.GetHeight(); ++y) {
        for (uint32_t x = 0; x < image.GetWidth(); ++x) {
            image.GetPixel(x, y) = new_colors[y][x];
        }
    }
}

BaseFilter* PixelateFilter::Create(const FilterInfo& info) {
    const auto& params = info.GetParams();

    if (params.size() != 1) {
        throw AppError(AppError::PixelateFilterParamsError);
    }

    double res_multiplier = SVToType<double>(params[0]);

    if (res_multiplier > 1) {
        throw AppError(AppError::PixelateFilterMultiplierLimit);
    }

    return new PixelateFilter(res_multiplier);
}

void PixelateFilter::Apply(Bitmap& image) const {
    uint32_t new_width = std::round(image.GetWidth() * res_multiplier_);
    uint32_t new_height = std::round(image.GetHeight() * res_multiplier_);

    int32_t radius = std::round(1 / res_multiplier_);

    for (uint32_t new_y = 0; new_y < new_height; ++new_y) {
        for (uint32_t new_x = 0; new_x < new_width; ++new_x) {
            Color new_color(0, 0, 0);
            for (uint32_t y = new_y * radius; y < (new_y + 1) * radius; ++y) {
                for (uint32_t x = new_x * radius; x < (new_x + 1) * radius; ++x) {
                    Color& color = image.GetClosestPixel(x, y);
                    new_color.R += color.R;
                    new_color.G += color.G;
                    new_color.B += color.B;
                }
            }

            new_color.R /= radius * radius;
            new_color.G /= radius * radius;
            new_color.B /= radius * radius;

            image.GetPixel(new_x, new_y) = new_color;
        }
    }

    image.Crop(new_width, new_height);
}
