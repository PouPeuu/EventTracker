cmake_minimum_required(VERSION 3.20)
set(CXX_STANDARD 23)

project(EventTracker VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(eventtracker STATIC
	src/event.cpp
	src/complex_value.cpp
	src/serializer.cpp
	src/database.cpp
	src/event_builder.cpp
)

target_include_directories(eventtracker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS eventtracker
	LIBRARY DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

