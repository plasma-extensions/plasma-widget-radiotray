import QtQuick 2.2
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.private.radioplayer 1.0

import '../code/Config.js' as Config

Item {
    ListModel {
        id: stationsModel
    }
    
    Component.onCompleted: {
        var stations = Config.getResourcesObjectArray()
        stations.forEach(function (stationObj) {
            stationsModel.append(stationObj)
        })
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.bottomMargin: 40;
        anchors.topMargin: 10;
        anchors.leftMargin: 15;
        anchors.rightMargin: 10;
               
        Text {
            text: i18n("General Configuration");
            font.pointSize: 14;
        }
        
        PlayListView {
            id: playListView;
            model: stationsModel
            
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            Layout.topMargin: 20
            
            onItemClicked: showAddOrEditStationDialog(index, item);
        }
        

        
        RowLayout  {
            Layout.fillWidth: true
            Button {
                text: i18n("Add")
                onClicked: {
                    showAddOrEditStationDialog(-1, null);
                }
            }
            Button {
                text: i18n("Remove")
                onClicked: {
                    stationsModel.remove(playListView.currentIndex)
                    stationsModelChanged()
                }
            }
            
            Button {
                //text: i18n("Move Up")
                iconName: "arrow-up"
                onClicked: {
                    print ("Moving up "  + playListView.currentIndex)
                    stationsModel.move(playListView.currentIndex, playListView.currentIndex - 1, 1)
                    stationsModelChanged()
                }
            }
            Button {
                //text: i18n("Move Down")
                iconName: "arrow-down"
                onClicked: {
                    print ("Moving down "  + playListView.currentIndex)
                    stationsModel.move(playListView.currentIndex, playListView.currentIndex + 1, 1)
                    stationsModelChanged()
                }
            }
        }
    }
    
    function stationsModelChanged() {
        var newStationsArray = []
        for (var i = 0; i < stationsModel.count; i++) {
            var obj = stationsModel.get(i)
            newStationsArray.push({
                url: obj.url,
                name: obj.name,
                genre: obj.genre
            })
            
        }
        
        plasmoid.configuration.radioStations = JSON.stringify(newStationsArray)
        print('cfg_radioStations: ' + plasmoid.configuration.radioStations)
    }
    
    function showAddOrEditStationDialog(index, stationObj) {
        if (index < 0) {
            stationObj =  {
                idx: -1,
                url: "",
                name: "",
                genre: ""
            }
        }
        
        addOrEditStationDialog.idx = index;
        addOrEditStationDialog.url = stationObj.url ;
        addOrEditStationDialog.name = stationObj.name || "";
        addOrEditStationDialog.genre = stationObj.genre || "";
        
        console.log(stationObj)
        // open dialog
        addOrEditStationDialog.open()
    }
    
    Dialog {
        id: addOrEditStationDialog;
        
        property bool addStation: true
        title: addStation ? i18n('Add Station') : i18n('Edit Station')
        
        property int idx: 0;
        property alias url: urlTextField.text;
        property alias name: nameTextField.text;
        property alias genre: genreTextField.text;
        
        width: 400;
        
        GridLayout {
            columns: 2
            anchors.left: parent.left; anchors.right: parent.right;
            
            Label {Layout.row: 0; Layout.column: 1; text: "URL:"}
            TextField {Layout.row: 0; Layout.column: 2;  id: urlTextField; Layout.fillWidth: true;}
            Label {Layout.row: 1; Layout.column: 1; text: "Name:"}
            TextField {Layout.row: 1; Layout.column: 2;  id: nameTextField; Layout.fillWidth: true;}
            Label {Layout.row: 2; Layout.column: 1; text: "Genre:"}
            TextField {Layout.row: 2; Layout.column: 2;  id: genreTextField; Layout.fillWidth: true;}
        }
        
        onAccepted: {
            var newObject = {
                url: addOrEditStationDialog.url,
                name: addOrEditStationDialog.name, 
                genre: addOrEditStationDialog.genre
            }
            
            if (addOrEditStationDialog.idx >= 0) {
                stationsModel.set(addOrEditStationDialog.idx, newObject);
            } else {
                stationsModel.append(newObject);
            }
            
            stationsModelChanged()
            addOrEditStationDialog.close()
        }
    }
}
 
