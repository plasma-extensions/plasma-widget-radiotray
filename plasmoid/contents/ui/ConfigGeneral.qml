import QtQuick 2.2
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import org.kde.private.radioplayer 1.0

import '../code/Config.js' as Config

Item {
  RadioPlayerSettings {
    id: rps
    onUpdated: Config.updateModel(rps, playListModel);
    Component.onCompleted: Config.updateModel(rps, playListModel);
  }
  
  ListModel {
      id: playListModel
  }
  
  ColumnLayout {
    RowLayout  {
      Rectangle {
	width: urlInput.width;
	height: urlInput.height;
	color: "white";
	
	TextInput {
	  id: urlInput
	  anchors.verticalCenter : parent.verticalCenter;
	  height: 24; width: 300;
	}
	
      }
      
      Button {
	text: "Add"
	onClicked: {
	  rps.addRadioStation(urlInput.text);
	}
      }
    }
      
    Label {
      text: i18n("Radio stations:")
    }

    
    ScrollView {
    id: playListScrollView

    highlightOnFocus: true
    PlayListView {
      id: mutablePlayList;
      delegate: EditPlayListDelegate {}
      model:  playListModel;
    }
    }
    Button {
      text: "Remove"
      onClicked: {
	var url = playListModel.get(mutablePlayList.currentIndex).station
	rps.removeRadioStation(url);
      }
    }
  }

  Component.onCompleted: {
    /*var i = 0;
    for (var url in plasmoid.configuration.mediasUrl) {
      editModel.add( {idx: i, station: url} );
    }
    console.log("Plasmoid introspection. ")
    for (var prop in plasmoid.configuration) {
	  print(prop += " (" + typeof(plasmoid.configuration[prop]) + ") = " + plasmoid.configuration[prop]);
    }
    
    console.log("Config introspection. ")
    for (var prop in parent) {
	  print(prop += " (" + typeof(parent[prop]) + ") = " + parent[prop]);
    }*/
      
  }
}
 
