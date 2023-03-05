#include "app.h"

#include <vector>
#include <string_view>

#include "bitmap.h"
#include "filter_pipeline.h"
#include "app_error.h"

void App::Run() const {
    try {
        FiltersParser parser(argc_, argv_);

        std::string_view input_path = parser.ParseInputPath();
        std::string_view output_path = parser.ParseOutputPath();
        std::vector<FilterInfo> parsed_filters = parser.ParseFilters();

        Bitmap image;
        image.LoadFromBMP(input_path);

        FiltersPipeline filter_pipeline(parsed_filters);
        filter_pipeline.Apply(image);

        image.ExportAsBMP(output_path);
    } catch (const AppError& e) {
        e.PrintMessage();
    }
}
