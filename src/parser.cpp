#include "parser.h"
#include "point.h"
#include "exception.h"


#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


parser::parser(const char* file_name) 
    : _in(file_name, std::ifstream::in)
{}

bool parser::eof() const noexcept {
    return _in.eof();
}

simlpe_struct_parser::simlpe_struct_parser(const char* file_name) 
    : parser(file_name)
{}


std::vector<point> simlpe_struct_parser::get_all_points() {
    std::vector<point> res;
    while (!eof()) {
        res.emplace_back(get_point());
    }
    return res;
}


text_parser::text_parser(const char* file_name) 
    : simlpe_struct_parser(file_name) 
{}

point text_parser::get_point() {
    std::string to_parse;
    std::getline(_in, to_parse);
    size_t group_name_end = to_parse.find(':');
    if (to_parse.size() <= group_name_end) throw parsing_error("bad format, expected symbol ':'");
    std::string group_name(to_parse.substr(0, group_name_end));
    size_t comma_pos = to_parse.find(',', group_name_end);
    if (to_parse.size() <= comma_pos) throw parsing_error("bad format, expected symbol ','");

    try {
        uint32_t x = std::stoi(to_parse.substr(group_name_end + 1, comma_pos));
        uint32_t y = std::stoi(to_parse.substr(comma_pos + 1, to_parse.size()));
        return {group_name, x, y};
    } catch(std::invalid_argument const& e) {
        throw parsing_error(e.what());
    } catch(std::out_of_range const& e) {
        throw parsing_error(e.what());
    }
}




bin_parser::bin_parser(const char* file_name) 
    : simlpe_struct_parser(file_name)
{}

point bin_parser::get_point() {
    _in.read(_buffer, 4);
    bin_point *res = reinterpret_cast<bin_point*>(_buffer);
    return {std::to_string(res->group), res->x, res->y};
}
    


void json_parser::expect_symb(char symb) {
    char s;
    _in >> s;
    if (s != symb) {
        std::ostringstream message;
        message << "expected char: " << symb << ", found: " << s;
        throw parsing_error(message.str());
    }
}

bool json_parser::try_symb(char symb) {
    char chr;
    _in >> chr;
    if (symb == chr) return true;
    _in.unget();
    return false;
}

std::string json_parser::parse_string() {
    std::string res;
    expect_symb('\"');

    char chr;
    while (!eof()) {
        _in >> chr;
        if (chr == '\"') {
            return res;
        }
        res.push_back(chr);
    }
    throw parsing_error("expected \"");
}

std::vector<point> json_parser::parse_points() {
    std::vector<point> res;
    expect_symb('[');
    do {
        expect_symb('{');
        int magic = 0;
        std::string group;
        uint32_t x;
        uint32_t y;
        do {
            std::string tag = parse_string();
            if (tag == "group") {
                if ((magic & 1) != 0) throw parsing_error("repeating group found");
                magic |= 1;
                expect_symb(':');
                group = parse_string();
            } else if (tag == "x") {
                if (((magic >> 1) & 1) != 0) throw parsing_error("repeating x found");
                magic |= 1 << 1;
                expect_symb(':');
                _in >> x;
            } else if (tag == "y") {
                if (((magic >> 2) & 1) != 0) throw parsing_error("repeating y found");
                magic |= 1 << 2;
                expect_symb(':');
                _in >> y;
            } else {
                std::string message("unknown option: ");
                message.append(tag);
                throw parsing_error(message);
            }
            
        } while(try_symb(','));
        if (magic != 7) throw parsing_error("not all point fields are specified");
        expect_symb('}');
        res.emplace_back(group, x, y);
    } while((try_symb(',')));
    expect_symb(']');
    return res;
}



json_parser::json_parser(const char* file_name) 
    : parser(file_name) 
{}

std::vector<point> json_parser::get_all_points() {
    expect_symb('{');
    std::string tag = parse_string();
    expect_symb(':');
    std::vector<point> res = parse_points();
    expect_symb('}');
    return res;
}


std::unique_ptr<parser> make_parser(const char *f_name) {
    const char* dot_pos = strchr(f_name, '.');
    
    if (std::strcmp(dot_pos, ".txt") == 0) {
        return std::make_unique<text_parser>(f_name);
    } else if (std::strcmp(dot_pos, ".bin") == 0) {
        return std::make_unique<bin_parser>(f_name);
    } else if (std::strcmp(dot_pos, ".json") == 0) {
        return std::make_unique<json_parser>(f_name);
    } else {
        throw parsing_error("unkown format found");
    }
}