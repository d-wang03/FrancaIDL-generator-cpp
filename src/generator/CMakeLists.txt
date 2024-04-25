file(GLOB_RECURSE SRCS "*.cpp")
list(SORT SRCS)
add_library(FGenerator ${SRCS})