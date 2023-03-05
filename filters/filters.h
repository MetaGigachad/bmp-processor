#pragma once

#include "parser.h"
#include "bitmap.h"

class BaseFilter {
public:
    virtual void Apply(Bitmap& image) const = 0;

    virtual ~BaseFilter() {}
};

class CropFilter : BaseFilter {
public:
    CropFilter(uint32_t new_width, uint32_t new_height) : new_width_(new_width), new_height_(new_height) {}

    void Apply(Bitmap& image) const override;

    static BaseFilter* Create(const FilterInfo& info);

private:
    uint32_t new_width_;
    uint32_t new_height_;
};

class GrayscaleFilter : BaseFilter {
public:
    void Apply(Bitmap& image) const override;

    static BaseFilter* Create(const FilterInfo& info);
};

class NegativeFilter : BaseFilter {
public:
    void Apply(Bitmap& image) const override;

    static BaseFilter* Create(const FilterInfo& info);
};

class SharpeningFilter : BaseFilter {
public:
    void Apply(Bitmap& image) const override;

    static BaseFilter* Create(const FilterInfo& info);
};

class EdgeDetectionFilter : BaseFilter {
public:
    EdgeDetectionFilter(double threshold) : threshold_(threshold) {}

    void Apply(Bitmap& image) const override;

    static BaseFilter* Create(const FilterInfo& info);

private:
    double threshold_;
};

class GaussianBlurFilter : BaseFilter {
public:
    explicit GaussianBlurFilter(double sigma);

    void Apply(Bitmap& image) const override;

    double GaussFunc(int32_t i) const;
    Color CalcNewPixelHorizontal(Bitmap& image, uint32_t x, uint32_t y) const;
    Color CalcNewPixelVertical(Bitmap& image, uint32_t x, uint32_t y) const;

    static BaseFilter* Create(const FilterInfo& info);

private:
    double sigma_;
    int32_t radius_;
};

class PixelateFilter : BaseFilter {
public:
    PixelateFilter(double res_multiplier) : res_multiplier_(res_multiplier) {}

    void Apply(Bitmap& image) const override;

    static BaseFilter* Create(const FilterInfo& info);

private:
    double res_multiplier_;
};
