project(compiler)
cmake_minimum_required(VERSION 3.17.1)
set(CXX_STANDARD 17)
set(CXX_STANDARD TRUE)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(MyParser app/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(MyScanner app/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


include_directories(inc)
add_executable(${PROJECT_NAME}
  ${BISON_MyParser_OUTPUTS}
  ${FLEX_MyScanner_OUTPUTS}
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} ${FLEX_LIBRARIES})