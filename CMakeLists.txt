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
	Network
	Sql
)

qt_add_executable(Tracker
	"resources/resources.qrc"
	"resources/resources.rc"

	"src/main.cpp"


	"src/DatabaseManager/DatabaseManager.h"
	"src/DatabaseManager/DatabaseManager.cpp"
	"src/DatabaseManager/Work.h"
	"src/DatabaseManager/Creator.h"


	"src/UI/MainWindow/MainWindow.h"
	"src/UI/MainWindow/MainWindow.cpp"
	"src/UI/MainWindow/MainWindow.ui"

	"src/UI/UpdatePage/UpdatePage.h"
	"src/UI/UpdatePage/UpdatePage.cpp"
	"src/UI/UpdatePage/UpdatePage.ui"

	"src/UI/UpdateEntry/UpdateEntry.h"
	"src/UI/UpdateEntry/UpdateEntry.cpp"
	"src/UI/UpdateEntry/UpdateEntry.ui"

	"src/UI/BrowsePage/BrowsePage.h"
	"src/UI/BrowsePage/BrowsePage.cpp"
	"src/UI/BrowsePage/BrowsePage.ui"

	"src/UI/AddWorkDialog/AddWorkDialog.h"
	"src/UI/AddWorkDialog/AddWorkDialog.cpp"
	"src/UI/AddWorkDialog/AddWorkDialog.ui"

	"src/UI/AddCreatorDialog/AddCreatorDialog.h"
	"src/UI/AddCreatorDialog/AddCreatorDialog.cpp"
	"src/UI/AddCreatorDialog/AddCreatorDialog.ui"

	"src/UI/WorkPage/WorkPage.h"
	"src/UI/WorkPage/WorkPage.cpp"
	"src/UI/WorkPage/WorkPage.ui"

	"src/UI/CreatorPage/CreatorPage.h"
	"src/UI/CreatorPage/CreatorPage.cpp"
	"src/UI/CreatorPage/CreatorPage.ui"

	"src/UI/SearchCreatorDialog/SearchCreatorDialog.h"
	"src/UI/SearchCreatorDialog/SearchCreatorDialog.cpp"
	"src/UI/SearchCreatorDialog/SearchCreatorDialog.ui"


	"src/RequestsManager/RequestsManager.h"
	"src/RequestsManager/RequestsManager.cpp"
)

target_link_libraries(Tracker
	PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
	Qt6::Network
	Qt6::Sql
)

target_include_directories(Tracker
	PRIVATE
	"${CMAKE_SOURCE_DIR}/src/"
	"${CMAKE_SOURCE_DIR}/src/UI/"
)
