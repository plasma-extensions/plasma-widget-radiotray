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
struct libvlc_event_manager_t;
    
class RadioPlayer : public QObject
{
    Q_OBJECT

    enum State{
      RADIO_READY = 0,
      RADIO_BUSY = 0,
      RADIO_PLAYING,
      RADIO_PAUSED,
      RADIO_FINISHED,
      RADIO_ERROR
    };
public:
    RadioPlayer();
    ~RadioPlayer();
    
public Q_SLOTS:
  
  void next();
  void togglePause();

  State getState();
  QString getErrorMsg();
  
  QString getMediaTitle();
  QString getMediaGenre();
  QString getMediaArtworkUrl();
  QString getMediaBitrate();
  
  void playMedia(QString url);
  int getCurrentTrack();
  int getTrackCount();
  
private:
  static void handleMediaListPlayerEvents(const struct libvlc_event_t * event, void * userData);
  static void handleMediaPlayerEvents(const struct libvlc_event_t * event, void * userData);
  
  void createCoreConnections();
  void removeCoreConnections();
  libvlc_media_t * getCurrentMedia();
  
Q_SIGNALS:
   void played();
   void stopped();
   void finished();
   void updateInfo();
   
private:
    libvlc_instance_t *_instance;
    libvlc_media_list_player_t * _mediaListPlayer;
    
    int _listPosition;
    int _size;
    State _state;
    bool _propagateEvents;
    QString _settingsFile;
};

#endif // RADIOPLAYER_H
