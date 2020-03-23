cmake_minimum_required(VERSION 3.17)
project(QtHexGrid)

set(CMAKE_CXX_STANDARD 11)

#grab QT
find_package(Qt5 REQUIRED COMPONENTS core widgets svg opengl gui)
if(APPLE)
    include_directories(BEFORE /usr/local/opt/qt5/include)
endif()


add_executable(QtHexGrid main.cpp QtHexGrid.cpp QtHexGrid.hpp)
target_link_libraries(QtHexGrid PRIVATE Qt5::Core Qt5::Widgets Qt5::Svg Qt5::OpenGL Qt5::Gui)