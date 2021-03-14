
find_package(fmt REQUIRED)

add_library(modelTraits bcExprtkFunction.cc bcAssociatedModelTraits.cc)
target_include_directories(modelTraits PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(modelTraits PUBLIC fmt::fmt PRIVATE exprtk)
target_compile_definitions(modelTraits PRIVATE -Dexprtk_disable_string_capabilities)

add_subdirectory(backends)
