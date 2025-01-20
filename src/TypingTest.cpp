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


TypingTest::TypingTest(QWidget* words_widget, QObject* parent)
    : QObject(parent),
    words_widget(words_widget)
{
    words = getWordsFromFile();
}

TypingTest::~TypingTest() {

}

void TypingTest::handleTyping()
{
}

void TypingTest::startTest()
{
    if (is_playing) return;

    auto results = generateTest(GameType::Standard);
       
    for (auto it = results.begin(); it != results.end(); it++) {
        qDebug() << *it << " ";
    }

    is_playing = true;

    drawWords(results);
     
}

void TypingTest::drawWords(QVector<QString> words) {

    auto generated_words = generateTest(GameType::Standard);


    // this is so bad holy shit (im gonna change it later i just want a fast easy way to do this)
    QHBoxLayout* row_1 = words_widget->findChild<QHBoxLayout*>("words_row1");
    QHBoxLayout* row_2 = words_widget->findChild<QHBoxLayout*>("words_row2");
    QHBoxLayout* row_3 = words_widget->findChild<QHBoxLayout*>("words_row3");
    QHBoxLayout* row_4 = words_widget->findChild<QHBoxLayout*>("words_row4");
    QHBoxLayout* row_5 = words_widget->findChild<QHBoxLayout*>("words_row5");
    QHBoxLayout* row_6 = words_widget->findChild<QHBoxLayout*>("words_row6");

    QList<QHBoxLayout*> rows = { row_1, row_2, row_3, row_4, row_5, row_6 };  
    const int wordsPerRow = 10; 

    int rowIndex = 0;  
    int wordCount = 0;

    for (int i = 0; i < generated_words.size(); ++i) {
        // dont go beyond num rows
        if (rowIndex >= rows.size()) {
            break;
        }

        QHBoxLayout* currentRowLayout = rows[rowIndex];

        QLabel* label = new QLabel(generated_words[i]);
        label->setStyleSheet("font-size: 16px; margin: 5px;");

        currentRowLayout->addWidget(label);

        wordCount++;

        // fill words until it hits limit then reset and go next row
        if (wordCount >= wordsPerRow) {
            rowIndex++;
            wordCount = 0; 
        }
    }

    // if parent widget exists, update it
    if (words_widget->parentWidget()) { 
        words_widget->parentWidget()->update();
    }
}


QVector<QString> TypingTest::getWordsFromFile()
{
	QVector<QString> words;
	QString file_path = "./resources/english1k.txt";
	QFile file(file_path);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Unable to open file:" << file_path;
		return words;
	}

	QTextStream in(&file);

	while (!in.atEnd()) {
		QString line = in.readLine().trimmed();
		if (!line.isEmpty()) {
			words.append(line);
		}
	}

	file.close();
	return words;
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
        words.begin(),
        words.end(),
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