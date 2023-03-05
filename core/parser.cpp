#include "parser.h"

#include "app_error.h"

FiltersParser::FiltersParser(int argc, const char** argv) : argc_(argc - 1), argv_(argv + 1) {
    if (argc_ < 2) {
        throw AppError(AppError::NotEnoughFileEntries);
    }
}

std::vector<FilterInfo> FiltersParser::ParseFilters() {
    int filters_argc = argc_ - 2;
    const char** filters_argv = nullptr;
    if (filters_argc > 0) {
        filters_argv = &argv_[2];
    }

    std::vector<FilterInfo> filters_info;
    FilterInfo* current_filter = nullptr;

    for (int i = 0; i < filters_argc; ++i) {
        std::string_view arg = filters_argv[i];

        if (arg[0] == '-') {
            if (current_filter) {
                filters_info.push_back(*current_filter);
            }
            current_filter = new FilterInfo(arg.substr(1, arg.size() - 1));
        } else {
            if (!current_filter) {
                throw AppError(AppError::FilterNameNotSpecified);
            }
            current_filter->AddParam(arg);
        }
    }
    if (current_filter) {
        filters_info.push_back(*current_filter);
    }

    return filters_info;
}
