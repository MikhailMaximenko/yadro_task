#include "parser.h"
#include "serializer.h"
#include "src/exception.h"
#include "src/serializer.h"
#include <iostream>

int main(int argc, const char* argv[]) {
    auto ser = serializer("all_points.txt");
    for (size_t i = 1; i < argc; ++i) {
        try {
            auto p = make_parser(argv[i]);
            ser.serialize(p->get_all_points());
        } catch(parsing_error const& e) {
            std::cout << "parsing error occurred on file " << argv[i] << " with message: " << e.what() << "\n"; 
            return -1;
        }
    }
    
    
    return 0;
}