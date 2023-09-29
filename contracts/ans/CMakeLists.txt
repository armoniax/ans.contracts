add_contract(ans ans ${CMAKE_CURRENT_SOURCE_DIR}/src/ans.cpp)

target_include_directories(ans
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../amax.bootdao/include)

set_target_properties(ans
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/ans.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/ans.contracts.md @ONLY )

target_compile_options( ans PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
