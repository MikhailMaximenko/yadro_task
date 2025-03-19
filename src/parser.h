#pragma once

#include "point.h"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <memory>
#include <vector>

class parser {
protected:
    std::ifstream _in;
    
    parser(const char*);
public:

    virtual std::vector<point> get_all_points() = 0;

    bool eof() const noexcept;
};


class simlpe_struct_parser : public parser {
public:
    simlpe_struct_parser(const char* );

    virtual point get_point() = 0;

    std::vector<point> get_all_points() override;

};

class text_parser : public simlpe_struct_parser {

public:
    text_parser(const char* );

    point get_point() override;
};

class bin_parser : public simlpe_struct_parser {
    struct bin_point {
        uint32_t group:8;
        uint32_t x:12;
        uint32_t y:12;
    };

    char _buffer[sizeof(bin_point)];

public:
    bin_parser(const char*);

    point get_point() override;
    
};


class json_parser : public parser {
    void expect_symb(char );

    bool try_symb(char );

    std::string parse_string();

    std::vector<point> parse_points();

public:
    json_parser(const char* );

    std::vector<point> get_all_points();
};


std::unique_ptr<parser> make_parser(const char *);