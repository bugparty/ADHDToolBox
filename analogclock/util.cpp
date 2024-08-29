#include "util.h"
#include <QSettings>
#include <QCoreApplication>
void setAutoStart(bool enable) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    QString appName = QCoreApplication::applicationName();
    QString appPath = QCoreApplication::applicationFilePath();

    if (enable) {
        settings.setValue(appName, appPath);
    } else {
        settings.remove(appName);
    }
}
bool getAutoStart(){
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appName = QCoreApplication::applicationName();

    return settings.contains(appName);
}
