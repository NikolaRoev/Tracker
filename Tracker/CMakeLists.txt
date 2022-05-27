qt_add_library(Tracker STATIC
	"${CMAKE_SOURCE_DIR}/resources/resources.qrc"

	"src/pch.h"


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

	"src/UI/MangaDexPage/MangaDexPage.h"
	"src/UI/MangaDexPage/MangaDexPage.cpp"
	"src/UI/MangaDexPage/MangaDexPage.ui"

	"src/UI/MangaUpdatesPage/MangaUpdatesPage.h"
	"src/UI/MangaUpdatesPage/MangaUpdatesPage.cpp"
	"src/UI/MangaUpdatesPage/MangaUpdatesPage.ui"

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

target_link_libraries(Tracker
	PUBLIC
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
	Qt6::Network
	Qt6::Sql
)

target_include_directories(Tracker
	PUBLIC
	"src/"
	"src/UI/"
)

target_precompile_headers(Tracker
	PUBLIC
	"src/pch.h"
)

target_compile_options(Tracker
	PUBLIC
	"/W4"
)
