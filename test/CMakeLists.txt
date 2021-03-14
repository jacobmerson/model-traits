find_package(Catch2 3 REQUIRED)

add_executable(test_model_traits
        test_category_node.cc
        test_model_traits.cc test_function.cc test_associated_model_traits.cc)
target_link_libraries(test_model_traits Catch2::Catch2WithMain modelTraits bcIOBackend)

add_executable(generic_test generic_test.cc)
target_link_libraries(generic_test modelTraits bcIOBackend)

add_executable(read_yaml test_reader.cc)
target_link_libraries(read_yaml modelTraits bcIOBackend)

find_package(fmt REQUIRED)
add_executable(exprtk_test exprtk_test.cc)
target_link_libraries(exprtk_test modelTraits fmt::fmt)

#if (ENABLE_SIMMETRIX)
#    find_package(SIMMODSUITE MODULE REQUIRED)
#    add_executable(test_sim sim_test.cc)
#    target_link_libraries(test_sim PRIVATE modelTraits fmt::fmt ${SIMMODSUITE_LIBS})
#    target_include_directories(test_sim PRIVATE ${SIMMODSUITE_INCLUDE_DIR})
#endif ()
