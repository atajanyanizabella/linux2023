#include "../../include/gui/login_window.hpp"
#include "../../include/gui/signup_window.hpp"
#include "../../include/gui/chat_window.hpp"
#include "../../include/client/client_class.hpp"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QScreen>
#include <string>
#include <cctype>
#include <cstring>
#include <QKeyEvent>
#include <iostream>

login_window::login_window(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Login");
    m_client = new client;
    setFixedSize(geometry().width(), geometry().height());
    setLayout(new QVBoxLayout(this));
    QLayout* l = layout();
    create_members();
    l->setAlignment(Qt::AlignCenter);
    l->addWidget(m_username);
    l->addWidget(m_username_label);
    l->addWidget(m_password);
    l->addWidget(m_password_label);
    l->addWidget(m_login_button);
    l->addWidget(m_sign_up_button);
    connect(m_login_button, &QPushButton::clicked,
            this, &login_window::on_login_button_clicked);
    connect(m_sign_up_button, &QPushButton::clicked,
            this, &login_window::on_sign_up_button_clicked);
}

void login_window::create_members()
{
    m_username = new QLineEdit(this);
    m_username->setPlaceholderText("Username");
    m_username->setFixedSize(350, 30);
    QString label_txt = "The Username must be from 4 to 16 characters long:";
    m_username_label = new QLabel(label_txt);
    m_username_label->setFixedHeight(10);
    m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : grey; }");
    m_password = new QLineEdit(this);
    m_password->setPlaceholderText("Password");
    m_password->setEchoMode(QLineEdit::Password);
    m_password->setFixedSize(350, 30);
    label_txt = "The Password must be from 4 to 16 characters long:";
    m_password_label = new QLabel(label_txt);
    m_password_label->setFixedHeight(10);
    m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : grey; }");
    m_login_button = new QPushButton("Login", this);
    m_sign_up_button = new QPushButton("Sign Up", this);
}

void login_window::on_login_button_clicked()
{
    std::cout << "entered\n";
    QString username = m_username->text();
    QString password = m_password->text();
    if (!check_valid_input(username) && !check_valid_input(password)) {
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : red; }");
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : red; }");
    } else if (!check_valid_input(username)) {
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : red; }");
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
    } else if (!check_valid_input(password)) {
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : red; }");
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
    }
    else {
        std::cout << "valid login\n";
        m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }"); //TODO
        m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : green; }");
        std::cout << "valid json to be created\n";
        m_client->create_login_json(username.toStdString(), password.toStdString());
        std::cout << "Login json created\n";
        if (m_client->get_error_message() != "") {
            m_username_label->setStyleSheet("QLabel { font-size: 7pt; color : grey; }");
            m_password_label->setStyleSheet("QLabel { font-size: 7pt; color : grey; }");
            QMessageBox::critical(this, "Login", "Login failed!");

        } else {
            //switching windows
            std::cout << "reached to switching windows/n";
            m_window = new main_window(m_client);
            std::cout << "main window created\n";
            m_window->move(QGuiApplication::primaryScreen()->availableGeometry().center()
                           - m_window->rect().center());
            std::cout << "switching windows\n";
            m_window->show();
            std::cout << "showing new window\n";
            hide();
        }
    }
}

void login_window::on_sign_up_button_clicked()
{
    sign_up_window sign_window(m_client);
    sign_window.move(QGuiApplication::primaryScreen()->availableGeometry().center()
                     - sign_window.rect().center());
    sign_window.exec();
}

bool login_window::check_valid_input(QString str)
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

void login_window::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        on_login_button_clicked();
    }
}
