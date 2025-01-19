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
TypingTest::TypingTest(QObject* parent) : QObject(parent) {

	words = getWordsFromFile();
}

TypingTest::~TypingTest() {

}

void TypingTest::handleTyping()
{
}

void TypingTest::startTest()
{
    auto results = generateTest(GameType::Standard);
       
    qDebug() << "These are the words that will now be drawn onto the screen to play" << "\n";
    
    for (auto it = results.begin(); it != results.end(); it++) {
        qDebug() << *it << " ";
    }
     
}

void TypingTest::drawWords()
{
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