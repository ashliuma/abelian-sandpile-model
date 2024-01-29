add_executable(${PROJECT_NAME} labwork3.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ArgParser)
target_link_libraries(${PROJECT_NAME} PRIVATE bmp)
target_link_libraries(${PROJECT_NAME} PRIVATE sandpile)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
