cmake_minimum_required(VERSION 3.19)
project(LiveScope LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network Sql)

qt_standard_project_setup()

qt_add_resources(RESOURCES assets.qrc)

qt_add_executable(LiveScope
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    widget/roomwidget.h widget/roomwidget.cpp
    network/bilibilidanmakuapi.h network/bilibilidanmakuapi.cpp
    model/Danmaku.h
    model/LiveRoom.h
    network/bilibililiveapi.h network/bilibililiveapi.cpp
    model/BilibiliUser.h
    network/bilibiliuserapi.h network/bilibiliuserapi.cpp
    ${RESOURCES}
    network/bilibiliroominitapi.h network/bilibiliroominitapi.cpp
)

target_include_directories(LiveScope
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LiveScope
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        Qt::Sql
)

install(TARGETS LiveScope
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET LiveScope
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
