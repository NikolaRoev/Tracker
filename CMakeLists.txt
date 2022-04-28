cmake_minimum_required(VERSION 3.23)

project(Tracker CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6
	REQUIRED COMPONENTS
	Core
	Widgets
	Gui
	Sql
)

qt_add_executable(Tracker
	"resources/resources.qrc"
	"resources/resources.rc"
	"src/main.cpp"

	"src/MainWindow/MainWindow.h"
	"src/MainWindow/MainWindow.cpp"
	"src/MainWindow/MainWindow.ui"

	"src/DatabaseManager/DatabaseManager.h"
	"src/DatabaseManager/DatabaseManager.cpp"
	"src/DatabaseManager/Work.h"
	"src/DatabaseManager/Creator.h"

	"src/UpdateEntry/UpdateEntry.h"
	"src/UpdateEntry/UpdateEntry.cpp"
	"src/UpdateEntry/UpdateEntry.ui"
)

target_link_libraries(Tracker
	PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
	Qt6::Sql
)

target_include_directories(Tracker
	PRIVATE
	"${CMAKE_SOURCE_DIR}/src/"
)
