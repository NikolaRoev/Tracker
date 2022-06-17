qt_add_executable(Tracker-app WIN32
	"${CMAKE_SOURCE_DIR}/resources/resources.qrc"
	"${CMAKE_SOURCE_DIR}/resources/resources.rc"

	"src/main.cpp"
)

target_link_libraries(Tracker-app PRIVATE Tracker-lib)

set_target_properties(Tracker-app
	PROPERTIES
	OUTPUT_NAME Tracker
)
