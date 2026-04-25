#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMqttClient>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QEvent>
#include "mqttmodel.h"
#include "zmotiondevice.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QModelIndex;
class QTcpSocket;
class QTimer;
class QMqttSubscription;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    void on_connectBtn_clicked();
    void on_sendBtn_clicked();
    void on_setValueBtn_clicked();
    void onSaveClicked();
    void onRestoreClicked();
    void onImportClicked();
    void onConfigCellClicked(const QModelIndex &index);
    void onDeviceConnectClicked();
    void onDeviceTypeChanged(int index);
    void onZmcConnTypeChanged(int index);

private:
    void reloadConfigItems();
    void loadIotConfig();
    void saveIotConfig();
    void autoConnectIot();
    void autoConnectDevice();
    void loadDeviceConfig();
    void saveDeviceConfig();
    void updateDeviceUi();
    void updateDeviceStatus(const QString &text);
    bool readDeviceValues();
    bool readZmcValue(int address, const QString &typeText, QString *outValue);
    void startDevicePolling();
    void stopDevicePolling();
    void beginDeviceReconnect();
    void attemptDeviceReconnect();
    void onDevicePollTick();
    void onAutoSendTick();
    void setupSystemTray();

    Ui::MainWindow *ui;
    QMqttClient *m_client;
    QString m_configFilePath;
    MqttModel m_model;
    ZmotionDevice m_zmc;
    QTcpSocket *m_modbusSocket;
    QTimer *m_devicePollTimer;
    QTimer *m_autoSendTimer;
    int m_reconnectTry;
    bool m_autoReconnecting;
    bool m_userDisconnecting;

    QMqttSubscription *m_sub;
    QMqttSubscription *m_sub2;

    bool m_syncingSplitters;
    
    QList<QString> m_recvMessages; // 存储接收的消息
    QList<QString> m_sendMessages; // 存储发送的消息
    
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
};

#endif // MAINWINDOW_H
