#include "mylistwidget.h"
#include <QDebug>


MyListWidget::MyListWidget(QWidget *parent):

    QListWidget(parent)

{
    timer=new QTimer();
    holdTime=0;
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
}
MyListWidget::~MyListWidget(){
    delete timer;
}

void MyListWidget::mousePressEvent(QMouseEvent *event)

{
    QListWidget::mousePressEvent(event);

    timer->start(500);

}

void MyListWidget::mouseReleaseEvent(QMouseEvent *e)

{
    if(holdTime<1500){
        QListWidget::mouseReleaseEvent(e);
    }
    timer->stop();
    holdTime=0;

}
void MyListWidget::onTimeOut(){
    holdTime+=500;
    if(holdTime>=1500){
        qDebug()<<"---wan to delte!----"<<this->currentIndex().row();
        emit deleteSignal(this->currentIndex().row());
        timer->stop();
        //holdTime=0;
    }
}
//void MyListWidget::deleteItemAt()

//{
//    QList<QListWidgetItem*> list=this->selectedItems();
//    if(list.size()==0)
//        return;
//    QListWidgetItem* sel=list[0];
//    if(sel)
//    {
//        int r=this->row(sel);
//        delete takeItem(r);
//    }
//}
