
if (ENABLE_YAML)
    find_package(yaml-cpp REQUIRED)
    set(SOURCES ${SOURCES} Yaml.cc YamlConvert.cc)
endif ()

if (ENABLE_SIMMETRIX)
    find_package(SIMMODSUITE MODULE REQUIRED)
    set(SOURCES ${SOURCES} Simmetrix.cc SimmetrixConvert.cc)
endif ()

find_package(fmt REQUIRED)
add_library(IO ${SOURCES})
target_link_libraries(IO PUBLIC modelTraits PRIVATE fmt::fmt)

if (ENABLE_YAML)
    target_link_libraries(IO PRIVATE yaml-cpp)
    target_compile_definitions(IO PUBLIC -DMT_HAVE_YAML)
endif ()
if (ENABLE_SIMMETRIX)
    target_link_libraries(IO PRIVATE ${SIMMODSUITE_LIBS})
    target_include_directories(IO PRIVATE ${SIMMODSUITE_INCLUDE_DIR})
    target_compile_definitions(IO PUBLIC -DMT_HAVE_SIMMETRIX)
endif ()
