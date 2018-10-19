#include "camelcasecursor.h"

#include <QString>
#include <QtTest>

class CamelCaseEditingTest : public QObject
{
    Q_OBJECT

private slots:
    void camelCaseLeft_data()
    {
        QTest::addColumn<QString>("given");
        QTest::addColumn<QString>("expected");

        QTest::newRow("camel")          << "ThisIsCamelCase|Text"
                                        << "ThisIsCamel|CaseText";
        QTest::newRow("upper-lower")    << "THISisCAMELcase|TEXT"
                                        << "THISisCAME|LcaseTEXT";
        QTest::newRow("natural-before") << "This is natural |text"
                                        << "This is |natural text";
        QTest::newRow("natural-within") << "This is natu|ral text"
                                        << "This is |natural text";
        QTest::newRow("natural-after")  << "This is natural| text"
                                        << "This is |natural text";
        QTest::newRow("number")         << "ThisIsText4Fun|"
                                        << "ThisIsText4|Fun";
        QTest::newRow("number-2")       << "ThisIsText22|Fun"
                                        << "ThisIs|Text22Fun";
        QTest::newRow("special")        << "ThisIsText#And|More"
                                        << "ThisIsText#|AndMore";
        QTest::newRow("special-2")      << "ThisIsText#|AndMore"
                                        << "ThisIsText|#AndMore";
        QTest::newRow("snake-within")   << "this_is_sna|ke_text"
                                        << "this_is_|snake_text";
        QTest::newRow("SNAKE-within")   << "THIS_IS_SNA|KE_TEXT"
                                        << "THIS_IS_|SNAKE_TEXT";
        QTest::newRow("snake-before")   << "this_is_snake_|text"
                                        << "this_is_|snake_text";
        QTest::newRow("snake-after")    << "this_is_snake|_text"
                                        << "this_is_|snake_text";
        QTest::newRow("namespace")      << "Namespace::Class|::function"
                                        << "Namespace::|Class::function";
        QTest::newRow("namespace-2")    << "Namespace::|Class::function"
                                        << "Namespace|::Class::function";
        QTest::newRow("namespace-3")    << "::|memset()"
                                        << "|::memset()";
        QTest::newRow("include-1")      << "#include <QString>|"
                                        << "#include <QString|>";
        QTest::newRow("include-2")      << "#include <QString|>"
                                        << "#include <Q|String>";
    }

    void camelCaseLeft()
    {
        QFETCH(QString, given);
        QFETCH(QString, expected);

        const int position = given.indexOf('|') - 1;
        QString result = given.remove('|');
        const int resultPosition = CamelCaseCursor::camelCaseLeft(given, position);
        result.insert(resultPosition, '|');

        QCOMPARE(result, expected);
    }

    void camelCaseRight_data()
    {
        QTest::addColumn<QString>("given");
        QTest::addColumn<QString>("expected");

        QTest::newRow("camel")          << "ThisIsCamel|CaseText"
                                        << "ThisIsCamelCase|Text";
        QTest::newRow("upper-lower")    << "THISisCAMEL|caseTEXT"
                                        << "THISisCAMELcase|TEXT";
        QTest::newRow("natural-before") << "This is |natural |text"
                                        << "This is natural |text";
        QTest::newRow("natural-within") << "This is nat|ural text"
                                        << "This is natural |text";
        QTest::newRow("natural-after")  << "This is| natural text"
                                        << "This is |natural text";
        QTest::newRow("natural-eol")    << "This is natural |text"
                                        << "This is natural text|";
        QTest::newRow("number")         << "ThisIs|Text4Fun"
                                        << "ThisIsText4|Fun";
        QTest::newRow("number-2")       << "ThisIsText|22Fun"
                                        << "ThisIsText22|Fun";
        QTest::newRow("special")        << "ThisIs|Text#AndMore"
                                        << "ThisIsText|#AndMore";
        QTest::newRow("special-2")      << "ThisIsText|#AndMore"
                                        << "ThisIsText#|AndMore";
        QTest::newRow("snake-within")   << "this_is_sna|ke_text"
                                        << "this_is_snake_|text";
        QTest::newRow("SNAKE-within")   << "THIS_IS_SNA|KE_TEXT"
                                        << "THIS_IS_SNAKE_|TEXT";
        QTest::newRow("snake-before")   << "this_is_|snake_text"
                                        << "this_is_snake_|text";
        QTest::newRow("snake-after")    << "this_is|_snake_text"
                                        << "this_is_|snake_text";
        QTest::newRow("namespace")      << "|Namespace::Class::function"
                                        << "Namespace|::Class::function";
        QTest::newRow("namespace-2")    << "Namespace|::Class::function"
                                        << "Namespace::|Class::function";
        QTest::newRow("namespace-3")    << "Namespace::Class::|function"
                                        << "Namespace::Class::function|";
        QTest::newRow("struct-member")  << "struct|.member"
                                        << "struct.|member";
        QTest::newRow("struct-member-2") << "struct|->member"
                                         << "struct->|member";
        QTest::newRow("include-1")      << "#include |<QString>"
                                        << "#include <|QString>";
        QTest::newRow("include-2")      << "#include <|QString>"
                                        << "#include <Q|String>";
        QTest::newRow("include-3")      << "#include <Q|String>"
                                        << "#include <QString|>";
    }

    void camelCaseRight()
    {
        QFETCH(QString, given);
        QFETCH(QString, expected);

        const int position = given.indexOf('|');
        QString result = given.remove('|');
        const int resultPosition = CamelCaseCursor::camelCaseRight(given, position);
        result.insert(resultPosition, '|');

        QCOMPARE(result, expected);
    }
};

QTEST_APPLESS_MAIN(CamelCaseEditingTest)

#include "tst_camelcaseeditingtest.moc"
