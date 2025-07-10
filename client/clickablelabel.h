#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QPixmap>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget* parent = nullptr);
    ~ClickableLabel() = default;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override; // normal to hover
    virtual void leaveEvent(QEvent *event) override; // hover to normal
    void updatePixmap();
    bool getState() const;
private:
    bool isClicked;
    QPixmap hidePasswordPixmap;
    QPixmap hidePasswordHoverPixmap;
    QPixmap showPasswordPixmap;
    QPixmap showPasswordHoverPixmap;
signals:
    void clicked();
};

#endif // CLICKABLELABEL_H
