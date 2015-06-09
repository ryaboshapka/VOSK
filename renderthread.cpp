#include "renderthread.h"
#include <QtWidgets>
#include <math.h>

RenderThread::RenderThread(QObject *parent)
    : QThread(parent)
{
    // В конструкторе мы инициализируем переменные restart и abort значением false.
    // Эти переменные управляют течением функции run()
    restart = false;
    abort = false;
}


// Деструктор может быть вызван в любой момент, пока активен поток.
RenderThread::~RenderThread()
{
    mutex.lock();
    //  Мы устанавливаем abort в значение true,
    // чтобы сказать run(), что надо как можно скорее прекратить работу.
    abort = true;

    // Мы также вызываем QWaitCondition::wakeOne(), чтобы разбудить поток, если он спит.
    // (поток переводится в сон, когда ему нечего делать.)
    condition.wakeOne();
    mutex.unlock();
    // В конце деструктора мы вызываем QThread::wait() для ожидания до тех пор,
    // пока run() не завершится, перед тем, как будет вызван деструктор класса.
    wait();
}

// Функция render() вызывается из Widget всякий раз, когда ему необходимо создать новое
// изображение. Функция сохраняет параметры в переменных-членах.
// Если поток не запущен, она запускает его start(LowPriority);
// в противном случае она устанавливает restart в true
// (сообщает run(), что необходимо остановить любое
// незаконченное вычисление и начать все заново с новыми параметрами)
// и пробуждает поток, который мог бы спать.
void RenderThread::render()
{    
    QMutexLocker locker(&mutex);
    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

// Защищенная функция run() переопределена от QThread.
// Она автоматически вызывается, когда запускается поток.

// Важно отметить, что run() выполняется в собственном потоке (рабочий поток), в то время как
// конструктор и деструктор RenderThread (а также функция render()) вызываются в потоке, который был
// создан рабочим потоком. Поэтому мы должны использовать мьютекс для защиты доступа к переменным
// abort и condition, которые могут быть в любое время доступны из run().

void RenderThread::run()
{
    if (abort)
        return;
    QImage image(resultSize, QImage::Format_RGB32);
   //

    // Всякий раз, когда поток заканчивает формировать изображение,
    // он испускает сигнал renderedImage().
    if (!restart)
    {
        image.load("/home/korega/2.jpg");
        emit renderedImage(image);
    }
    mutex.lock();
    if (!restart)
        condition.wait(&mutex);
    restart = false;
    mutex.unlock();
}
