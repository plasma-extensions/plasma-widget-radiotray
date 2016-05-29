
#include "plasmoidplugin.h"
#include "radioplayer.h"
#include "radioplayersettings.h"

#include <QtQml>
#include <QDebug>



void PlasmoidPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.private.radioplayer"));
    
    qmlRegisterType<RadioPlayer>(uri, 1, 0, "RadioPlayer");
    qmlRegisterType<RadioPlayerSettings>(uri, 1, 0, "RadioPlayerSettings");
}
