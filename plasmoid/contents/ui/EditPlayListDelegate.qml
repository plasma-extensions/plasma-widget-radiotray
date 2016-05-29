import QtQuick 2.0
import QtQuick.Layouts 1.1
 
RowLayout  {
  
  spacing: 10
  Text {
    id: idxText
    text: idx
  }
  
  Text {
    id: stationText
    text: station
    wrapMode: Text.WrapAnywhere
    elide: Text.ElideRight
  }
  
  MouseArea {
    anchors.fill: parent
    onClicked: {
      listView.currentIndex = index
    }
  }

  /* TODO: Implement bitrate for radio sources
  Text {
    id: bitrateText
    anchors.left: stationText.right
    text: station
  }*/
   
}