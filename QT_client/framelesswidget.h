#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QWidget>

class FramelessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWidget(QWidget *parent = nullptr);

protected:
    // 重写鼠标事件以实现窗口拖动和调整大小
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool m_bPressed;
    QPoint m_point;

};

#endif // FRAMELESSWIDGET_H
