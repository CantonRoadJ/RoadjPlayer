#ifndef EQSETTINGSPAGE_H
#define EQSETTINGSPAGE_H

#include <QDialog>
#include "fft/include/view.h"
#include "app/music.h"
#include "qtimer.h"
namespace Ui {
class EqSettingsPage;
}

class EqSettingsPage : public QDialog
{
    Q_OBJECT

public:
    explicit EqSettingsPage(QWidget *parent = 0,Music *music=0);
    ~EqSettingsPage();

private slots:
    void on_back_clicked();

    void on_startTest_clicked();
private slots:
    // void on_pushButton_clicked();
    void onEqDoneTest(QList<QPointF> points);
    void onEqTestError(QList<QPointF> m_points);
    void hideTips();
    void on_commit_clicked();

private:
    Ui::EqSettingsPage *ui;
    View *m_view;
    Music *music;
    QSettings myQSettings;

};

#endif // EQSETTINGSPAGE_H
