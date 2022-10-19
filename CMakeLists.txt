cmake_minimum_required(VERSION 3.10)

project(logic_parser C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(parser)

if(DEBUG)
	add_compile_definitions(DEBUG=1)
	if(MSVC)
	  target_compile_options(parser PRIVATE /W4 /WX)
	else()
	  target_compile_options(parser PRIVATE -g -fsanitize=address -Wall -Wextra -Wpedantic -std=c99)
	  target_link_options(   parser PRIVATE -fsanitize=address)
	endif()
else()
	if(MSVC)
	  target_compile_options(parser PRIVATE /W4 /WX)
	else()
	  target_compile_options(parser PRIVATE -O3 -Wall -Wextra -Wpedantic -std=c99)
	endif()
endif()


target_sources(	parser PRIVATE
				src/main.c
				src/lex.c
				src/parse.c
				src/tokens.c
				src/eval.c		)

include_directories(${CMAKE_SOURCE_DIR})
