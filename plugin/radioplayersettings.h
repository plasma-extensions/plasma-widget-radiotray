#ifndef RADIOPLAYERSETTINGS_H
#define RADIOPLAYERSETTINGS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QSettings>
#include <QFileSystemWatcher>

class RadioPlayerSettings : public QObject
{
  Q_OBJECT

public:
  RadioPlayerSettings();
  ~RadioPlayerSettings();

public Q_SLOTS:
  QVariantList radioStations();
  void setRadioStations(QVariantList radioStations);
  
  int getRadioStationsSize();
  QString getRadioStationUrl(int idx);
  void removeRadioStation(int idx);
  void addRadioStation(QString radio);
  void saveRadioStations();
  void loadRadioStations();
  
Q_SIGNALS:
  void updated();

private Q_SLOTS:
  void onSettingsFileChanged(const QString &file);
private:

  RadioPlayerSettings * m_instance;
  
  QString m_settingsPath;
  QStringList m_radioStations;
  QSettings *m_settings;
  QFileSystemWatcher *m_watcher;
};

#endif // RADIOPLAYERSETTINGS_H
