#include "PTrecovery.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QProcess>
#include <QByteArrayMatcher>
#include <QByteArray>
#include <QNetworkInterface>
#include <QQmlContext>




PTrecovery::PTrecovery(QObject *parent)
    : QObject(parent)

{


}


void PTrecovery::initializeWifiscanner()
{


    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PTrecovery::getconnectionStatus);
    timer->start(3000); // Check the status of the connection every 3 Seconds too
    PTrecovery::getconnectionStatus(); //Temporay measure
    getavailablenetworks();

}

void PTrecovery::getavailablenetworks()
{
    qDebug()<< "check available networks";
    result.clear();
    QString raw;
        QProcess proc2;
        proc2.start("sh", QStringList()<<"-c"<<"iw wlan0 scan |grep SSID");
        proc2.waitForFinished();
        QString output2 = proc2.readAllStandardOutput();
        QStringList fields = output2.split(QRegExp("[\n]"));
        foreach (const QString &str, fields) {
                raw = str;
                raw.replace("SSID: ","");
                //raw.replace("\xe2\x80\x99","'");  //for some reason ' is shown as \xe2\x80\x99
                raw.remove(0,1); // Remove the white space before the SSID
                result += raw;
        }
        setwifilist(result);
}
void PTrecovery::getconnectionStatus()
{
// displays the wlan0 and eth0 IP adresses
    QNetworkInterface wlan0IP = QNetworkInterface::interfaceFromName("wlan0");
    QList<QNetworkAddressEntry> entries = wlan0IP.addressEntries();
    if (!entries.isEmpty()) {
        QNetworkAddressEntry entry = entries.first();
        wlanip = entry.ip().toString();
        wlanip.replace("QHostAddress(","");
        wlanip.remove(QChar(')'));
        wlanip.remove(QChar('"'));
       if (m_wlanipstat != wlanip)
        {
         setwlanipstat(wlanip);
        }
    }
    else{
        if (m_wlanipstat != "NOT CONNECTED")
        {
           setwlanipstat("NOT CONNECTED");

        }
    }

    QNetworkInterface eth0IP = QNetworkInterface::interfaceFromName("eth0");
    QList<QNetworkAddressEntry> test = eth0IP.addressEntries();
    if (!test.isEmpty()) {
        QNetworkAddressEntry entry2 = test.first();
        eth0ip = entry2.ip().toString();
        eth0ip.replace("QHostAddress(","");
        eth0ip.remove(QChar(')'));
        eth0ip.remove(QChar('"'));
        if (m_eth0ipstat != eth0ip)
        {
         seteth0ipstat(eth0ip);
        }
    }
    else{
        if (m_eth0ipstat != "NOT CONNECTED")
        {
            seteth0ipstat("NOT CONNECTED");

        }
}

}

void PTrecovery::setwifi(const QString &country,const QString &ssid1,const QString &psk1)
{

    //Check where the wpa_supplicant file is located !
    QString path = "/etc/wpa_supplicant/";
    QString filepath;
    if (QFileInfo::exists(path))
    {
        filepath = "/etc/wpa_supplicant/wpa_supplicant.conf";

    }
    else
    {
        filepath = "/etc/wpa_supplicant.conf";
    }

    ///
    QFile file(filepath);
    file.remove(); //remove file if it exists to avoid appending of existing file
    if ( file.open(QIODevice::ReadWrite) )
    {
        if (QFileInfo::exists(path))
        {
            QTextStream out(&file);

            out     << "ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev" << "\r\n"
                    << "update_config=1" << "\r\n"
                    << "country="+country << "\r\n"
                    << "#Primary WIFI" << "\r\n"
                    << "network={" << "\r\n"
                    << "ssid=" << "\"" << ssid1 << "\"" << "\r\n"
                    << "psk=" << "\"" << psk1  << "\"" << "\r\n"
                    << "}" << "\r\n"
                       // << "#Secondary WIFI" << "\r\n"
                       // << "network={" << "\r\n"
                       // << "ssid="<< "\"" << ssid2 << "\"" << "\r\n"
                       // << "psk=" << "\"" << psk2 << "\"" << "\r\n"
                       // << "}" << "\r\n"
                    << endl;
            file.close();
        }
        else
        {
        QTextStream out(&file);

        out     << "update_config=1" << "\r\n"
                << "country="+country << "\r\n"
                << "#Primary WIFI" << "\r\n"
                << "network={" << "\r\n"
                << "ssid=" << "\"" << ssid1 << "\"" << "\r\n"
                << "psk=" << "\"" << psk1  << "\"" << "\r\n"
                << "}" << "\r\n"
                   // << "#Secondary WIFI" << "\r\n"
                   // << "network={" << "\r\n"
                   // << "ssid="<< "\"" << ssid2 << "\"" << "\r\n"
                   // << "psk=" << "\"" << psk2 << "\"" << "\r\n"
                   // << "}" << "\r\n"
                << endl;
        file.close();
        }
    }
    //wait a bit before shutting down so all write operations are finished
     QTimer::singleShot(6000, this, &PTrecovery::restartwifi);

}
void PTrecovery::restartwifi()
{

    QProcess proc3;
    proc3.start("sh", QStringList()<<"-c"<<"sudo reboot");
    proc3.waitForFinished();

}
void PTrecovery::setwifilist(const QStringList &wifilist)
{
    if(m_wifilist == wifilist)
    {
        return;
    }
    m_wifilist = wifilist;
    emit wifilistChanged(wifilist);
}
QStringList PTrecovery::wifilist() const { return m_wifilist; }

void PTrecovery::seteth0ipstat(const QString &eth0ipstat)
{
    if(m_eth0ipstat == eth0ipstat)
    {
        return;
    }
    m_eth0ipstat = eth0ipstat;
    emit eth0ipstatChanged(eth0ipstat);
}
QString PTrecovery::eth0ipstat() const { return m_eth0ipstat; }


void PTrecovery::setwlanipstat(const QString &wlanipstat)
{
    if(m_wlanipstat == wlanipstat)
    {
        return;
    }
    m_wlanipstat = wlanipstat;
    emit wlanipstatChanged(wlanipstat);
}
QString PTrecovery::wlanipstat() const { return m_wlanipstat; }

void PTrecovery::update()
{

    QProcess *p = new QProcess( this );

    if (p)
    {
        p->setEnvironment( QProcess::systemEnvironment() );
        p->setProcessChannelMode( QProcess::MergedChannels );
        p->start("/home/pi/src/updatePowerTune.sh", QStringList() << "echo" << "hye" );
        p->waitForStarted();

        connect( p, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()) );
    }
}

void PTrecovery::processOutput()
{

    QProcess *p = dynamic_cast<QProcess *>( sender() );
        QString output = p->readAllStandardOutput();
        setprocout(output);
}

void PTrecovery::setprocout(const QString &procout)
{
    if(m_procout == procout)
    {
        return;
    }
    m_procout = procout;
    emit procoutChanged(procout);
}
QString PTrecovery::procout() const { return m_procout; }

void PTrecovery::deletesource()
{
    QProcess proc3;
    proc3.start("sh", QStringList()<<"-c"<<"sudo rm -r /home/pi/src");
    proc3.waitForFinished();
    makesrcfolder();
}
void PTrecovery::makesrcfolder()
{
    QProcess proc4;
    proc4.start("sh", QStringList()<<"-c"<<"mkdir /home/pi/src");
    proc4.waitForFinished();
    clonerepo();
}

void PTrecovery::clonerepo()
{
    QProcess *p = new QProcess( this );

    if (p)
    {
        p->setEnvironment( QProcess::systemEnvironment() );
        p->setProcessChannelMode( QProcess::MergedChannels );
        p->start("sh", QStringList()<<"-c"<<"git clone https://github.com/PowerTuneDigital/PowerTuneDigitalOfficial.git /home/pi/src");
        connect( p, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()) );
        connect( p, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(update()) );
        p->waitForStarted(10000);

    }
}
