find_package(fmt REQUIRED)

if (${ENABLE_SIMMETRIX} AND ${ENABLE_YAML})
    add_executable(smd2yaml smd2yaml.cc)
    target_link_libraries(smd2yaml modelTraits IO)

    add_executable(smd2phasta smd2yaml_phasta.cc)
    target_link_libraries(smd2phasta modelTraits IO)
endif ()

if (${ENABLE_YAML})
    add_executable(yaml_with_backend yaml_with_backend.cc)
    target_link_libraries(yaml_with_backend modelTraits IO fmt::fmt)
endif ()

add_executable(core_usage core_usage.cc)
target_link_libraries(core_usage modelTraits fmt::fmt)

if(${ENABLE_SCOREC})
  find_package(SCOREC CONFIG REQUIRED)
  if(${SCOREC_FOUND})
      add_executable(pumi pumi.cc)
      target_link_libraries(pumi SCOREC::core SCOREC::apf SCOREC::gmi modelTraits IO)
  endif()
endif()
