cmake_minimum_required(VERSION 3.23)

project(Tracker CXX)

option(BUILD_TESTS "Build tests for Tracker." ON)

set(CMAKE_CXX_STANDARD 23)
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

add_subdirectory(Tracker)
add_subdirectory(Application)

if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
