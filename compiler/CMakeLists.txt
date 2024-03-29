project(compiler)
cmake_minimum_required(VERSION 3.16.0)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

set(
    LOGIC_SOURCES src/Assignment.cpp src/GlobalBlock.cpp src/IntermidiateParser.cpp
    src/MainBlock.cpp src/IOCommand.cpp src/Block.cpp src/IfBlock.cpp src/IntermidiateCode.cpp
    src/WhileBlock.cpp src/Procedure.cpp src/Multiplication.cpp src/DivMod.cpp
  )

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(MyParser app/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(MyScanner app/lexer.ll ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


add_library(compilerLogic STATIC ${LOGIC_SOURCES})

include_directories(inc)
add_executable(${PROJECT_NAME}
  ${BISON_MyParser_OUTPUTS}
  ${FLEX_MyScanner_OUTPUTS}
)

#target_include_directories(${PROJECT_NAME} PRIVATE src inc)
target_link_libraries(${PROJECT_NAME} ${FLEX_LIBRARIES} compilerLogic)