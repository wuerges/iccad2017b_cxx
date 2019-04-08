#pragma once

#include <model.hpp>

namespace iccad
{

namespace parser
{
    template <typename Iterator>
    int parse_it(iccad::Input & m, Iterator first, Iterator last);
    int parse_file(iccad::Input &m, char* filename);
}

}
