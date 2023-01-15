#ifndef PTPTrecovery_H
#define PTPTrecovery_H

#include <QTime>
#include <QTimer>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QProcess>
#include <QQmlContext>
#include <QQmlApplicationEngine>

class PTrecovery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList wifilist READ wifilist WRITE setwifilist NOTIFY wifilistChanged)
    Q_PROPERTY(QString eth0ipstat READ eth0ipstat WRITE seteth0ipstat NOTIFY eth0ipstatChanged)
    Q_PROPERTY(QString wlanipstat READ wlanipstat WRITE setwlanipstat NOTIFY wlanipstatChanged)
    Q_PROPERTY(QString procout READ procout WRITE setprocout NOTIFY procoutChanged)

public:

    explicit PTrecovery(QObject *parent = 0);
    int foundCount;
    QNetworkConfiguration netcfg;
    QList<QNetworkConfiguration> netcfgList;
    Q_INVOKABLE void initializeWifiscanner();
    Q_INVOKABLE void setwifi(const QString &country,const QString &ssid1,const QString &psk1);
    void restartwifi();
    void setwifilist(const QStringList &wifilist);
    void seteth0ipstat(const QString &eth0ipstat);
    void setwlanipstat(const QString &wlanipstat);
    void setprocout(const QString &procout);
    Q_INVOKABLE void getavailablenetworks();
    Q_INVOKABLE void update();
    Q_INVOKABLE void deletesource();
    void makesrcfolder();
    void clonerepo();
    QStringList wifilist() const;
    QStringList m_wifilist;
    QString eth0ipstat() const;
    QString m_eth0ipstat;
    QString wlanipstat() const;
    QString m_wlanipstat;
    QString procout() const;
    QString m_procout;

private:

    QString outputline;
    QStringList result;
    QString eth0ip;
    QString wlanip;

signals:
    void wifilistChanged(QStringList wifilist);
    void eth0ipstatChanged(QString eth0ipstat);
    void wlanipstatChanged(QString wlanipstat);
    void procoutChanged(QString procout);


public slots:
    void getconnectionStatus();
    void processOutput();
private:
    QStandardItemModel* listModel;
    QNetworkSession *session;
    QQmlApplicationEngine engine;
    QProcess *process;

};

#endif // PTPTrecovery_H
