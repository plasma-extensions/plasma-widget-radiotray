set(plasmoidplugin_SRCS
    plasmoidplugin.cpp
    radioplayer.cpp

    qmldir
    )

add_library(plasmoidplugin SHARED ${plasmoidplugin_SRCS})

target_include_directories(plasmoidplugin PUBLIC ${LIBVLC_INCLUDE_DIR} )
target_link_libraries(plasmoidplugin Qt5::Widgets Qt5::Quick KF5::Plasma ${LIBVLC_LIBRARY})

install(TARGETS plasmoidplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/private/radioplayer)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/private/radioplayer)
