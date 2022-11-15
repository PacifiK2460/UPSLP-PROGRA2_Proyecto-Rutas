set(NAME tui)
file(GLOB "${NAME}_SRC" *.c)
add_library(${NAME} SHARED "${${NAME}_SRC}")