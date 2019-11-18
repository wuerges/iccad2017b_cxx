#pragma once

enum MST_ALGORITHM {
    LOCAL_MST,
    GLOBAL_MST,
    ITERATIVE_MST
};

namespace iccad {
    // true : will only find 6 neighbor for each node.
    // false : will incrementaly increase the distance for all nodes.
    constexpr int CONFIG_MST = ITERATIVE_MST; 
    
    // config 2 step
    constexpr bool CONFIG_2STEP_MST = true; 

    // true : will use astar
    // falze : will use dijkstra.
    constexpr bool CONFIG_ASTAR = true;

    // true : will perform detailed routing using neighborhood.
    // false : will perform dailed rouiting using the whole circuit.
    constexpr bool CONFIG_FAST_ASTAR = true; 

    // the number of pixels expanded in all directions 
    // the routing window is increased;
    constexpr int ROUTING_WINDOW = 100;

    // the number of neighboors in the local solution
    constexpr int LOCAL_NEIGHBOORS = 20;
}
