#ifndef KENGINERESOURCETABLEVIEW_HPP
#define KENGINERESOURCETABLEVIEW_HPP

#include <QTableView>


class KengineResourceTableView : public QTableView
{
public:
    explicit KengineResourceTableView(QWidget *parent = Q_NULLPTR);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
};

#endif // KENGINERESOURCETABLEVIEW_HPP
