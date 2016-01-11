#ifndef USBSERIALDEVICE_H
#define USBSERIALDEVICE_H

#include <QString>
#include <QList>
#include <QPair>
#include <QDebug>

class UsbSerialDevice {
public:
    UsbSerialDevice(int vendorId, int productId, const QString &portName);

    int vendorId() const;
    int productId() const;
    QString portName() const;

    static QList<UsbSerialDevice> findDevices(const QList<QPair<int, int> > &usbIdentifiers);

private:
    int m_vendorId;
    int m_productId;
    QString m_portName;
};

QDebug operator<<(QDebug dbg, const UsbSerialDevice &device);

#endif // USBSERIALDEVICE_H
