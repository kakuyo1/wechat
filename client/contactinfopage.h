#ifndef CONTACTINFOPAGE_H
#define CONTACTINFOPAGE_H

#include <QWidget>
#include <memory>
#include "contactinfodata.h"

namespace Ui {
class ContactInfoPage;
}

class ContactInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit ContactInfoPage(QWidget *parent = nullptr);
    ~ContactInfoPage();
    std::shared_ptr<AuthResponse> getContactInfo() const { return _contactInfo; } // 获取联系人信息
    void updateContactInfoUIAndInfo(std::shared_ptr<AuthResponse> contactInfo); // 更新UI显示联系人信息
private slots:
    void on_chat_btn_clicked();
signals:
    void signal_jumpTo_sessionItem(std::shared_ptr<AuthResponse> contactInfo); // 跳转到对应的会话项
private:
    Ui::ContactInfoPage *ui;
    std::shared_ptr<AuthResponse> _contactInfo; // 联系人信息
};

#endif // CONTACTINFOPAGE_H
