#pragma once

#include <string_view>
#include <vector>
#include <tuple>

class FilterInfo {
public:
    explicit FilterInfo(const std::string_view filter_name) : filter_name_(filter_name) {}

    const std::string_view GetFilterName() const {
        return filter_name_;
    }
    const std::vector<std::string_view>& GetParams() const {
        return params_;
    }

    void AddParam(const std::string_view param) {
        params_.push_back(param);
    }

    bool operator==(const FilterInfo& other) const {
        return std::tie(filter_name_, params_) == std::tie(other.filter_name_, other.params_);
    }

private:
    const std::string_view filter_name_;
    std::vector<std::string_view> params_;
};

class FiltersParser {
public:
    FiltersParser(int argc, const char* argv[]);

    std::string_view ParseInputPath() const {
        return static_cast<std::string_view>(argv_[0]);
    }

    std::string_view ParseOutputPath() const {
        return static_cast<std::string_view>(argv_[1]);
    }

    std::vector<FilterInfo> ParseFilters();

private:
    int argc_;
    const char** argv_;
};
