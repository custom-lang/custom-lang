cmake_minimum_required(VERSION 3.23.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(custom-lang)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(print_all tests/print_all.cpp
                         ling/units/phoneme.cpp
                         ling/units/consonant.cpp
                         ling/units/vowel.cpp
                         ling/units/soundsystem.cpp)

add_executable(phon_rules tests/phon_rules.cpp
                          ling/units/phoneme.cpp
                          ling/units/consonant.cpp
                          ling/units/vowel.cpp
                          ling/units/soundsystem.cpp)

add_executable(sequence_tool tests/sequence_tool.cpp
                             ling/units/phoneme.cpp
                             ling/units/consonant.cpp
                             ling/units/vowel.cpp
                             ling/units/soundsystem.cpp)

target_include_directories(print_all PRIVATE include)
target_include_directories(phon_rules PRIVATE include)
target_include_directories(sequence_tool PRIVATE include)

target_link_libraries(sequence_tool PRIVATE nlohmann_json::nlohmann_json)
