import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    id: main
  
    Layout.fillWidth: true
    Layout.fillHeight: true
    
    property string displayName: i18n("Radio Player")

    Layout.minimumHeight: units.gridUnit * 18
    Layout.minimumWidth: units.gridUnit * 18
    Layout.preferredHeight: units.gridUnit * 20
    Layout.preferredWidth: units.gridUnit * 20

    /* This little shit is breaking my code !!!!!!
     * Plasmoid.switchWidth: units.gridUnit * 18
    Plasmoid.switchHeight: units.gridUnit * 18*/
    Plasmoid.toolTipMainText: displayName
    Plasmoid.toolTipSubText: ""
    
    Plasmoid.fullRepresentation:  FullRepresentation {}
    Plasmoid.compactRepresentation: CompactRepresentation {}

    Plasmoid.preferredRepresentation: Plasmoid.compactRepresentation   
    
    
}