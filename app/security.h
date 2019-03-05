#ifndef SECURITY_H
#define SECURITY_H
#include <QObject>
class Security :public QObject
{
    Q_OBJECT
public:
    Security();
    ~Security();
     bool checkSafety();
private:

    void securityDeal(QString& cfg, QString& mac);
    void readRegister(QString filepath, QString &cfg);

};
#endif // SECURITY_H
