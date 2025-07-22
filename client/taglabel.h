#ifndef TAGLABEL_H
#define TAGLABEL_H
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

/*
    该类用于好友申请的"更多"按钮
*/

class TagLabel : public QLabel
{
    Q_OBJECT
public:
    explicit TagLabel(QWidget* parent = nullptr);
    ~TagLabel() = default;
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    void clicked();
};

#endif // TAGLABEL_H
