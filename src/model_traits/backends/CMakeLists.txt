
SET(IO_PUBLIC_HEADERS
        Yaml.h
        Simmetrix.h
        )
if (ENABLE_YAML)
    if (NOT ${BUILD_EXTERNAL})
        find_package(yaml-cpp REQUIRED)
    endif ()
    set(SOURCES ${SOURCES} Yaml.cc YamlConvert.cc)
endif ()

if (ENABLE_SIMMETRIX)
    find_package(SIMMODSUITE MODULE REQUIRED)
    set(SOURCES ${SOURCES} Simmetrix.cc SimmetrixConvert.cc)
endif ()

if (NOT ${BUILD_EXTERNAL})
    find_package(fmt REQUIRED)
endif ()
add_library(ModelTraits_IO ${SOURCES})
add_library(ModelTraits::IO ALIAS ModelTraits_IO)
set_target_properties(ModelTraits_IO PROPERTIES EXPORT_NAME IO
        PUBLIC_HEADER "${IO_PUBLIC_HEADERS}")
target_link_libraries(ModelTraits_IO PUBLIC ModelTraits::ModelTraits PRIVATE fmt::fmt)

if (ENABLE_YAML)
    target_link_libraries(ModelTraits_IO PRIVATE yaml-cpp)
    target_compile_definitions(ModelTraits_IO PUBLIC -DMT_HAVE_YAML)
endif ()
if (ENABLE_SIMMETRIX)
    target_link_libraries(ModelTraits_IO PRIVATE ${SIMMODSUITE_LIBS})
    target_include_directories(ModelTraits_IO PRIVATE ${SIMMODSUITE_INCLUDE_DIR})
    target_compile_definitions(ModelTraits_IO PUBLIC -DMT_HAVE_SIMMETRIX)
endif ()

install(TARGETS ModelTraits_IO
        EXPORT ModelTraits_IO
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/model_traits/backends
        )
install(EXPORT ModelTraits_IO
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ModelTraits
        NAMESPACE ModelTraits::
        COMPONENT ModelTraits_IO
        FILE ModelTraits_IO.cmake
        )
