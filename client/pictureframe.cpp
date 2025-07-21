#include "pictureframe.h"

PictureFrame::PictureFrame(const QPixmap &pixmap, MessageRole role, QWidget *parent) : BubbleFrame(role, parent)
{
    QLabel *imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);
    QPixmap scaledPixmap = pixmap.scaled(QSize(PICTURE_MAX_WIDTH, PICTURE_MAX_HEIGHT),
                                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(scaledPixmap);
    setBubbleWidget(imageLabel);

    // 设置图片Frame大小固定，不随窗口拉伸变化
    int left_margin = this->layout()->contentsMargins().left();
    int right_margin = this->layout()->contentsMargins().right();
    int top_margin = this->layout()->contentsMargins().top();
    int bottom_margin = this->layout()->contentsMargins().bottom();
    this->setFixedSize(scaledPixmap.width() + left_margin + right_margin,
                       scaledPixmap.height() + top_margin + bottom_margin);
}
