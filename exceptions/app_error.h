#pragma once

#include <exception>
#include <map>

class AppError : std::exception {
public:
    enum ErrorCode {
        NotEnoughFileEntries,
        FilterNameNotSpecified,FilterArgumentCastError,
        FileSignatureError, InputFileIsNotOpen, OutputFileIsNotOpen,

        CropFilterParamsError, GrayscaleFilterParamsError,
        NegativeFilterParamsError, SharpeningFilterParamsError,
        EdgeDetectionFilterParamsError, GaussianBlurFilterParamsError,
        PixelateFilterParamsError, PixelateFilterMultiplierLimit
    };

    AppError(ErrorCode error_code) : error_code_(error_code) {}

    void PrintMessage() const;

private:
    ErrorCode error_code_;

    static std::map<ErrorCode, const char*> error_messages;
};