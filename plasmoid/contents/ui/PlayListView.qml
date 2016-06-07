import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Rectangle {
    id: root
    property alias model: stationsListView.model;
    property alias delegate: stationsListView.delegate;
    property alias currentIndex: stationsListView.currentIndex;
    property int itemsHeight: 20;
    signal itemClicked(int index, var item);

    color: "white";
    border.color: "lightgray"
    border.width: 1
    radius: 2;

    
    ScrollView {
        anchors.fill: root
        // Disable horizontal scroll
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff;
        
        //frameVisible: true;    
        
        ListView {
            id: stationsListView
            width: root.width; height: root.height;
            
            highlight: Rectangle {
                width: root.width; height: itemsHeight;
                color: "#3daee9";
            }
        
            highlightFollowsCurrentItem: true
            focus: true;
            
            headerPositioning: ListView.OverlayHeader
            header: Rectangle {
                width: root.width
                height: itemsHeight
                color: "white"
                radius: 2;
                
                border.color: "lightgray"
                border.width: 1

                RowLayout {
                    spacing: 4
                    Layout.fillWidth: true;
                    
                    Text {
                        Layout.leftMargin: 5;
                        Layout.minimumWidth: 100; Layout.maximumWidth: 100;
                        
                        text: "Name";
                        id: nameHeader;
                        //width: 100;
                        //width: 40; height: itemsHeight;
                    }
            
                    Text {
                        Layout.minimumWidth: 100; Layout.maximumWidth: 100;
                        text: "Genre";
                        width: 100;
                        //anchors.fill: parent;
                    }
                    Text {
                        id: urlHeader;
                        //width: 80;
                        text: "Url";
                        //anchors.fill: parent;
                    }
                }
            }
            
            delegate: RowLayout  {
                width: root.width; height: itemsHeight;
                Text {
                    id: nameText
                    text: name
                    Layout.minimumWidth: 100; Layout.maximumWidth: 100;
                    Layout.leftMargin: 5;
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                }
                
                Text {
                    id: genreText
                    Layout.minimumWidth: 100; Layout.maximumWidth: 100;
                    text: genre
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                }
                
                Text {
                    id: urlText
                    text: url
                    wrapMode: Text.WrapAnywhere
                    elide: Text.ElideRight
                }
                
                MouseArea {
                    id: itemMouseArea;
                    anchors.fill: parent;
                    onClicked: stationsListView.currentIndex = index;
                    onDoubleClicked: root.itemClicked(stationsListView.currentIndex, stationsListView.model.get(stationsListView.currentIndex));
                }
            }
        }
    }
}
 /*
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
  
}*/