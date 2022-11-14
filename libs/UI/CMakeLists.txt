set(NAME ui)
set(UILIBS io sys win)
foreach(UILIB)
    
endforeach()

file(GLOB "${NAME}_SRC" *.c)
add_library(${NAME} SHARED "${${NAME}_SRC}")