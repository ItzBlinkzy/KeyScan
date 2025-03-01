#pragma once
#include <QObject>
#include <QHBoxLayout>
#include <QVector>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <QStackedWidget>
#include <QRadioButton>

class TypingTest: public QObject
{
	Q_OBJECT


public:
	explicit TypingTest(
		QWidget* words_widget,
		QLabel* time_label,
		QLabel* duration_label,
		QLabel* wpm_label,
		QLabel* accuracy_label,
		QStackedWidget* stacked_widget,
		QRadioButton* radio_standard,
		QRadioButton* radio_pro,
		QRadioButton* radio_extreme,
		QObject* parent = nullptr
	);
	~TypingTest();
	
	enum GameType {Standard, Professional, Extreme};
	void startTest();
	void startGame();
	void endGame();
	void resetGame();
	void keyPressEvent(QKeyEvent* event);
private:
	void drawWords(QVector<QString> words);
	void cursorNext();
	void cursorPrev();
	QVector<QString> getWordsFromFile();
	QVector<QString> generateTest(GameType game);
	void updateDisplay();
	void showStatsDisplay();
	std::optional<std::tuple<int, int, QChar>> getCurrentLetterInfo() const;
	void updateTimer();
	double calculateAccuracy() const;
	QWidget* words_widget;
	QVector<QString> all_words;
	QVector<QString> generated_words;
	bool is_playing = false;
	bool is_timer_started = false;
	uint16_t cursor = 0;

	uint16_t total_chars = 0;
	QGraphicsScene* scene = nullptr;
	QVector<QVector<QGraphicsTextItem*>> wordItems;
	QMap<int, bool> letter_states;
	QFont font;
	QFontMetrics fontMetrics;
	QGraphicsTextItem* cursorItem = nullptr;  // Store the cursor item
	QTimer* timer = new QTimer(this);
	QElapsedTimer* elapsed_timer = new QElapsedTimer();
	QLabel* time_label;
	QLabel* duration_label;
	QLabel* accuracy_label;
	QLabel* wpm_label;
	QStackedWidget* stacked_widget;
	QRadioButton* radio_standard;
	QRadioButton* radio_pro;
	QRadioButton* radio_extreme;
};
