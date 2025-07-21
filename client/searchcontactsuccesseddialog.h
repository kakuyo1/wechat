#ifndef SEARCHCONTACTSUCCESSEDDIALOG_H
#define SEARCHCONTACTSUCCESSEDDIALOG_H

#include <QDialog>
#include "contactinfodata.h"
#include <memory>
#include <QDir>

namespace Ui {
class searchContactSuccessedDialog;
}

class searchContactSuccessedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchContactSuccessedDialog(QWidget *parent = nullptr);
    ~searchContactSuccessedDialog();
    void setContactInfo(std::shared_ptr<SearchInfo> contactInfo);
private slots:
    void on_add_contact_btn_clicked();
    void on_cancel_btn_clicked();

private:
    Ui::searchContactSuccessedDialog *ui;
    std::shared_ptr<SearchInfo> _contactInfo;
};

#endif // SEARCHCONTACTSUCCESSEDDIALOG_H
