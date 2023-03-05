#pragma once

#include <vector>
#include <functional>
#include <string_view>
#include <map>

#include "bitmap.h"
#include "filters.h"

class FiltersPipeline {
public:
    using FilterTable = std::map<std::string_view, std::function<BaseFilter*(const FilterInfo&)>>;

    FiltersPipeline(std::vector<FilterInfo>& filter_infos);

    Bitmap& Apply(Bitmap& image);

    ~FiltersPipeline();

private:
    std::vector<BaseFilter*> filters_;

    static FilterTable filter_table;
};
