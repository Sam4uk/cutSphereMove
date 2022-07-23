cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(
#   -Werror
#   -Wcast-align
#   -Wcast-qual
#   -Wctor-dtor-privacy
#   -Wfloat-equal

#   -Wnon-virtual-dtor
#   -Wold-style-cast
#   -Woverloaded-virtual
#   -Wredundant-decls
#   -Wsign-promo
#   -Wall 
)

project(test_task)

file(WRITE ${PROJECT_BINARY_DIR}/.gitignore [=[
*.*
*
]=])

set(SRC cutSphereMove.cpp
		CreateSkin.cpp
		cutSphereMove.cpp
		mwArcFunction.cpp
		mwException.cpp
		mwIdxRangeException.cpp
)

include_directories(.)

add_executable(${PROJECT_NAME} ${SRC})