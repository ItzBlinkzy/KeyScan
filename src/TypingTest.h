#pragma once
#include <QObject>
#include <QHBoxLayout>
#include <QVector>
#include <QWidget>

class TypingTest : public QObject
{
	Q_OBJECT


public:
	explicit TypingTest(QWidget* words_widget, QObject* parent = nullptr);
	~TypingTest();
	
	enum GameType {Standard, Professional, Extreme};
	void handleTyping();
	void startTest();
private:
	void drawWords(QVector<QString> words);
	QVector<QString> getWordsFromFile();
	QVector<QString> generateTest(GameType game);
	QWidget* words_widget;
	QVector<QString> words;
	bool is_playing = false;
};
