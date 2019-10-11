find_package (Threads)

add_executable(RoutingReport routing_report.cxx)
target_link_libraries(RoutingReport
  Model
  Parser
  cadgeometry
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
  )
