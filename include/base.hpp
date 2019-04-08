#pragma once

#include <array>

namespace iccad {
    /**
     * An array representing a 3D point.
     * Each position represents the x, y and z coordinates, in this order.
     */
    using PT = std::array<int, 3>;
    /**
     * Two points that represent the oposite points of a cuboid
     * The first point is the low value for the x, y and z coordinates.
     * The second point is the high value for the x, y, and z coordinates.
     */
    using Shape = std::array<PT, 2>;
}
