#ifndef SIGNUP_WINDOW_H
#define SIGNUP_WINDOW_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class QLabel;
class client;

class sign_up_window : public QDialog
{
    Q_OBJECT

public:
    sign_up_window(client* my_client, QWidget *parent = nullptr);

private slots:
    void on_sign_up_button_clicked();

private:
    QLineEdit* m_username;
    QLineEdit* m_password;
    QLineEdit* m_password_confirm;
    QPushButton* m_sign_up_button;
    QLabel* m_username_label;
    QLabel* m_password_label;
    client* m_client;

    bool check_valid_input(QString q_str);
    void create_members();
};

#endif

