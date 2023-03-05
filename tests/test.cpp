#include "catch.hpp"
#include <cctype>
#include <iostream>
#include <exception>
#include <string_view>

#include "core/parser.h"
#include "core/app.h"
#include "core/bitmap.h"
#include "core/utils.h"
#include "filters/filter_pipeline.h"
#include "filters/filters.h"

using namespace std::literals::string_view_literals;

std::string path1 = "examples/example.bmp";
std::string path2 = "examples/output.bmp";

TEST_CASE("FilterParser") {
    size_t argc = 8;
    const char* argv[] = {"image_processor", "input.bmp", "output.bmp", "-filter1", "56", "mode1", "808", "-filter2"};

    FiltersParser parser(argc, argv);

    std::vector<FilterInfo> parsed;

    FilterInfo filter_info1("filter1");
    filter_info1.AddParam("56");
    filter_info1.AddParam("mode1");
    filter_info1.AddParam("808");
    parsed.push_back(filter_info1);

    FilterInfo filter_info2("filter2");
    parsed.push_back(filter_info2);

    REQUIRE(parser.ParseFilters() == parsed);
    REQUIRE(parser.ParseInputPath() == argv[1]);
    REQUIRE(parser.ParseOutputPath() == argv[2]);
}

TEST_CASE("Bitmap") {
    Bitmap img1;
    Bitmap img2;

    img1.LoadFromBMP(path1);
    img1.ExportAsBMP(path2);

    img2.LoadFromBMP(path2);

    Color color1 = img1.GetPixel(0, 0);
    Color color2 = img2.GetPixel(0, 0);
    Color color3 = img1.GetClosestPixel(-1, -1);

    REQUIRE(img1 == img2);
    REQUIRE(color1 == color2);
    REQUIRE(color2 == color3);
    REQUIRE(img1.GetWidth() == img2.GetWidth());
    REQUIRE(img1.GetHeight() == img2.GetHeight());
}

TEST_CASE("Utils") {
    REQUIRE(SVToType<uint32_t>("15"sv) == 15);
    REQUIRE(SVToType<double>("0.666"sv) == 0.666);
}

TEST_CASE("FiltersPipeline") {
    FilterInfo crop("crop"sv);
    crop.AddParam("100"sv);
    crop.AddParam("100"sv);
    FilterInfo gs("gs"sv);
    FilterInfo neg("neg"sv);
    FilterInfo sharp("sharp"sv);
    FilterInfo edge("edge"sv);
    edge.AddParam("0.01"sv);
    FilterInfo blur("blur"sv);
    blur.AddParam("2");
    FilterInfo pixelate("pixelate"sv);
    pixelate.AddParam("0.5"sv);
    std::vector<FilterInfo> infos = {crop, gs, neg, sharp, edge, blur, pixelate,
                                     crop, gs, neg, sharp, edge, blur, pixelate};

    FiltersPipeline pipeline(infos);

    Bitmap img1;
    Bitmap img2;
    img1.LoadFromBMP(path1);
    img2.LoadFromBMP(path2);

    CropFilter(100, 100).Apply(img1);
    GrayscaleFilter().Apply(img1);
    NegativeFilter().Apply(img1);
    SharpeningFilter().Apply(img1);
    EdgeDetectionFilter(0.01).Apply(img1);
    GaussianBlurFilter(2).Apply(img1);
    PixelateFilter(0.5).Apply(img1);
    CropFilter(100, 100).Apply(img1);
    GrayscaleFilter().Apply(img1);
    NegativeFilter().Apply(img1);
    SharpeningFilter().Apply(img1);
    EdgeDetectionFilter(0.01).Apply(img1);
    GaussianBlurFilter(2).Apply(img1);
    PixelateFilter(0.5).Apply(img1);

    pipeline.Apply(img2);

    REQUIRE(img1 == img2);
}
