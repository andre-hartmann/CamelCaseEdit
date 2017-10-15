#include "camelcaseedit.h"

#include <QString>
#include <QtTest>

class CamelCaseEditingTest : public QObject
{
    Q_OBJECT

public:
    CamelCaseEditingTest() { }

private slots:
    void camelCaseLeft_data()
    {
       QTest::addColumn<QString>("text");
       QTest::addColumn<int>("position");
       QTest::addColumn<int>("expected");

       QTest::newRow("before-beginning")       << "Text"             <<  -1 <<  0;
       QTest::newRow("at-beginning")           << "Text"             <<   0 <<  0;
       QTest::newRow("after-end")              << "Text"             <<   4 <<  3;
       QTest::newRow("after-word")             << "Text"             <<   3 <<  0;
       QTest::newRow("after-camel-word")       << "CamelCaseText"    <<  12 <<  9;
       QTest::newRow("after-lower-camel-word") << "camelCase"        <<   8 <<  5;
       QTest::newRow("after-snake-word")       << "snake_case"       <<   9 <<  6;
       QTest::newRow("after-space-word")       << "natural word"     <<  11 <<  8;
       QTest::newRow("in-camel-word")          << "CamelCaseText"    <<   7 <<  5;
       QTest::newRow("at-word-begin")          << "Lorem Ipsum"      <<   6 <<  0;
    }

    void camelCaseLeft()
    {
        QFETCH(QString, text);
        QFETCH(int, position);
        QFETCH(int, expected);

        const int result = CamelCaseEdit::camelCaseLeft(text, position);

        QCOMPARE(result, expected);
    }
};

QTEST_APPLESS_MAIN(CamelCaseEditingTest)

#include "tst_camelcaseeditingtest.moc"
