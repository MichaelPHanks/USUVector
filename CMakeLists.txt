cmake_minimum_required(VERSION 3.12)
project(USUVector)
set(UNIT_TEST_RUNNER UnitTestRunner)

set (HEADER_FILES vector.hpp)

set (UNIT_TEST_FILES TestVector.cpp)

set(SOURCE_FILES )
add_executable(USUVector ${SOURCE_FILES} ${HEADER_FILES} main.cpp)
add_executable(${UNIT_TEST_RUNNER} ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES})
set_property(TARGET USUVector PROPERTY CXX_STANDARD 20)
set_property(TARGET ${UNIT_TEST_RUNNER} PROPERTY CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(USUVector PRIVATE /W4 /permissive-)
	target_compile_options(${UNIT_TEST_RUNNER} PRIVATE /W4 /permissive-)


elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(USUVector PRIVATE -Wall -Wextra -pedantic )
	target_compile_options(${UNIT_TEST_RUNNER} PRIVATE -Wall -Wextra -pedantic )

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(USUVector PRIVATE -Wall -Wextra -pedantic)
	target_compile_options(${UNIT_TEST_RUNNER} PRIVATE -Wall -Wextra -pedantic )

endif()


find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
	message("Found clang-format at: " ${CLANG_FORMAT})
	unset(SOURCE_FILES_PATHS)
	foreach(SOURCE_FILE ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES} main.cpp)
		get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
		message("The file " ${SOURCE_FILE} " full pathname is: " ${WHERE})
		set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
	endforeach()
	add_custom_target(
	ClangFormat
	COMMAND ${CLANG_FORMAT}
	-i
	-style=file
	${SOURCE_FILES_PATHS})
	add_dependencies(USUVector ClangFormat)

else()
	message("Didn't find clang-format." ${CLANG_FORMAT})
endif()




include(FetchContent)

FetchContent_Declare (
	googletest
	GIT_REPOSITORY	https://github.com/google/googletest.git
	GIT_TAG		v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${UNIT_TEST_RUNNER} gtest_main)



