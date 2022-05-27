qt_add_executable(Tracker
	"../resources/resources.qrc"
	"../resources/resources.rc"

	"main.cpp"
	"pch.h"


	"DatabaseManager/DatabaseManager.h"
	"DatabaseManager/DatabaseManager.cpp"
	"DatabaseManager/Work.h"
	"DatabaseManager/Creator.h"


	"UI/MainWindow/MainWindow.h"
	"UI/MainWindow/MainWindow.cpp"
	"UI/MainWindow/MainWindow.ui"

	"UI/UpdatePage/UpdatePage.h"
	"UI/UpdatePage/UpdatePage.cpp"
	"UI/UpdatePage/UpdatePage.ui"

	"UI/UpdateEntry/UpdateEntry.h"
	"UI/UpdateEntry/UpdateEntry.cpp"
	"UI/UpdateEntry/UpdateEntry.ui"

	"UI/BrowsePage/BrowsePage.h"
	"UI/BrowsePage/BrowsePage.cpp"
	"UI/BrowsePage/BrowsePage.ui"

	"UI/MangaDexPage/MangaDexPage.h"
	"UI/MangaDexPage/MangaDexPage.cpp"
	"UI/MangaDexPage/MangaDexPage.ui"

	"UI/MangaUpdatesPage/MangaUpdatesPage.h"
	"UI/MangaUpdatesPage/MangaUpdatesPage.cpp"
	"UI/MangaUpdatesPage/MangaUpdatesPage.ui"

	"UI/AddWorkDialog/AddWorkDialog.h"
	"UI/AddWorkDialog/AddWorkDialog.cpp"
	"UI/AddWorkDialog/AddWorkDialog.ui"

	"UI/AddCreatorDialog/AddCreatorDialog.h"
	"UI/AddCreatorDialog/AddCreatorDialog.cpp"
	"UI/AddCreatorDialog/AddCreatorDialog.ui"

	"UI/WorkPage/WorkPage.h"
	"UI/WorkPage/WorkPage.cpp"
	"UI/WorkPage/WorkPage.ui"

	"UI/CreatorPage/CreatorPage.h"
	"UI/CreatorPage/CreatorPage.cpp"
	"UI/CreatorPage/CreatorPage.ui"

	"UI/SearchCreatorDialog/SearchCreatorDialog.h"
	"UI/SearchCreatorDialog/SearchCreatorDialog.cpp"
	"UI/SearchCreatorDialog/SearchCreatorDialog.ui"
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
	"/"
	"UI/"
)

target_precompile_headers(Tracker
	PRIVATE
	"pch.h"
)

target_compile_options(Tracker
	PRIVATE
	"/W4"
)
