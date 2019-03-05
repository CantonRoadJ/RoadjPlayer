//.h文件
#ifndef CJWLISTWIDGET_H

#define CJWLISTWIDGET_H
#include <QTimer>
#include <QAction>
#include <QListWidget>
#include <QMenu>

class MyListWidget : public QListWidget

{

Q_OBJECT

public:

explicit MyListWidget(QWidget *parent = 0);
    ~MyListWidget();

private:

QTimer *timer;
int holdTime;

QAction *action;

void mousePressEvent(QMouseEvent *event);

void mouseReleaseEvent(QMouseEvent *e);


signals:
  void deleteSignal(int index);

public slots:
  void onTimeOut();
//void deleteItemAt();

};

#endif // CJWLISTWIDGET_H
