#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>
#include <QtQml>
#include <PTrecovery.h>


int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qmlRegisterType<PTrecovery>("com.powertune", 1, 0, "ConnectObject");
    engine.rootContext()->setContextProperty("PTrecovery", new PTrecovery(&engine));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();

}
