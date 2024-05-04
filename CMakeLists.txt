cmake_minimum_required(VERSION 3.10)

project(bool_tool C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(booltool)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message("Compiling in debug mode")
	add_compile_definitions(DEBUG=1)
	if(MSVC)
	  target_compile_options(booltool PRIVATE /W4 /WX)
	else()
	  target_compile_options(booltool PRIVATE -g -fsanitize=address -Wall -Wextra -Wpedantic -std=c99)
	  target_link_options(   booltool PRIVATE -fsanitize=address)
	endif()
else()
	if(MSVC)
	  target_compile_options(booltool PRIVATE /W4 /WX)
	else()
	  target_compile_options(booltool PRIVATE -O3 -Wall -Wextra -Wpedantic -std=c99)
	endif()
endif()


target_sources(	booltool PRIVATE
				src/main.c
				src/lex.c
				src/parse.c
				src/tokens.c
				src/formula.c		)

include_directories(${CMAKE_SOURCE_DIR})
