cmake_minimum_required(VERSION 3.10)
project(QLink)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\QT\\QT5.14.2\\5.14.2\\mingw73_64\\lib")
set(Qt5_DIR "C:\\QT\\QT5.14.2\\5.14.2\\mingw73_64\\lib")

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(*)
target_link_libraries(QtWidgets)
