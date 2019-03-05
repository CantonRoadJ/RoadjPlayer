#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QLabel>
namespace Ui {
class listItem;
}

class listItem : public QWidget
{
    Q_OBJECT

public:
    explicit listItem(QWidget *parent = 0);
    void setIndex(int index);
    void setSongName(QString name);
    void setSinger(QString singer);
    QLabel* getNameLabel();
    QLabel* getSingerLabel();
    QLabel* getIndexLabel();
    ~listItem();

private:
    Ui::listItem *ui;

};

#endif // LISTITEM_H
