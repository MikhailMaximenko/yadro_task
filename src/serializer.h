#pragma once

#include "point.h"

#include <vector>
#include <fstream>

class serializer {
    std::ofstream _out;

public:
    serializer(const char*);
    void serialize(std::vector<point> const&);
};