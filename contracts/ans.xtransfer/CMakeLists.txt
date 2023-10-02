add_contract(ans.xtransfer ans.xtransfer ${CMAKE_CURRENT_SOURCE_DIR}/src/ans.xtransfer.cpp)

target_include_directories(ans.xtransfer
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include )

set_target_properties(ans.xtransfer
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/ans.xtransfer.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/ans.xtransfer.contracts.md @ONLY )

target_compile_options( ans.xtransfer PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
