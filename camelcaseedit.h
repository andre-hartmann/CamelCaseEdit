#ifndef CAMELCASEEDIT_H
#define CAMELCASEEDIT_H

#include <QObject>

class CamelCaseEdit
{
public:
    static int camelCaseLeft(const QString &text, int position);

private:
    enum class Input {
        Upper,
        Lower,
        Underscore,
        Space,
        Other
    };

    enum class State {
        Default = 0,
        Upper = 1,
        Lower = 2,
        Underscore = 3,
        Space = 4
    };

    static CamelCaseEdit::Input classifyInput(QChar input);
};

#endif // CAMELCASEEDIT_H
