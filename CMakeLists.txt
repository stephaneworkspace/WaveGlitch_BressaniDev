cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(JUCE_MODAL_LOOPS_PERMITTED=1)

# Read each line in the .env file
file(STRINGS .env ENV_LINES)

# Process each line
foreach(LINE ${ENV_LINES})
    # Split the line into NAME and VALUE
    string(REPLACE "=" ";" LINE_LIST ${LINE})
    list(GET LINE_LIST 0 NAME)
    list(GET LINE_LIST 1 VALUE)

    # Process each name
    if("${NAME}" STREQUAL "APP_NAME")
        set(APP_NAME ${VALUE})
    elseif("${NAME}" STREQUAL "APP_VERSION")
        set(APP_VERSION ${VALUE})
    elseif("${NAME}" STREQUAL "APP_BUNDLE")
        set(APP_BUNDLE ${VALUE})
    endif()
endforeach()

message(STATUS "APP_NAME: ${APP_NAME}")
message(STATUS "APP_VERSION: ${APP_VERSION}")
message(STATUS "APP_BUNDLE: ${APP_BUNDLE}")

set(ENV_FILE_PATH "${CMAKE_SOURCE_DIR}/.env")

if (UNIX AND NOT APPLE)
    set(CMAKE_C_COMPILER "/usr/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
    add_compile_definitions(JUCE_USE_XRANDR=0)
    add_definitions(-DJUCE_ENABLE_WIN32_FILETYPE_ASSOCIATION=1)
endif()

project(WaveGlitch_BressaniDev VERSION 0.0.1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB_RECURSE ASSETS_FILES RELATIVE ${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/Assets/*")
foreach(ASSETS_FILE ${ASSETS_FILES})
    configure_file(${ASSETS_FILE} ${ASSETS_FILE} COPYONLY)
endforeach()

include(FetchContent)

FetchContent_Declare(
    libsndfile
    URL https://github.com/libsndfile/libsndfile/archive/refs/tags/1.2.2.tar.gz
)

FetchContent_Declare(
    rtmidi
    URL https://github.com/thestk/rtmidi/archive/refs/tags/6.0.0.tar.gz
)

FetchContent_Declare(
    juce
    URL https://github.com/juce-framework/JUCE/archive/refs/tags/7.0.6.tar.gz
)

FetchContent_MakeAvailable(libsndfile rtmidi juce)

# Si les bibliothèques déclarent des cibles, vous pouvez les utiliser dans target_link_libraries
#add_executable(WaveGlitch_BressaniDev main.cpp WAVPlayer.cpp WAVPlayer.h)

if (APPLE)
    # Add these to your plist
    set(MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${APP_VERSION}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${APP_VERSION}")
    set(MACOSX_BUNDLE_ICON_FILE MyIcon.icns)
    set(APP_ICON_MACOSX ${CMAKE_SOURCE_DIR}/icon/MyIcon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set(MY_ICON_NAME "MyIcon")
    set(MY_APP_NAME "${APP_NAME}")
    set(MY_APP_VERSION "${APP_VERSION}")
    set(MY_APP_BUNDLE "${APP_BUNDLE}")
    configure_file(${CMAKE_SOURCE_DIR}/Info.plist.in ${CMAKE_BINARY_DIR}/Info.plist)
    juce_add_gui_app(WaveGlitch_BressaniDev
            PRODUCT_NAME "WaveGlitch"
            COMPANY_NAME "Stéphane Bressani - bressani.dev"
            BUNDLE_ID "bressani.dev.waveglitch"
            ICON_BIG ${CMAKE_SOURCE_DIR}/icon/MyIcon.icns
            INFOPLIST_FILE ${CMAKE_BINARY_DIR}/Info.plist
            # Other options...
    )
    set_target_properties(WaveGlitch_BressaniDev PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/Info.plist)
    target_sources(WaveGlitch_BressaniDev PRIVATE
            .env
            Source/MyObjCWrapper.h
            Source/MyObjCWrapper.mm
            Source/Misc.h
            Source/Misc.cpp
            Source/CustomLookAndFeel.h
            Source/CustomLookAndFeel.cpp
            Source/MainComponent.h
            Source/MainComponent.cpp
            Source/Main.cpp
            Source/WAVPlayer.h
            Source/WAVPlayer.cpp
            Source/AudioFileProperties.cpp
            Source/AudioFileProperties.h
            ${APP_ICON_MACOSX}
    )
else ()
    juce_add_gui_app(WaveGlitch_BressaniDev
            PRODUCT_NAME "WaveGlitch"
            COMPANY_NAME "Stéphane Bressani - bressani.dev"
            BUNDLE_ID "bressani.dev.waveglitch"
            ICON_BIG ${CMAKE_SOURCE_DIR}/icon/MyIcon.icns
            # Other options...
    )
    target_sources(WaveGlitch_BressaniDev PRIVATE
            .env
            Source/Misc.h
            Source/Misc.cpp
            Source/CustomLookAndFeel.h
            Source/CustomLookAndFeel.cpp
            Source/MainComponent.h
            Source/MainComponent.cpp
            Source/Main.cpp
            Source/WAVPlayer.h
            Source/WAVPlayer.cpp
            Source/AudioFileProperties.cpp
            Source/AudioFileProperties.h
    )
endif ()

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
endif ()

if (APPLE)
    target_link_libraries(WaveGlitch_BressaniDev PRIVATE
        sndfile
        rtmidi
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_gui_basics
        juce::juce_gui_extra
        ${COCOA_LIBRARY}
    )
endif ()
add_custom_command(TARGET WaveGlitch_BressaniDev POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets/ $<TARGET_FILE_DIR:WaveGlitch_BressaniDev>/Assets/)
add_custom_command(TARGET WaveGlitch_BressaniDev POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${ENV_FILE_PATH}
        $<TARGET_FILE_DIR:WaveGlitch_BressaniDev>)
