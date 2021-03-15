
if (ENABLE_YAML)
    find_package(yaml-cpp REQUIRED)
    set(SOURCES ${SOURCES} Yaml.cc YamlConvert.cc)
endif ()

if (ENABLE_SIMMETRIX)
    find_package(SIMMODSUITE MODULE REQUIRED)
    set(SOURCES ${SOURCES} Simmetrix.cc SimmetrixConvert.cc)
endif ()

add_library(bcIOBackend ${SOURCES})
target_link_libraries(bcIOBackend PUBLIC modelTraits)

if (ENABLE_YAML)
    target_link_libraries(bcIOBackend PRIVATE yaml-cpp)
    target_compile_definitions(bcIOBackend PUBLIC -DMT_HAVE_YAML)
endif ()
if (ENABLE_SIMMETRIX)
    target_link_libraries(bcIOBackend PRIVATE ${SIMMODSUITE_LIBS})
    target_include_directories(bcIOBackend PRIVATE ${SIMMODSUITE_INCLUDE_DIR})
    target_compile_definitions(bcIOBackend PUBLIC -DMT_HAVE_SIMMETRIX)
endif ()
