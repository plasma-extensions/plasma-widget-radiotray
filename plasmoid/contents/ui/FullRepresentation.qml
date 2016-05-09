import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.private.radioplayer 1.0


ColumnLayout {
  id: root
  anchors.fill: parent
  
  property int negative_margin: -7
  anchors.topMargin: negative_margin 
  anchors.bottomMargin: negative_margin
  
  anchors.leftMargin: negative_margin + 1
  anchors.rightMargin: negative_margin + 1
  
  RadioPlayer {
    id: radioplayer;
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
    id: actio1nsBox
    //columnSpacing: 4
    columns: 5
    rows: 4
    
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
      Layout.column: 2
      Layout.row: 2
      Layout.alignment: Qt.AlignHCenter
      
      iconName: "media-skip-backward"
      tooltip : i18n("Full screen capture.")
      
      onClicked: {
	radioplayer.setMedia("mms://ucimedia.uci.cu/rtaino");
	radioplayer.play()
      }
    }
      


    Button {
      Layout.column: 3
      Layout.row: 2
      Layout.alignment: Qt.AlignHCenter
      
      iconName: "media-playback-start"
      tooltip : i18n("Only the active window.")

      onClicked: {
	radioplayer.setMedia("mms://ucimedia.uci.cu/rtaino");
	radioplayer.play()
	iconName: "media-playback-pause"
      }
    }


      Button {
	Layout.column: 4
	Layout.row: 2
	Layout.alignment: Qt.AlignHCenter
	
	iconName: "media-skip-forward"
	tooltip : i18n("From a rectangular area.")

	onClicked: {
	  
	}
	}
	
	
    }
  
}

