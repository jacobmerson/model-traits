
add_library(exprtk INTERFACE)
target_include_directories(exprtk INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
