cmake_minimum_required(VERSION 3.10)
project(ContactsManager C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Find GTK3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Include directories
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

# Executable
add_executable(contacts_app projet_gui.c)

# Link libraries
target_link_libraries(contacts_app ${GTK3_LIBRARIES})

# Compiler flags
target_compile_options(contacts_app PRIVATE ${GTK3_CFLAGS_OTHER})

# Installation
install(TARGETS contacts_app DESTINATION bin)
