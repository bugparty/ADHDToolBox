/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QApplication>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QScopedPointer>
#include "rasterwindow.h"


//! [5]
class AnalogClockWindow : public RasterWindow
{
public:
    AnalogClockWindow();

protected:
    void timerEvent(QTimerEvent *) override;
    void render(QPainter *p) override;

private:
    int m_timerId;
};
//! [5]


//! [6]
AnalogClockWindow::AnalogClockWindow()
{
    // 设置无边框窗口
    setFlags(Qt::FramelessWindowHint);

    // 设置窗口半透明，透明度为 0.5
    setOpacity(0.5);
    setTitle("Analog Clock");
    resize(200, 200);

    m_timerId = startTimer(500);
}
//! [6]

//! [7]
void AnalogClockWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId)
        renderLater();
}
//! [7]

//! [1] //! [14]
void AnalogClockWindow::render(QPainter *p)
{
//! [14]
//! [8]
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };
    static const QPoint secondHand[3] = {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -80)
    };


    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 255);
    QColor secondColor(64, 64, 64,255);
    p->eraseRect(0,0,width(),height());
//! [8]

//! [9]
    p->setRenderHint(QPainter::Antialiasing);
//! [9] //! [10]
    p->translate(width() / 2, height() / 2);

    int side = qMin(width(), height());
    p->scale(side / 200.0, side / 200.0);
//! [1] //! [10]

//! [11]
    p->setPen(Qt::NoPen);
    p->setBrush(hourColor);
//! [11]

//! [2]
    QTime time = QTime::currentTime();

    p->save();
    p->rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    p->drawConvexPolygon(hourHand, 3);
    p->restore();
//! [2]

//! [12]
    p->setPen(hourColor);
    //画hour的表盘刻度
    for (int i = 0; i < 12; ++i) {
        p->drawLine(88, 0, 96, 0);
        p->rotate(30.0);
    }
//! [12] //! [13]
    p->setPen(Qt::NoPen);
    p->setBrush(minuteColor);
//! [13]

//! [3]
    p->save();
    p->rotate(6.0 * (time.minute() + time.second() / 60.0));
    p->drawConvexPolygon(minuteHand, 3);
    p->restore();
//! [3]

//! [4]
    p->setPen(minuteColor);

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            p->drawLine(92, 0, 96, 0);
        p->rotate(6.0);
    }
//! [4]
//! //draw second hand
    p->setPen(Qt::NoPen);
    p->setBrush(secondColor);
    p->save();
    p->rotate(6.0 * time.second());
    p->drawConvexPolygon(secondHand, 3);
    p->restore();
}


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("BowmanSoft");
    QCoreApplication::setApplicationName("ADHDToolBox");
    AnalogClockWindow clock;
    // 检查是否支持系统托盘
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "错误", "系统托盘不可用！");
        return 1;
    }
    // 创建一个托盘图标
    QSystemTrayIcon trayIcon;
    // 设置托盘图标
    trayIcon.setIcon(QIcon(":/images/tray_icon")); // 使用资源中的图标
    app.setQuitOnLastWindowClosed(true);
    // 创建菜单
    QScopedPointer<QMenu> fileMenu(new QMenu("File"));

    // 创建动作并设置图标
    fileMenu->addAction(QIcon(":/images/bring_to_top"), "bring to front", [&clock](){
        qDebug() << "bring to top" ;
        clock.bringToForeground();
    });
    fileMenu->addSeparator(); // 添加分隔符
    fileMenu->addAction(QIcon(":/images/exit"), "Exit", [&clock, &app](){
        qDebug() << "exit" ;
        clock.destroy();
        app.exit();
    });

    trayIcon.setContextMenu(fileMenu.get());
    trayIcon.show();

    clock.show();

    return app.exec();
}
