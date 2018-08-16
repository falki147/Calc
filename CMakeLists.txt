cmake_minimum_required(VERSION 3.11)

project(Calc)

option(CALC_BUILD_EXAMPLE "Build example" OFF)

add_library(Calc STATIC src/Calc.c)
target_include_directories(Calc
                           PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)

if(CALC_BUILD_EXAMPLE)
  add_executable(Example example/Example.c)
  target_link_libraries(Example PUBLIC Calc)
endif()

install(TARGETS Calc EXPORT Calc ARCHIVE DESTINATION "lib/Calc")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION "./"
        FILES_MATCHING PATTERN "*.h")

install(EXPORT Calc DESTINATION "lib/cmake/Calc" FILE "CalcConfig.cmake")
