#pragma once
#include <QObject>
#include <QHBoxLayout>
#include <QVector>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>


class TypingTest : public QObject
{
	Q_OBJECT


public:
	explicit TypingTest(QWidget* words_widget, QObject* parent = nullptr);
	~TypingTest();
	
	enum GameType {Standard, Professional, Extreme};
	void startTest();
	void startGame();
	void resetGame();
	void keyPressEvent(QKeyEvent* event);
private:
	void drawWords(QVector<QString> words);
	void cursorNext();
	void cursorPrev();
	QVector<QString> getWordsFromFile();
	QVector<QString> generateTest(GameType game);
	void updateDisplay();
	std::optional<std::tuple<int, int, QChar>> getCurrentLetterInfo() const;
	QWidget* words_widget;
	QVector<QString> all_words;
	QVector<QString> generated_words;
	bool is_playing = false;
	uint16_t cursor = 0;

	// New members:
	QGraphicsScene* scene = nullptr;
	// Store each word’s letter items. wordItems[i][j] is letter j of word i.
	QVector<QVector<QGraphicsTextItem*>> wordItems;
	QMap<int, bool> letterStates;
	QFont font;
	QFontMetrics fontMetrics;
	QGraphicsTextItem* cursorItem = nullptr;  // Store the cursor item
};
