#pragma once
#include <QObject>
#include <QVector>

class TypingTest : public QObject
{
	Q_OBJECT


public:
	explicit TypingTest(QObject* parent = nullptr);
	~TypingTest();
	
	enum GameType {Standard, Professional, Extreme};
	void handleTyping();
	void startTest();
private:
	void drawWords();
	QVector<QString> getWordsFromFile();
	QVector<QString> generateTest(GameType game);

	QVector<QString> words;
};
