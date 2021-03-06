#ifndef KEYPAD_H
#define KEYPAD_H

#include <QDialog>
#include <QLineEdit>
namespace Ui {
    class Keypad;
}

class Keypad : public QDialog
{
    Q_OBJECT

public:
    explicit Keypad(QLineEdit *line,bool tgl,QWidget *parent = 0);
    ~Keypad();

private slots:
    void keypadHandler();
    void symbolChange();
    void keyclose();
    void boardchange();

private:
    Ui::Keypad *ui;
    QLineEdit *pad_edit;
    bool toggle,t1;
    void setalphabet();
    void setsymbol();
    void setnumeric();
    //bool event(QEvent *event);


};
#endif // KEYPAD_H
