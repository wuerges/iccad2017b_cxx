#pragma once

#include <model.hpp>

namespace model
{

namespace parser
{
    template <typename Iterator>
    int parse_it(model::Input & m, Iterator first, Iterator last);
    int parse_file(model::Input &m, char* filename);
}

}
