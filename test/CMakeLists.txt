
find_package(Boost COMPONENTS system filesystem REQUIRED)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

find_package (Threads)

add_executable(TestParse test_parse.cxx)
target_link_libraries(TestParse
  Model
  Parser
  cadgeometry
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
  )

add_executable(TestMST test_mst.cxx)
target_link_libraries(TestMST
  Model
  Parser
  cadgeometry
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
  )


add_executable(TestGlobalRouter test_router.cxx)
target_link_libraries(TestGlobalRouter
  Model
  Parser
  cadgeometry
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
  )


