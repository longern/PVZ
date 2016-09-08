#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class Sound : public QObject
{
	Q_OBJECT
private:
	Sound();

public:
	static Sound *ins();
	void play(const QString &source = QString());
	void playSound(const QString &source);
	void setVolume(int volume);

signals:

public slots:
	void stop();

private:
	static Sound *sInstance;

	QMediaPlayer *player;
	QMediaPlaylist *playlist;
};

#endif // SOUND_H
