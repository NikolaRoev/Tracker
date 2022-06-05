qt_add_executable(DatabaseManagerBenchmark
	"src/DatabaseManagerBenchmark.cpp"
)

target_link_libraries(DatabaseManagerBenchmark
	PRIVATE
	Qt6::Test
	Tracker-lib
)
