cmake_minimum_required(VERSION 3.16)

project(Mario_ VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Mario_
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        game.h game.cpp
        player.h player.cpp
        inputhandler.h inputhandler.cpp
        resources.qrc
        images
        gamescene.h gamescene.cpp
        gameview.h gameview.cpp
        score.h score.cpp
        platform.h platform.cpp
        spike.h spike.cpp
        life.h life.cpp
        enemy.h enemy.cpp
        bonusblock.h bonusblock.cpp
    )
else()
    if(ANDROID)
        add_library(Mario_ SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Mario_
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Mario_ PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia   # <-- ADDED FOR QSOUNDEFFECT
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Mario_)
endif()

set_target_properties(Mario_ PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Mario_
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Mario_)
endif()
