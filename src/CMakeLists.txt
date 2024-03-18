cmake_minimum_required(VERSION 3.14)

project(PWLottie LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#############################
# INCLUDE QT MODULES: srart #
#############################

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Concurrent
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Concurrent
)

###########################
# INCLUDE QT MODULES: end #
###########################


###############################
# INCLUDE MAIN SOURCES: srart #
###############################

set(INCLUDES
    include/PWLottieItem/PWLottieItem.h
    include/PWLottieControllers/PWLottieAbstractController.h
    include/PWLottieControllers/PWLottieIconController.h
    include/PWLottieControllers/PWLottieBaseController.h
    include/PWControllerMediator/PWControllerMediator.h
)

set(SOURCES
    sources/PWLottieItem/PWLottieItem.cpp
    sources/PWLottieControllers/PWLottieIconController.cpp
    sources/PWLottieControllers/PWLottieBaseController.cpp
    sources/PWControllerMediator/PWControllerMediator.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${INCLUDES}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Include PWLottieItem dierectly to avoid qml_module auto generated errors
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/PWLottieItem")

#############################
# INCLUDE MAIN SOURCES: end #
#############################


#################################
# INCLUDE rlottie MODULE: start #
#################################

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/modules/rlottie/inc")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/rlottie/)

###############################
# INCLUDE rlottie MODULE: end #
###############################


###############################
# INCLUDE rlottie MODULE: end #
###############################

qt_policy(SET QTP0001 NEW) # Suppress warning https://doc.qt.io/qt-6/qt-cmake-policy-qtp0001.html

qt_add_qml_module(${PROJECT_NAME}
    URI "PrivateWeb.${PROJECT_NAME}"
    VERSION 2.0
    SOURCES include/PWLottieItem/PWLottieItem.h sources/PWLottieItem/PWLottieItem.cpp
)

###############################
# INCLUDE rlottie MODULE: end #
###############################

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Concurrent
    rlottie
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PWLOTTIE_LIBRARY)
