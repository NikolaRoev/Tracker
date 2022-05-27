qt_add_executable(DatabaseManagerTest
	"src/DatabaseManagerTest.cpp"
)


qt_add_executable(TestTest
	"src/TestTest.cpp"
)

add_test(DatabaseManagerTest DatabaseManagerTest)
add_test(TestTest TestTest)

target_link_libraries(DatabaseManagerTest
	PRIVATE
	Qt6::Test
	Tracker
)

target_link_libraries(TestTest
	PRIVATE
	Qt6::Test
	Tracker
)
