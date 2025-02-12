# we need to specify the minimum version of CMake
cmake_minimum_required(VERSION 3.25)
# we define the name of our CMake project
project(ExampleMixer)
# we use C++17 to write the UGen - this can be changed
set(CMAKE_CXX_STANDARD 17)

# on non-linux OS we need to add the `.scx` file extension to our binary
if(APPLE OR WIN32)
    set(CMAKE_SHARED_MODULE_SUFFIX ".scx")
else()
    set(CMAKE_SHARED_MODULE_PREFIX "")
endif()

# include our C++ source code file to our compilation target "Example Mixer"
# as a Module (see https://cmake.org/cmake/help/book/mastering-cmake/chapter/Key%20Concepts.html)
# "MODULE indicates that the library must be created so that it can be dynamically-loaded into an executable."
add_library(ExampleMixer MODULE src/ExampleMixer.cpp)

# in order that scsynth/supernova knows how to use our UGen we need to write our
# UGen against the Plugin API and therefore need to include the Plugin API interface
# to our compilation target.
target_include_directories(ExampleMixer PUBLIC ${SC_SRC_PATH}/include/plugin_interface)
target_include_directories(ExampleMixer PUBLIC ${SC_SRC_PATH}/include/common)
target_include_directories(ExampleMixer PUBLIC ${SC_SRC_PATH}/common)

# define the target folder for our complete build - aka the install target
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)

# add our compiled ExampleMixer target to the install target
install(TARGETS ExampleMixer LIBRARY DESTINATION ${PROJECT_NAME})
# also copy the ExampleMixer.sc file ...
install(FILES src/ExampleMixer.sc DESTINATION ${PROJECT_NAME}/Classes)
# ...and the help file
install(FILES src/ExampleMixer.schelp DESTINATION ${PROJECT_NAME}/HelpSource/Classes)
