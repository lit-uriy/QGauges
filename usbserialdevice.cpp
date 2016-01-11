#include "usbserialdevice.h"

UsbSerialDevice::UsbSerialDevice(int vendorId, int productId, const QString &portName) {
    m_vendorId = vendorId;
    m_productId = productId;
    m_portName = portName;
}

int UsbSerialDevice::vendorId() const {
    return m_vendorId;
}

int UsbSerialDevice::productId() const {
    return m_productId;
}

QString UsbSerialDevice::portName() const {
    return m_portName;
}

#ifdef Q_OS_LINUX

// -----
// Based on this code:
//   http://www.signal11.us/oss/udev/
// -----

extern "C" {
# include <libudev.h>
}

QList<UsbSerialDevice> UsbSerialDevice::findDevices(const QList<QPair<int, int> > &usbIdentifiers) {
    QList<UsbSerialDevice> usbSerialDevices;

    qDebug() << "Scanning udev...";
    struct udev *u_udev = udev_new();
    if (u_udev) {
        struct udev_enumerate *u_enumerate = udev_enumerate_new(u_udev);
        udev_enumerate_add_match_subsystem(u_enumerate, "tty");
        udev_enumerate_scan_devices(u_enumerate);

        struct udev_list_entry *u_devices = udev_enumerate_get_list_entry(u_enumerate);
        struct udev_list_entry *u_devListEntry;
        udev_list_entry_foreach(u_devListEntry, u_devices) {
            const char *c_deviceSysfsPath = udev_list_entry_get_name(u_devListEntry);
            struct udev_device *u_device = udev_device_new_from_syspath(u_udev, c_deviceSysfsPath);
            struct udev_device *u_deviceParent = udev_device_get_parent_with_subsystem_devtype(u_device, "usb", "usb_device");
            if (!u_deviceParent) {
                udev_device_unref(u_device);
                continue;
            }

            QString portName(udev_device_get_devnode(u_device));
            int vendorId = QString(udev_device_get_sysattr_value(u_deviceParent,"idVendor")).toInt(0, 16);
            int productId = QString(udev_device_get_sysattr_value(u_deviceParent,"idProduct")).toInt(0, 16);

            udev_device_unref(u_device);
            udev_device_unref(u_deviceParent);

            qDebug() << "-- Found USB-TTY:" << portName;
            qDebug() << "---- VID =" << vendorId;
            qDebug() << "---- PID =" << productId;

            if (usbIdentifiers.contains(qMakePair(vendorId, productId))) {
                usbSerialDevices << UsbSerialDevice(vendorId, productId, portName);
            } else {
                qDebug() << "---- Rejected by VID:PID";
            }
        }

        udev_enumerate_unref(u_enumerate);
        udev_unref(u_udev);
    } else {
        qDebug() << "-- Can't create udev";
    }

    qDebug() << "-- Devices:" << usbSerialDevices;
    return usbSerialDevices;
}
#else
# error Unsupported OS
#endif // Q_OS_LINUX

QDebug operator<<(QDebug dbg, const UsbSerialDevice &device) {
    dbg.nospace() << QString("%1[%2:%3]")
                     .arg(device.portName())
                     .arg(device.vendorId())
                     .arg(device.productId());
    return dbg.maybeSpace();
}
