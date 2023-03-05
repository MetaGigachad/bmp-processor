#include "filter_pipeline.h"

using namespace std::string_view_literals;

FiltersPipeline::FilterTable FiltersPipeline::filter_table {
    {"crop"sv, CropFilter::Create},
    {"gs"sv, GrayscaleFilter::Create},
    {"neg"sv, NegativeFilter::Create},
    {"sharp"sv, SharpeningFilter::Create},
    {"edge"sv, EdgeDetectionFilter::Create},
    {"blur"sv, GaussianBlurFilter::Create},
    {"pixelate"sv, PixelateFilter::Create}
};

FiltersPipeline::FiltersPipeline(std::vector<FilterInfo>& filter_infos) {
    for (const auto& filter_info : filter_infos) {
        auto& filter_create = filter_table[filter_info.GetFilterName()];
        filters_.push_back(filter_create(filter_info));
    }
}

Bitmap& FiltersPipeline::Apply(Bitmap& image) {
    for (const auto& filter : filters_) {
        filter->Apply(image);
    }

    return image;
}

FiltersPipeline::~FiltersPipeline() {
    for (auto& filter : filters_) {
        delete filter;
    }
}
