cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)


set(QT_MIN_VERSION "5.4.0")
set(KF5_MIN_VERSION "5.0.0")

find_package(ECM 0.0.11 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)


find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Widgets Quick)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Plasma)

# Add additional libraries
find_package(LIBVLC REQUIRED COMPONENTS Core)

add_subdirectory(plugin)
# add_subdirectory(translations)

plasma_install_package(plasmoid org.plasma-light.radio)

# install( DIRECTORY image/breeze DESTINATION ${ICON_INSTALL_DIR})
# install( DIRECTORY image/breeze-dark DESTINATION ${ICON_INSTALL_DIR})