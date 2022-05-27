qt_add_executable(Application
	"${CMAKE_SOURCE_DIR}/resources/resources.qrc"
	"${CMAKE_SOURCE_DIR}/resources/resources.rc"

	"src/main.cpp"
)

target_link_libraries(Application PRIVATE Tracker)

set_target_properties(Application PROPERTIES OUTPUT_NAME Tracker)
