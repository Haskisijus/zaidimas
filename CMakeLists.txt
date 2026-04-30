cmake_minimum_required(VERSION 3.20)
project(zaidimas_nd)

set(CMAKE_CXX_STANDARD 17)

# ------------------------------------------------------
# PROJEKTO ŠALTINIAI
# ------------------------------------------------------
add_executable(zaidimas_nd
        src/main.cpp
        src/utils.cpp
        src/utils.h
        src/defines.h
        src/objects.h
        src/button.h
        src/button.cpp
        src/asset_manager.h
        src/asset_manager.cpp
        src/graphics_utils.h
        src/graphics_utils.cpp
        src/leaderboard_manager.h
        src/leaderboard_manager.cpp
        src/leaderboard_manager.cpp
)

# ------------------------------------------------------
# KONFIGŪRACIJA PAGAL OPERACINĘ SISTEMĄ
# ------------------------------------------------------

if(WIN32)
    # --- WINDOWS KONFIGŪRACIJA (Tavo originali logika) ---
    if (EXISTS "${CMAKE_SOURCE_DIR}/SFML_aaa")
        set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML_aaa")
    else()
        set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML")
    endif()

    set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
    set(SFML_LIB_DIR "${SFML_ROOT}/lib")
    include_directories(${SFML_INCLUDE_DIR})
    link_directories(${SFML_LIB_DIR})

    add_definitions(-DSFML_STATIC)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries(zaidimas_nd
                sfml-graphics-s-d sfml-window-s-d sfml-system-s-d sfml-audio-s-d sfml-main-d
                openal32 opengl32 winmm gdi32 freetype flac vorbisenc vorbisfile vorbis ogg
                kernel32 user32 winspool comdlg32 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32
        )
    else()
        target_link_libraries(zaidimas_nd
                sfml-graphics-s sfml-window-s sfml-system-s sfml-audio-s sfml-main
                openal32 opengl32 winmm gdi32 freetype flac vorbisenc vorbisfile vorbis ogg
                kernel32 user32 winspool comdlg32 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32
        )
    endif()

else()
    # --- LINUX KONFIGŪRACIJA ---
    # Naudojame standartinį find_package, nes Linux sistemoje SFML bibliotekos
    # dažniausiai būna įdiegtos sistemoje (/usr/lib)
    find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

    if(SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
        target_link_libraries(zaidimas_nd sfml-graphics sfml-window sfml-system sfml-audio)
    endif()
endif()

# ------------------------------------------------------
# AUTOMATIŠKAS RESURSŲ KOPIJAVIMAS
# ------------------------------------------------------
add_custom_command(TARGET zaidimas_nd POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:zaidimas_nd>/resources"
)