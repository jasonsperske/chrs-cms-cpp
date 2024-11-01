cmake_minimum_required(VERSION 3.14)
project(chrs-cms-cpp CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compile flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -Wall -Wextra)
endif()

# Find required packages
find_package(Drogon REQUIRED)
find_package(OpenCV REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(jsoncpp REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/controllers/EntryController.cpp
    src/controllers/VisionController.cpp
    src/models/Entry.cpp
    src/models/Library.cpp
)

# Header files
set(HEADERS
    src/controllers/EntryController.h
    src/controllers/VisionController.h
    src/models/Entry.h
    src/models/Library.h
    src/models/Section.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OpenCV_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Drogon::Drogon
    ${OpenCV_LIBS}
    SQLite::SQLite3
    jsoncpp_lib
)

# Copy templates and static files
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/templates DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/static DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Install configuration
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY templates/
    DESTINATION share/${PROJECT_NAME}/templates
)

install(DIRECTORY static/
    DESTINATION share/${PROJECT_NAME}/static
)

# Generate CSP templates
# This assumes Drogon's template compiler is available as drogon_ctl
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/templates/index.h
    COMMAND drogon_ctl
    ARGS create view ${CMAKE_CURRENT_SOURCE_DIR}/templates/index.csp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/templates/index.csp
    COMMENT "Generating CSP template headers"
)

# Testing configuration (optional)
option(BUILD_TESTING "Build the testing tree." OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Version information
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

# Configure version header
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/version.h
)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Print status information
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")