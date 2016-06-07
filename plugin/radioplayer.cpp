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
    char ** args = NULL;
    _instance =  libvlc_new(0, args);
    libvlc_set_user_agent(_instance, "RadioPlayer Widget 0.1", "RadioPlayer/0.1");

    _mediaListPlayer = libvlc_media_list_player_new(_instance);
    createCoreConnections();

	_propagateEvents = true;
    qDebug() << "RadioPlayer: Crated.";
}

RadioPlayer::~RadioPlayer() {
    removeCoreConnections();
    libvlc_media_list_player_release(_mediaListPlayer);
    libvlc_release(_instance);
    qDebug() << "RadioPlayer: Destroyed.";
}


void RadioPlayer::createCoreConnections()
{
    // Connect MediaListPlayer Events
	Q_ASSERT(_mediaListPlayer);
    libvlc_event_manager_t* _vlcEvents = libvlc_media_list_player_event_manager(_mediaListPlayer);
    QList<libvlc_event_e> list;
    list << libvlc_MediaListPlayerPlayed
         << libvlc_MediaListPlayerNextItemSet
         << libvlc_MediaListPlayerStopped;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_attach(_vlcEvents, event, handleMediaListPlayerEvents, this);
    }
    
    // Connect MediaPlayer Events
    list.clear();
	libvlc_media_player_t * mp = libvlc_media_list_player_get_media_player(_mediaListPlayer);
    libvlc_event_manager_t * mpEvents = libvlc_media_player_event_manager(mp);
	libvlc_media_player_release(mp);
	
    list << libvlc_MediaPlayerEncounteredError
         << libvlc_MediaPlayerStopped
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPaused;
		 

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_attach(mpEvents, event, handleMediaPlayerEvents, this);
    }
    
}
RadioPlayer::State RadioPlayer::getState()
{
    switch (libvlc_media_list_player_get_state(_mediaListPlayer)) {
        case libvlc_NothingSpecial:
        case libvlc_Opening:
        case libvlc_Buffering:
            return RadioPlayer::RADIO_BUSY;
        case libvlc_Playing:
            return RadioPlayer::RADIO_PLAYING;
        case libvlc_Paused:
            return RadioPlayer::RADIO_PAUSED;
        case libvlc_Stopped:
        case libvlc_Ended:
            return RadioPlayer::RADIO_FINISHED;
        case libvlc_Error:
            return RadioPlayer::RADIO_ERROR;
    }
    return RadioPlayer::RADIO_READY;
}

QString RadioPlayer::getErrorMsg()
{
    return QString(libvlc_errmsg());
}


void RadioPlayer::removeCoreConnections()
{
    // Disconnect MediaListPlayer Events
    Q_ASSERT(_mediaListPlayer);
    libvlc_event_manager_t* _vlcEvents = libvlc_media_list_player_event_manager(_mediaListPlayer);
    QList<libvlc_event_e> list;
    list << libvlc_MediaListPlayerPlayed
         << libvlc_MediaListPlayerNextItemSet
         << libvlc_MediaListPlayerStopped;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_detach(_vlcEvents, event, handleMediaListPlayerEvents, this);
    }

    // Disconnect MediaPlayer Events
    list.clear();
	libvlc_media_player_t * mp = libvlc_media_list_player_get_media_player(_mediaListPlayer);
    libvlc_event_manager_t * mpEvents = libvlc_media_player_event_manager(mp);
	libvlc_media_player_release(mp);
	
    list << libvlc_MediaPlayerEncounteredError
         << libvlc_MediaPlayerStopped
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPaused;
		 

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_detach(mpEvents, event, handleMediaPlayerEvents, this);
	}
}

void RadioPlayer::togglePause()
{
    // qDebug() << "RadioPlayer: pause";
    libvlc_media_player_t *player = libvlc_media_list_player_get_media_player(_mediaListPlayer);
    libvlc_media_player_pause(player);
    libvlc_media_player_release(player);
}

void RadioPlayer::next()
{
    _listPosition ++;
    libvlc_media_list_player_play_item_at_index(_mediaListPlayer, _listPosition);
}

/**
 * Esure to release media after usage with libvlc_media_release.
 */
libvlc_media_t* RadioPlayer::getCurrentMedia()
{
    libvlc_media_player_t *player = libvlc_media_list_player_get_media_player(_mediaListPlayer);
    libvlc_media_t * media = libvlc_media_player_get_media(player);
    libvlc_media_player_release(player);
    return media;
}


QString RadioPlayer::getMediaArtworkUrl()
{
    libvlc_media_t * media = getCurrentMedia();
    if (media != NULL) {
        char * value = libvlc_media_get_meta(media, libvlc_meta_ArtworkURL);
        libvlc_media_release(media);
        if (value != NULL) {
            QString sValue(value);
            delete value;
            return sValue;
        }
    }
    
    return "../images/media-album-cover-manager-amarok.svg";
}

QString RadioPlayer::getMediaBitrate()
{
    QString bitRate = "";
    libvlc_media_t * media = getCurrentMedia();
    if (media != NULL) {
        libvlc_media_track_t ** tracks;
        unsigned int count = libvlc_media_tracks_get(media, &tracks);
        libvlc_media_release(media);
        if (count > 0) {
        bitRate += QString::number(tracks[0]->audio->i_rate / 1000);
        } else
            bitRate = "0";
        
        libvlc_media_tracks_release(tracks, count);
    }
    return bitRate;
}

QString RadioPlayer::getMediaTitle()
{
    libvlc_media_t * media = getCurrentMedia();
    if (media != NULL) {
        char * value = libvlc_media_get_meta(media, libvlc_meta_Title);
        libvlc_media_release(media);
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
    libvlc_media_t * media = getCurrentMedia();
    if (media != NULL) {
        char * value = libvlc_media_get_meta(media, libvlc_meta_Genre);
        libvlc_media_release(media);
        if (value != NULL) {
            QString sValue(value);
            delete value;
            return sValue;
        }
    }
    return "Unknown";
}


int RadioPlayer::getTrackCount()
{
    return _size;
}
int RadioPlayer::getCurrentTrack()
{
    return _listPosition;
}


void RadioPlayer::playMedia(QString url)
{
	_propagateEvents = false;
    libvlc_media_list_player_stop(_mediaListPlayer);

    qDebug() << "RadioPlayer: Playing media: " << url;

    libvlc_media_t *md;
    libvlc_media_list_t *ml;

    md = libvlc_media_new_location(_instance, url.toLocal8Bit());
    Q_ASSERT(md);

    libvlc_media_parse(md);

    ml = libvlc_media_list_new (_instance);
    libvlc_media_list_t* subitems = libvlc_media_subitems(md);
    int subitemsCount = libvlc_media_list_count(subitems);
    if (subitemsCount > 0) {
        qDebug() << "RadioPlayer: Current media subitems count " << subitemsCount;

        _listPosition = 0;
        _size = subitemsCount;

        for (int i = 0; i < subitemsCount; i ++) {
            libvlc_media_t * media = libvlc_media_list_item_at_index(subitems, i);
            libvlc_media_list_add_media(ml, media);
            libvlc_media_release(media);
        }
        
        libvlc_media_list_release(subitems);
    } else {
        _size = 1;
        _listPosition = 1;
        Q_ASSERT (ml != NULL);
        libvlc_media_list_add_media (ml, md);
        libvlc_media_release(md);
    }


    
    Q_ASSERT(_mediaListPlayer != NULL);
	Q_ASSERT(ml != NULL);
    libvlc_media_list_player_set_media_list (_mediaListPlayer, ml);
    libvlc_media_list_release(ml);

	_propagateEvents = true;
    libvlc_media_list_player_play(_mediaListPlayer);
}

void RadioPlayer::handleMediaListPlayerEvents(const struct libvlc_event_t * event, void * userData)
{
    // qDebug() << "RadioPlayer: MediaListPlayerEvent recieved " << libvlc_event_type_name(event->type);
    RadioPlayer * rp = (RadioPlayer*) userData;
    if (rp == NULL) {
        qWarning() << "RadioPlayer: Unable to cast userData to RadioPlayer instance" ;
        return;
    }

    switch(event->type)    {
    case libvlc_MediaListPlayerPlayed:
		if (rp->_propagateEvents)
			emit rp->played();
        break;
    case libvlc_MediaListPlayerNextItemSet:
        qDebug () << event->u.media_list_player_next_item_set.item;
        if (event->u.media_list_player_next_item_set.item == NULL) {
            if (rp->_listPosition < rp->_size) {
				if (rp->_propagateEvents)
					Q_EMIT(rp->stopped());
			}else {
                if (rp->_propagateEvents)
					Q_EMIT(rp->finished());
			}
        }
        break;
    case libvlc_MediaListPlayerStopped:
		if (rp->_propagateEvents)
			emit rp->stopped();
        break;
    default:
        break;
    }
}

void RadioPlayer::handleMediaPlayerEvents(const libvlc_event_t* event, void* userData)
{
    // qDebug() << "RadioPlayer: MediaPlayerEvent recieved " << libvlc_event_type_name(event->type);
    RadioPlayer * rp = (RadioPlayer*) userData;
    if (rp == NULL) {
        qWarning() << "RadioPlayer: Unable to cast userData to RadioPlayer instance" ;
        return;
    }
    
    switch(event->type) {
		case libvlc_MediaPlayerPlaying:
			if (rp->_propagateEvents)
				Q_EMIT(rp->updateInfo());
			break;
	}
}

#include "radioplayer.moc"
