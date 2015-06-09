#include <QPainter>
#include <QKeyEvent>
#include <math.h>

#include "buterwidget.h"

ButerWidget::ButerWidget(QWidget *parent)
    : QWidget(parent)
{
    // Хотя это выглядит как стандартное соединение сигнал слот между двумя QObject,
    // это не так, поскольку сигнал испускается в потоке, отличном от того, где живет получатель,
    // поэтому фактически это соединение через очередь.
    //Эти соединения являются асинхронными (т.е. неблокирующими), а это
    // означает, что слот будет вызван через некоторое время после оператора emit.
    // Более того, слот будет вызван в потоке, в котором живет приёмник.
    // В данном случае сигнал испускается в рабочем потоке, а слот будет выполнен в потоке ГПИ,
    // когда управление будет возвращено в цикл обработки событий.

    connect(&thread, SIGNAL(renderedImage(QImage)), this, SLOT(updatePixmap(QImage)));

    // При использовании соединений через очередь Qt должен сохранять копию аргументов, которые были
    // переданы с сигналом, чтобы их можно было позже передать в слот.
    // Qt знает, как получить копии многих
    // типов C++ и Qt, теперь и QImage входит в их число.

//#ifndef QT_NO_CURSOR
//    setCursor(Qt::CrossCursor);
//#endif
}

void ButerWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::red);

    if (pixmap.isNull()) {
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, tr("Rendering initial image, please wait..."));
        return;
    }
        painter.drawPixmap(pixmapOffset, pixmap);
}

void ButerWidget::resizeEvent(QResizeEvent * /* event */)
{
    // Ключевое событие, когда форма всплывает.
    // Мы полагаемся на автоматический вызов resizeEvent() самим Qt при
    // отображении виджета в первый раз, для первичной генерации изображения.
    thread.render();
}

// Слот updatePixmap() выполняется, когда рабочий поток завершает формирование изображения.
// В нормальном случае мы сохраняем изображение в pixmap
// В завершении мы вызываем QWidget::update() для обновления экрана.
void ButerWidget::updatePixmap(const QImage &image)
{
    // Мы используем QImage для параметра, а QPixmap как член данных.
    // Почему бы не придерживаться одного типа? Причина в том, что QImage это
    // единственный класс, который поддерживает прямое манипулирование пикселями,
    // которые нам нужны в рабочем потоке.
    // С другой стороны, прежде чем изображение может быть отображено, оно должно быть
    // преобразовано в растровое изображение. Лучше сделать преобразование однократно здесь,
    // а не в paintEvent().
   pixmap = QPixmap::fromImage(image);   
   update();
}

