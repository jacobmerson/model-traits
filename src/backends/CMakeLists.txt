
option(YAML-CPP "Enables yaml-cpp backend. Requires yaml-cpp." ON)
if(YAML-CPP)
  find_package(yaml-cpp REQUIRED)
  add_library(bcBackendYamlCpp bcBackendYamlCpp.cc)
  target_include_directories(bcBackendYamlCpp PUBLIC {CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(bcBackendYamlCpp PUBLIC modelTraits yaml-cpp)
endif()
