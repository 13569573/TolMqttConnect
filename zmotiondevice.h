#ifndef ZMOTIONDEVICE_H
#define ZMOTIONDEVICE_H

#include <QObject>
#include <QLibrary>
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QtGlobal>

#if !defined(ZMOTION_CALL)
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
  #if defined(__i386__) || defined(_M_IX86)
    #define ZMOTION_CALL __stdcall
  #else
    #define ZMOTION_CALL
  #endif
#else
  #define ZMOTION_CALL
#endif
#endif

class ZmotionDevice : public QObject
{
    Q_OBJECT

public:
    explicit ZmotionDevice(QObject *parent = nullptr);

    bool isLoaded() const;
    bool isConnected() const;
    void setLibraryPath(const QString &path);

    int openEthernet(const QString &ip);
    int openSerial(quint32 comId);
    int close();
    void forceDisconnect();

    int execute(const QString &command, QByteArray *response, quint32 responseMaxLen = 8192, quint32 waitMs = 3000);
    int directCommand(const QString &command, QByteArray *response, quint32 responseMaxLen = 8192);

    int modbusGet0x(quint16 start, quint16 inum, QByteArray *pdata);
    int modbusGet4x(quint16 start, quint16 inum, QVector<quint16> *pdata);
    int modbusGet4xFloat(quint16 start, quint16 inum, QVector<float> *pdata);
    int modbusGet4xLong(quint16 start, quint16 inum, QVector<qint32> *pdata);
    int modbusGet4xString(quint16 start, quint16 inum, QByteArray *pdata);
    
    int modbusSet0x(quint16 start, quint16 inum, const QByteArray &data);
    int modbusSet4x(quint16 start, quint16 inum, const QVector<quint16> &data);
    int modbusSet4xFloat(quint16 start, quint16 inum, const QVector<float> &data);
    int modbusSet4xLong(quint16 start, quint16 inum, const QVector<qint32> &data);

    QString lastErrorString() const;

private:
    using ZMC_HANDLE = void *;
    using int32 = qint32;
    using uint32 = quint32;

    typedef int32 (ZMOTION_CALL *ZMC_OpenEth_t)(char *ipaddr, ZMC_HANDLE *phandle);
    typedef int32 (ZMOTION_CALL *ZMC_OpenCom_t)(uint32 comid, ZMC_HANDLE *phandle);
    typedef int32 (ZMOTION_CALL *ZMC_Close_t)(ZMC_HANDLE handle);
    typedef int32 (ZMOTION_CALL *ZMC_Execute_t)(ZMC_HANDLE handle, const char *pszCommand, uint32 uimswait, char *psResponse, uint32 uiResponseLength);
    typedef int32 (ZMOTION_CALL *ZMC_DirectCommand_t)(ZMC_HANDLE handle, const char *pszCommand, char *psResponse, uint32 uiResponseLength);

    typedef int32 (ZMOTION_CALL *ZMC_GetConnectType_t)(ZMC_HANDLE handle);
    typedef int32 (ZMOTION_CALL *ZMC_RegisterRead_t)(ZMC_HANDLE handle, const char *pname, qint32 type, quint16 start, quint16 inum, quint8 *pdata);
    typedef int32 (ZMOTION_CALL *ZMC_RegisterWrite_t)(ZMC_HANDLE handle, const char *pname, qint32 type, quint16 start, quint16 inum, quint8 *pdata);
    typedef int32 (ZMOTION_CALL *ZAux_Modbus_Get4x_Long_t)(ZMC_HANDLE handle, quint16 start, quint16 inum, qint32 *pidata);
    typedef int32 (ZMOTION_CALL *ZAux_Modbus_Get4x_String_t)(ZMC_HANDLE handle, quint16 start, quint16 inum, char *pidata);

    bool ensureLoaded();
    void resetApi();

    QString m_libraryPath;
    QLibrary m_lib;
    ZMC_HANDLE m_handle = nullptr;
    QString m_lastError;

    ZMC_OpenEth_t m_openEth = nullptr;
    ZMC_OpenCom_t m_openCom = nullptr;
    ZMC_Close_t m_close = nullptr;
    ZMC_Execute_t m_execute = nullptr;
    ZMC_DirectCommand_t m_directCommand = nullptr;
    ZMC_GetConnectType_t m_getConnectType = nullptr;
    ZMC_RegisterRead_t m_registerRead = nullptr;
    ZMC_RegisterWrite_t m_registerWrite = nullptr;
    ZAux_Modbus_Get4x_Long_t m_zauxModbusGet4xLong = nullptr;
    ZAux_Modbus_Get4x_String_t m_zauxModbusGet4xString = nullptr;
};

#endif // ZMOTIONDEVICE_H
