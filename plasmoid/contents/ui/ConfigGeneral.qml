import QtQuick 2.2
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0


Item {
    property alias cfg_mediaUrl: mediaUrl.text

    property int textfieldWidth: theme.defaultFont.pointSize * 30

    GridLayout {
        columns: 2

        Label {
	  Layout.column: 1
	  Layout.row: 1
	  text: i18n("Radio url:")
        }

        TextInput {
	  id: mediaUrl
	  
	  Layout.column: 2
	  Layout.row: 1
        }
    }
}
 
