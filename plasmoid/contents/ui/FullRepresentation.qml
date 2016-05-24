import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.private.radioplayer 1.0

import '../code/Config.js' as Config


ColumnLayout {
  id: root
  //anchors.fill: parent
  
  Layout.preferredWidth: 380; Layout.maximumWidth: 380;
  Layout.preferredHeight : 88; Layout.maximumHeight : 88
  
  property int negative_margin: -7
  anchors.topMargin: negative_margin 
  anchors.bottomMargin: negative_margin
  
  anchors.leftMargin: negative_margin + 1
  anchors.rightMargin: negative_margin + 1
  
  RadioPlayer {
    id: radioplayer;
    
    onUpdateInfo: {
      console.log("Refreshing INFO");
      
      /* LOG PLAYLIST
      console.log("list size: " + radioplayer.getMediaListSize());
      for (var i = 0; i < radioplayer.getMediaListSize(); i ++) 
	console.log(radioplayer.getListItemUrl(i));
      var idx = radioplayer.getCurrentMediaIdx();
      console.log("Track idx: " + idx);*/
      
      title.text = radioplayer.getMediaTitle() 
      ratio.text = radioplayer.getMediaBitrate() + "kb";
      genre.text = radioplayer.getMediaGenre();
      artwork.source = radioplayer.getMediaArtworkUrl();
    }
    
    Component.onCompleted: {
      radioplayer.addMedia(plasmoid.configuration.mediaUrl);
      
      /* LOG PLAYLIST
      console.log("list size: " + radioplayer.getMediaListSize());
      for (var i = 0; i < radioplayer.getMediaListSize(); i ++)
	console.log(radioplayer.getListItemUrl(i));*/
      
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
    font.family: "Noto Sans"; font.pointSize: 12;
    }
  }


  GridLayout {
    id: info
    //columnSpacing: 4
    columns: 6
    rows: 7
    
    Layout.preferredWidth: 380; Layout.maximumWidth: 380;
    Layout.preferredHeight : 88; Layout.maximumHeight : 88
    
    Layout.topMargin : 15;
    Layout.bottomMargin : 10;
    Layout.leftMargin: 21;
    Layout.rightMargin: 21;

    Label {
      Layout.column: 2
      Layout.row: 2
      
      text: i18n("Now playing:")
      font.family: "Noto Sans"; font.pointSize: 11;
    }
    
    Label {
      id: title
      Layout.column: 2
      Layout.row: 3
      
      Layout.preferredWidth: 150;
      Layout.maximumWidth: 150;
      wrapMode: Text.WrapAnywhere
      elide: Text.ElideRight
      maximumLineCount: 1;
      
      text: i18n("Nothing...")
      font.family: "Noto Sans"; font.pointSize: 11;
    }
    
    Label {
      id: ratio
      Layout.column: 3
      Layout.row: 3
      
      Layout.preferredWidth: 50;
      Layout.maximumWidth: 50;
      wrapMode: Text.WrapAnywhere
      elide: Text.ElideRight
      maximumLineCount: 1;
      
      text: i18n("0.0 kbit/s")
      font.family: "Noto Sans"; font.pointSize: 11;
    }
    
    Label {
      id: genre
      Layout.column: 2
      Layout.row: 4
      
      Layout.preferredWidth: 150;
      Layout.maximumWidth: 150;
      wrapMode: Text.WrapAnywhere
      elide: Text.ElideRight
      maximumLineCount: 1;
      
      text: i18n("Genre")
      font.family: "Noto Sans"; font.pointSize: 11;
    }
    
    Image {
      id: artwork 
      Layout.column: 4
      Layout.row: 2
      Layout.rowSpan: 3
      
      Layout.margins: 2
      sourceSize.width: 88
      sourceSize.height: 88

      fillMode: Image.PreserveAspectFit
      
      source: radioplayer.getMediaArtworkUrl()
    }
    
  }


  ScrollView {
    id: playListScrollView
    visible: false
    highlightOnFocus: true
    
    Layout.preferredWidth: 380; Layout.maximumWidth: 380;
    Layout.preferredHeight : 88; Layout.maximumHeight : 88
    
    Layout.topMargin : 15;
    Layout.bottomMargin : 10;
    Layout.leftMargin: 21;
    Layout.rightMargin: 21;

    ListView {
      id: playListView
      model: PlayListModel {}          // concrete model
      delegate: PlayListDelegate { }   // provide delegate component.
      
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
      highlight: Rectangle {
	width: parent.width;
	height: childrenRect.height;
	color: "lightsteelblue";
	focus: true
      }
      
    }
  }
    

  
  RowLayout {
    id: ctr_box
    Layout.alignment: Qt.AlignHCenter
    Layout.bottomMargin: 17
    
    
    Button {
      id: playmode_btn
      Layout.leftMargin: 23
      Layout.rightMargin: 36
      
      iconName: "media-playlist-shuffle"
      tooltip : i18n("Shuffle")
      
      onClicked: {
      }
    }
    
    Button {
      id: backward_btn
      Layout.rightMargin: 16
      
      iconName: "media-skip-backward"
      tooltip : i18n("Prevoius media in playlist")
      
      onClicked: {
	radioplayer.backward()
      }
    }

    Button {
      id: play_btn
      Layout.rightMargin: 16
      
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
	Layout.rightMargin: 36
	
	iconName: "media-skip-forward"
	tooltip : i18n("Next media in playlist")

	onClicked: {
	  radioplayer.forward()
	}
    }
    
    Button {
	id: list_btn
	Layout.rightMargin: 23
	
	iconName: "amarok_playlist"
	tooltip : i18n("Show playlist")

	onClicked: {
	  //console.log(info)
	  info.visible = !info.visible
	  playListScrollView.visible = !playListScrollView.visible 
	  
	  //var playList = radioplayer.getPlayList();
	  //console.log(radioplayer.getPlayList());
	    
	}
    }
  }
}

