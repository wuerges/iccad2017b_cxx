#pragma once

#include <model.hpp>

namespace model
{

namespace parser
{
    template <typename Iterator>
    int parse_it(model::Model & m, Iterator first, Iterator last);
    int parse_file(model::Model &m, char* filename);
}

}
