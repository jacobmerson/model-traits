
add_subdirectory(exprtk)

set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL YES)

if (${BUILD_EXTERNAL})
    include(FetchContent)
    set(FMT_INSTALL ON CACHE INTERNAL "")
    FetchContent_Declare(fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt
            GIT_TAG 2f3f3862fadf50941affad2afc7115f7355dc9c7)

    FetchContent_Declare(catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG 037ddbc75cc5e58b93cf5a010a94b32333ad824d)

    FetchContent_MakeAvailable(fmt catch2)
    if (${ENABLE_YAML})
        set(YAML_CPP_INSTALL ON CACHE INTERNAL "")
        FetchContent_Declare(yaml-cpp
                GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
                GIT_TAG 9a3624205e8774953ef18f57067b3426c1c5ada6)
        FetchContent_MakeAvailable(yaml-cpp)
    endif ()
    # pumi cmake script is not architected in a way that allows it to be added to the project using add_subdirectory
endif ()
