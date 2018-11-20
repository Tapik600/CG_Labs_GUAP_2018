cmake_minimum_required(VERSION 3.12)
project(CG_Labs_GUAP_2018)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glxw)

include_directories(lib/glxw/include)
include_directories(lib/glfw/include)
include_directories(lib/glm)

set(LABS
        Lab1_OGLStart
        Lab2_lighting
        Lab3_moreLight
        Lab4_DepthBuffer
        )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -Wall -Wextra -Werror -Wno-deprecated-register -Wno-shift-op-parentheses")
set(LIBRARIES glfw glxw ${GLFW_LIBRARIES} ${GLXW_LIBRARY} ${OPENGL_LIBRARY})


foreach(LAB ${LABS})

    file(GLOB SOURCE
            "src/${LAB}/*.h"
            "src/${LAB}/*.cpp"
            "src/${LAB}/*.glsl"
            )
    set(NAME "${LAB}")
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBRARIES})
    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${LAB}")
    elseif(UNIX AND NOT APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/${LAB}")
    elseif(APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/${LAB}")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin/${LAB}")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin/${LAB}")
    endif(WIN32)
    # copy shader files to build directory
    file(GLOB SHADERS
            "src/${LAB}/*.vs"
            "src/${LAB}/*.fs"
            "src/${LAB}/*.gs"
            )
    foreach(SHADER ${SHADERS})
        if(WIN32)
            # configure_file(${SHADER} "test")
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
        elseif(UNIX AND NOT APPLE)
            file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/${LAB})
        elseif(APPLE)
            # create symbolic link for *.vs *.fs *.gs
            get_filename_component(SHADERNAME ${SHADER} NAME)
            makeLink(${SHADER} ${CMAKE_CURRENT_BINARY_DIR}/bin/${LAB}/${SHADERNAME} ${NAME})
        endif(WIN32)
    endforeach(SHADER)

endforeach(LAB)

include_directories(${CMAKE_SOURCE_DIR}/includes)