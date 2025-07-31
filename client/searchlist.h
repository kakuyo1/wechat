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
#include "loadingdialog.h"
#include "searchcontactsuccesseddialog.h"
#include "searchcontactfaileddialog.h"

class SearchList : public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget* parent = nullptr);
    void setLineEdit(QWidget* searchLineEdit);
    ~SearchList() = default;

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private:
    void intialAddContactTipItem();
    bool isPureDigit(const QString& str) const; // 判断字符串是否全为数字
private:
    std::shared_ptr<searchContactSuccessedDialog> _searchContactSuccessedDialog; // 搜索联系人成功的对话框
    std::shared_ptr<searchcontactfaileddialog> _searchContactFailedDialog; // 搜索联系人失败的对话框
    LoadingDialog* _loadingDialog; // 加载对话框, 出现搜索过程中的加载动画
    bool _isLoading; // 是否正在加载搜索结果
    QWidget* _search_lineEdit; // 搜索框
private slots:
    void slot_itemClicked(QListWidgetItem* item);
    void slot_search_user_success(std::shared_ptr<SearchInfo> searchInfoPtr);
    void slot_search_user_failed(QString errorMessage);
signals:

};

#endif // SEARCHLIST_H
