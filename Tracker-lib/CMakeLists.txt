qt_add_library(Tracker-lib STATIC
	"src/pch.h"

	"src/Core/Core.h"
	"src/Core/Core.cpp"

	"src/DatabaseManager/DatabaseManager.h"
	"src/DatabaseManager/DatabaseManager.cpp"
	"src/DatabaseManager/Work.h"
	"src/DatabaseManager/Creator.h"
	"src/DatabaseManager/AttachedCreator.h"

	"src/UI/MainWindow/MainWindow.h"
	"src/UI/MainWindow/MainWindow.cpp"
	"src/UI/MainWindow/MainWindow.ui"

	"src/UI/Page/Page.h"
	"src/UI/Page/UpdatePage.h"
	"src/UI/Page/UpdatePage.cpp"
	"src/UI/Page/UpdatePage.ui"
	"src/UI/Page/UpdateEntry.h"
	"src/UI/Page/UpdateEntry.cpp"
	"src/UI/Page/UpdateEntry.ui"
	"src/UI/Page/BrowsePage.h"
	"src/UI/Page/BrowsePage.cpp"
	"src/UI/Page/BrowsePage.ui"
	"src/UI/Page/WorkPage.h"
	"src/UI/Page/WorkPage.cpp"
	"src/UI/Page/WorkPage.ui"
	"src/UI/Page/CreatorPage.h"
	"src/UI/Page/CreatorPage.cpp"
	"src/UI/Page/CreatorPage.ui"

	"src/UI/Dialog/AddWorkDialog.h"
	"src/UI/Dialog/AddWorkDialog.cpp"
	"src/UI/Dialog/AddWorkDialog.ui"
	"src/UI/Dialog/AddCreatorDialog.h"
	"src/UI/Dialog/AddCreatorDialog.cpp"
	"src/UI/Dialog/AddCreatorDialog.ui"
	"src/UI/Dialog/SearchCreatorDialog.h"
	"src/UI/Dialog/SearchCreatorDialog.cpp"
	"src/UI/Dialog/SearchCreatorDialog.ui"

	"src/Utility/Utility.h"
	"src/Utility/Utility.cpp"
)

target_link_libraries(Tracker-lib
	PUBLIC
	Qt6::Core
	Qt6::Widgets
	Qt6::Gui
	Qt6::Sql
)

target_include_directories(Tracker-lib
	PUBLIC
	"src/"
	"src/UI/MainWindow/"
	"src/UI/Page/"
	"src/UI/Dialog/"
	"src/Utility/"

	INTERFACE
	"src/Core/"
)

target_precompile_headers(Tracker-lib
	PRIVATE
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
