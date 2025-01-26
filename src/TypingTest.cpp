#include "TypingTest.h"
#include <algorithm>
#include <random>
#include <vector>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <QDir>
#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include "KeyScan.h"

TypingTest::TypingTest(QWidget* words_widget, QObject* parent)
    : QObject(parent),
    words_widget(words_widget)
{
    all_words = getWordsFromFile();
}

TypingTest::~TypingTest() {

}

void TypingTest::handleTyping()
{

}

void TypingTest::startTest()
{
    if (is_playing) {
        resetGame();
        return;
    }

    startGame();
}

void TypingTest::startGame() {
    QVector<QString> results = generateTest(GameType::Standard);
    generated_words = results;
    is_playing = true;

    drawWords(results);
}
void TypingTest::resetGame() {
    KeyScan::clearLayout(words_widget->layout());

    startGame();
}

void TypingTest::drawWords(QVector<QString> gen_words) {
    QGraphicsScene* scene = new QGraphicsScene(words_widget);
    QGraphicsView* view = new QGraphicsView(scene, words_widget);

    view->setAlignment(Qt::AlignLeft | Qt::AlignTop); // Align the view to the top-left
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);


    QLayout* layout = words_widget->layout();
    if (layout) {
        while (QLayoutItem* item = layout->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                delete widget;
            }
            delete item;
        }
        layout->addWidget(view);
    }
    else {
        QVBoxLayout* newLayout = new QVBoxLayout(words_widget);
        newLayout->addWidget(view);
    }

    // Positioning setup
    int x = 0, y = 0;
    const int letterSpacing = 0; 
    const int wordSpacing = 20;  
    const int rowSpacing = 10;  
    const int viewWidth = words_widget->width(); 

    QFont font("Arial", 35);
    QFontMetrics fontMetrics(font);

    for (const QString& word : gen_words) {
        int wordWidth = 0;
        for (const QChar& letter : word) {
            wordWidth += fontMetrics.horizontalAdvance(letter) + letterSpacing;
        }
        wordWidth -= letterSpacing; 

        // wrap to next row if full word doesnt fit in the current row
        if (x + wordWidth > viewWidth) {
            x = 0;
            y += fontMetrics.height() + rowSpacing;
        }

        // Add each letter of the word
        for (const QChar& letter : word) {
            // QGraphicsTextItem for each letter to manipulate for colours etc.
            QGraphicsTextItem* letterItem = scene->addText(QString(letter));
            letterItem->setFont(font);
            letterItem->setDefaultTextColor(Qt::black);

            int letterWidth = fontMetrics.horizontalAdvance(letter);

            // Set position
            letterItem->setPos(x, y);

            // Move x for the next letter
            x += letterWidth + letterSpacing;
        }

        // add space after the word
        x += wordSpacing;
    }

    scene->setSceneRect(0, 0, viewWidth, y + fontMetrics.height());
}


void TypingTest::keyPressEvent(QKeyEvent* event) {

    qDebug() << "A BUTTON WAS PRESSED ON TYPING TEST GONNA TRACK";
}


QVector<QString> TypingTest::getWordsFromFile()
{
	QVector<QString> all_words;
	QString file_path = "./resources/english1k.txt";
	QFile file(file_path);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Unable to open file:" << file_path;
		return all_words;
	}

	QTextStream in(&file);

	while (!in.atEnd()) {
		QString line = in.readLine().trimmed();
		if (!line.isEmpty()) {
			all_words.append(line);
		}
	}

	file.close();
	return all_words;
}

QVector<QString> TypingTest::generateTest(GameType game) {

    QVector<QString> generated_words;
    size_t num_words;

    if (game == GameType::Standard) {
        num_words = 36;
    }

    else if (game == GameType::Professional) {
        num_words = 48;
    }
    else {
        num_words = 60;
    }


    std::vector<QString> temp_out;
    std::sample(
        all_words.begin(),
        all_words.end(),
        std::back_inserter(temp_out),
        num_words,
        std::mt19937{ std::random_device{}() }
    );

    // move results from std::vector to  qvector
    for (const auto& word : temp_out) {
        generated_words.append(word);
    }

    return generated_words;
}