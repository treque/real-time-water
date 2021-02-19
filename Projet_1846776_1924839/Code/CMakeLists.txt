cmake_minimum_required(VERSION 3.12)
project(TP4)

set(HEADER_FILES
    Cst.h
    FBO.h
    Gazon.h
    GrilleQuads.h
    Lumiere.h
    Materiau.h
    Modele3dAbstrait.h
    Modele3dObj.h
    noise.h
    NuanceurProg.h
    ObjParser/Geometry.h
    ObjParser/Global.h
    ObjParser/MathUtils.h
    ObjParser/ObjParser.h
    ObjParser/ParserTextAbstrait.h
    ObjParser/StringUtils.h
    ObjParser/Vecteur3.h
    Singleton.h
    Skybox.h
    SurfaceBSplinaire.h
    textfile.h
    Texture2D.h
    TextureAbstraite.h
    TextureCubemap.h
    Var.h
)

set(SOURCE_FILES
    Cst.cpp
    FBO.cpp
    Gazon.cpp
    GrilleQuads.cpp
    Lumiere.cpp
    main.cpp
    Materiau.cpp
    Modele3dAbstrait.cpp
    Modele3dObj.cpp
    noise.cpp
    NuanceurProg.cpp
    ObjParser/ObjParser.cpp
    ObjParser/ParserTextAbstrait.cpp
    ObjParser/StringUtils.cpp
    ObjParser/Vecteur3.cpp
    Skybox.cpp
    SurfaceBSplinaire.cpp
    textfile.cpp
    Texture2D.cpp
    TextureAbstraite.cpp
    TextureCubemap.cpp
    Var.cpp
)

file(GLOB SHADER_FILES
    Nuanceurs/*.glsl
)

file(GLOB TEXTURE_FILES
    Textures/*
    )

file(GLOB MODEL_FILES
    Modeles/*
    )

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES} ${SCENE_FILES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glfw)

target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Weverything)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-c++98-compat -Wno-c++98-compat-pedantic)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-padded)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-global-constructors -Wno-exit-time-destructors)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-documentation)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-source-uses-openmp)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-float-equal)
endif()

if (USE_OPENMP)
    target_compile_options(${PROJECT_NAME} PRIVATE -fopenmp)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fopenmp")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
endif()

foreach(item ${SHADER_FILES})
    get_filename_component(FILE_NAME ${item} NAME)
    set(DEST "${PROJECT_BINARY_DIR}/Nuanceurs/${FILE_NAME}")
    add_custom_command(
        OUTPUT ${DEST}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/Nuanceurs/"
        COMMAND ${CMAKE_COMMAND} -E copy ${item} ${DEST}
        DEPENDS ${item}
    )
    list(APPEND SHADER_DEST_FILES ${DEST})
endforeach()

add_custom_target(
    Nuanceurs ALL
    DEPENDS ${SHADER_DEST_FILES}
    )

foreach(item ${TEXTURE_FILES})
    get_filename_component(FILE_NAME ${item} NAME)
    set(DEST "${PROJECT_BINARY_DIR}/Textures/${FILE_NAME}")
    add_custom_command(
        OUTPUT ${DEST}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/Textures/"
        COMMAND ${CMAKE_COMMAND} -E copy ${item} ${DEST}
        DEPENDS ${item}
    )
    list(APPEND TEXTURE_DEST_FILES ${DEST})
endforeach()

add_custom_target(
    Textures ALL
    DEPENDS ${TEXTURE_DEST_FILES}
    )

foreach(item ${MODEL_FILES})
    get_filename_component(FILE_NAME ${item} NAME)
    set(DEST "${PROJECT_BINARY_DIR}/Modeles/${FILE_NAME}")
    add_custom_command(
        OUTPUT ${DEST}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/Modeles/"
        COMMAND ${CMAKE_COMMAND} -E copy ${item} ${DEST}
        DEPENDS ${item}
    )
    list(APPEND MODEL_DEST_FILES ${DEST})
endforeach()

add_custom_target(
    Modeles ALL
    DEPENDS ${MODEL_DEST_FILES}
    )
