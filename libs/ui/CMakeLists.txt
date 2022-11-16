# set(NAME ui)
# file(GLOB "${NAME}_SOURCES"
#     io/*.c
#     win/*.c
#     )

# # print all sources
# message(STATUS "Sources: ${${NAME}_SOURCES}")

# add_library(${NAME} SHARED "${NAME}_SOURCES")

set(NAME ui)
file(GLOB "${NAME}_SRC" io/*.c win/*.c)
 message(STATUS "Sources: ${${NAME}_SRC}")

add_library(${NAME} SHARED "${${NAME}_SRC}")