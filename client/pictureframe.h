#ifndef PICTUREFRAME_H
#define PICTUREFRAME_H

#include "bubbleframe.h"
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>

#define PICTURE_MAX_WIDTH 160
#define PICTURE_MAX_HEIGHT 90

class PictureFrame : public BubbleFrame
{
    Q_OBJECT
public:
    PictureFrame(const QPixmap &pixmap, MessageRole role, QWidget *parent = nullptr);
    ~PictureFrame() = default;
};

#endif // PICTUREFRAME_H
