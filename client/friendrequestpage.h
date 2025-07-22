#ifndef FRIENDREQUESTPAGE_H
#define FRIENDREQUESTPAGE_H

#include <QWidget>

namespace Ui {
class FriendRequestPage;
}

class FriendRequestPage : public QWidget
{
    Q_OBJECT

public:
    explicit FriendRequestPage(QWidget *parent = nullptr);
    ~FriendRequestPage();

private:
    Ui::FriendRequestPage *ui;
};

#endif // FRIENDREQUESTPAGE_H
