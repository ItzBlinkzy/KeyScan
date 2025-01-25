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
    if (is_playing) return;

    QVector<QString> results = generateTest(GameType::Standard);
    // watch out here
    generated_words = results;
    is_playing = true;

    drawWords(results);
     
}

void TypingTest::drawWords(QVector<QString> gen_words) {
    // Create a new QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene(words_widget);

    // Create a QGraphicsView to display the scene
    QGraphicsView* view = new QGraphicsView(scene, words_widget);

    // Set up the QGraphicsView
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

    // positioning stuffs
    int x = 0, y = 0;                 
    const int wordSpacing = 20;          
    const int rowSpacing = 10;          
    const int viewWidth = words_widget->width(); // Available width for words

    // add each word to the graphics scene
    for (const QString& word : gen_words) {
        QGraphicsTextItem* textItem = scene->addText(word);

        // font, color and font size
        QFont font("Arial", 35); 
        textItem->setFont(font);
        textItem->setDefaultTextColor(Qt::black);

        textItem->setPos(x, y);

        x += textItem->boundingRect().width() + wordSpacing;

        // wrap next row if word exceeds the view width
        if (x > viewWidth - textItem->boundingRect().width()) {
            x = 0;                
            y += textItem->boundingRect().height() + rowSpacing; // move to next row
        }
    }

    scene->setSceneRect(0, 0, viewWidth, y + 50); 
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