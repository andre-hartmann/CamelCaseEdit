#include "camelcasecursor.h"

CamelCaseCursor::Input CamelCaseCursor::classifyInput(QChar input)
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

static int charLeft(const QString &text, int position)
{
    Q_UNUSED(text);

    return (position > 0) ? --position : 0;
}

static int charRight(const QString &text, int position)
{
    const int size = text.size();

    return (position < size) ? ++position : size;
}

static bool isWordSeparator(QChar c)
{
    return c.isSpace() || c.isPunct() || c == '_' || c == '-' || c == '>' || c == '<' || c == '~';
}

static int wordLeft(const QString &text, int position)
{
    if (isWordSeparator(text.at(position))) {
        while (position > 0 && isWordSeparator(text.at(position)))
            --position;

        if (position > 0)
            return charRight(text, position);
    }

    return position;
}

static int wordRight(const QString &text, int position)
{
    const int size = text.size() - 1;

    if (isWordSeparator(text.at(position))) {
        while (position < size && isWordSeparator(text.at(position)))
            ++position;

        if (position == size)
            return charLeft(text, position);
    }

    return position;
}

static bool isInvalidPosition(const QString &text, int *position)
{
    if (*position >= text.size()) {
        *position = text.size() - 1;
        return true;
    }

    if (*position < 0) {
        *position = 0;
        return true;
    }

    return false;
}

int CamelCaseCursor::camelCaseLeft(const QString &text, int position)
{
    State state = State::Default;

    if (isInvalidPosition(text, &position))
        return position;

    for (;;) {
        const QChar c = text.at(position);
        Input input = classifyInput(c);

        switch (state) {
        case State::Default:
            switch (input) {
            case Input::Upper:
                state = State::Upper;
                break;
            case Input::Lower:
                state = State::Lower;
                break;
            case Input::Underscore:
                state = State::Underscore;
                break;
            case Input::Space:
                state = State::Space;
                break;
            default:
                return wordLeft(text, position);
            }
            break;
        case State::Upper:
            switch (input) {
            case Input::Upper:
                break;
            default:
                return charRight(text, position);
            }
            break;
        case State::Lower:
            switch (input) {
            case Input::Upper:
                return position;
            case Input::Lower:
                break;
            default:
                return charRight(text, position);
            }
            break;
        case State::Underscore:
            switch (input) {
            case Input::Underscore:
                break;
            case Input::Upper:
                state = State::Upper;
                break;
            case Input::Lower:
                state = State::Lower;
                break;
            default:
                return charRight(text, position);
            }
            break;
        case State::Space:
            switch (input) {
            case Input::Space:
                break;
            case Input::Upper:
                state = State::Upper;
                break;
            case Input::Lower:
                state = State::Lower;
                break;
            case Input::Underscore:
                state = State::Underscore;
                break;
            default:
                return wordLeft(text, position);
            }
            break;
        }

        position = charLeft(text, position);
        if (position == 0)
            return position;
    }
}

int CamelCaseCursor::camelCaseRight(const QString &text, int position)
{
    State state = State::Default;

    if (isInvalidPosition(text, &position))
        return position;

    for (;;) {
        const QChar c = text.at(position);
        const Input input = classifyInput(c);

        switch (state) {
        case State::Default:
            switch (input) {
            case Input::Upper:
                state = State::Upper;
                break;
            case Input::Lower:
                state = State::Lower;
                break;
            case Input::Underscore:
                state = State::Underscore;
                break;
            default:
                return wordRight(text, position);
            }
            break;
        case State::Lower:
            switch (input) {
            case Input::Upper:
                return position;
            case Input::Lower:
                break;
            case Input::Underscore:
                state = State::Underscore;
                break;
            case Input::Space:
                state = State::Space;
                break;
            default:
                return position;
            }
            break;
        case State::UpperContinuation:
            switch (input) {
            case Input::Upper:
                break;
            case Input::Lower:
                return charLeft(text, position);
            case Input::Underscore:
                state = State::Underscore;
                break;
            case Input::Space:
                state = State::Space;
                break;
            default:
                return true;
            }
            break;
        case State::Upper:
            switch (input) {
            case Input::Upper:
                state = State::UpperContinuation;
                break;
            case Input::Lower:
                state = State::Lower;
                break;
            case Input::Underscore:
                state = State::Underscore;
                break;
            case Input::Space:
                state = State::Space;
                break;
            default:
                return position;
            }
            break;
        case State::Underscore:
            switch (input) {
            case Input::Underscore:
                break;
            case Input::Space:
                state = State::Space;
                break;
            default:
                return position;
            }
            break;
        case State::Space:
            switch (input) {
            case Input::Space:
                break;
            default:
                return position;
            }
            break;
        }

        position = charRight(text, position);
        if (position == text.size())
            return position;
    }
}