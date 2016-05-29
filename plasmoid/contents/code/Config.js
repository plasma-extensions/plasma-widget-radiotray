function onChanged() {
  console.log("Config Changed!");
  radioplayer.play(plasmoid.configuration.mediaUrl);
} 


function setDefaultStations(radioPlayerSettings) {
  console.log("settings default stations");
  radioPlayerSettings.addRadioStation("file:///home/alexis/Escritorio/pl.xspf");
  radioPlayerSettings.addRadioStation("file:///home/alexis/Escritorio/pl.m3u");
}

function updateModel(radioPlayerSettings, listModel) {
  console.log("updating model: " + listModel);
  listModel.clear()
  /*
  if (radioPlayerSettings.getRadioStationsSize() <= 0)
    setDefaultStations(radioPlayerSettings);
  */
  
  for (var i = 0; i < radioPlayerSettings.getRadioStationsSize(); i ++) {
    playListModel.append( { idx: i, station: radioPlayerSettings.getRadioStationUrl(i) } );
  }
}