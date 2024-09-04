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

#include "rasterwindow.h"
#include <QMenu>
#include <QApplication>
#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#endif
//! [1]
RasterWindow::RasterWindow(QWindow *parent)
    : QWindow(parent)
    , m_backingStore(new QBackingStore(this))
{
    auto previous_windows_position = settings.value(WINDOW_POSITION_KEY, QPoint(100,100)).toPoint();
    m_alwaysOnTop =  settings.value(BRING_WINDOW_TO_TOP_KEY, false).toBool();

    setGeometry(previous_windows_position.x(), previous_windows_position.y() , 300, 200);

}
//! [1]


//! [7]
bool RasterWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        renderNow();
        return true;
    }
    return QWindow::event(event);
}
//! [7]

//! [6]
void RasterWindow::renderLater()
{
    requestUpdate();
}
//! [6]


//! [5]
void RasterWindow::resizeEvent(QResizeEvent *resizeEvent)
{
    m_backingStore->resize(resizeEvent->size());
}
//! [5]

//! [2]
void RasterWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed()){
        if(m_alwaysOnTop){
            setAlwaysOnTop(m_alwaysOnTop);
        }
        renderNow();
    }
}
//! [2]


//! [3]
void RasterWindow::renderNow()
{
    if (!isExposed())
        return;

    QRect rect(0, 0, width(), height());
    m_backingStore->beginPaint(rect);

    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);

    //painter.fillRect(0, 0, width(), height(), QGradient::NightFade);
    QColor transparentColor(255, 255, 255, 0);
    painter.fillRect(0,0,width(),height(), transparentColor);
    render(&painter);
    painter.end();

    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}
//! [3]

//! [4]
void RasterWindow::render(QPainter *painter)
{
    painter->drawText(QRectF(0, 0, width(), height()), Qt::AlignCenter, QStringLiteral("raster window"));
}


void RasterWindow::mousePressEvent(QMouseEvent *event)  {
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        //event->globalPosition() Retrieves the global position of the cursor (in screen coordinates) at the time of the event
        //position(): Returns the current position of the window (the top-left corner in global screen coordinates).
        m_dragPosition = event->globalPosition().toPoint() - position();
    }else if (event->button() == Qt::RightButton){
        m_dragging = false;
        showContextMenu(event->globalPosition().toPoint());
    }

}

void RasterWindow::mouseMoveEvent(QMouseEvent *event)  {
    if (m_dragging) {
        setPosition(event->globalPosition().toPoint() - m_dragPosition);
    }
}

void RasterWindow::mouseReleaseEvent(QMouseEvent *event)  {
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        //best time to save the position
        settings.setValue(WINDOW_POSITION_KEY, position());
    }
}

void RasterWindow::setAlwaysOnTop(bool onTop){
    if(onTop){
        this->setFlags(this->flags() | Qt::WindowStaysOnTopHint);
        raise();

    }else{
        this->setFlags(this->flags() ^ Qt::WindowStaysOnTopHint);
    }
}
void RasterWindow::showContextMenu(const QPoint &position)   {
    QMenu menu;
    auto* stay_on_top = menu.addAction("stay on top", [this](){
        qDebug() << "clicked on top";
        m_alwaysOnTop =!m_alwaysOnTop;
        setAlwaysOnTop(m_alwaysOnTop);
        QSettings settings;
        settings.setValue(BRING_WINDOW_TO_TOP_KEY, m_alwaysOnTop);
    });
    stay_on_top->setCheckable(true);
    stay_on_top->setChecked(m_alwaysOnTop);
    menu.addAction("quit", [this](){
        this->destroy();
        QApplication::quit(); });
    menu.exec(position);
}

void RasterWindow::bringToForeground() {
    #ifdef _WIN32
    HWND hCurWnd = ::GetForegroundWindow();
    DWORD dwMyID = ::GetCurrentThreadId();
    DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);

    // Attach the thread input to ensure the window can be brought to the foreground
    ::AttachThreadInput(dwCurID, dwMyID, TRUE);

    HWND hwnd = reinterpret_cast<HWND>(winId());

    // Make this window topmost, then not topmost, to bring it to the front
    ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

    // Set the window as the foreground, focused, and active window
    ::SetForegroundWindow(hwnd);
    ::SetFocus(hwnd);
    ::SetActiveWindow(hwnd);

    // Detach the thread input
    ::AttachThreadInput(dwCurID, dwMyID, FALSE);
#else
    requestActivate();
    raise();
    setVisible(true);
#endif
}
