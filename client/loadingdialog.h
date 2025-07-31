#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H
#include <QWidget>
#include <QDialog>
#include <QMovie>

class LoadingDialog : public QDialog
{
public:
    explicit LoadingDialog(QWidget* parent = nullptr);
    ~LoadingDialog() = default;
};

#endif // LOADINGDIALOG_H
