#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

class client;
class QLineEdit;
class QPushButton;
class QLabel;
class main_window;
class QKeyEvent;

class login_window : public QWidget
{
    Q_OBJECT

public:
    login_window(QWidget *parent = nullptr);

private slots:
    void on_login_button_clicked();
    void on_sign_up_button_clicked();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    main_window* m_window;
    QLineEdit* m_username;
    QLineEdit* m_password;
    QPushButton* m_login_button;
    QPushButton* m_sign_up_button;
    QLabel* m_username_label;
    QLabel* m_password_label;
    client* m_client;

    bool check_valid_input(QString str);
    void create_members();
};

#endif
