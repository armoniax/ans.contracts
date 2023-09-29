# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/richardchen/work/wasm.contracts/ans.contracts/tests"
  "/Users/richardchen/work/wasm.contracts/ans.contracts/build/tests"
  "/Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix"
  "/Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/tmp"
  "/Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp"
  "/Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src"
  "/Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/richardchen/work/wasm.contracts/ans.contracts/build/contracts_unit_tests-prefix/src/contracts_unit_tests-stamp${cfgdir}") # cfgdir has leading slash
endif()
