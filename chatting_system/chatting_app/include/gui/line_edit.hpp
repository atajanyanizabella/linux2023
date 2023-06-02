#ifndef LINE_EDIT_H
#define LINE_EDIT_H

#include <QTextEdit>

class custom_text_edit : public QTextEdit
{
    Q_OBJECT

    public:
        explicit custom_text_edit(QWidget* parent = nullptr);
    signals:
        void send_message(bool status); // TODO
    protected:
        void keyPressEvent(QKeyEvent* event) override;
};

#endif
