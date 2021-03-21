
add_library(exprtk INTERFACE)
target_include_directories(exprtk INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)


install(TARGETS exprtk
        EXPORT ModelTraits_Exprtk)
export(EXPORT ModelTraits_Exprtk
       FILE ModelTraits_Exprtk.cmake
       NAMESPACE ModelTraits::
       )
