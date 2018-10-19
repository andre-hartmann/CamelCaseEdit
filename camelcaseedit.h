#ifndef CAMELCASEEDIT_H
#define CAMELCASEEDIT_H

#include <QObject>

class CamelCaseEdit
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

    static CamelCaseEdit::Input classifyInput(QChar input);
};

#endif // CAMELCASEEDIT_H
