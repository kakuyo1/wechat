#ifndef CHATVIEWAREA_H
#define CHATVIEWAREA_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QPainter>
#include <QStyleOption>
#include <QLabel>
namespace Ui {
class ChatViewArea;
}

class ChatViewArea : public QWidget
{
    Q_OBJECT

public:
    explicit ChatViewArea(QWidget *parent = nullptr);
    ~ChatViewArea();
    void appendMessageWidget(QWidget* item);
    void testAddMessages();
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private:
    Ui::ChatViewArea *ui;
    QVBoxLayout *_messageLayout;
};

#endif // CHATVIEWAREA_H
