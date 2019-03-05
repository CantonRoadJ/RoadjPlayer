#include "keypad.h"
#include "ui_keypad.h"
#include <QDebug>
#include <QFile>

Keypad::Keypad(QLineEdit *line,bool tgl,QWidget *parent) :
    QDialog(parent,Qt::FramelessWindowHint),
    ui(new Ui::Keypad)
{

    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    //this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(QRect(0, 0, 540, 227));
    toggle =true;

    t1= false;

    pad_edit=line;

    if(toggle)

        setsymbol();

    else

        setalphabet();


    connect ( ui->abutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->bbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->cbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->dbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->ebutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->fbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->gbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->hbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->ibutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->jbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );


    connect ( ui->kbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->lbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->mbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->nbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->obutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->pbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->qbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->rbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->sbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->tbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->ubutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->vbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->wbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->xbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->ybutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->zbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    //connect ( ui->clrbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->bpbutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->numbutton, SIGNAL( clicked() ), this, SLOT(symbolChange()) );

    connect ( ui->numbutton_2, SIGNAL( clicked() ), this, SLOT(symbolChange()) );

    connect ( ui->spacebutton, SIGNAL( clicked() ), this, SLOT( keypadHandler() ) );

    connect ( ui->donebutton, SIGNAL( clicked() ), this, SLOT(keyclose()) );

    connect ( ui->donebutton_2, SIGNAL( clicked() ), this, SLOT(keyclose()) );

    connect ( ui->apostrophe, SIGNAL( clicked() ), this, SLOT(keypadHandler()) );

    connect ( ui->comma, SIGNAL( clicked() ), this, SLOT(keypadHandler()) );

    connect ( ui->fstop, SIGNAL( clicked() ), this, SLOT(keypadHandler()) );

    connect ( ui->question, SIGNAL( clicked() ), this, SLOT(keypadHandler()) );

    connect( ui->numbutton_3,SIGNAL(clicked()),this,SLOT(boardchange()));

    connect( ui->numbutton_4,SIGNAL(clicked()),this,SLOT(boardchange()));

    //setStyleSheet("background-image: url(:/800x600/Touch-Keyboard1.png);");
    //    ui->abutton->setStyleSheet("");
    //    ui->bbutton->setStyleSheet("QPushButton{color: white;"
    //                               "background-color:""#2a2c31"";"
    //                               "border-radius: 5px;"
    //                               "border-style: flat;}"
    //                               "QPushButton:pressed{"
    //                               "background-color:rgb(24, 25, 27);"
    //                               "border-style: flat; }");
    QFile file(":/qss/keys.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    ui->abutton->setStyleSheet(styleSheet);
    ui->bbutton->setStyleSheet(styleSheet);
    ui->cbutton->setStyleSheet(styleSheet);
    ui->dbutton->setStyleSheet(styleSheet);
    ui->ebutton->setStyleSheet(styleSheet);
    ui->fbutton->setStyleSheet(styleSheet);
    ui->gbutton->setStyleSheet(styleSheet);
    ui->hbutton->setStyleSheet(styleSheet);
    ui->ibutton->setStyleSheet(styleSheet);
    ui->jbutton->setStyleSheet(styleSheet);

    ui->kbutton->setStyleSheet(styleSheet);
    ui->lbutton->setStyleSheet(styleSheet);
    ui->mbutton->setStyleSheet(styleSheet);
    ui->nbutton->setStyleSheet(styleSheet);
    ui->obutton->setStyleSheet(styleSheet);
    ui->pbutton->setStyleSheet(styleSheet);
    ui->qbutton->setStyleSheet(styleSheet);
    ui->rbutton->setStyleSheet(styleSheet);
    ui->sbutton->setStyleSheet(styleSheet);
    ui->tbutton->setStyleSheet(styleSheet);

    ui->ubutton->setStyleSheet(styleSheet);
    ui->vbutton->setStyleSheet(styleSheet);
    ui->wbutton->setStyleSheet(styleSheet);
    ui->xbutton->setStyleSheet(styleSheet);
    ui->ybutton->setStyleSheet(styleSheet);
    ui->zbutton->setStyleSheet(styleSheet);
    ui->bpbutton->setStyleSheet(styleSheet);
    ui->donebutton->setStyleSheet(styleSheet);

    ui->numbutton_2->setStyleSheet(styleSheet);
    ui->numbutton->setStyleSheet(styleSheet);
    ui->numbutton_3->setStyleSheet(styleSheet);
    ui->apostrophe->setStyleSheet(styleSheet);
    ui->spacebutton->setStyleSheet(styleSheet);

    ui->numbutton_4->setStyleSheet(styleSheet);
    ui->donebutton_2->setStyleSheet(styleSheet);
    ui->question->setStyleSheet(styleSheet);
    ui->fstop->setStyleSheet(styleSheet);
    ui->comma->setStyleSheet(styleSheet);

}

//bool Keypad::event(QEvent *event){
//    if (event->type() == QEvent::ActivationChange)
//    {
//        if(QApplication::activeWindow() != this)
//        {
//            this->close();
//        }
//    }
//    return QWidget::event(event);

//}
void Keypad::boardchange()

{

    if(t1)

    {

        setalphabet();

        t1=false;
    }

    else

    {
        setnumeric();
        t1=true;
    }

}
//切换到数字
void Keypad::setnumeric()

{

    //setStyleSheet("background-image: url(:/800x600/Touch-Keyboard2.png);");

    ui->abutton->setText("-");

    ui->bbutton->setText("'");

    ui->cbutton->setText("?");

    ui->dbutton->setText(":");

    ui->ebutton->setText("3");

    ui->fbutton->setText(";");

    ui->gbutton->setText("(");

    ui->hbutton->setText(")");

    ui->ibutton->setText("8");

    ui->jbutton->setText("$");

    ui->kbutton->setText("&");

    ui->lbutton->setText("@");

    ui->mbutton->setText("#");

    ui->nbutton->setText("\"");

    ui->obutton->setText("9");

    ui->pbutton->setText("0");

    ui->qbutton->setText("1");

    ui->rbutton->setText("4");

    ui->sbutton->setText("/");

    ui->tbutton->setText("5");

    ui->ubutton->setText("7");

    ui->vbutton->setText("!");

    ui->wbutton->setText("2");

    ui->xbutton->setText(",");

    ui->ybutton->setText("6");

    ui->zbutton->setText(".");

    ui->numbutton->setText("↑");

    ui->comma->setText("_");
    //'
    //ui->apostrophe->setEnabled(0);
    ui->apostrophe->setText("%");
    //.
    //ui->fstop->setEnabled(0);
    ui->fstop->setText("^");
    //?
    //ui->question->setEnabled(0);
    ui->question->setText("*");

    ui->numbutton->setEnabled(0);

    ui->numbutton_2->setEnabled(0);



}

Keypad::~Keypad()

{

    delete ui;

}

void Keypad::keypadHandler()

{

    QPushButton *button = (QPushButton *)sender();

    QString inputText = button->text();
    //qDebug()<<inputText;

    if(pad_edit != NULL){

        if (inputText == "SPACE"||inputText=="space")

        {
            pad_edit->insert(" ");

            //setsymbol();

        }

        else if(inputText == "BACK")

        {

            pad_edit->backspace();

        }

        else if(inputText == "CLR")

        {

            pad_edit->clear();


        }

        else

        {

            if(pad_edit->text().isEmpty()){

                pad_edit->insert(inputText );
                if(t1)

                    setnumeric();
                else

                    setalphabet();

            }

            else if(!pad_edit->text().isEmpty()){

                int currentPos = pad_edit->cursorPosition();

                QString currentText = pad_edit->text();

                QChar currentChar;

                if(currentPos >= 1){

                    currentChar = currentText.at(currentPos-1);

                    if(currentChar == ' ' || currentChar == ',')

                    {

                        pad_edit->insert(inputText);

                        if(t1)

                            setnumeric();
                        else

                            setalphabet();

                    }

                    else

                    {
                        pad_edit->insert(inputText);

                        if(t1)

                            setnumeric();
                        else

                            setalphabet();
                    }
                }else

                {

                    pad_edit->insert(inputText );

                    if(t1)

                        setnumeric();
                    else

                        setalphabet();



                }


            }

        }

    }

}
//切换到小写
void Keypad::setalphabet()

{

    // setStyleSheet("background-color: rgb(252, 175, 62);");

    ui->abutton->setText("a");

    ui->bbutton->setText("b");

    ui->cbutton->setText("c");

    ui->dbutton->setText("d");

    ui->ebutton->setText("e");

    ui->fbutton->setText("f");

    ui->gbutton->setText("g");

    ui->hbutton->setText("h");

    ui->ibutton->setText("i");

    ui->jbutton->setText("j");

    ui->kbutton->setText("k");

    ui->lbutton->setText("l");

    ui->mbutton->setText("m");

    ui->nbutton->setText("n");

    ui->obutton->setText("o");

    ui->pbutton->setText("p");

    ui->qbutton->setText("q");

    ui->rbutton->setText("r");

    ui->sbutton->setText("s");

    ui->tbutton->setText("t");

    ui->ubutton->setText("u");


    ui->vbutton->setText("v");

    ui->wbutton->setText("w");

    ui->xbutton->setText("x");

    ui->ybutton->setText("y");

    ui->zbutton->setText("z");

    ui->numbutton->setText("↑");

    ui->comma->setText("'");

    //ui->apostrophe->setEnabled(1);
    ui->apostrophe->setText("'");


    ui->fstop->setText(".");

    ui->question->setText("?");
    ui->numbutton->setEnabled(1);

    ui->numbutton_2->setEnabled(1);

}
//切换到大写
void Keypad::setsymbol()

{

    //setStyleSheet("background-image: url(:/800x600/Touch-Keyboard1.png);");

    ui->abutton->setText("A");

    ui->bbutton->setText("B");

    ui->cbutton->setText("C");



    ui->dbutton->setText("D");

    ui->ebutton->setText("E");

    ui->fbutton->setText("F");



    ui->gbutton->setText("G");

    ui->hbutton->setText("H");

    ui->ibutton->setText("I");



    ui->jbutton->setText("J");

    ui->kbutton->setText("K");

    ui->lbutton->setText("L");



    ui->mbutton->setText("M");

    ui->nbutton->setText("N");

    ui->obutton->setText("O");



    ui->pbutton->setText("P");

    ui->qbutton->setText("Q");

    ui->rbutton->setText("R");



    ui->sbutton->setText("S");

    ui->tbutton->setText("T");

    ui->ubutton->setText("U");



    ui->vbutton->setText("V");

    ui->wbutton->setText("W");

    ui->xbutton->setText("X");

    ui->ybutton->setText("Y");

    ui->zbutton->setText("Z");



    ui->numbutton->setText("↑");

    ui->comma->setText("'");

//    ui->apostrophe->setEnabled(1);

//    ui->fstop->setEnabled(1);

//    ui->question->setEnabled(1);
    ui->apostrophe->setText("'");


    ui->fstop->setText(".");

    ui->question->setText("?");

    ui->numbutton->setEnabled(1);

    ui->numbutton_2->setEnabled(1);

}

void Keypad::symbolChange()

{

    if(toggle){

        setsymbol();

        toggle = false;

    }

    else{

        setalphabet();

        toggle = true;

    }

}

void Keypad::keyclose()

{

    this->done(QDialog::Accepted);

}


