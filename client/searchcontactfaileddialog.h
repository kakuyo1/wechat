#ifndef SEARCHCONTACTFAILEDDIALOG_H
#define SEARCHCONTACTFAILEDDIALOG_H

#include <QDialog>

namespace Ui {
class searchcontactfaileddialog;
}

class searchcontactfaileddialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchcontactfaileddialog(QWidget *parent = nullptr);
    ~searchcontactfaileddialog();

private slots:
    void on_searchFailed_ReturnBtn_clicked();

private:
    Ui::searchcontactfaileddialog *ui;
};

#endif // SEARCHCONTACTFAILEDDIALOG_H
