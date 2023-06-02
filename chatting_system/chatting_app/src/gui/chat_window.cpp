#include "../../include/gui/chat_window.hpp"
#include "../../include/client/client_class.hpp"
#include <iostream>
#include <QLineEdit>
#include <QTextEdit>
#include <QKeyEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <thread>
#include <QTimer>
#include <QScrollBar>
#include <QFont>

main_window::main_window(client* my_client, QWidget* parent)
    : my_client(my_client) , QWidget(parent)
{
    setWindowTitle(tr("ChatApp"));
    qRegisterMetaType<std::string>("std::string");
    this->create_members();
    this->set_layout();
    connect(m_messege_box, &custom_text_edit::send_message,
            m_send_button, &QPushButton::clicked);
    connect(m_send_button, &QPushButton::clicked,
            this, &main_window::on_send_button_clicked);
    connect(m_users_list, &QListWidget::itemClicked,
            this, &main_window::on_users_list_item_clicked);
    connect(this, &main_window::message_recieved,
            this, &main_window::on_message_recieved);
    connect(this, &main_window::close_connection,
            this, &main_window::on_connection_closed);
    connect(this, &main_window::users_list_changed,
            this, &main_window::refresh_users_list);
    std::thread message_get_thread(&main_window::message_get, this);
    message_get_thread.detach();
    std::thread refresh_thread(&client::refresh, my_client);
    refresh_thread.detach();
    connection_check_thread = new std::thread(&main_window::connection_check, this);
}

void main_window::message_get()
{
    while (true) {
        my_client->waiting_for_message();
        std::string message = my_client->get_message();
        emit this->message_recieved(my_client->get_sender_username(),
                                    QString::fromStdString(message));
    }
}

void main_window::on_users_list_item_clicked(QListWidgetItem* item)
{
    QFont font = item->font();
    font.setBold(false);
    item->setFont(font);
    std::string selected_username = item->text().toStdString();
    m_notification[selected_username] = false;
    if (m_username->text().toStdString() != selected_username) {
        m_username->setText(QString::fromStdString(selected_username));

        bool is_in_vector = false;
        for (user_chat* chat : m_chats_vector) {
            if (chat->get_username() == selected_username) {
                is_in_vector = true;
                m_chat_box->setDocument(chat->get_chat());
                QScrollBar* scroll = m_chat_box->verticalScrollBar();
                scroll->setValue(scroll->maximum());
                break;
            }
        }
        if (!is_in_vector) {
            user_chat* new_user = new user_chat(selected_username);
            m_chats_vector.push_back(new_user);
            m_chat_box->setDocument(new_user->get_chat());
        }
    }
}

void main_window::on_send_button_clicked()
{
    QString message = m_messege_box->toPlainText();
    QString username = m_username->text();
    if (username.isEmpty()) {
        QMessageBox::critical(this, "Send", "Select a user!");
    } else if (message.size() > 300) {
        QMessageBox::warning(this, "Send", "Too much characters!");
    } else if(!message.isEmpty() && !check_spaces(message.toStdString())) {
        for (user_chat* chat : m_chats_vector) {
            if (chat->get_username() == username.toStdString()) {
                message = this->add_time_for_message(message);
                chat->append_sending_message(message);
                QScrollBar* scroll = m_chat_box->verticalScrollBar();
                scroll->setValue(scroll->maximum());
                break;
            }
        }
        my_client->create_message_json(username.toStdString(),
                                       message.toStdString());
        m_messege_box->clear();
        m_messege_box->setFocus();
    }
}

main_window::~main_window()
{
    connection_check_thread->join();
    delete m_messege_box;
    delete m_chat_box;
    delete m_send_button;
    delete m_users_list;
    delete m_username;
    delete m_my_username;
}

void main_window::create_members()
{
    m_chat_box = new QTextEdit();
    m_chat_box->setReadOnly(true);
    m_messege_box = new custom_text_edit();
    m_send_button = new QPushButton(tr("Send"));
    m_username = new QLabel();
    m_username->setAlignment(Qt::AlignCenter);
    m_my_username = new QLabel("(me) " +  QString::fromStdString(my_client->get_client_username()));
    m_my_username->setAlignment(Qt::AlignRight);
    m_users_list = new QListWidget();
    // Create and start the QTimer for refreshing the users list
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            this, &main_window::refresh_user_list);
    timer->start(1000); // Refresh every 1 seconds
    refresh_user_list(); // Initial refresh
}

void main_window::refresh_user_list()
{
    // Clear the QListWidget
    m_users_list->clear();
    std::vector<chatting::message_data*> online_users = my_client->get_online_users_vector();
    for (unsigned int i = 0; i < online_users.size(); ++i) {
        if (online_users[i]->get_uid() == my_client->get_client_uid()) {
            continue;
        }
        emit users_list_changed(QString::fromStdString(online_users[i]->get_username()));
    }
}

void main_window::connection_check()
{
    while (!my_client->get_connection()) {
        std::cout << "";
    }
    emit close_connection();
}


void main_window::set_layout()
{
    QHBoxLayout* mess_send_layout = new QHBoxLayout();
    mess_send_layout->addWidget(m_messege_box, 3);
    mess_send_layout->addWidget(m_send_button, 1);
    QVBoxLayout* messege_layout = new QVBoxLayout();
    messege_layout->addWidget(m_username);
    messege_layout->addWidget(m_chat_box);
    messege_layout->addLayout(mess_send_layout);
    QHBoxLayout* me_and_users = new QHBoxLayout();
    me_and_users->addWidget(new QLabel("Users"));
    me_and_users->addWidget(m_my_username);
    QVBoxLayout* users_layout = new QVBoxLayout();
    users_layout->addLayout(me_and_users);
    users_layout->addWidget(m_users_list);
    QHBoxLayout* main_layout = new QHBoxLayout();
    main_layout->addLayout(messege_layout, 3);
    main_layout->addLayout(users_layout, 1);
    setLayout(main_layout);
}

void main_window::on_message_recieved(const std::string& username
                                      , const QString& msg)
{
    std::string sender_name = my_client->get_sender_username();
    bool is_in_vector = false;
    if (!msg.isEmpty()) {
        for (user_chat* chat : m_chats_vector) {
            if (chat->get_username() == sender_name) {
                is_in_vector = true;
                chat->append_receiving_message(msg);
                QScrollBar* scroll = m_chat_box->verticalScrollBar();
                scroll->setValue(scroll->maximum());
                break;
            }
        }
        if (!is_in_vector) {
            user_chat* new_user = new user_chat(username);
            new_user->append_receiving_message(msg);
            m_chats_vector.push_back(new_user);
        }
    }
    for (int i = 0; i < m_users_list->count(); ++i) {
        if (m_users_list->item(i)->text().toStdString() == sender_name
            && m_username->text() != m_users_list->item(i)->text()) {
            m_notification[sender_name] = true;
            QFont font = m_users_list->item(i)->font();
            font.setBold(true);
            m_users_list->item(i)->setFont(font);
            break;
        }
    }
}

void main_window::on_connection_closed()
{
    QMessageBox::critical(this, "Connection", "Please check your connection!");
    this->close();
}

QString main_window::add_time_for_message(const QString& msg)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("hh:mm");
    QString new_msg = msg + "\n" + timeString + "\n";
    return new_msg;
}

void main_window::refresh_users_list(const QString& username)
{
    m_users_list->addItem(username);
    for (unsigned int i = 0; i < m_users_list->count(); ++i) {
        if (m_notification.count(m_users_list->item(i)->text().toStdString()) == 1) {
            QFont font = m_users_list->item(i)->font();
            font.setBold(m_notification[m_users_list->item(i)->text().toStdString()]);
            m_users_list->item(i)->setFont(font);
        }
    }
}

bool main_window::check_spaces(const std::string& msg)
{
	for (char c : msg) {
		if (!std::isspace(c)) {
			return false;
		}
	}
	return true;
}
