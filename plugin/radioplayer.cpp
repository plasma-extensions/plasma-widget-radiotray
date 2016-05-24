/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "radioplayer.h"

#include <QStringList>
#include <QSettings>
#include <QApplication>

#include <QDebug>

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaList.h>
#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/MediaListPlayer.h>
#include <vlc-qt/MetaManager.h>

#include <vlc/vlc.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_media_list.h>


RadioPlayer::RadioPlayer()
{
    _instance =  new VlcInstance(QStringList());
    _instance->setUserAgent("RadioPlayer Widget 0.1", "RadioPlayer/0.1");
    
    _mediaList = new VlcMediaList(_instance);
    _mediaListPlayer = new VlcMediaListPlayer(_instance);
    _mediaListPlayer->setMediaList(_mediaList);
    
    //_metaManager = NULL;
    _currentMedia = NULL;
    
    //connect(_mediaListPlayer, SIGNAL(nextItemSet(libvlc_media_t*)),  this,  SLOT(handleNextItemSet(libvlc_media_t*)) );
    //connect(_mediaListPlayer->mediaPlayer(), SIGNAL(mediaChanged(libvlc_media_t*)),  this,  SLOT(handleNextItemSet(libvlc_media_t*)) );
    connect(_mediaListPlayer->mediaPlayer(), SIGNAL(playing()),  this,  SLOT(handlePlaying()) );
    qDebug() << "Object Created";
    
    _settingsFile = QApplication::applicationDirPath().left(1) + ":/radioplayersettings.desktop";
    _listidx = 0;
    loadPlayList();
}

RadioPlayer::~RadioPlayer() {
    delete _mediaListPlayer;
    delete _mediaList;
    delete _instance;
}

void RadioPlayer::play()
{
  _mediaListPlayer->play();
}

void RadioPlayer::pause()
{
    _mediaListPlayer->stop();
}

void RadioPlayer::backward()
{
  _mediaListPlayer->previous();
}

void RadioPlayer::forward()
{
  _mediaListPlayer->next();
}

QString RadioPlayer::getMediaArtworkUrl()
{
  libvlc_media_t * media = _mediaListPlayer->mediaPlayer()->currentMediaCore();
  if (media != NULL) {
    char * value = libvlc_media_get_meta(media, libvlc_meta_ArtworkURL);
    if (value != NULL) {
      QString sValue(value);
      delete value;
      return sValue;
    } 
  }
  //FIXME: RETURN DEFAULT ARTWORK IMAGE
  return "media-album-cover-manager-amarok";
}

QString RadioPlayer::getMediaBitrate()
{
  libvlc_media_t * media = _mediaListPlayer->mediaPlayer()->currentMediaCore();
  if (media != NULL) {
    libvlc_media_stats_t stats;
    if (libvlc_media_get_stats(media, &stats) ) {
      return QString::number(stats.f_input_bitrate);
    }
  }
  return "--kb";
}

QString RadioPlayer::getMediaTitle()
{
  libvlc_media_t * media = _mediaListPlayer->mediaPlayer()->currentMediaCore();
  if (media != NULL) {
    char * value = libvlc_media_get_meta(media, libvlc_meta_Title);
    if (value != NULL) {
      QString sValue(value);
      delete value;
      return sValue;
    } 
  }
  return "Unknown";
}

QString RadioPlayer::getMediaGenre()
{
  libvlc_media_t * media = _mediaListPlayer->mediaPlayer()->currentMediaCore();
  if (media != NULL) {
    char * value = libvlc_media_get_meta(media, libvlc_meta_Genre);
    if (value != NULL) {
      QString sValue(value);
      delete value;
      return sValue;
    } 
  }
  return "Unknown";
}


int RadioPlayer::getMediaListSize()
{   
   return _mediaList->count();
}


void RadioPlayer::addMedia(QString url)
{
  _mediaList->addMedia(new VlcMedia(url, _instance));
}

void RadioPlayer::playMedia(QString url)
{
  _mediaListPlayer->stop();
  delete _mediaList;
  _mediaList = new VlcMediaList(_instance);
  _mediaList->addMedia(new VlcMedia(url, _instance));
  _mediaListPlayer->setMediaList(_mediaList);
  _mediaListPlayer->play();
}


void RadioPlayer::removeMedia(int idx)
{
    _mediaList->removeMedia(idx);
}



void RadioPlayer::handleNextItemSet(libvlc_media_t* mediaCore)
{
  _currentMedia = NULL;
  QString url(libvlc_media_get_mrl(mediaCore));
  qDebug() << "CURRENT MEDIA ITEM: " << url;
  qDebug() << "CURRENT MEDIA ITEM STATE: " << libvlc_media_get_state(mediaCore);
  
  if (libvlc_media_get_state(mediaCore) != libvlc_NothingSpecial) {
    libvlc_event_manager_t * eventManager = libvlc_media_event_manager(mediaCore);
    libvlc_event_attach(eventManager, libvlc_MediaMetaChanged, &RadioPlayer::handleMetaChangedEvent, this);
    _currentMedia = mediaCore;   
  }
}

void RadioPlayer::handlePlaying()
{
  _currentMedia = NULL;
  libvlc_media_t *mediaCore = _mediaListPlayer->mediaPlayer()->currentMediaCore();
  QString url(libvlc_media_get_mrl(mediaCore));
  qDebug() << "CURRENT MEDIA ITEM: " << url;
  qDebug() << "CURRENT MEDIA ITEM STATE: " << libvlc_media_get_state(mediaCore);
  
  if (libvlc_media_get_state(mediaCore) == libvlc_Playing) {
    _currentMedia = mediaCore;   
    qDebug() << "TRIGERING SIGNAL";
    Q_EMIT( updateInfo());
  }
}


void RadioPlayer::handleMetaChangedEvent(const libvlc_event_t* event, void* player)
{
  if (event->type == libvlc_MediaMetaChanged) {
    RadioPlayer *rp = (RadioPlayer*) player;
    Q_EMIT( rp->updateInfo());
  }
}

QString RadioPlayer::getListItemUrl(int idx)
{
  return _mediaList->at(idx)->currentLocation();
}


int RadioPlayer::getCurrentMediaIdx()
{
  return _listidx;
}

void RadioPlayer::loadPlayList()
{
  QSettings settings(_settingsFile, QSettings::NativeFormat);
  int size = settings.beginReadArray("urls");
  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);
    QString url = settings.value("url").toString();
    
    VlcMedia *media = new VlcMedia(url, _instance);
    _mediaList->addMedia(media);
    delete media;
  }
}
void RadioPlayer::savePlayList()
{
  QSettings settings(_settingsFile, QSettings::NativeFormat);
  int size = _mediaList->count();
  settings.beginWriteArray("urls");
  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);
    VlcMedia *media = _mediaList->at(i);
    settings.setValue("url", media->currentLocation());
  }
}


#include "radioplayer.moc"
