#ifndef BUTERWIDGET_H
#define BUTERWIDGET_H

#include <QPixmap>
#include <QWidget>
#include "renderthread.h"

class ButerWidget : public QWidget
{
    Q_OBJECT

public:
    ButerWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updatePixmap(const QImage &image);

private:
    RenderThread thread;
    QPixmap pixmap;
    QPoint pixmapOffset;

};

#endif // BUTERWIDGET_H
