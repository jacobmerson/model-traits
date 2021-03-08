
find_package(fmt REQUIRED)
#add_library(modelTraits INTERFACE)
##target_include_directories(modelTraits PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
#target_include_directories(modelTraits INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
## set spdlog to use externally build fmt library
#target_compile_definitions(modelTraits INTERFACE SPDLOG_FMT_EXTERNAL)
#target_link_libraries(modelTraits INTERFACE fmt::fmt)


add_library(modelTraits bcExprtkFunction.cc)
#target_include_directories(modelTraits PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(modelTraits PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# set spdlog to use externally build fmt library
#target_compile_definitions(modelTraits PRIVATE SPDLOG_FMT_EXTERNAL)
target_link_libraries(modelTraits PUBLIC fmt::fmt PRIVATE exprtk)

add_subdirectory(backends)
