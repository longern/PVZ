#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QMediaPlayer>

class Sound : public QObject
{
	Q_OBJECT
private:
	Sound();

public:
	static Sound *ins();
	void play(const QString &source = QString());
	void setVolume(int volume);

signals:

public slots:

private:
	static Sound *sInstance;

	QMediaPlayer *player;
};

#endif // SOUND_H
