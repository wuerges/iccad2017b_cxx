
find_package(Boost COMPONENTS system filesystem REQUIRED)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

add_executable(TestParse test_parse.cxx)
target_link_libraries(TestParse
  Model
  Parser
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

add_executable(TestMST test_mst.cxx)
target_link_libraries(TestMST
  Model
  Parser
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

add_executable(TestGlobalRouter test_router.cxx)
target_link_libraries(TestGlobalRouter
  Model
  Parser
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

add_executable(TestTreap test_treap.cxx)
target_link_libraries(TestTreap
  Model rapidcheck)
