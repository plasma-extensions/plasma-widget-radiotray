import QtQuick 2.0
 
Rectangle {
  id: delegateItem
  width: parent.width; height: 100
  color: "blue"
 
  Text {
    id: idxText
    text: idx
  }
  
  Text {
    id: stationText
    anchors.left: idxText.right
    text: station
  }
  
  Text {
    id: bitrateText
    anchors.left: stationText.right
    text: station
  }
}