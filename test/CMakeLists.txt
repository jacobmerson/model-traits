if (NOT ${MODEL_TRAITS_BUILD_EXTERNAL})
    find_package(Catch2 3 REQUIRED)
endif ()

include(Catch)

add_executable(test_model_traits
        test_category_node.cc
        test_model_traits.cc test_function.cc test_associated_model_traits.cc)
target_link_libraries(test_model_traits Catch2::Catch2WithMain ModelTraits::ModelTraits)
catch_discover_tests(test_model_traits)
