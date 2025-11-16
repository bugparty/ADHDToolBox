#include "util.h"
#include <QSettings>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStandardPaths>
#include <QDebug>

void setAutoStart(bool enable) {
#ifdef _WIN32
    // Windows: Use registry
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    QString appName = QCoreApplication::applicationName();
    QString appPath = QCoreApplication::applicationFilePath();

    if (enable) {
        settings.setValue(appName, appPath);
    } else {
        settings.remove(appName);
    }
#elif defined(__linux__)
    // Linux: Use autostart .desktop file
    QString appName = QCoreApplication::applicationName();
    QString appPath = QCoreApplication::applicationFilePath();

    QString autostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";
    QDir autostartDir(autostartPath);

    if (!autostartDir.exists()) {
        autostartDir.mkpath(".");
    }

    QString desktopFilePath = autostartPath + "/" + appName + ".desktop";

    if (enable) {
        QFile desktopFile(desktopFilePath);
        if (desktopFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&desktopFile);
            out << "[Desktop Entry]\n";
            out << "Type=Application\n";
            out << "Name=" << appName << "\n";
            out << "Exec=" << appPath << "\n";
            out << "Hidden=false\n";
            out << "NoDisplay=false\n";
            out << "X-GNOME-Autostart-enabled=true\n";
            desktopFile.close();
        } else {
            qWarning() << "Failed to create autostart file:" << desktopFilePath;
        }
    } else {
        QFile::remove(desktopFilePath);
    }
#else
    qWarning() << "Auto-start is not implemented for this platform";
    Q_UNUSED(enable);
#endif
}

bool getAutoStart() {
#ifdef _WIN32
    // Windows: Check registry
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appName = QCoreApplication::applicationName();
    return settings.contains(appName);
#elif defined(__linux__)
    // Linux: Check if .desktop file exists
    QString appName = QCoreApplication::applicationName();
    QString autostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";
    QString desktopFilePath = autostartPath + "/" + appName + ".desktop";
    return QFile::exists(desktopFilePath);
#else
    qWarning() << "Auto-start check is not implemented for this platform";
    return false;
#endif
}
