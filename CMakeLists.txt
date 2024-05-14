cmake_minimum_required(VERSION 3.28)
project(ds_chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(ds_chess ds_chess/main.cpp
        ds_chess/evaluation.cpp
        ds_chess/evaluation.h
        ds_chess/board.cpp
        ds_chess/board.h
        ds_chess/move.cpp
        ds_chess/move.h
        ds_chess/move_generator.cpp
        ds_chess/move_generator.h
        ds_chess/search.cpp
        ds_chess/search.h
        ds_chess/string_handling.cpp
        ds_chess/string_handling.h
        ds_chess/uci.cpp
        ds_chess/uci.h
)
