import QtQuick 2.5
import QtQuick.Layouts 1.1
 
ListView {
  //model: PlayListModel {}          // concrete model
  //delegate: PlayListDelegate { }   // provide delegate component.
  
  headerPositioning: ListView.OverlayHeader
  header: Rectangle {
    width: parent.width
    height: childrenRect.height
    color: "white"
    z: 100;

    RowLayout {
      spacing: 10
      Text {
	text: "#";
      }
      Text {
	text: "Station";
      }
    }
  }

  highlightFollowsCurrentItem: true
  focus: true
      
  highlight: Rectangle {
    width: parent.width;
    height: childrenRect.height;
    color: "#3DAEE9";
  }
  
}