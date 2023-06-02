#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QWidget>
#include <vector>
#include "user_chat.hpp"
#include "line_edit.hpp"
#include <thread>
#include <unordered_map>
class QLineEdit;
class QPushButton;
class QTextEdit;
class QListWidget;
class QListWidgetItem;
class QLabel;
class client;
class QKeyEvent;

class main_window : public QWidget
{
    Q_OBJECT

public:
    main_window(client* my_client, QWidget* parent = nullptr);
    ~main_window();

private slots:
    void on_send_button_clicked();
    void on_users_list_item_clicked(QListWidgetItem* item);
    void on_message_recieved(const std::string& username, const QString& msg);
    void on_connection_closed();
    void refresh_users_list(const QString&);

signals:
    void message_recieved(const std::string&, const QString&);
    void close_connection();
    void users_list_changed(const QString&);

private:
    custom_text_edit* m_messege_box;
    QTextEdit* m_chat_box;
    QPushButton* m_send_button;
    QListWidget* m_users_list;
    QLabel* m_username;
    QLabel* m_my_username;
    std::vector<user_chat*> m_chats_vector;
    client* my_client;
    std::thread* connection_check_thread;
    std::unordered_map<std::string, bool> m_notification;

    void create_members();
    void set_layout();
    void message_get();
    void refresh_user_list();
    void connection_check();
	bool check_spaces(const std::string&);
    QString add_time_for_message(const QString&);
};

#endif

