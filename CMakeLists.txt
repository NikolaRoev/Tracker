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


	"src/DatabaseManager/DatabaseManager.h"
	"src/DatabaseManager/DatabaseManager.cpp"
	"src/DatabaseManager/Work.h"
	"src/DatabaseManager/Creator.h"


	"src/UI/MainWindow/MainWindow.h"
	"src/UI/MainWindow/MainWindow.cpp"
	"src/UI/MainWindow/MainWindow.ui"

	"src/UI/UpdateEntry/UpdateEntry.h"
	"src/UI/UpdateEntry/UpdateEntry.cpp"
	"src/UI/UpdateEntry/UpdateEntry.ui"

	"src/UI/AddWorkDialog/AddWorkDialog.h"
	"src/UI/AddWorkDialog/AddWorkDialog.cpp"
	"src/UI/AddWorkDialog/AddWorkDialog.ui"

	"src/UI/AddCreatorDialog/AddCreatorDialog.h"
	"src/UI/AddCreatorDialog/AddCreatorDialog.cpp"
	"src/UI/AddCreatorDialog/AddCreatorDialog.ui"

	"src/UI/AttachCreatorDialog/AttachCreatorDialog.h"
	"src/UI/AttachCreatorDialog/AttachCreatorDialog.cpp"
	"src/UI/AttachCreatorDialog/AttachCreatorDialog.ui"

	"src/UI/WorkDialog/WorkDialog.h"
	"src/UI/WorkDialog/WorkDialog.cpp"
	"src/UI/WorkDialog/WorkDialog.ui"

	"src/UI/CreatorDialog/CreatorDialog.h"
	"src/UI/CreatorDialog/CreatorDialog.cpp"
	"src/UI/CreatorDialog/CreatorDialog.ui"
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
	"${CMAKE_SOURCE_DIR}/src/UI/"
)
