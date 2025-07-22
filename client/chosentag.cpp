#include "chosentag.h"
#include "ui_chosentag.h"
#include "customized_label.h"

ChosenTag::ChosenTag(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::ChosenTag)
{
    ui->setupUi(this);
    ui->remove_label->setCursor(Qt::PointingHandCursor);
    ui->remove_label->setState("normal", "hover", "pressed");
    connect(ui->remove_label, &Customized_Label::clicked, this, &ChosenTag::slot_removeTag_clicked);
}

void ChosenTag::setText(const QString &text)
{
    _text = text;
    ui->text_label->setText(text);
    ui->text_label->adjustSize(); // Adjust size to fit text
    // Calculate text width and height based on the font metrics
    QFontMetrics fm(ui->text_label->font());
    _textWidth = fm.horizontalAdvance(text);
    _textHeight = fm.height();
    ui->text_label->setFixedSize(_textWidth, _textHeight);
    this->setFixedSize(ui->text_label->width() + ui->remove_label->width() + 10, _textHeight + 2); // Adjust frame size
    _textWidth = this->width();
    _textHeight = this->height();
}

int ChosenTag::getTextWidth() const
{
    return _textWidth;
}

int ChosenTag::getTextHeight() const
{
    return _textHeight;
}

QString ChosenTag::getText() const
{
    return _text;
}

void ChosenTag::slot_removeTag_clicked()
{
    emit signal_removeTag(_text);
}
