qt_add_executable(DatabaseManagerTest
	"DatabaseManagerTest.cpp"
)


qt_add_executable(TestTest
	"TestTest.cpp"
)

add_test(DatabaseManagerTest DatabaseManagerTest)
add_test(TestTest TestTest)

target_link_libraries(DatabaseManagerTest
	PRIVATE
	Qt6::Core
	Qt6::Sql
	Qt6::Test
)

target_link_libraries(TestTest
	PRIVATE
	Qt6::Core
	Qt6::Sql
	Qt6::Test
)
