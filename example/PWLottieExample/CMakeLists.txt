cmake_minimum_required(VERSION 3.16)

project(PWLottieExample VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Quick
    Core
    Gui
    Concurrent
)

set(SOURCES
    main.cpp
)

set(RCC_SOURCES
    assets/assets.qrc
)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_resources(${RCC_SOURCES})

qt_add_executable(appPWLottieExample
    ${SOURCES}
    ${RCC_SOURCES}
)

qt_add_qml_module(appPWLottieExample
    URI PWLottieExample
    VERSION 1.0
    QML_FILES ui/Main.qml
)


##################################
# INCLUDE PWLottie MODULE: start #
##################################

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/PrivateWeb/PWLottie/include/")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PrivateWeb/PWLottie)

################################
# INCLUDE PWLottie MODULE: end #
################################


set_target_properties(appPWLottieExample PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPWLottieExample PRIVATE
    Qt6::Quick
    Qt6::Core
    Qt6::Gui
    Qt6::Concurrent
    PWLottie
)

include(GNUInstallDirs)
install(TARGETS appPWLottieExample
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
