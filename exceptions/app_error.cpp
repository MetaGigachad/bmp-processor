#include "app_error.h"

#include <iostream>

std::map<AppError::ErrorCode, const char*> AppError::error_messages {
    {NotEnoughFileEntries,
     "Usage: bmp_processor <input_file> <output_file> [<-filter_name> [filter_params]]"
     "\nAvailable filters:"
     "\n  -crop <width> <height>          Crops image."
     "\n  -gs                             Applies grayscale filter."
     "\n  -neg                            Applies negative filter."
     "\n  -sharp                          Sharpens image."
     "\n  -edge <threshold>               Produces grayscale image with white edges."
     "\n  -blur <sigma>                   Applies gaussian blur to image."
     "\n  -pixelate <res_multiplier>      Reduces image resolution."},

    {FilterNameNotSpecified, "No <-filter_name> before [filter_params]"},
    {FilterArgumentCastError, "Invalid filter argument was provided"},

    {FileSignatureError, "Invalid file signature."},
    {InputFileIsNotOpen, "Input file cannot be opened."},
    {OutputFileIsNotOpen, "Output file cannot be opened."},

    {CropFilterParamsError, "Params <width> <height> should be supplied for -crop filter."},
    {GrayscaleFilterParamsError, "No params should be supplied for -gs filter."},
    {NegativeFilterParamsError, "No params should be supplied for -neg filter."},
    {SharpeningFilterParamsError, "No params should be supplied for -sharp filter."},
    {EdgeDetectionFilterParamsError, "Param <threshold> should be supplied for -edge filter."},
    {GaussianBlurFilterParamsError, "Param <sigma> should be supplied for -blur filter."},
    {PixelateFilterParamsError, "Param <res_multiplier> should be supplied for -pixelate filter."},
    {PixelateFilterMultiplierLimit, "Param <res_multiplier> can't be bigger than 1."}
};

void AppError::PrintMessage() const {
    switch (error_code_) {
        case NotEnoughFileEntries:
            std::cout << error_messages[error_code_] << std::endl;
            break;
        default:
            std::cerr << error_messages[error_code_] << std::endl;
    }
}
