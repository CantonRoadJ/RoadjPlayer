#include "wifiitem.h"
#include "ui_wifiitem.h"

WifiItem::WifiItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiItem)
{
    ui->setupUi(this);
}

WifiItem::~WifiItem()
{
    delete ui;
}
void WifiItem::setText(QString text){
    ui->itemText->setText(text);
}
