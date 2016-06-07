function getResourcesObjectArray() {
    var cfgResources = plasmoid.configuration.radioStations
    print('Reading resources from configuration: ' + cfgResources)
    if (!cfgResources) {
        return []
    }
    return JSON.parse(cfgResources)
}
