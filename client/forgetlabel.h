#ifndef FORGETLABEL_H
#define FORGETLABEL_H
#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class ForgetLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ForgetLabel(QWidget*parent = nullptr);
    ~ForgetLabel() = default;

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // FORGETLABEL_H
