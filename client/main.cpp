#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QDir>

void loadQssFile(QApplication& a);
void loadConfig();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load the stylesheet from a file
    loadQssFile(a);

    // load the config.ini
    loadConfig();

    MainWindow w;
    w.show();
    return a.exec();
}

void loadQssFile(QApplication& a) {
    QFile qssFile(":/style/style.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(qssFile.readAll());
        a.setStyleSheet(styleSheet);
        qssFile.close();
    } else {
        qDebug() << "Failed to load stylesheet:" << qssFile.errorString();
    }
}

void loadConfig() {
    // current working directory
    QString appDir = QCoreApplication::applicationDirPath();
    QString fileName = "config.ini";
    QString configFilePath = QDir::toNativeSeparators(appDir + QDir::separator() + fileName);

    // read the config file
    QSettings settings(configFilePath, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    if (gate_host.isEmpty() || gate_port.isEmpty()) {
        qDebug() << "GateServer configuration is missing in config.ini";
    } else {
        gate_url_prefix = QString("http://%1:%2").arg(gate_host, gate_port);
    }
}
