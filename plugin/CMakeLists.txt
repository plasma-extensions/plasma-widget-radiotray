set(plasmoidplugin_SRCS
    plasmoidplugin.cpp
    radioplayer.cpp

    qmldir
    )

add_library(plasmoidplugin SHARED ${plasmoidplugin_SRCS})

target_include_directories(plasmoidplugin PUBLIC ${LIBVLCQT_INCLUDE_DIR} ${LIBVLCQT_INCLUDE_DIR} )
target_link_libraries(plasmoidplugin Qt5::Widgets Qt5::Quick KF5::Plasma vlc vlc-qt)

install(TARGETS plasmoidplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/private/radioplayer)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/private/radioplayer)
