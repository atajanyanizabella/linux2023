#include "gui/line_edit.hpp"
#include <QKeyEvent>
#include <QTextCursor>

custom_text_edit::custom_text_edit(QWidget* parent)
    : QTextEdit(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMinimumHeight(fontMetrics().height());
    setMaximumHeight(4 * fontMetrics().height());
}

void custom_text_edit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return  && event->modifiers() == Qt::ShiftModifier)
    {
        QTextCursor cursor = textCursor();
        cursor.insertBlock();
        setTextCursor(cursor);
    } else if (event->key() == Qt::Key_Return) {
        emit please_sent_message(true);
    } else {
        QTextEdit::keyPressEvent(event);
    }
    int contentHeight = document()->size().toSize().height();
    if (contentHeight <= 4 * fontMetrics().height()) {
        setMinimumHeight(contentHeight);
    } else {
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
}
