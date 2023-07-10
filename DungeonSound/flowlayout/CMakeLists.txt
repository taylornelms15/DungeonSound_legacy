cmake_minimum_required(VERSION 3.16)
project(flowlayout LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/widgets/layouts/flowlayout")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_add_library(flowlayout_lib
    flowlayout.cpp
    flowlayout.h
)

target_link_libraries(flowlayout_lib PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)
