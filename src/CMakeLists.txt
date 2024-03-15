cmake_minimum_required(VERSION 3.14)

project(PWLottie LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
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

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(INCLUDES

)

set(SOURCES

)

add_library(PWLottie SHARED
    ${INCLUDES}
    ${SOURCES}
)

#############################
# INCLUDE MAIN SOURCES: end #
#############################


#################################
# INCLUDE rlottie MODULE: start #
#################################

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/modules/rlottie/inc")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/rlottie/ EXCLUDE_FROM_ALL)

###############################
# INCLUDE rlottie MODULE: end #
###############################


target_link_libraries(PWLottie PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Concurrent
)

target_compile_definitions(PWLottie PRIVATE PWLOTTIE_LIBRARY)
