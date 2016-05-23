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
    
    connect(_mediaListPlayer, SIGNAL(nextItemSet(libvlc_media_t*)),  this,  SLOT(handleNextItemSet(libvlc_media_t*)) );
    qDebug() << "Object Created";
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

QString RadioPlayer::getCurrentTrackArtworkUrl()
{
  if (_currentMedia != NULL) {
    char * value = libvlc_media_get_meta(_currentMedia, libvlc_meta_ArtworkURL);
    if (value != NULL) {
      QString sValue(value);
      delete value;
      return sValue;
    } 
  }
  //FIXME: RETURN DEFAULT ARTWORK IMAGE
  return "";
}

QString RadioPlayer::getCurrentTrackBitrate()
{
    return "48 kb/s";
}

QString RadioPlayer::getCurrentTrackTitle()
{
  if (_currentMedia != NULL) {
    char * value = libvlc_media_get_meta(_currentMedia, libvlc_meta_Title);
    if (value != NULL) {
      QString sValue(value);
      delete value;
      return sValue;
    } 
  }
  return "Unknown";
}

QString RadioPlayer::getCurrentTrackGenre()
{
  if (_currentMedia != NULL) {
    char * value = libvlc_media_get_meta(_currentMedia, libvlc_meta_Genre);
    if (value != NULL) {
      QString sValue(value);
      delete value;
      return sValue;
    } 
  }
  return "Unknown";
}


QStringList RadioPlayer::getPlaylist()
{
    QStringList list;
    for (int i = 0; i < _mediaList->count(); i ++)
        list << _mediaList->at(i)->currentLocation();

    return list;
}


void RadioPlayer::addMedia(QString url)
{
  _mediaList->addMedia(new VlcMedia(url, _instance));
}

void RadioPlayer::removeMedia(int idx)
{
    _mediaList->removeMedia(idx);
}



void RadioPlayer::handleNextItemSet(libvlc_media_t* mediaCore)
{
  QString url(libvlc_media_get_mrl(mediaCore));
  qDebug() << "CURRENT MEDIA ITEM: " << url;
  libvlc_event_manager_t *eventManager = libvlc_media_event_manager(mediaCore);
  libvlc_event_attach(eventManager, libvlc_MediaMetaChanged, &RadioPlayer::handleMetaChangedEvent, this);
  
  _currentMedia = mediaCore; 
}

void RadioPlayer::handleMetaChangedEvent(const libvlc_event_t* event, void* player)
{
  if (event->type == libvlc_MediaMetaChanged) {
    RadioPlayer *rp = (RadioPlayer*) player;
    Q_EMIT( rp->updateInfo());
  }
}

#include "radioplayer.moc"
