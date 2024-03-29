cmake_minimum_required(VERSION 3.14)
project(ModelTraits VERSION 0.2.1 LANGUAGES CXX)

# don't build TESTS unless model-traits is the top level package
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()
# Set the language standard to C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Wno-unknown-pragmas")
endif ()

# Let CMake know where to find custom FindFoo.cmake files
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")


# RPATHS stuff...this is important to make spack happy in all situations
# enable @rpath in the install name for any shared library being built
# note: it is planned that a future version of CMake will enable this by default
set(CMAKE_MACOSX_RPATH 1)
# Always use full RPATH
# http://www.cmake.org/Wiki/CMake_RPATH_handling
# http://www.kitware.com/blog/home/post/510
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF ("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF ("${isSystemDir}" STREQUAL "-1")

# setup variables for installing
include(GNUInstallDirs)
# Explicitly set DOCDIR location each time (see Scott, Professional CMAKE)
set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME})


option(MODEL_TRAITS_ENABLE_SIMMETRIX "Build with Simmetrix support" OFF)
message(STATUS "MODEL_TRAITS_ENABLE_SIMMETRIX=${MODEL_TRAITS_ENABLE_SIMMETRIX}")
option(MODEL_TRAITS_ENABLE_YAML "Build with YAML support" ON)
message(STATUS "MODEL_TRAITS_ENABLE_YAML=${MODEL_TRAITS_ENABLE_YAML}")
option(MODEL_TRAITS_ENABLE_SCOREC "Build the pumi example" OFF)
message(STATUS "MODEL_TRAITS_ENABLE_SCOREC=${MODEL_TRAITS_ENABLE_SCOREC}")
option(MODEL_TRAITS_BUILD_EXTERNAL "build external libraries" ON)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(doc)

# don't build TESTS unless model-traits is the top level package
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    if (NOT ${MODEL_TRAITS_BUILD_EXTERNAL})
        find_package(Catch2 REQUIRED)
    endif ()
    add_subdirectory(test)
endif ()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ModelTraitsConfig.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/cmake/ModelTraitsConfig.cmake
               @ONLY)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/ModelTraitsConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ModelTraits)
