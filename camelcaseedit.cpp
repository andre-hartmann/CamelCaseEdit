#include "camelcaseedit.h"

#include <QTextCursor>

CamelCaseEdit::Input CamelCaseEdit::classifyInput(QChar input)
{
    if (input.isUpper())
        return Input::Upper;

    if (input.isLower() || input.isDigit())
        return Input::Lower;

    if (input == '_')
        return Input::Underscore;

    if (input.isSpace() && input != QChar::ParagraphSeparator)
        return Input::Space;

    return Input::Other;
}

int CamelCaseEdit::camelCaseLeft(const QString &text, int position)
{
    int state = 0;

    if (position >= text.size())
        return text.size() - 1;

    if (position <= 0)
        return 0;

    for (;;) {
        Input input = classifyInput(text.at(position));

        switch (state) {
        case 0:
            switch (input) {
            case Input::Upper:
                state = 1;
                break;
            case Input::Lower:
                state = 2;
                break;
            case Input::Underscore:
                state = 3;
                break;
            case Input::Space:
                state = 4;
                break;
            default:
                ++position;
                return -1; //move(QTextCursor::WordLeft, mode);
            }
            break;
        case 1:
            switch (input) {
            case Input::Upper:
                break;
            default:
                return ++position; // move(QTextCursor::Right, mode);
            }
            break;
        case 2:
            switch (input) {
            case Input::Upper:
                return position;
            case Input::Lower:
                break;
            default:
                return ++position; // move(QTextCursor::Right, mode);
            }
            break;
        case 3:
            switch (input) {
            case Input::Underscore:
                break;
            case Input::Upper:
                state = 1;
                break;
            case Input::Lower:
                state = 2;
                break;
            default:
                return ++position; // move(QTextCursor::Right, mode);
            }
            break;
        case 4:
            switch (input) {
            case Input::Space:
                break;
            case Input::Upper:
                state = 1;
                break;
            case Input::Lower:
                state = 2;
                break;
            case Input::Underscore:
                state = 3;
                break;
            default:
                ++position;
                if (position == 0)
                    return true;
                return -1; // move(QTextCursor::WordLeft, mode);
            }
        }

        if (position > 0)
            --position;
    }

    return position;
}
