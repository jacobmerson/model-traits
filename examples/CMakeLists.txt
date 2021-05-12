if (NOT ${MODEL_TRAITS_BUILD_EXTERNAL})
    find_package(fmt REQUIRED)
endif ()

if (${MODEL_TRAITS_ENABLE_SIMMETRIX} AND ${MODEL_TRAITS_ENABLE_YAML})
    add_executable(smd2yaml smd2yaml.cc)
    target_link_libraries(smd2yaml PUBLIC ModelTraits::ModelTraits ModelTraits::IO)

    add_executable(smd2phasta smd2yaml_phasta.cc)
    target_link_libraries(smd2phasta PUBLIC ModelTraits::ModelTraits ModelTraits::IO)

    install(TARGETS smd2yaml smd2phasta
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif ()

if (${MODEL_TRAITS_ENABLE_YAML})
    add_executable(yaml_with_backend yaml_with_backend.cc)
    target_link_libraries(yaml_with_backend PUBLIC ModelTraits::ModelTraits ModelTraits::IO fmt::fmt)
    install(TARGETS yaml_with_backend
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
endif ()

add_executable(core_usage core_usage.cc)
target_link_libraries(core_usage PUBLIC ModelTraits::ModelTraits fmt::fmt)

add_executable(basic_usage basic_usage.cc)
target_link_libraries(basic_usage PUBLIC ModelTraits::ModelTraits fmt::fmt)
install(TARGETS basic_usage core_usage
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

if (${MODEL_TRAITS_ENABLE_SCOREC})
    find_package(SCOREC CONFIG REQUIRED)
    if (${SCOREC_FOUND})
        add_executable(pumi pumi.cc)
        target_link_libraries(pumi SCOREC::core SCOREC::apf SCOREC::gmi ModelTraits::ModelTraits ModelTraits::IO fmt::fmt)
        install(TARGETS pumi
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                )
    endif ()
endif ()

#install(TARGETS
#        DESTINATION ${CMAKE_INSTALL_BINDIR}
#        )
