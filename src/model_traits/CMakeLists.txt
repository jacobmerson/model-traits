
find_package(fmt REQUIRED)

add_library(modelTraits ExprtkFunction.cc AssociatedModelTraits.cc CategoryNode.cc ModelTraitNode.cc ModelTraits.cc)
target_include_directories(modelTraits PUBLIC ..)
target_link_libraries(modelTraits PRIVATE exprtk fmt::fmt)
target_compile_definitions(modelTraits PRIVATE -Dexprtk_disable_string_capabilities)

add_subdirectory(backends)
