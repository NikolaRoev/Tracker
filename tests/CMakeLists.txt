#Libraries linked to each test executable.
set(LIBS Qt6::Test Tracker-lib)

#DatabaseManager test.
qt_add_executable(DatabaseManagerTest "src/DatabaseManagerTest.cpp")
add_test(DatabaseManagerTest DatabaseManagerTest)
target_link_libraries(DatabaseManagerTest PRIVATE ${LIBS})
