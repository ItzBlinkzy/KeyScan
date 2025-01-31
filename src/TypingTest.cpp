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
#include "KeyScan.h"

TypingTest::TypingTest(QWidget* words_widget, QObject* parent)
    : QObject(parent), words_widget(words_widget),
    font("Arial", 35), fontMetrics(font) 
{
    all_words = getWordsFromFile();
}

TypingTest::~TypingTest() {

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
    letterStates.clear();
    cursor = 0;
    startGame();
}

void TypingTest::drawWords(QVector<QString> gen_words) {
    if (scene) {
        delete scene;
        scene = nullptr;
    }

    scene = new QGraphicsScene(words_widget);
    QGraphicsView* view = new QGraphicsView(scene, words_widget);

    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
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

    wordItems.clear();

    int x = 0, y = 0;
    const int letterSpacing = 0;
    const int wordSpacing = 20;
    const int rowSpacing = 10;
    const int viewWidth = words_widget->width();

    for (const QString& word : gen_words) {
        QVector<QGraphicsTextItem*> letterItems;
        QString displayWord = word;

        if (word == " ") {
            displayWord = "_";
        }

        int wordWidth = 0;
        for (const QChar& letter : displayWord) {
            wordWidth += fontMetrics.horizontalAdvance(letter) + letterSpacing;
        }
        wordWidth -= letterSpacing;

        if (x + wordWidth > viewWidth) {
            x = 0;
            y += fontMetrics.height() + rowSpacing;
        }

        for (const QChar& letter : displayWord) {
            QGraphicsTextItem* letterItem = scene->addText(QString(letter));
            letterItem->setFont(font);
            letterItem->setDefaultTextColor(Qt::black);
            int letterWidth = fontMetrics.horizontalAdvance(letter);

            letterItem->setPos(x, y);
            letterItems.append(letterItem);

            x += letterWidth + letterSpacing;
        }

        wordItems.append(letterItems);
        x += wordSpacing;
    }

    scene->setSceneRect(0, 0, viewWidth, y + fontMetrics.height());

    updateDisplay();
}

void TypingTest::keyPressEvent(QKeyEvent* event) {
    if (generated_words.isEmpty()) return;

    QString inputChar = event->text();

    if (event->key() == Qt::Key_Backspace) {
        if (cursor > 0) {
            int lastIncorrectPos = -1;

            for (int i = cursor - 1; i >= 0; i--) {
                if (!letterStates.contains(i) || !letterStates[i]) {
                    lastIncorrectPos = i;
                    break;
                }
            }

            if (lastIncorrectPos != -1) {
                for (int i = lastIncorrectPos; i < cursor; i++) {
                    letterStates.remove(i);
                }
                cursor = lastIncorrectPos;
                updateDisplay();
            }
        }
        return;
    }

    // ignore shift, ctrl keys etc 
    if (inputChar.isEmpty()) return;

    if (event->key() == Qt::Key_Space) {
        inputChar = " ";
    }

    auto letterInfo = getCurrentLetterInfo();
    if (!letterInfo) return;

    int wordIdx, letterPos;
    QChar expectedChar;
    std::tie(wordIdx, letterPos, expectedChar) = *letterInfo;

    if (inputChar == QString(expectedChar)) {
        letterStates[cursor] = true;
    }
    else {
        letterStates[cursor] = false;
    }

    cursorNext();
}
void TypingTest::cursorNext() {
    cursor++;
    updateDisplay();
    return;
}

void TypingTest::cursorPrev() {
    if (cursor <= 0) return;
    cursor--;
    updateDisplay();
    return;
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

    // ading spacebar char to the generated_words list also 
    for (size_t i = 0; i < temp_out.size(); ++i) {
        generated_words.append(temp_out[i]);

        if (i < temp_out.size() - 1) {
            generated_words.append(" ");
        }
    }


    return generated_words;
}


void TypingTest::updateDisplay() {
    if (generated_words.isEmpty() || wordItems.empty()) return;

    auto letterInfo = getCurrentLetterInfo();
    int currentWordIdx = -1, currentLetterPos = -1;

    if (letterInfo) {
        std::tie(currentWordIdx, currentLetterPos, std::ignore) = *letterInfo;
    }

    int runningIndex = 0;

    for (int wordIdx = 0; wordIdx < generated_words.size(); ++wordIdx) {
        const QString& word = generated_words[wordIdx];
        bool isCurrentWord = (wordIdx == currentWordIdx);

        for (int letterIdx = 0; letterIdx < wordItems[wordIdx].size(); ++letterIdx) {
            QGraphicsTextItem* item = wordItems[wordIdx][letterIdx];
            int letterGlobalIndex = runningIndex + letterIdx;

            QColor defaultColor = (word == " ") ? Qt::gray : Qt::black;

            if (letterStates.contains(letterGlobalIndex)) {

                item->setDefaultTextColor(letterStates[letterGlobalIndex] ? Qt::green : Qt::red);
            }
            else if (isCurrentWord && letterIdx == currentLetterPos) {
                item->setDefaultTextColor(Qt::blue);
            }
            else {
                item->setDefaultTextColor(defaultColor);
            }
        }
        runningIndex += word.length();
    }

    scene->setSceneRect(0, 0, words_widget->width(), wordItems.last().last()->y() + fontMetrics.height());
}


std::optional<std::tuple<int, int, QChar>> TypingTest::getCurrentLetterInfo() const {
    if (generated_words.isEmpty()) return std::nullopt;

    int runningIndex = 0; 

    for (int wordIdx = 0; wordIdx < generated_words.size(); ++wordIdx) {
        const QString& word = generated_words[wordIdx];

        if (cursor >= runningIndex && cursor < runningIndex + word.length()) {
            int letterPos = cursor - runningIndex; 
            return std::make_tuple(wordIdx, letterPos, word[letterPos]);
        }

        runningIndex += word.length();
    }

    return std::nullopt; // this shouldnt happen hopefully cursor (OOB)
}
