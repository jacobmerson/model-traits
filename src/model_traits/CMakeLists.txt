
if (NOT ${BUILD_EXTERNAL})
    find_package(fmt REQUIRED)
endif ()

SET(MODEL_TRAITS_PUBLIC_HEADERS
        AssociatedModelTraits.h
        CategoryNode.h
        Convert.h
        ExprtkFunction.h
        FunctionHelper.h
        GeometrySet.h
        ModelTrait.h
        ModelTraitNode.h
        ModelTraits.h
        ModelTraitsIO.h
        NamedFunction.h
        Typedefs.h
        TypeTraits.h
        Utility.h
        )

add_library(ModelTraits_ModelTraits ExprtkFunction.cc AssociatedModelTraits.cc CategoryNode.cc ModelTraitNode.cc ModelTraits.cc)
add_library(ModelTraits::ModelTraits ALIAS ModelTraits_ModelTraits)
set_target_properties(ModelTraits_ModelTraits PROPERTIES EXPORT_NAME ModelTraits
        PUBLIC_HEADER "${MODEL_TRAITS_PUBLIC_HEADERS}")
target_include_directories(ModelTraits_ModelTraits PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )
target_link_libraries(ModelTraits_ModelTraits PRIVATE "$<BUILD_INTERFACE:exprtk>" fmt::fmt)
target_compile_definitions(ModelTraits_ModelTraits PRIVATE -Dexprtk_disable_string_capabilities)

install(TARGETS ModelTraits_ModelTraits
        EXPORT ModelTraits_ModelTraits
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/model_traits
        )
export(EXPORT ModelTraits_ModelTraits
        NAMESPACE ModelTraits::
        FILE ModelTraits_ModelTraits.cmake
        )

install(EXPORT ModelTraits_ModelTraits
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ModelTraits
        NAMESPACE ModelTraits::
        )
add_subdirectory(backends)
