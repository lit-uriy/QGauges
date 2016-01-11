#ifndef SERIALFINDER_H
#define SERIALFINDER_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QSerialPortInfo>

class SerialFinder : public QObject
{
    Q_OBJECT
public:
    explicit SerialFinder(QObject *parent = 0);
    ~SerialFinder();

private:
    QThread *workerThread;

signals:
    void portsChanged(QList<QSerialPortInfo> &);
};

#endif // SERIALFINDER_H
