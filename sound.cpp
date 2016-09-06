#include "sound.h"

Sound *Sound::sInstance = nullptr;

Sound::Sound()
{
	player = new QMediaPlayer(this);
	player->setVolume(100);
	playlist = nullptr;
}

Sound *Sound::ins()
{
	if (sInstance == nullptr)
		sInstance = new Sound;
	return sInstance;
}

void Sound::play(const QString &source)
{
	delete playlist;
	playlist = new QMediaPlaylist;
	playlist->addMedia(QUrl(source));
	playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	if (!source.isEmpty())
		player->setPlaylist(playlist);
	player->play();
}

void Sound::stop()
{
	player->stop();
}
