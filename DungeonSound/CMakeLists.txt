cmake_minimum_required(VERSION 3.5)

project(DungeonSound VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

#[[
# ffmpeg
set(FFMPEG_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../ffmpeg-6.0)
if (NOT EXISTS ${FFMPEG_ROOT_DIRECTORY} OR NOT IS_DIRECTORY ${FFMPEG_ROOT_DIRECTORY})
    message(FATAL_ERROR "Could not find ${FFMPEG_ROOT_DIRECTORY}")
endif()

find_path(AVFORMAT_INCLUDE_DIR REQUIRED
    NAMES "avformat.h"
    HINTS ${FFMPEG_ROOT_DIRECTORY}
    PATH_SUFFIXES "libavformat"
)
find_path(AVCODEC_INCLUDE_DIR REQUIRED
    NAMES "avcodec.h"
    HINTS ${FFMPEG_ROOT_DIRECTORY}
    PATH_SUFFIXES "libavcodec"
)
find_path(AVUTIL_INCLUDE_DIR REQUIRED
    NAMES "avutil.h"
    HINTS ${FFMPEG_ROOT_DIRECTORY}/libavutil
    PATH_SUFFIXES "libavutil"
)
find_path(AVDEVICE_INCLUDE_DIR REQUIRED
    NAMES "avdevice.h"
    HINTS ${FFMPEG_ROOT_DIRECTORY}
    PATH_SUFFIXES "libavdevice"
)
find_library(AVFORMAT_LIBRARY REQUIRED
    NAMES "avformat"
    HINTS ${FFMPEG_ROOT_DIRECTORY}/libavformat
    NO_DEFAULT_PATH
)
find_library(AVCODEC_LIBRARY REQUIRED
    NAMES "avcodec"
    HINTS ${FFMPEG_ROOT_DIRECTORY}
    PATH_SUFFIXES "libavcodec"
)
find_library(AVUTIL_LIBRARY REQUIRED
    NAMES "avutil"
    HINTS ${FFMPEG_ROOT_DIRECTORY}
    PATH_SUFFIXES "libavutil"
)
find_library(AVDEVICE_LIBRARY REQUIRED
    NAMES "avdevice"
    HINTS ${FFMPEG_ROOT_DIRECTORY}
    PATH_SUFFIXES "libavdevice"
)
]]

# id3v2lib

set(id3v2lib_DIR
    "C:/Program Files (x86)/id3v2lib"
)
set(id3v2lib_library_file
    ${id3v2lib_DIR}/lib/libid3v2lib.a
)
set(id3v2lib_inc_DIR
    ${id3v2lib_DIR}/include/id3v2lib-2.0
)
add_library(id3v2lib STATIC IMPORTED)
set_target_properties(id3v2lib PROPERTIES
    IMPORTED_LOCATION ${id3v2lib_library_file}
    INTERFACE_INCLUDE_DIRECTORIES ${id3v2lib_inc_DIR}
)

# testing subdirectory
add_subdirectory(tests)

# project library

set(PROJECT_SOURCES
        playbackmainwindow.cpp
        playbackmainwindow.h
        playbackmainwindow.ui
        nonstandard_icons.qrc
        soundsample.h
        soundsample.cpp
        playlist.h
        playlist.cpp
        showfile.h
        showfile.cpp
        savable.h
)

qt_add_library(DungeonSoundLib
    SHARED
    ${PROJECT_SOURCES}
)
target_link_libraries(DungeonSoundLib
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia
    id3v2lib
)
target_include_directories(DungeonSoundLib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

# Project binary

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DungeonSound
        MANUAL_FINALIZATION
        main.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DungeonSound APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(DungeonSound SHARED
            main.cpp
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(DungeonSound
            main.cpp
        )
    endif()
endif()

include("C:/Users/taylo/AppData/Local/Android/Sdk/android_openssl/android_openssl.cmake")

target_link_libraries(DungeonSound
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia
    DungeonSoundLib
)

set_target_properties(DungeonSound PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS DungeonSound
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DungeonSound)
endif()
