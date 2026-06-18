cmake_minimum_required(VERSION 3.19)
project(DriveShare LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(DriveShare
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    AuthManager.h
    BookingManager.h
    Car.h
    CarManager.h
    SessionManager.h
    PasswordRecovery.h
    PaymentProxy.h
    Observer.h
    AuthManager.cpp
    BookingManager.cpp
    Car.cpp
    CarManager.cpp
    SessionManager.cpp
    PasswordRecovery.cpp
    PaymentProxy.cpp
    Observer.cpp
    DatabaseManager.h
    DatabaseManager.cpp
    .gitignore
)

target_link_libraries(DriveShare
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

install(TARGETS DriveShare
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET DriveShare
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
