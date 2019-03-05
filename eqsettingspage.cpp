#include "eqsettingspage.h"
#include "ui_eqsettingspage.h"
#include "mainwindow.h"
#include <QDebug>

EqSettingsPage::EqSettingsPage(QWidget *parent,Music *music) :
    QDialog(parent),
    ui(new Ui::EqSettingsPage)
{
    setAttribute(Qt::WA_DeleteOnClose);
    //qDebug()<<"EqSettingsPage";
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(QRect(0, 0, 540, 960));
    ui->back->setStyleSheet(
                "QPushButton:pressed{"
                "background-color:rgb(37, 38, 43);"
                "border-style: flat; }"
                );
    ui->startTest->setStyleSheet("QPushButton{color: white;"
                                 "background-color:""#2a2c31"";"
                                 "border-radius: 5px;"
                                 "border-style: flat;}"
                                 "QPushButton:pressed{"
                                 "background-color:rgb(24, 25, 27);"
                                 "border-style: flat; }");
    ui->commit->setStyleSheet("QPushButton{color: white;"
                              "background-color:""#2a2c31"";"
                              "border-radius: 5px;"
                              "border-style: flat;}"
                              "QPushButton:pressed{"
                              "background-color:rgb(24, 25, 27);"
                              "border-style: flat; }");
    ui->bg->setStyleSheet("border-radius: 5px;"
                          "background-color: rgb(37, 38, 43);");
    ui->label->setStyleSheet("border-radius: 5px;"
                             "color:white;"
                             "background-color: rgb(37, 38, 43);");

    ui->resultDialog->setVisible(false);
    ui->dialog->setVisible(false);

    this->music=music;
    connect(music,SIGNAL(eqDoneTest(QList<QPointF>)),this,
            SLOT(onEqDoneTest(QList<QPointF>)));
    connect(music,SIGNAL(eqError(QList<QPointF>)),this,SLOT(onEqTestError(QList<QPointF>)));

    m_view = new View(ui->widget);
    m_view->setWindowSize(0, 0, ui->widget->width(),ui->widget->height());
    m_view->setColor("#383533","white");
    m_view->setFont(10);

    QList<QPointF> points=myQSettings.value("points").value<QList<QPointF> >();;
    if(!points.isEmpty()){
        qDebug()<<"---------points="<<points;
        m_view->setData(points);
        m_view->showGraphics("Speaker Graph");
    }else{
        qDebug()<<"----points.isEmpty()-----";
    }
}

EqSettingsPage::~EqSettingsPage()
{
    // qDebug()<<"~EqSettingsPage";
    delete m_view;
    delete ui;

}
void EqSettingsPage::onEqDoneTest(QList<QPointF> points){
    //qDebug()<<"--page-onEqDoneTest";
    myQSettings.setValue("points",QVariant::fromValue(points));
    m_view->clearAll();
    m_view->setData(points);
    m_view->showGraphics("Speaker Graph");

    ui->dialog->setVisible(false);
    ui->testTips->setText("调试完成");
    ui->resultDialog->setVisible(true);
}
void EqSettingsPage::onEqTestError(QList<QPointF> m_points){
    // qDebug()<<"---onEqTestError";
    //myQSettings.setValue("points", QVariant::fromValue(m_points));
    ui->dialog->setVisible(false);
    ui->testTips->setText("调试出错，请确保录音设备连接正常！");
    ui->resultDialog->setVisible(true);
    m_view->clearAll();
    m_view->setData(m_points);
    m_view->showGraphics("Speaker Graph");

}
void EqSettingsPage::hideTips(){
    ui->resultDialog->setVisible(false);
}
void EqSettingsPage::on_back_clicked()
{
    //    delete m_view;
    //    delete ui;
    close();
}

void EqSettingsPage::on_startTest_clicked()
{
    music->eqStartTest();
    ui->dialog->setVisible(true);
}

void EqSettingsPage::on_commit_clicked()
{
    hideTips();
}
