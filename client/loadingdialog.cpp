#include "loadingdialog.h"
#include <QVBoxLayout>
#include <QLabel>

LoadingDialog::LoadingDialog(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); // 设置无边框窗口
    setAttribute(Qt::WA_TranslucentBackground, true); // 设置背景透明

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel* loadingLabel = new QLabel(this);
    QMovie *movie = new QMovie(":/images/loading.gif"); // 使用资源文件中的加载动画
    loadingLabel->setMovie(movie);
    movie->start(); // 启动动画

    layout->addWidget(loadingLabel);
    setLayout(layout);
    setFixedSize(100, 100);
}
