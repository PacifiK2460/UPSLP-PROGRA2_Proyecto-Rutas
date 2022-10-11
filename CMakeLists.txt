cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)

set(SOURCES main.c)
add_executable(RutasDeTransporte ${SOURCES})

set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
project("Rutas de Transporte")

set(LIBS auth core routes tui llist)

foreach(LIB ${LIBS})
    add_subdirectory("${LIBS_DIR}/${LIB}")
    target_link_libraries(RutasDeTransporte ${LIB})
    # target_include_directories(RutasDeTransporte PUBLIC "${LIBS_DIR}/${LIB}/")
endforeach()

    
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()