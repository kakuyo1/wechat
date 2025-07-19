#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QDialog>
#include <QPainter>
#include <QStyleOption>

namespace Ui {
class ChatPage;
}

class ChatPage : public QDialog
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
private slots:
    void on_send_btn_clicked();

private:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    Ui::ChatPage *ui;
};

#endif // CHATPAGE_H
