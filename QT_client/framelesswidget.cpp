#include "framelesswidget.h"
#include <QMouseEvent>
#include <QDebug>
#include <QWindow>
#include <QGuiApplication>
#include <QScreen>
#include <QGraphicsDropShadowEffect>

FramelessWidget::FramelessWidget(QWidget *parent)
    : QWidget(parent)
    , m_bPressed(false)
{
    // 设置窗口标志为无边框
    setWindowFlags(Qt::FramelessWindowHint);

    // // 设置背景透明（如果需要圆角等效果）
    // setAttribute(Qt::WA_TranslucentBackground);

    // setStyleSheet("background-color: black; border-radius: 20px;");

    setStyleSheet("background-color: white;");

    // 启用鼠标跟踪
    setMouseTracking(true);

}


void FramelessWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bPressed = true;
        m_point = event->globalPosition().toPoint() - pos();
    }
    QWidget::mousePressEvent(event);
}


void FramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bPressed) {
        move(event->globalPosition().toPoint() - m_point);
    }
    QWidget::mouseMoveEvent(event);
}

void FramelessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_bPressed = false;
    QWidget::mouseReleaseEvent(event);
}

