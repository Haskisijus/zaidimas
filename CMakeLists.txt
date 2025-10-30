cmake_minimum_required(VERSION 3.20)
project(zaidimas_nd)

set(CMAKE_CXX_STANDARD 17)

# ------------------------------------------------------
# SFML KONFIGŪRACIJA
# Automatiškai parenka tinkamą SFML aplanką (SFML_aaa arba SFML)
# ------------------------------------------------------
if (EXISTS "${CMAKE_SOURCE_DIR}/SFML_aaa")
    set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML_aaa")
else()
    set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML")
endif()

set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIB_DIR "${SFML_ROOT}/lib")

include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIB_DIR})

# ------------------------------------------------------
# STATINIS SFML NAUDOJIMAS (tas pats, kaip VS projekte)
# ------------------------------------------------------
add_definitions(-DSFML_STATIC)

# ------------------------------------------------------
# PROJEKTO ŠALTINIAI
# ------------------------------------------------------
add_executable(zaidimas_nd
        src/main.cpp
        src/utils.cpp
)

# ------------------------------------------------------
# LINKINAMOS BIBLIOTEKOS (pagal tavo .vcxproj)
# ------------------------------------------------------
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Linkinamos DEBUG bibliotekos")

    target_link_libraries(zaidimas_nd
            sfml-graphics-s-d
            sfml-window-s-d
            sfml-system-s-d
            sfml-audio-s-d
            sfml-main-d
            openal32
            opengl32
            winmm
            gdi32
            freetype
            flac
            vorbisenc
            vorbisfile
            vorbis
            ogg
            kernel32
            user32
            winspool
            comdlg32
            advapi32
            shell32
            ole32
            oleaut32
            uuid
            odbc32
            odbccp32
    )
else()
    message(STATUS "Linkinamos RELEASE bibliotekos")

    target_link_libraries(zaidimas_nd
            sfml-graphics-s
            sfml-window-s
            sfml-system-s
            sfml-audio-s
            sfml-main
            openal32
            opengl32
            winmm
            gdi32
            freetype
            flac
            vorbisenc
            vorbisfile
            vorbis
            ogg
            kernel32
            user32
            winspool
            comdlg32
            advapi32
            shell32
            ole32
            oleaut32
            uuid
            odbc32
            odbccp32
    )
endif()

# ------------------------------------------------------
# AUTOMATIŠKAS RESURSŲ KOPIJAVIMAS
# (kad veiktų background.png, doodle*.png, fontai ir muzika)
# ------------------------------------------------------
add_custom_command(TARGET zaidimas_nd POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:zaidimas_nd>/resources"
)

# ------------------------------------------------------
# INFORMACIJA KONSOLĖJE
# ------------------------------------------------------
message(STATUS "SFML_ROOT     = ${SFML_ROOT}")
message(STATUS "SFML_INCLUDE  = ${SFML_INCLUDE_DIR}")
message(STATUS "SFML_LIB_DIR  = ${SFML_LIB_DIR}")
message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")