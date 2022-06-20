#Libraries linked to each benchmark executable.
set(LIBS Qt6::Test Tracker-lib)

#DatabaseManager benchmark.
qt_add_executable(DatabaseManagerBenchmark "src/DatabaseManagerBenchmark.cpp")
target_link_libraries(DatabaseManagerBenchmark PRIVATE ${LIBS})
