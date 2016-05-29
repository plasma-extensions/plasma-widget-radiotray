#include "radioplayersettings.h"
#include <QApplication>
#include <QVariant>

#include <QDebug>


RadioPlayerSettings::RadioPlayerSettings()
{
  m_settings = new QSettings("plasma-light.org", "plasma-widget-radio");
  m_settingsPath = m_settings->fileName();
  qDebug() << "Settings path: " << m_settingsPath;
  
  m_radioStations = QStringList();
  delete m_settings;
  
  m_watcher =  new QFileSystemWatcher();
  m_watcher->addPath(m_settingsPath);
  connect(m_watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(onSettingsFileChanged(QString)));

  loadRadioStations();
  qDebug() << "Radio Player Settins created.";
}

RadioPlayerSettings::~RadioPlayerSettings()
{
  qDebug() << "Radio Player Settins destroyed.";
  delete m_watcher;
}



void RadioPlayerSettings::onSettingsFileChanged(const QString& file)
{
  qDebug() << "Settings file change detected, reloading RadioStations.";
  m_watcher->addPath(m_settingsPath);
  loadRadioStations();
  Q_EMIT(updated());
}

QVariantList RadioPlayerSettings::radioStations()
{
  QList<QVariant> list;
  for (QString radioStation:  m_radioStations)
    list << QVariant(radioStation);
  return list;
}

void RadioPlayerSettings::setRadioStations(QVariantList radioStations)
{
  QStringList list;
  for (QVariant radioStation:  radioStations)
    list << radioStation.toString();
  m_radioStations = list;

  saveRadioStations();
}

int RadioPlayerSettings::getRadioStationsSize()
{
  return m_radioStations.size();
}

QString RadioPlayerSettings::getRadioStationUrl(int idx)
{
  return m_radioStations.at(idx);
}


void RadioPlayerSettings::addRadioStation(QString radio)
{
  m_radioStations.push_back(radio);
  saveRadioStations();
  Q_EMIT(updated());
}

void RadioPlayerSettings::removeRadioStation(int idx)
{
  m_radioStations.removeAt(idx);
  saveRadioStations();
  Q_EMIT(updated());
}

void RadioPlayerSettings::saveRadioStations() {
  m_settings = new QSettings("plasma-light.org", "plasma-widget-radio");
  qDebug() << "Saving radio stations to: " << m_settings->fileName(); 
  int size = m_radioStations.size();
  m_settings->beginWriteArray("urls");
  for (int i = 0; i < size; ++i) {
    m_settings->setArrayIndex(i);
    m_settings->setValue("url", m_radioStations.at(i));
  }
  m_settings->endArray();
  m_settings->sync();
  
  delete m_settings;
}

void RadioPlayerSettings::loadRadioStations()
{
  m_radioStations = QStringList();
  m_settings = new QSettings("plasma-light.org", "plasma-widget-radio");
  qDebug() << "Loading radio stations from: " << m_settings->fileName(); 
  
  int size = m_settings->beginReadArray("urls");
  for (int i = 0; i < size; ++i) {
    m_settings->setArrayIndex(i);
    QString url = m_settings->value("url").toString();
    m_radioStations << url;
  }
  m_settings->endArray();
  delete m_settings;
}


#include "radioplayersettings.moc"
