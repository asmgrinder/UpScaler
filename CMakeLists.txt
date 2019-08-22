cmake_minimum_required(VERSION 2.6)
project(upscaler)

add_executable(upscaler main.cpp CApplication.cpp upscaler.rc)

#include_directories("/usr/include/GLFW")
# find_package(OpenMP)
# if (OPENMP_FOUND)
#     set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#     set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
# endif()
set (CMAKE_COMPILER_FLAGS "-march=core2 -mtune=core2 -DUNICODE -D_UNICODE -Wall -Wextra -Wold-style-cast -Wcast-qual -Wcast-align -Wcomments -Wundef -Wunused-macros -Werror=array-bounds -static -static-libgcc -static-libstdc++")
# set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNICODE -D_UNICODE")  # -fsanitize=address
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE -D_UNICODE")  #-fsanitize=address

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_COMPILER_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_COMPILER_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -Wl,--stack=8388608")

#-Wall -Wextra -Wfloat-equal -Wshadow -Wunused-macros -Wold-style-cast -Woverloaded-virtual
set (CMAKE_C_FLAGS_DEBUG "-g -D_DEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "-g -D_DEBUG")
set (CMAKE_EXE_LINKER_FLAGS_DEBUG "-g")

set (CMAKE_C_FLAGS_RELEASE "-O2 -s")
set (CMAKE_CXX_FLAGS_RELEASE "-O2 -s")
set (CMAKE_EXE_LINKER_FLAGS_RELEASE "-O2 -s")

set (CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O2")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2")
set (CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "-g -O2")

set (CMAKE_C_FLAGS_MINSIZEREL "-Os -s")
set (CMAKE_CXX_FLAGS_MINSIZEREL "-Os -s")
set (CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "-Os -s")

set (EXTRA_LIBS ${EXTRA_LIBS} )
target_link_libraries (upscaler  ${EXTRA_LIBS} gdiplus gdi32 oleaut32 shlwapi cmutil comdlg32 comctl32 mingw32)

install(TARGETS upscaler RUNTIME DESTINATION bin)
