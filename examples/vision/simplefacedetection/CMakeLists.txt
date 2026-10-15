# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.22)

project(simplefacedetection VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT QT_BUILDING_QT)
    find_package(Qt6 6.12 REQUIRED COMPONENTS Core Qml Quick Multimedia EdgeAi)
    qt_standard_project_setup(REQUIRES 6.12)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_executable(simplefacedetection
    main.cpp
)

qt_add_qml_module(simplefacedetection
    URI SimpleFaceDetection
    # The module has to end up under one of the engine's default import paths, so
    # that loadFromModule("SimpleFaceDetection", "Main") finds it. Leaving this out
    # would put the QML files at the resource root instead, where the engine does
    # not look for modules.
    RESOURCE_PREFIX "/qt/qml/"
    QML_FILES
        Main.qml
    VERSION 1.0
)

target_link_libraries(simplefacedetection
    PRIVATE
        Qt::Core
        Qt::Qml
        Qt::Quick
        Qt::Multimedia
        Qt::EdgeAi
)

target_include_directories(simplefacedetection
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(simplefacedetection PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGstreamerPoC
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# qt_internal_add_example() points the install prefix at this example's own
# directory under the Qt examples tree, so install into it rather than into the
# plain bin dir.
install(TARGETS simplefacedetection
    BUNDLE DESTINATION .
    LIBRARY DESTINATION .
    RUNTIME DESTINATION .
)
