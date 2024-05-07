cmake_minimum_required(VERSION 3.24)
project(ds_chess)

# Our Project
set(SOURCE_FILES ds_chess/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

link_directories(ds_chess lib)
target_include_directories(ds_chess PUBLIC include)
target_link_libraries(ds_chess SMFL)
