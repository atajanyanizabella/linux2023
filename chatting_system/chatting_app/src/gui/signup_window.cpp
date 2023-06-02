#include "../../include/gui/signup_window.hpp"
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include "../../include/client/client_class.hpp"

sign_up_window::sign_up_window(client* m_client, QWidget *parent)
    : QDialog(parent) , m_client(m_client)
{
    setWindowTitle("Sign Up");
    create_members();
    connect(m_sign_up_button, &QPushButton::clicked,
                this, &sign_up_window::on_sign_up_button_clicked);
}

void sign_up_window::create_members()
{
    this->setFixedSize(400, 230);
    m_username = new QLineEdit(this);
    m_username->setPlaceholderText("Username");
    m_username->setGeometry(20, 20, 350, 30);

    QString label_txt = "The Username must be from 4 to 16 characters long:";
    m_username_label = new QLabel(label_txt, this);
    m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : grey; }");
    m_username_label->setGeometry(20, 60, 350, 10);

    m_password = new QLineEdit(this);
    m_password->setPlaceholderText("Password");
    m_password->setEchoMode(QLineEdit::Password);
    m_password->setGeometry(20, 80, 350, 30);

    label_txt = "The Password must be from 4 to 16 characters long:";
    m_password_label = new QLabel(label_txt, this);
    m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : grey; }");
    m_password_label->setGeometry(20, 120, 350, 10);

    m_password_confirm = new QLineEdit(this);
    m_password_confirm->setPlaceholderText("Confirm Password");
    m_password_confirm->setEchoMode(QLineEdit::Password);
    m_password_confirm->setGeometry(20, 140, 350, 30);
    m_sign_up_button = new QPushButton("Sign Up", this);
    m_sign_up_button->setGeometry(20, 180, 100, 30);
}

void sign_up_window::on_sign_up_button_clicked()
{
    QString username = m_username->text();
    QString password = m_password->text();
    QString password_confirm = m_password_confirm->text();
    if (!check_valid_input(username) && !check_valid_input(password)) {
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : red; }");
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : red; }");
        QMessageBox::critical(this, "Sign Up", "Wrong username and password");
    } else if (!check_valid_input(username)) {
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : red; }");
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
        QMessageBox::critical(this, "Sign Up", "Wrong username");
    } else if (!check_valid_input(password)) {
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : red; }");
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
        QMessageBox::critical(this, "Sign Up", "Wrong password");
    } else if (password != password_confirm) {
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
        QMessageBox::critical(this, "Sign Up", "Passwords do not match!");
        return;
    } else {
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
        m_client->create_register_json(username.toStdString(), password.toStdString());
        if (m_client->get_error_message() != "")
        {
            QMessageBox::critical(this, "Sign Up",
                                     "Registration failed!");
        } else {
            QMessageBox::information(this, "Sign Up",
                                     "Sign Up successful!");
            accept();
        }
    }
}

bool sign_up_window::check_valid_input(QString str)
{
    std::string s = str.toStdString();
    if (s.length() < 4 || s.length() > 16) {
        return false;
    }
    for (size_t i = 0; i < s.length(); ++i) {
        if (std::isspace(s[i])) {
            return false;
        }
    }
    return true;
}
