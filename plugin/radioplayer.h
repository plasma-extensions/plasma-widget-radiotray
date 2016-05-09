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

#include <vlc/libvlc.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_media_player.h>

#include <QObject>



class RadioPlayer : public QObject
{
    Q_OBJECT

public:
    RadioPlayer();
    ~RadioPlayer();

public Q_SLOTS:
  void setMedia(QString url);
  void play();
  void pause();
  
  
private:
     libvlc_instance_t * inst;
     libvlc_media_player_t *mp;

     libvlc_time_t length;
     int wait_time=5000;
};

#endif // RADIOPLAYER_H
