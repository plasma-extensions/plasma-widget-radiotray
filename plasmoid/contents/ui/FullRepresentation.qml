import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.private.radioplayer 1.0

import '../code/Config.js' as Config


ColumnLayout {
  id: root
  anchors.fill: parent
  
  property int negative_margin: -7
  anchors.topMargin: negative_margin 
  anchors.bottomMargin: negative_margin
  
  anchors.leftMargin: negative_margin + 1
  anchors.rightMargin: negative_margin + 1
  
  Component.onCompleted: {
    plasmoid.addEventListener('ConfigChanged', Config.configChanged);
  }
  
  
  RadioPlayer {
    id: radioplayer;
    
    onUpdateInfo: {
      console.log("Updating !!")
      title.text = radioplayer.getCurrentTrackTitle()
      genre.text = radioplayer.getCurrentTrackGenre()
      artwork.source = radioplayer.getCurrentTrackArtworkUrl()
    }
    
    Component.onCompleted: {
      radioplayer.addMedia(plasmoid.configuration.mediaUrl);
    }
  }
  
  
  
  Rectangle {
  id: header
  
  color: "#3DAEE9"
  height: header_text.height + 8;
  Layout.fillWidth: true
  Text {
    id: header_text	
    
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.leftMargin: 10 
    anchors.topMargin: 4
    
    color: "white"
    
    text: i18n("Radio"); 
    font.family: "Noto Sans"; font.pointSize: 11;
    }
  }


  GridLayout {
    id: info
    //columnSpacing: 4
    columns: 6
    rows: 7
    
    Item {
      Layout.column: 1
      Layout.row: 1
      width: 10
    }
    Item {
      Layout.column: 5
      Layout.row: 1
      width: 10
    }

    Label {
      Layout.column: 2
      Layout.row: 1
      text: i18n("Now playing:")
    }
    
    Label {
      id: title
      Layout.column: 2
      Layout.row: 3
      text: i18n("Nothing... - 0.0 kbit/s")
    }
    
    
    Label {
      id: genre
      Layout.column: 2
      Layout.row: 4
      text: i18n("Genre")
    }
    
    Image {
      id: artwork 
      Layout.column: 4
      Layout.row: 2
      
      source: radioplayer.getCurrentTrackArtworkUrl()
    }
    
  }
  
   
  GridLayout {
    id: ctr_box
    columns: 6
    rows: 3
    
    Item {
      Layout.column: 1
      Layout.row: 1
      width: 10
    }
    Item {
      Layout.column: 5
      Layout.row: 1
      width: 10
    }
    
    Button {
      id: backward_btn
      Layout.column: 2
      Layout.row: 2
      Layout.alignment: Qt.AlignHCenter
      
      iconName: "media-skip-backward"
      tooltip : i18n("Prevoius media in playlist.")
      
      onClicked: {
	radioplayer.backward()
      }
    }

    Button {
      id: play_btn
      Layout.column: 3
      Layout.row: 2
      Layout.alignment: Qt.AlignHCenter
      
      iconName: "media-playback-start"
      tooltip : i18n("Play.")

      onClicked: {
	if ( iconName == "media-playback-start") {
	  radioplayer.play()
	  iconName = "media-playback-pause"
	} else {
	  radioplayer.pause()
	  iconName = "media-playback-start"
	  
	}
      }
    }

    Button {
	id: forward_btn
	Layout.column: 4
	Layout.row: 2
	Layout.alignment: Qt.AlignHCenter
	
	iconName: "media-skip-forward"
	tooltip : i18n("Next media in playlist.")

	onClicked: {
	  radioplayer.forward()
	}
    }
    
    Button {
	id: list_btn
	Layout.column: 5 
	Layout.row: 2
	Layout.alignment: Qt.AlignHCenter
	
	iconName: "amarok_playlist"
	tooltip : i18n("Show playlist.")

	onClicked: {
	  console.log("show playlist not implemented yet.")
	}
    }
  }
}

