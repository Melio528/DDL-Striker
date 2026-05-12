cmake_minimum_required(VERSION 3.19)
project(DDLstriker LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(DDLstriker
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    gameobject.cpp
        gameobject.h
        player.cpp
        player.h
        bullet.cpp
        bullet.h
)

qt_add_translations(
    TARGETS DDLstriker
    TS_FILES DDLstriker_zh_CN.ts
)

target_link_libraries(DDLstriker
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS DDLstriker
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET DDLstriker
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
