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

RadioPlayer::RadioPlayer()
{
  /* Load the VLC engine */
  inst = libvlc_new (0, NULL);
  libvlc_set_user_agent(inst, "RadioPlayer Widget 0.1", "RadioPlayer/0.1");
  mp = NULL;
}

RadioPlayer::~RadioPlayer() {
  libvlc_media_player_release (mp);
  libvlc_release (inst);
}
void RadioPlayer::setMedia(QString url)
{
  if (mp)
    libvlc_media_player_release (mp);
  
  libvlc_media_t *m;
  m = libvlc_media_new_location(inst, url.toLocal8Bit());
  /* Create a media player playing environement */
  mp = libvlc_media_player_new_from_media (m);
  
  /* No need to keep the media now */
  libvlc_media_release (m);
}

void RadioPlayer::play()
{
  // play the media_player
  libvlc_media_player_play (mp);
}

void RadioPlayer::pause()
{
  libvlc_media_player_stop (mp);
}



#include "radioplayer.moc"
