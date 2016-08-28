#include "sound.h"

Sound *Sound::sInstance = nullptr;

Sound::Sound()
{
	player = new QMediaPlayer(this);
}

Sound *Sound::ins()
{
	if (sInstance == nullptr)
		sInstance = new Sound;
	return sInstance;
}

void Sound::play(const QString &source)
{
	if (!source.isEmpty())
		player->setMedia(QUrl(source));
	player->play();
}
