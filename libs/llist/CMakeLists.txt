set(NAME llist)
file(GLOB "${NAME}_SRC" *.c)
add_library(${NAME} "${${NAME}_SRC}")
target_compile_definitions(${NAME} PUBLIC ${NAME})