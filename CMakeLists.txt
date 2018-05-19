cmake_minimum_required (VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

project(StormLibCxx CXX)

include(build_flags)
include(conan)

# Work around vs 15.7 bug https://github.com/Microsoft/VSLinux/issues/288
# The workaround is to define at least one preprocessor symbol in your CMakeLists.txt file
add_definitions(-DFOO)

option(STORMLIBCXX_BUILD_TESTS "Build test programs" OFF)
option(STORMLIBCXX_BUILD_EXAMPLES "Build test programs" OFF)
option(STORMLIBCXX_STATIC_RUNTIMES "Link stl statically" OFF)

if( ${STORMLIBCXX_STATIC_RUNTIMES} )
	bf_set_runtime(STATIC)
endif()

# Note: conan should be run after setting runtime
# Note: Using conanfile.txt is less configurable
set(CONAN_ARGS REQUIRES StormLib/9.22@yarikth/testing
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing
				OPTIONS StormLib:shared=False
				IMPORTS "bin, *.dll -> ./bin" # Copies all dll files from packages bin folder to my "bin" folder
				IMPORTS "lib, *.dylib* -> ./bin" # Copies all dylib files from packages lib folder to my "bin" folder
				IMPORTS "lib, *.so* -> ./bin" # Copies all dylib files from packages lib folder to my "bin" folder
				)

if( STORMLIBCXX_BUILD_TESTS )
	list(APPEND CONAN_ARGS 
				REQUIRES gtest/1.8.0@bincrafters/stable
				OPTIONS gtest:shared=False
				OPTIONS gtest:fPIC=False
				OPTIONS gtest:build_gmock=False
				)
    enable_testing()
endif()

conan_cmake_run(${CONAN_ARGS})

#############################################
# StormLibCxx staff
#############################################
add_library(${PROJECT_NAME} STATIC src/StormLibCxx.cpp src/StormLibCxxImpl.cpp)

target_link_libraries(${PROJECT_NAME}
	CONAN_PKG::StormLib)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS ${PROJECT_NAME} 
	RUNTIME DESTINATION bin 
	LIBRARY DESTINATION lib 
	ARCHIVE DESTINATION lib 
	FRAMEWORK DESTINATION /Library/Frameworks
	PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
	INCLUDES DESTINATION include/${PROJECT_NAME})

#############################################
# Unit tests
#############################################
if( STORMLIBCXX_BUILD_TESTS )
	add_executable(${PROJECT_NAME}Tests tests/tests.cpp)

	target_link_libraries(${PROJECT_NAME}Tests
		${PROJECT_NAME}
		CONAN_PKG::gtest
	)

	install(TARGETS ${PROJECT_NAME}Tests
			DESTINATION ".")

	add_test(Test${PROJECT_NAME} ${PROJECT_NAME}Tests)
endif()