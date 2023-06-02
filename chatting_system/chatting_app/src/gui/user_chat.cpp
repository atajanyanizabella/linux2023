#include "../../include/gui/user_chat.hpp"
#include <QTextDocument>
#include <QTextCursor>


user_chat::user_chat(const std::string& username)
    : m_user_side_name(username)
{
    m_chat = new QTextDocument();
}


std::string user_chat::get_username()
{
    return m_user_side_name;
}


QTextDocument* user_chat::get_chat()
{
    return m_chat;
}


void user_chat::append_sending_message(const QString& message)
{
    QTextCursor cursor(m_chat);
    cursor.movePosition(QTextCursor::End);
    QTextBlockFormat block_format = cursor.blockFormat();
    block_format.setAlignment(Qt::AlignRight);
    cursor.setBlockFormat(block_format);
    cursor.insertText(message + "\n");
}


void user_chat::append_receiving_message(const QString& message)
{
    QTextCursor cursor(m_chat);
    cursor.movePosition(QTextCursor::End);
    QTextBlockFormat block_format = cursor.blockFormat();
    block_format.setAlignment(Qt::AlignLeft);
    cursor.setBlockFormat(block_format);
    cursor.insertText(message + "\n");
}
