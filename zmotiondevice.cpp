#include "zmotiondevice.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QStringList>
#include <cstring>

ZmotionDevice::ZmotionDevice(QObject *parent)
    : QObject(parent)
    , m_libraryPath(QStringLiteral("zmotion"))
    , m_lib(m_libraryPath)
{
}

bool ZmotionDevice::isLoaded() const
{
    return m_lib.isLoaded() && m_openEth && m_openCom && m_close && m_execute && m_directCommand;
}

bool ZmotionDevice::isConnected() const
{
    return m_handle != nullptr;
}

void ZmotionDevice::setLibraryPath(const QString &path)
{
    if (m_libraryPath == path)
        return;
    m_libraryPath = path;
    if (m_lib.isLoaded()) {
        m_lib.unload();
    }
    m_lib.setFileName(m_libraryPath);
    resetApi();
}

QString ZmotionDevice::lastErrorString() const
{
    return m_lastError;
}

void ZmotionDevice::resetApi()
{
    m_openEth = nullptr;
    m_openCom = nullptr;
    m_close = nullptr;
    m_execute = nullptr;
    m_directCommand = nullptr;
    m_getConnectType = nullptr;
    m_registerRead = nullptr;
    m_registerWrite = nullptr;
    m_zauxModbusGet4xLong = nullptr;
    m_zauxModbusGet4xString = nullptr;
}

bool ZmotionDevice::ensureLoaded()
{
    if (isLoaded())
        return true;

    if (!m_lib.isLoaded()) {
        QStringList candidates;
        candidates << m_libraryPath;

        const QFileInfo fi(m_libraryPath);
        const QString base = fi.completeBaseName().isEmpty() ? m_libraryPath : fi.completeBaseName();
        const QString dllName = base.endsWith(".dll", Qt::CaseInsensitive) ? base : (base + ".dll");

        const QString appDir = QCoreApplication::applicationDirPath();
        candidates << (appDir + "/" + dllName);
        candidates << (appDir + "/../zauxdll/" + dllName);
        candidates << QStringLiteral("D:/ProgramData/qt_test/zauxdll/%1").arg(dllName);

        bool loaded = false;
        QString lastErr;
        for (const auto &c : candidates) {
            m_lib.setFileName(c);
            if (m_lib.load()) {
                loaded = true;
                m_libraryPath = c;
                break;
            }
            lastErr = m_lib.errorString();
        }

        if (!loaded) {
            m_lastError = lastErr.isEmpty() ? QStringLiteral("Failed to load zmotion") : lastErr;
            resetApi();
            return false;
        }
    }

    m_openEth = reinterpret_cast<ZMC_OpenEth_t>(m_lib.resolve("ZMC_OpenEth"));
    m_openCom = reinterpret_cast<ZMC_OpenCom_t>(m_lib.resolve("ZMC_OpenCom"));
    m_close = reinterpret_cast<ZMC_Close_t>(m_lib.resolve("ZMC_Close"));
    m_execute = reinterpret_cast<ZMC_Execute_t>(m_lib.resolve("ZMC_Execute"));
    m_directCommand = reinterpret_cast<ZMC_DirectCommand_t>(m_lib.resolve("ZMC_DirectCommand"));
    m_getConnectType = reinterpret_cast<ZMC_GetConnectType_t>(m_lib.resolve("ZMC_GetConnectType"));
    m_registerRead = reinterpret_cast<ZMC_RegisterRead_t>(m_lib.resolve("ZMC_RegisterRead"));
    m_registerWrite = reinterpret_cast<ZMC_RegisterWrite_t>(m_lib.resolve("ZMC_RegisterWrite"));
    m_zauxModbusGet4xLong = reinterpret_cast<ZAux_Modbus_Get4x_Long_t>(m_lib.resolve("ZAux_Modbus_Get4x_Long"));
    m_zauxModbusGet4xString = reinterpret_cast<ZAux_Modbus_Get4x_String_t>(m_lib.resolve("ZAux_Modbus_Get4x_String"));

    if (!m_openEth || !m_openCom || !m_close || !m_execute || !m_directCommand) {
        m_lastError = QStringLiteral("Resolve ZMC_* failed: %1").arg(m_lib.errorString());
        resetApi();
        return false;
    }

    return true;
}

int ZmotionDevice::modbusGet0x(quint16 start, quint16 inum, QByteArray *pdata)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (!m_getConnectType || !m_registerRead) {
        m_lastError = QStringLiteral("ZMC_RegisterRead/ZMC_GetConnectType not available in zmotion.dll");
        return -3;
    }
    if (!pdata || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    quint16 tmpval = 0;
    const int ctype = m_getConnectType(m_handle);
    tmpval = (ctype == 1) ? 80 : 400;

    const int bytes = (inum + 7) / 8;
    pdata->resize(bytes);
    pdata->fill('\0');

    int isend = 0;
    while (true) {
        int icur = static_cast<int>(inum) - isend;
        const int maxBits = static_cast<int>(tmpval) * 8;
        if (icur > maxBits) {
            icur = maxBits;
        }
        const int rc = m_registerRead(m_handle, "M", 1, static_cast<quint16>(start + isend), static_cast<quint16>(icur),
                                      reinterpret_cast<quint8 *>(pdata->data()) + (isend / 8));
        if (rc != 0) {
            m_lastError = QStringLiteral("ZMC_RegisterRead(M) failed (%1)").arg(rc);
            return rc;
        }
        isend += icur;
        if (isend >= inum) {
            break;
        }
    }

    return 0;
}

int ZmotionDevice::modbusGet4x(quint16 start, quint16 inum, QVector<quint16> *pdata)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (!m_getConnectType || !m_registerRead) {
        m_lastError = QStringLiteral("ZMC_RegisterRead/ZMC_GetConnectType not available in zmotion.dll");
        return -3;
    }
    if (!pdata || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    quint16 tmpval = 0;
    const int ctype = m_getConnectType(m_handle);
    tmpval = (ctype == 1) ? 80 : 400;

    pdata->resize(inum);
    int isend = 0;
    while (true) {
        int icur = static_cast<int>(inum) - isend;
        if (icur > tmpval) {
            icur = tmpval;
        }
        const int rc = m_registerRead(m_handle, "D", 16, static_cast<quint16>(start + isend), static_cast<quint16>(icur),
                                      reinterpret_cast<quint8 *>(pdata->data() + isend));
        if (rc != 0) {
            m_lastError = QStringLiteral("ZMC_RegisterRead(D) failed (%1)").arg(rc);
            return rc;
        }
        isend += icur;
        if (isend >= inum) {
            break;
        }
    }

    return 0;
}

int ZmotionDevice::modbusGet4xFloat(quint16 start, quint16 inum, QVector<float> *pdata)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (!pdata || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    QVector<quint16> tmp;
    const int rc = modbusGet4x(start, static_cast<quint16>(inum * 2), &tmp);
    if (rc != 0) {
        return rc;
    }

    pdata->resize(inum);
    for (int i = 0; i < inum; ++i) {
        const quint32 bits = static_cast<quint32>(tmp.at(i * 2)) | (static_cast<quint32>(tmp.at(i * 2 + 1)) << 16);
        float f = 0.0f;
        static_assert(sizeof(float) == sizeof(quint32));
        memcpy(&f, &bits, sizeof(float));
        (*pdata)[i] = f;
    }
    return 0;
}

int ZmotionDevice::modbusGet4xLong(quint16 start, quint16 inum, QVector<qint32> *pdata)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (!pdata || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    if (m_zauxModbusGet4xLong) {
        pdata->resize(inum);
        const int rc = m_zauxModbusGet4xLong(m_handle, start, inum, pdata->data());
        if (rc != 0) {
            m_lastError = QStringLiteral("ZAux_Modbus_Get4x_Long failed (%1)").arg(rc);
        }
        return rc;
    }

    QVector<quint16> tmp;
    const int rc = modbusGet4x(start, static_cast<quint16>(inum * 2), &tmp);
    if (rc != 0) {
        return rc;
    }

    pdata->resize(inum);
    for (int i = 0; i < inum; ++i) {
        const quint32 bits = static_cast<quint32>(tmp.at(i * 2)) | (static_cast<quint32>(tmp.at(i * 2 + 1)) << 16);
        qint32 v = 0;
        static_assert(sizeof(qint32) == sizeof(quint32));
        memcpy(&v, &bits, sizeof(qint32));
        (*pdata)[i] = v;
    }
    return 0;
}

int ZmotionDevice::modbusGet4xString(quint16 start, quint16 inum, QByteArray *pdata)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (!pdata || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }
    if (inum > 200) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    if (m_zauxModbusGet4xString) {
        pdata->resize(static_cast<int>(inum));
        pdata->fill('\0');
        const int rc = m_zauxModbusGet4xString(m_handle, start, inum, pdata->data());
        if (rc != 0) {
            m_lastError = QStringLiteral("ZAux_Modbus_Get4x_String failed (%1)").arg(rc);
        }
        return rc;
    }

    if (!m_registerRead) {
        m_lastError = QStringLiteral("ZMC_RegisterRead not available in zmotion.dll");
        return -3;
    }

    pdata->resize(static_cast<int>(inum));
    pdata->fill('\0');
    const int rc = m_registerRead(m_handle, "D", 0, start, inum, reinterpret_cast<quint8 *>(pdata->data()));
    if (rc != 0) {
        m_lastError = QStringLiteral("ZMC_RegisterRead(D,type=0) failed (%1)").arg(rc);
    }
    return rc;
}

int ZmotionDevice::modbusSet0x(quint16 start, quint16 inum, const QByteArray &data)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (data.isEmpty() || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    if (!m_registerWrite) {
        m_lastError = QStringLiteral("ZMC_RegisterWrite not available in zmotion.dll");
        return -3;
    }

    const int rc = m_registerWrite(m_handle, "M", 1, start, inum, const_cast<quint8 *>(reinterpret_cast<const quint8 *>(data.constData())));
    if (rc != 0) {
        m_lastError = QStringLiteral("ZMC_RegisterWrite(M) failed (%1)").arg(rc);
    }
    return rc;
}

int ZmotionDevice::modbusSet4x(quint16 start, quint16 inum, const QVector<quint16> &data)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (data.isEmpty() || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    if (!m_registerWrite) {
        m_lastError = QStringLiteral("ZMC_RegisterWrite not available in zmotion.dll");
        return -3;
    }

    const int rc = m_registerWrite(m_handle, "D", 16, start, inum, const_cast<quint8 *>(reinterpret_cast<const quint8 *>(data.constData())));
    if (rc != 0) {
        m_lastError = QStringLiteral("ZMC_RegisterWrite(D,type=16) failed (%1)").arg(rc);
    }
    return rc;
}

int ZmotionDevice::modbusSet4xFloat(quint16 start, quint16 inum, const QVector<float> &data)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (data.isEmpty() || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    QVector<quint16> tmp;
    tmp.resize(inum * 2);
    for (int i = 0; i < inum; ++i) {
        const float f = data.at(i);
        quint32 bits = 0;
        static_assert(sizeof(float) == sizeof(quint32));
        memcpy(&bits, &f, sizeof(float));
        tmp[i * 2] = static_cast<quint16>(bits & 0xFFFF);
        tmp[i * 2 + 1] = static_cast<quint16>((bits >> 16) & 0xFFFF);
    }

    return modbusSet4x(start, static_cast<quint16>(inum * 2), tmp);
}

int ZmotionDevice::modbusSet4xLong(quint16 start, quint16 inum, const QVector<qint32> &data)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }
    if (data.isEmpty() || inum == 0) {
        m_lastError = QStringLiteral("invalid args");
        return -4;
    }

    QVector<quint16> tmp;
    tmp.resize(inum * 2);
    for (int i = 0; i < inum; ++i) {
        const qint32 v = data.at(i);
        quint32 bits = 0;
        static_assert(sizeof(qint32) == sizeof(quint32));
        memcpy(&bits, &v, sizeof(qint32));
        tmp[i * 2] = static_cast<quint16>(bits & 0xFFFF);
        tmp[i * 2 + 1] = static_cast<quint16>((bits >> 16) & 0xFFFF);
    }

    return modbusSet4x(start, static_cast<quint16>(inum * 2), tmp);
}

int ZmotionDevice::openEthernet(const QString &ip)
{
    if (!ensureLoaded())
        return -1;
    if (m_handle) {
        m_lastError = QStringLiteral("ZMC already connected");
        return -2;
    }
    QByteArray ipBytes = ip.toLocal8Bit();
    char *ipPtr = ipBytes.data();
    const int rc = m_openEth(ipPtr, &m_handle);
    if (rc != 0) {
        m_lastError = QStringLiteral("ZMC_OpenEth failed (%1), ip=%2").arg(rc).arg(ip);
        m_handle = nullptr;
    }
    return rc;
}

int ZmotionDevice::openSerial(quint32 comId)
{
    if (!ensureLoaded())
        return -1;
    if (m_handle) {
        m_lastError = QStringLiteral("ZMC already connected");
        return -2;
    }
    const int rc = m_openCom(static_cast<uint32>(comId), &m_handle);
    if (rc != 0) {
        m_lastError = QStringLiteral("ZMC_OpenCom failed (%1)").arg(rc);
        m_handle = nullptr;
    }
    return rc;
}

int ZmotionDevice::close()
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle)
        return 0;
    const int rc = m_close(m_handle);
    if (rc != 0) {
        m_lastError = QStringLiteral("ZMC_Close failed (%1)").arg(rc);
        return rc;
    }
    m_handle = nullptr;
    return 0;
}

void ZmotionDevice::forceDisconnect()
{
    m_handle = nullptr;
}

int ZmotionDevice::execute(const QString &command, QByteArray *response, quint32 responseMaxLen, quint32 waitMs)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }

    QByteArray cmd = command.toLocal8Bit();
    QByteArray resp;
    resp.resize(static_cast<int>(responseMaxLen));
    resp.fill('\0');

    const int rc = m_execute(m_handle, cmd.constData(), static_cast<uint32>(waitMs), resp.data(), static_cast<uint32>(responseMaxLen));
    if (rc != 0) {
        m_lastError = QStringLiteral("ZMC_Execute failed (%1)").arg(rc);
        return rc;
    }

    if (response) {
        const int n = resp.indexOf('\0') >= 0 ? resp.indexOf('\0') : resp.size();
        *response = resp.left(n);
    }
    return 0;
}

int ZmotionDevice::directCommand(const QString &command, QByteArray *response, quint32 responseMaxLen)
{
    if (!ensureLoaded())
        return -1;
    if (!m_handle) {
        m_lastError = QStringLiteral("ZMC not connected");
        return -2;
    }

    QByteArray cmd = command.toLocal8Bit();
    QByteArray resp;
    resp.resize(static_cast<int>(responseMaxLen));
    resp.fill('\0');

    const int rc = m_directCommand(m_handle, cmd.constData(), resp.data(), static_cast<uint32>(responseMaxLen));
    if (rc != 0) {
        m_lastError = QStringLiteral("ZMC_DirectCommand failed (%1)").arg(rc);
        return rc;
    }

    if (response) {
        const int n = resp.indexOf('\0') >= 0 ? resp.indexOf('\0') : resp.size();
        *response = resp.left(n);
    }
    return 0;
}
