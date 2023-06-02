#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>

class custom_text_edit : public QTextEdit
{
    Q_OBJECT

public:
    explicit custom_text_edit(QWidget* parent = nullptr);

signals:
    void please_sent_message(bool);

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif

