qt_add_executable(DatabaseManagerTest
	"src/DatabaseManagerTest.cpp"
)

add_test(DatabaseManagerTest DatabaseManagerTest)

target_link_libraries(DatabaseManagerTest
	PRIVATE
	Qt6::Test
	Tracker
)
