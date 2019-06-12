
add_executable(RoutingReport test_router.cxx)
target_link_libraries(RoutingReport
  Model
  Parser
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )
