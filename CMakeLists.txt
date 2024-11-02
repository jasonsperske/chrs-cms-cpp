link_libraries(${PROJECT_NAME})

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
add_executable(cms ${SOURCES} ${HEADERS})

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

drogon_create_views(view
                    ${CMAKE_CURRENT_SOURCE_DIR}/templates
                    ${CMAKE_CURRENT_BINARY_DIR})


set(project_targets
    cms)

set_property(TARGET ${project_targets}
             PROPERTY CXX_STANDARD ${DROGON_CXX_STANDARD})
set_property(TARGET ${project_targets} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${project_targets} PROPERTY CXX_EXTENSIONS OFF)