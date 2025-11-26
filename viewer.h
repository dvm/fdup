#pragma once

#include "finder.h"

class viewer
{
public:
    using result_type = int;

    viewer() = default;

    viewer const& exec(finder const& finder);
};
