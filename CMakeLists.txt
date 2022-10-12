cmake_minimum_required(VERSION 3.8)
project("RutasdeTransporte")

set(SOURCES main.c)

set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
set(LIBS auth core routes tui llist)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE m)


foreach(LIB ${LIBS})
    add_subdirectory("${LIBS_DIR}/${LIB}")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB})
    # target_include_directories(RutasDeTransporte PUBLIC "${LIBS_DIR}/${LIB}/")
endforeach()
