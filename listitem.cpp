#include "listitem.h"
#include "ui_listitem.h"

listItem::listItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::listItem)
{
    ui->setupUi(this);

}
void listItem::setIndex(int index){
    ui->index->setText(QString::number(index));
}
void listItem::setSongName(QString name){
    ui->songName->setText(name);
}
void listItem::setSinger(QString singer){
    ui->singerName->setText(singer);
}
QLabel* listItem::getNameLabel(){
    return ui->songName;
}
QLabel* listItem::getSingerLabel(){
    return ui->singerName;
}
QLabel* listItem::getIndexLabel(){
    return ui->index;
}
listItem::~listItem()
{
    delete ui;
}
