#pragma once

namespace iccad {
    // true : will only find 6 neighbor for each node.
    // false : will incrementaly increase the distance for all nodes.
    const bool CONFIG_FAST_MST = false; 

    // true : will use astar
    // falze : will use dijkstra.
    const bool CONFIG_ASTAR = true;

    // true : will perform detailed routing using neighborhood.
    // false : will perform dailed rouiting using the whole circuit.
    const bool CONFIG_FAST_ASTAR = true; 
}