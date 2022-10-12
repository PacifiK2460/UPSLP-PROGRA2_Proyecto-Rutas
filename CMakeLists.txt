cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
project("RutasdeTransporte")

set(SOURCES main.c)
add_executable(${PROJECT_NAME} ${SOURCES})

set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

set(LIBS auth core routes tui llist)

target_link_libraries(${PROJECT_NAME} m)

foreach(LIB ${LIBS})
    add_subdirectory("${LIBS_DIR}/${LIB}")
    target_link_libraries(${PROJECT_NAME} ${LIB})
    # target_include_directories(RutasDeTransporte PUBLIC "${LIBS_DIR}/${LIB}/")
endforeach()
