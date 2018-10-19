#ifndef CAMELCASECURSOR_H
#define CAMELCASECURSOR_H

#include <QObject>

class CamelCaseCursor
{
public:
    static int camelCaseLeft(const QString &text, int position);
    static int camelCaseRight(const QString &text, int position);

private:
    enum class Input {
        Upper,
        Lower,
        Underscore,
        Space,
        Other
    };

    enum class State {
        Default,
        UpperContinuation,
        Upper,
        Lower,
        Underscore,
        Space
    };

    static CamelCaseCursor::Input classifyInput(QChar input);
};

#endif // CAMELCASECURSOR_H
