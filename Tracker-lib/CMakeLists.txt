qt_add_library(Tracker-lib STATIC
	"${CMAKE_SOURCE_DIR}/resources/resources.qrc"

	"src/pch.h"


	"src/DatabaseManager/DatabaseManager.h"
	"src/DatabaseManager/DatabaseManager.cpp"
	"src/DatabaseManager/Work.h"
	"src/DatabaseManager/Creator.h"

	"src/RequestsManager/RequestsManager.h"
	"src/RequestsManager/RequestsManager.cpp"


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

	"src/UI/ReleasesPage/ReleasesPage.h"
	"src/UI/ReleasesPage/ReleasesPage.cpp"
	"src/UI/ReleasesPage/ReleasesPage.ui"

	"src/UI/ReleaseEntry/ReleaseEntry.h"
	"src/UI/ReleaseEntry/ReleaseEntry.cpp"
	"src/UI/ReleaseEntry/ReleaseEntry.ui"

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
)

target_link_libraries(Tracker-lib
	PUBLIC
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
	Qt6::Network
	Qt6::Sql
)

target_include_directories(Tracker-lib
	PUBLIC
	"src/"
	"src/UI/"
)

target_precompile_headers(Tracker-lib
	PUBLIC
	"src/pch.h"
)

target_compile_options(Tracker-lib
	PUBLIC
	"/W4"
)

set_target_properties(Tracker-lib
	PROPERTIES
	OUTPUT_NAME Tracker
)
