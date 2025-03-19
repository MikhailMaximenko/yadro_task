#include "point.h"
#include "serializer.h"


serializer::serializer(const char* f_name)
    : _out(f_name, std::ofstream::out)
{}

void serializer::serialize(std::vector<point> const& points) {
    for (auto const& p : points) {
        _out << p.group << ':' << p.x << ',' << p.y <<'\n';
    }
}