import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.private.radioplayer 1.0

import '../code/Config.js' as Config
import '../code/PlayUtils.js' as PlayUtils


Item {
    id: root
    // anchors.fill: parent

    width: 313;
    height: 210;

    property int negative_margin: -7
    anchors.topMargin: negative_margin 
    anchors.bottomMargin: negative_margin + 24

    anchors.leftMargin: negative_margin + 1
    anchors.rightMargin: negative_margin + 1

    
    ListModel {
        id: stationsModel
    }
      
    property string radioStationsRaw: plasmoid.configuration.radioStations;
    onRadioStationsRawChanged: {
        stationsModel.clear()
        var stations = Config.getResourcesObjectArray()
        stations.forEach(function (stationObj) {
            stationsModel.append(stationObj)
        })
        
        PlayUtils.genPlayList(radioplayer.playMode, stationsModel.count, playListView.currentIndex);
    }

    RadioPlayer {
        id: radioplayer;
        property int playMode: 0;
        
        onUpdateInfo: info.refresh();
        
        onFinished: {
            // console.debug("Handling Player Finished...");
            // console.debug("PlayList currentIndex: " + playListView.currentIndex);
            playNextStation();
            info.refresh();
        }
        
        onStopped: {
            // console.debug("Handling Player Stopped...");
            // console.debug("RadioPlayer currentIndex: " + radioplayer.getCurrentTrack());
            if (radioplayer.getCurrentTrack() < radioplayer.getTrackCount())
                radioplayer.next();
        }
        
        function playItemAt(index) {
            PlayUtils.genPlayList(radioplayer.playMode, stationsModel.count, index);
            playNextStation();
        }
        
        function playNextStation() {
            var nextStation = PlayUtils.getNextStationIndex()
            if (nextStation != undefined) {
                playListView.currentIndex = nextStation;
                var stationUrl = stationsModel.get(nextStation).url;
                radioplayer.playMedia(stationUrl);
            }
        }
        
       function playPreviousStation() {
            var previousStation = PlayUtils.getPreviousStationIndex()
            if (previousStation != undefined) {
                playListView.currentIndex = previousStation;
                var stationUrl = stationsModel.get(previousStation).url;
                radioplayer.playMedia(stationUrl);
            }
        }
        
        Component.onCompleted: {
            }
    }

    Rectangle {
	id: header

	anchors.top: parent.top;
	anchors.left: parent.left; anchors.right: parent.right;
    
	color: "#3DAEE9"
	height: 40;
	
	    Text {
		id: header_text
		//anchors.top: parent.top
		anchors.left: parent.left
		anchors.verticalCenter: parent.verticalCenter
		anchors.leftMargin: 10 
		anchors.topMargin: 16; anchors.bottomMargin: 10;
		
        color: "white"
		text: i18n("Radio"); 
		font.family: "Noto Sans"; font.pointSize: 12;
	    }
    }

      
    GridLayout {
        id: info;

        columns: 2;
        rows: 4;

        anchors.top: header.bottom; anchors.bottom: ctr_box.top;
        anchors.left: parent.left; anchors.right: parent.right;
        anchors.leftMargin: 21; anchors.rightMargin: 21;
        anchors.topMargin: 15; anchors.bottomMargin: 10;


        Label {
            Layout.column: 1
            Layout.row: 1
            text: i18n("Now Playing:")
            font.family: "Noto Sans"; font.pointSize: 12;
        }

        Label {
            id: title
            Layout.column: 1
            Layout.row: 2
            Layout.preferredWidth: 150;
            Layout.maximumWidth: 150;
            wrapMode: Text.WrapAnywhere
            elide: Text.ElideRight
            maximumLineCount: 1;
            text: i18n("Nothing...")
            font.family: "Noto Sans"; font.pointSize: 10;
        }

        Label {
            id: ratio
            Layout.column: 1
            Layout.row: 3
            Layout.preferredWidth: 50;
            Layout.maximumWidth: 50;
            wrapMode: Text.WrapAnywhere
            elide: Text.ElideRight
            maximumLineCount: 1;

            text: i18n("0.0 kbit/s")
            font.family: "Noto Sans"; font.pointSize: 10;
        }

        Label {
            id: genre
            Layout.column: 1
            Layout.row: 4
            Layout.preferredWidth: 150;
            Layout.maximumWidth: 150;
            wrapMode: Text.WrapAnywhere
            elide: Text.ElideRight
            maximumLineCount: 1;
            text: i18n("Genre")
            font.family: "Noto Sans"; font.pointSize: 10;
        }

        Image {
            id: artwork 
            Layout.column: 2
            Layout.row: 1
            Layout.rowSpan: 4
            Layout.margins: 2
            sourceSize.width: 88
            sourceSize.height: 88
            fillMode: Image.PreserveAspectFit
            source: radioplayer.getMediaArtworkUrl()
        }
        
        /* 
        Timer {
            id: refreshTimer;
            interval: 500;
            repeat: true;
            running: true;
            triggeredOnStart: true;
            onTriggered: info.refresh();
        } */
        
        function refresh() {
            title.text = radioplayer.getMediaTitle() 
            ratio.text = radioplayer.getMediaBitrate() + "kb";
            genre.text = radioplayer.getMediaGenre();
            artwork.source = radioplayer.getMediaArtworkUrl();
            
            switch(radioplayer.state) {
                case RadioPlayer.RADIO_PLAYING:
                    // print ("Radio State: playing");
                    play_btn.state = "playing";
                    break;
                case RadioPlayer.RADIO_PAUSED:
                    // print ("Radio State: RADIO_PAUSED");
                    play_btn.state = "paused";
                    break;
                default:
                    // print ("Radio State: RADIO_STOP");
                    play_btn.state = "stoped";
            }
        }

    }

    PlayListView {
        id: playListView;
        model: stationsModel
        
        visible: false
        
        anchors.top: header.bottom; anchors.bottom: ctr_box.top;
        anchors.left: parent.left; anchors.right: parent.right;
        anchors.leftMargin: 21; anchors.rightMargin: 21;
        anchors.topMargin: 15; anchors.bottomMargin: 10;
    
        Layout.minimumHeight: 100; Layout.maximumHeight: 100;
        Layout.minimumWidth: 400; Layout.maximumWidth: 400;
        Layout.topMargin: 20 
        
        onItemClicked: radioplayer.playItemAt(index);
    }
  
    RowLayout {
	id: ctr_box
	
	anchors.bottom: parent.bottom;

	Button {
	    id: playmode_btn
	    Layout.leftMargin: 23
	    Layout.rightMargin: 36
	    iconName: "media-playlist-shuffle"
	    tooltip : i18n("Shuffle radio stations")
	    onClicked: {

	    }
	}

	Button {
	    id: backward_btn
	    Layout.rightMargin: 16
	    iconName: "media-skip-backward"
	    tooltip : i18n("Previous radio station")
	    onClicked: {
	    radioplayer.playPreviousStation()
	    }
	}

	Button {
        id: play_btn;
        Layout.rightMargin: 16;
        
        state: "stoped";
        states: [
            State {
                name: "playing";
                PropertyChanges {
                    target: play_btn;
                    tooltip : i18n("Stop radio station");
                    iconName:"media-playback-pause";
                    onClicked: {
                        radioplayer.togglePause();
                        info.refresh();
                    }
                }
            },
            State {
                name: "paused";
                PropertyChanges {
                    target: play_btn;
                    iconName: "media-playback-start";
                    tooltip : i18n("Play radio station");
                    onClicked: {
                        radioplayer.togglePause();
                        info.refresh();
                    }
                }
            },
            State {
                name: "stoped";
                PropertyChanges {
                    target: play_btn;
                    tooltip : i18n("Stop radio station");
                    iconName:"media-playback-start";
                    onClicked: {
                        radioplayer.playItemAt(playListView.currentIndex);
                        info.refresh();
                    }
                }
            }
        ]
    }

	Button {
	    id: forward_btn
	    Layout.rightMargin: 36
	    iconName: "media-skip-forward"
	    tooltip : i18n("Next radio station")
	    onClicked: {
		radioplayer.playNextStation()
	    }
	}

	Button {
	    id: list_btn
	    Layout.rightMargin: 23
	    iconName: "amarok_playlist"
	    tooltip : i18n("View radio stations")
	    onClicked: {
            info.visible = !info.visible
            playListView.visible = !playListView.visible                
	    }
	}
    }
}

