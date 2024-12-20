cmake_minimum_required(VERSION 3.28)
project(TSP_3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

add_executable(TSP_3 Source/Class/main.cpp
        Source/Libraries/ini.cpp
        Source/Libraries/ini.h
        Source/Libraries/INIReader.cpp
        Source/Libraries/INIReader.h
        Source/Headers/Structures.h
        Source/Headers/TimeCounter.h
        Source/Class/Run.cpp
        Source/Headers/Run.h
        Source/Class/NearestNeighbour.cpp
        Source/Headers/NearestNeighbour.h
        Source/Class/TabuSearch.cpp
        Source/Headers/TabuSearch.h
        Source/Class/SimulatedAnnealing.cpp
        Source/Headers/SimulatedAnnealing.h)
