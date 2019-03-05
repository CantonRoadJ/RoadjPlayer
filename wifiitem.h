#ifndef WIFIITEM_H
#define WIFIITEM_H

#include <QWidget>

namespace Ui {
class WifiItem;
}

class WifiItem : public QWidget
{
    Q_OBJECT

public:
    explicit WifiItem(QWidget *parent = 0);
    void setText(QString text);
    ~WifiItem();

private:
    Ui::WifiItem *ui;

};

#endif // WIFIITEM_H
