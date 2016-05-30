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

#include <vlc/vlc.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_media_list.h>


RadioPlayer::RadioPlayer()
{
    char ** args;
    _instance =  libvlc_new(0, args);
    libvlc_set_user_agent(_instance, "RadioPlayer Widget 0.1", "RadioPlayer/0.1");

    _mediaPlayer = libvlc_media_player_new(_instance);
    _mediaListPlayer = libvlc_media_list_player_new(_instance);
    _mediaList = NULL;
    _currentMedia = NULL;

    libvlc_media_list_player_set_media_player(_mediaListPlayer, _mediaPlayer);
    // Listen for media list player events
    libvlc_event_manager_t * eventManager= libvlc_media_list_player_event_manager(_mediaListPlayer);

    eventManager = libvlc_media_player_event_manager(_mediaPlayer);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerPlaying, handlePlaying, this);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerPlaying, handleEnd, this);

    qDebug() << "RadioPlayer: Crated.";
}

RadioPlayer::~RadioPlayer() {
    libvlc_event_manager_t * eventManager= libvlc_media_list_player_event_manager(_mediaListPlayer);

    libvlc_media_player_release(_mediaPlayer);
    libvlc_media_list_player_release(_mediaListPlayer);
    libvlc_media_list_release(_mediaList);
    libvlc_release(_instance);
    qDebug() << "RadioPlayer: Destroyed.";
}

void RadioPlayer::play()
{
    qDebug() << "RadioPlayer: Play.";
    if (_currentMedia != NULL)
      libvlc_media_list_player_play_item(_mediaListPlayer, _currentMedia);
}

void RadioPlayer::togglePause()
{
    qDebug() << "RadioPlayer: pause";
    libvlc_media_player_pause(_mediaPlayer);
}

void RadioPlayer::backward()
{
    libvlc_media_list_player_previous(_mediaListPlayer);
}

void RadioPlayer::forward()
{
    libvlc_media_list_player_next(_mediaListPlayer);
}

QString RadioPlayer::getMediaArtworkUrl()
{
    libvlc_media_player_t *player = libvlc_media_list_player_get_media_player(_mediaListPlayer);
    libvlc_media_t * media = libvlc_media_player_get_media(player);

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
    return QString::number(libvlc_media_player_get_fps(_mediaPlayer));
}

QString RadioPlayer::getMediaTitle()
{
    libvlc_media_t * media = libvlc_media_player_get_media(_mediaPlayer);
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
    libvlc_media_player_t *player = libvlc_media_list_player_get_media_player(_mediaListPlayer);
    libvlc_media_t * media = libvlc_media_player_get_media(player);
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
    return libvlc_media_list_count(_mediaList);
}


void RadioPlayer::playMedia(QString url)
{
    qDebug() << "RadioPlayer: Playing media: " << url;

    if (_mediaListPlayer != NULL)
        libvlc_media_list_player_stop(_mediaListPlayer);


    if (_mediaList != NULL)
        libvlc_media_list_release(_mediaList);

    _mediaList = libvlc_media_list_new(_instance);


    libvlc_media_t * media = libvlc_media_new_location(_instance, url.toLocal8Bit());
    _currentMedia = media;
    libvlc_media_list_add_media(_mediaList, media);

    libvlc_media_list_player_set_media_list(_mediaListPlayer, _mediaList);
    libvlc_media_list_player_play(_mediaListPlayer);
}

void RadioPlayer::handlePlaying(const struct libvlc_event_t * event, void * userData)
{
    qDebug() << "RadioPlayer: handlePlaying";
    RadioPlayer * rp = (RadioPlayer*) userData;
    if (rp == NULL) {
        qWarning() << "RadioPlayer: Unable to cast userData to RadioPlayer instance" ;
        return;
    }
    
    // Detect if the current media is a playlist, and add its items to the list
    libvlc_media_list_t * subItems = libvlc_media_subitems(rp->_currentMedia);
    if (libvlc_media_list_count(subItems) > 0 ) {
      qDebug() << "RadioPlayer: played media has subitems: " << libvlc_media_list_count(subItems);

      libvlc_media_list_player_set_media_list(rp->_mediaListPlayer, rp->_mediaList);
      for (int i =0; i < libvlc_media_list_count(subItems); i++) {
	libvlc_media_t * media = libvlc_media_list_item_at_index(subItems, i);
	qDebug() << "Adding " << libvlc_media_get_mrl(media);
	libvlc_media_list_add_media(rp->_mediaList, media);
      }
    }
    
    // Update Current Media
    rp->_currentMedia = libvlc_media_player_get_media(rp->_mediaPlayer);

    if (libvlc_media_get_state(rp->_currentMedia) == libvlc_Playing) {
        //qDebug() << "RadioPlayer: UpdateInfo SIGNAL triggered";
        Q_EMIT(rp->updateInfo());
    }
}

void RadioPlayer::handleEnd(const struct libvlc_event_t * event, void * userData)
{
    qDebug() << "RadioPlayer: handleEnd";
    RadioPlayer * rp = (RadioPlayer*) userData;
    if (rp == NULL) {
        qWarning() << "RadioPlayer: Unable to cast userData to RadioPlayer instance" ;
        return;
    }


    int idx = libvlc_media_list_index_of_item(rp->_mediaList, rp->_currentMedia);
    qDebug() << "RadioPlayer:\n\t\tCurrent media: " << libvlc_media_get_mrl(rp->_currentMedia) << "\n\t\tIdx: " << idx;
    if (idx < libvlc_media_list_count(rp->_mediaList)-1)
      libvlc_media_list_player_next(rp->_mediaListPlayer);
    else {
      // qDebug() << "RadioPlayer: finished SIGNAL triggered";
      Q_EMIT(rp->finished());
    }
}


#include "radioplayer.moc"
