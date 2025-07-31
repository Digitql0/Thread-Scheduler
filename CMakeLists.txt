cmake_minimum_required(VERSION 3.10)
project(Thread_Scheduler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

# Collect sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.c
)

# Build as a static library
add_library(Thread_Scheduler STATIC ${SOURCES})

# Build as an Executable
add_executable(Thread_SchedulerApp ${SOURCES})

# Include header for Thread_SchedulerApp
target_include_directories(Thread_SchedulerApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Thread_SchedulerApp PRIVATE Thread_Scheduler)

# Include headers for anyone linking Thread_Scheduler
target_include_directories(Thread_Scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
