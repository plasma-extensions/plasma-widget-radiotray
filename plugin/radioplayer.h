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


struct libvlc_instance_t;
struct libvlc_media_t;
struct libvlc_media_list_t;    
struct libvlc_media_player_t;
struct libvlc_media_list_player_t;

    
class RadioPlayer : public QObject
{
    Q_OBJECT

public:
    RadioPlayer();
    ~RadioPlayer();
    
public Q_SLOTS:
  
  void play();
  void togglePause();
  void forward();
  void backward();
  
  QString getMediaTitle();
  QString getMediaGenre();
  QString getMediaArtworkUrl();
  QString getMediaBitrate();
  
  void playMedia(QString url);
  int getMediaListSize();
  
private:
  static void handlePlaying(const struct libvlc_event_t * event, void * userData);
  static void handleEnd(const struct libvlc_event_t * event, void * userData);
   
Q_SIGNALS:
   void played();
   void finished();
   void updateInfo();
   
private:
    libvlc_instance_t *_instance;
    libvlc_media_player_t * _mediaPlayer;
    libvlc_media_list_player_t * _mediaListPlayer;
    libvlc_media_list_t * _mediaList;
    libvlc_media_t * _currentMedia;
    
    int _listidx;
    
    // VlcMedia * _currentMedia;
    //VlcMetaManager * _metaManager;
    
    /**
     * PlayList media items are not handled very well by the VLCPlayList so we must
     * make an small "cheat" to overcome this issue and detect when our playlist is
     * ended, so we count the times a item is played and the times a
     */
    int _playListExecution;
    //int wait_time=5000;
    QString _settingsFile;
};

#endif // RADIOPLAYER_H
