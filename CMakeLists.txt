cmake_minimum_required(VERSION 3.23)

project(Tracker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(Tracker
	"src/main.cpp"

	"src/MainWindow.h"
"src/MainWindow.cpp"
	"src/MainWindow.ui"
)

target_link_libraries(Tracker PRIVATE Qt6::Widgets)
