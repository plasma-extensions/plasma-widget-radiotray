find_package(PkgConfig)
pkg_search_module(LIBVLC REQUIRED libvlc)

message(STATUS "INCLUDE: " ${LIBVLC_INCLUDE_DIRS})

set(plasmoidplugin_SRCS
    plasmoidplugin.cpp
    radioplayer.cpp

    qmldir
    )

add_library(plasmoidplugin SHARED ${plasmoidplugin_SRCS})

target_include_directories(plasmoidplugin PUBLIC ${LIBVLC_INCLUDE_DIRS})
target_link_libraries(plasmoidplugin Qt5::Quick KF5::Plasma ${LIBVLC_LIBRARIES})

install(TARGETS plasmoidplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/private/radioplayer)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/private/radioplayer)
