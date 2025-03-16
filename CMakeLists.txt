cmake_minimum_required(VERSION 3.15)
project(SimpleGuitarAmp VERSION 1.0.0)

add_compile_definitions(JUCE_VST3_CAN_REPLACE_VST2=0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Statische Verlinkung für Standalone-Builds
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
    # Füge big object Unterstützung hinzu
    add_compile_options(-Wa,-mbig-obj)
endif()

# Add JUCE as a submodule or via FetchContent
include(FetchContent)
FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 7.0.5
)
FetchContent_MakeAvailable(JUCE)

# Initialize JUCE
juce_add_plugin(SimpleGuitarAmp
        VERSION 1.0.0
        COMPANY_NAME "Silas"
        COMPANY_WEBSITE "www.silas-scholz.de"
        COMPANY_EMAIL "info@silas-scholz.de"
        FORMATS VST3 AU Standalone
        VST_COPY_DIR ""  # VST2-Support deaktivieren
        PRODUCT_NAME "SimpleGuitarAmp")

# Add source files
target_sources(SimpleGuitarAmp
        PRIVATE
        SimpleGuitarAmp.cpp
        SimpleGuitarAmpEditor.cpp)

# Add include directories
target_include_directories(SimpleGuitarAmp
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

# Set plugin properties
juce_generate_juce_header(SimpleGuitarAmp)

# Add JUCE modules
target_link_libraries(SimpleGuitarAmp
        PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Plugin parameters
juce_add_binary_data(SimpleGuitarAmpBinaryData SOURCES
        Resources/knob.png
        Resources/background.png)

target_link_libraries(SimpleGuitarAmp
        PRIVATE
        SimpleGuitarAmpBinaryData)