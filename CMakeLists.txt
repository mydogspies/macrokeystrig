cmake_minimum_required(VERSION 3.30)
project(macrokeystrig)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.8.0\\mingw_64")

find_package(Qt6 COMPONENTS
  Core
  Gui
  Widgets
  REQUIRED)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${nlohmann_json_source}
        ${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json)

add_executable(macrokeystrig main.cpp
        mainwindow.h
        mainwindow.cpp)
target_link_libraries(macrokeystrig
  Qt::Core
  Qt::Gui
  Qt::Widgets
        nlohmann_json::nlohmann_json
)

if(NOT MSVC)
    add_executable(without_exceptions main.cpp)
    target_link_libraries(without_exceptions nlohmann_json::nlohmann_json)
    target_compile_definitions(without_exceptions PRIVATE JSON_NOEXCEPTION)
    target_compile_options(without_exceptions PRIVATE -fno-exceptions)
endif()

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif()
    endif()
    if(EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif()
    foreach(QT_LIB Core Gui Widgets )
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach(QT_LIB)
endif()
