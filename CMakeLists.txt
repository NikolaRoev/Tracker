cmake_minimum_required(VERSION 3.23)

project(Tracker CXX)

option(BUILD_APP "Build the Tracker Application." ON)
option(BUILD_TESTS "Build tests for Tracker." ON)
option(BUILD_BENCHMARKS "Build benchmarks for Tracker." ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6
	REQUIRED COMPONENTS
	Core
	Widgets
	Gui
	Network
	Sql
	Test
)

add_subdirectory(Tracker-lib)

if (BUILD_APP)
	add_subdirectory(Tracker-app)
endif()

if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

if (BUILD_BENCHMARKS)
	add_subdirectory(benchmarks)
endif()
