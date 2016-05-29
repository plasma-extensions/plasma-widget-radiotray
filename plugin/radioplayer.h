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

#ifndef RADIOPLAYER_H
#define RADIOPLAYER_H

#include <QObject>
#include <QStringList>
#include <QPixmap>

class VlcInstance;
class VlcMediaList;
class VlcMediaPlayer;
class VlcMediaListPlayer;
class VlcMetaManager;
class VlcMedia;

#include <vlc-qt/Enums.h>

struct libvlc_media_t;

class RadioPlayer : public QObject
{
    Q_OBJECT

public:
    RadioPlayer();
    ~RadioPlayer();
    
    static void handleMetaChangedEvent(const struct libvlc_event_t * event, void *player);

public Q_SLOTS:
  
  void play();
  void pause();
  void forward();
  void backward();
  
  QString getMediaTitle();
  QString getMediaGenre();
  QString getMediaArtworkUrl();
  QString getMediaBitrate();
  
  void playMedia(QString url);
  int getMediaListSize();
  
  void handlePlaying();
   
Q_SIGNALS:
   void played();
   void stopped();
   void updateInfo();
   
private:
    VlcInstance *_instance;
    VlcMediaListPlayer * _mediaListPlayer;
    VlcMediaList * _mediaList;
    // VlcMediaPlayer *_player;
    int _listidx;
    
    // VlcMedia * _currentMedia;
    libvlc_media_t * _currentMedia;
    //VlcMetaManager * _metaManager;
    
    // int wait_time=5000;
    QString _settingsFile;
};

#endif // RADIOPLAYER_H
