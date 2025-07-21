#ifndef SEARCHLIST_H
#define SEARCHLIST_H
#include <QListWidget>
#include <QWidget>
#include <memory>
#include <QDialog>
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include "addcontactitem.h"
#include "contactinfodata.h"
#include "searchcontactsuccesseddialog.h"

class SearchList : public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget* parent = nullptr);
    ~SearchList() = default;

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private:
    void intialAddContactTipItem();
private:
    std::shared_ptr<searchContactSuccessedDialog> _searchContactSuccessedDialog; // 搜索联系人成功的对话框
private slots:
    void slot_itemClicked(QListWidgetItem* item);
signals:

};

#endif // SEARCHLIST_H
