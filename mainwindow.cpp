#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mqttdelegate.h"
#include "mqttmodel.h"
#include <QAbstractItemView>
#include <QEvent>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QHeaderView>
#include <QCoreApplication>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QProgressDialog>
#include <QMqttTopicName>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QSettings>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QSignalBlocker>
#include <QRegularExpression>
#include <QTimer>
#include <QMqttSubscription>
#include <QCloseEvent>
#include <QStandardPaths>

static QString appConfigPath()
{
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appData);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    dir.mkpath("TolMqttConnect");
    return dir.absoluteFilePath("TolMqttConnect/app_config.ini");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_client(new QMqttClient(this))
    , m_configFilePath("./mqtt_config.json")
    , m_zmc(this)
    , m_modbusSocket(new QTcpSocket(this))
    , m_devicePollTimer(new QTimer(this))
    , m_autoSendTimer(new QTimer(this))
    , m_reconnectTry(0)
    , m_autoReconnecting(false)
    , m_userDisconnecting(false)
    , m_sub(nullptr)
    , m_sub2(nullptr)
    , m_syncingSplitters(false)
    , m_trayIcon(nullptr)
    , m_trayMenu(nullptr)
{
    ui->setupUi(this);
    resize(1200, 700);
    setMinimumSize(1100, 650);
    setWindowTitle(QStringLiteral("MQTT发送工具"));
    
    // 确保应用程序在系统托盘图标被点击时能够响应
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    
    // 设置系统托盘
    setupSystemTray();
    setStyleSheet(R"(
QMainWindow {
    background-color: #15181c;
}
QWidget#centralwidget {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                stop:0 #1a1e23,
                                stop:0.5 #161a1f,
                                stop:1 #101318);
}
QGroupBox {
    color: #d7dce2;
    border: 1px solid #2b323a;
    border-radius: 6px;
    margin-top: 10px;
    background-color: rgba(25, 30, 35, 180);
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    padding: 0 6px;
    left: 8px;
    color: #cfd6de;
}
QLabel {
    color: #c6cdd6;
}
QCheckBox {
    color: #c6cdd6;
}
QLabel#mqttStatusLabel, QLabel#deviceStatusLabel {
    padding-left: 8px;
    padding-right: 8px;
}
QLineEdit, QTextEdit {
    color: #e6edf3;
    background-color: #0f1317;
    border: 1px solid #2b323a;
    border-radius: 4px;
    padding: 4px 6px;
    selection-background-color: #2f81f7;
}
QLabel[cellValue="true"] {
    color: #e6edf3;
}
QTextEdit {
    font-family: Consolas, "Microsoft YaHei UI";
}
QPushButton {
    color: #e6edf3;
    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #2b3138,
                                      stop:1 #20262d);
    border: 1px solid #3a424c;
    border-radius: 4px;
    padding: 6px 10px;
}
QPushButton:hover {
    border-color: #4c5866;
    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #343c45,
                                      stop:1 #252c34);
}
QPushButton:pressed {
    background-color: #1b2026;
    border-color: #2f81f7;
}
QSplitter::handle {
    background-color: #20262d;
}
QScrollBar:vertical {
    background: #0f1317;
    width: 10px;
    margin: 0px;
}
QScrollBar::handle:vertical {
    background: #2b323a;
    min-height: 20px;
    border-radius: 4px;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}
QTabWidget::pane {
    border: 1px solid #2b323a;
    border-radius: 6px;
    top: -1px;
    background-color: rgba(10, 12, 15, 120);
}
QTabBar::tab {
    background: #20262d;
    color: #b9c2cd;
    border: 1px solid #2b323a;
    border-bottom: none;
    padding: 6px 12px;
    margin-right: 2px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    min-width: 60px;
}
QTabBar::tab:selected {
    background: #0f1317;
    color: #e6edf3;
    border-color: #2f81f7;
}
QTabBar::tab:hover {
    border-color: #4c5866;
}
QTableView {
    gridline-color: #2b323a;
    background-color: #0f1317;
    color: #e6edf3;
    alternate-background-color: #12171d;
    selection-background-color: rgba(47, 129, 247, 60);
    selection-color: #e6edf3;
}
QTableView::viewport {
    background-color: #0f1317;
}
QTableView::item {
    background-color: #0f1317;
    color: #e6edf3;
    padding: 2px 6px;
}
QTableView::item:alternate {
    background-color: #12171d;
}
QTableView::item:selected {
    background-color: rgba(47, 129, 247, 60);
    color: #e6edf3;
}
QTableCornerButton::section {
    background-color: #20262d;
    border: 1px solid #2b323a;
}
QHeaderView::section {
    background-color: #20262d;
    color: #cfd6de;
    border: 1px solid #2b323a;
    padding: 4px 6px;
}
)");

    ui->messTextEdit->setText("{ \"id\": \"admin\", \"type\": \"variant_data\", \"version\": \"1.0\", \"params\": { \"cutSpeed\": 10.00 } }");
    ui->recvTextEdit->setText(QStringLiteral(""));

    ui->mqttStatusLabel->setText(QStringLiteral("未连接"));
    ui->mqttStatusLabel->setStyleSheet(QStringLiteral("color:#c6cdd6;"));
    connect(m_client, &QMqttClient::stateChanged, this, [this](QMqttClient::ClientState s) {
        if (s == QMqttClient::Connected) {
            ui->mqttStatusLabel->setText(QStringLiteral("已连接"));
            ui->mqttStatusLabel->setStyleSheet(QStringLiteral("color:#7fb3ff;"));
            if (m_sub) {
                delete m_sub;
                m_sub = nullptr;
            }
            if (m_sub2) {
                delete m_sub2;
                m_sub2 = nullptr;
            }
            // 订阅第一个主题
            m_sub = m_client->subscribe(QMqttTopicFilter(ui->topicEdit->text()));
            if (!m_sub) {
                ui->recvTextEdit->append(QStringLiteral("[sys] 订阅失败：%1").arg(ui->topicEdit->text()));
            } else {
                ui->recvTextEdit->append(QStringLiteral("[sys] 已订阅：%1").arg(ui->topicEdit->text()));
            }
            // 订阅第二个主题
            if (!ui->topicEdit2->text().isEmpty()) {
                m_sub2 = m_client->subscribe(QMqttTopicFilter(ui->topicEdit2->text()));
                if (!m_sub2) {
                    ui->recvTextEdit->append(QStringLiteral("[sys] 订阅失败：%1").arg(ui->topicEdit2->text()));
                } else {
                    ui->recvTextEdit->append(QStringLiteral("[sys] 已订阅：%1").arg(ui->topicEdit2->text()));
                }
            }
        } else if (s == QMqttClient::Connecting) {
            ui->mqttStatusLabel->setText(QStringLiteral("连接中..."));
            ui->mqttStatusLabel->setStyleSheet(QStringLiteral("color:#cfd6de;"));
        } else {
            ui->mqttStatusLabel->setText(QStringLiteral("未连接"));
            ui->mqttStatusLabel->setStyleSheet(QStringLiteral("color:#c6cdd6;"));
            // 清理订阅对象
            if (m_sub) {
                delete m_sub;
                m_sub = nullptr;
            }
            if (m_sub2) {
                delete m_sub2;
                m_sub2 = nullptr;
            }
        }
    });
    connect(m_client, &QMqttClient::errorChanged, this, [this](QMqttClient::ClientError) {
        ui->mqttStatusLabel->setText(QStringLiteral("连接失败"));
        ui->mqttStatusLabel->setStyleSheet(QStringLiteral("color:#ff7f7f;"));
        ui->connectBtn->setText(QStringLiteral("连接MQTT"));
        // 清理订阅对象
        if (m_sub) {
            delete m_sub;
            m_sub = nullptr;
        }
        if (m_sub2) {
            delete m_sub2;
            m_sub2 = nullptr;
        }
    });

    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        // 只处理来自topic 2的消息
        if (topic.name() != ui->topicEdit2->text()) {
            return;
        }
        
        const QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        const QString line = QStringLiteral("[%1] [%2] %3")
                                 .arg(timestamp)
                                 .arg(topic.name())
                                 .arg(QString::fromUtf8(message));
        
        // 解析报文
        QJsonDocument doc = QJsonDocument::fromJson(message);
        if (!doc.isNull() && doc.isObject()) {
            QJsonObject root = doc.object();
            QString type = root["type"].toString();
            if (type == "write_variant") {
                QJsonObject params = root["params"].toObject();
                for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
                    QString addressStr = it.key();
                    QString valueStr = it.value().toString();
                    bool ok = false;
                    int address = addressStr.toInt(&ok);
                    if (ok) {
                        // 在参数表中找到对应的地址
                        for (int r = 0; r < ui->paramsTableWidget->rowCount(); ++r) {
                            auto *addrItem = ui->paramsTableWidget->item(r, 1);
                            auto *typeItem = ui->paramsTableWidget->item(r, 2);
                            auto *rwItem = ui->paramsTableWidget->item(r, 4);
                            if (!addrItem || !typeItem || !rwItem) continue;
                            
                            int paramAddr = addrItem->text().toInt(&ok);
                            if (ok && paramAddr == address) {
                                QString rwType = rwItem->text();
                                if (rwType == "1读写") {
                                    QString regType = typeItem->text();
                                    // 根据寄存器类型写入值
                                    bool writeOk = false;
                                    const int typeIndex = ui->deviceTypeCombo->currentIndex();
                                    
                                    if (regType == "00_X") {
                                        // 写入线圈
                                        int val = valueStr.toInt(&ok);
                                        if (ok) {
                                            if (typeIndex == 1 && m_zmc.isConnected()) {
                                                // ZMC 写入线圈
                                                QByteArray data;
                                                data.append(static_cast<char>(val & 0x01));
                                                int rc = m_zmc.modbusSet0x(static_cast<quint16>(address), 1, data);
                                                writeOk = (rc == 0);
                                            }
                                        }
                                    } else if (regType == "1REG") {
                                        // 写入寄存器
                                        int val = valueStr.toInt(&ok);
                                        if (ok) {
                                            if (typeIndex == 1 && m_zmc.isConnected()) {
                                                // ZMC 写入寄存器
                                                QVector<quint16> data;
                                                data.append(static_cast<quint16>(val));
                                                int rc = m_zmc.modbusSet4x(static_cast<quint16>(address), 1, data);
                                                writeOk = (rc == 0);
                                            }
                                        }
                                    } else if (regType == "2IEEE" || regType == "2") {
                                        // 写入浮点数
                                        float val = valueStr.toFloat(&ok);
                                        if (ok) {
                                            if (typeIndex == 1 && m_zmc.isConnected()) {
                                                // ZMC 写入浮点数
                                                QVector<float> data;
                                                data.append(val);
                                                int rc = m_zmc.modbusSet4xFloat(static_cast<quint16>(address), 1, data);
                                                writeOk = (rc == 0);
                                            }
                                        }
                                    } else if (regType == "3LONG") {
                                        // 写入长整数
                                        qint32 val = valueStr.toLong(&ok);
                                        if (ok) {
                                            if (typeIndex == 1 && m_zmc.isConnected()) {
                                                // ZMC 写入长整数
                                                QVector<qint32> data;
                                                data.append(val);
                                                int rc = m_zmc.modbusSet4xLong(static_cast<quint16>(address), 1, data);
                                                writeOk = (rc == 0);
                                            }
                                        }
                                    }
                                    
                                    if (writeOk) {
                                        // 更新UI中的值
                                        auto *valItem = ui->paramsTableWidget->item(r, 5);
                                        if (valItem) {
                                            valItem->setText(valueStr);
                                        }
                                        
                                        // 添加到接收消息列表
                                        QString writeLine = QStringLiteral("[%1] [sys] 成功写入地址 %2: %3")
                                                           .arg(timestamp)
                                                           .arg(address)
                                                           .arg(valueStr);
                                        m_recvMessages.prepend(writeLine);
                                    } else {
                                        // 添加错误消息
                                        QString errorLine = QStringLiteral("[%1] [sys] 写入地址 %2 失败: 无效的值 %3")
                                                           .arg(timestamp)
                                                           .arg(address)
                                                           .arg(valueStr);
                                        m_recvMessages.prepend(errorLine);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // 添加到消息列表的开头，使最新消息显示在最上方
        m_recvMessages.prepend(line);
        
        // 限制只保留最近20次消息
        if (m_recvMessages.size() > 20) {
            m_recvMessages.removeLast();
        }
        
        // 更新文本框
        if (ui->recvTextEdit) {
            ui->recvTextEdit->setText(m_recvMessages.join("\n"));
            
            // 滚动到最顶部（最新消息位置）
            QTextCursor cursor = ui->recvTextEdit->textCursor();
            cursor.movePosition(QTextCursor::Start);
            ui->recvTextEdit->setTextCursor(cursor);
        }
    });

    connect(ui->hostEdit, &QLineEdit::editingFinished, this, &MainWindow::saveIotConfig);
    connect(ui->portEdit, &QLineEdit::editingFinished, this, &MainWindow::saveIotConfig);
    connect(ui->userNameEdit, &QLineEdit::editingFinished, this, &MainWindow::saveIotConfig);
    connect(ui->PassWordEdit, &QLineEdit::editingFinished, this, &MainWindow::saveIotConfig);
    connect(ui->topicEdit, &QLineEdit::editingFinished, this, [this] {
        saveIotConfig();
        if (m_client->state() == QMqttClient::Connected) {
            if (m_sub) {
                delete m_sub;
                m_sub = nullptr;
            }
            m_sub = m_client->subscribe(QMqttTopicFilter(ui->topicEdit->text()));
            if (!m_sub) {
                ui->recvTextEdit->append(QStringLiteral("[sys] 订阅失败：%1").arg(ui->topicEdit->text()));
            } else {
                ui->recvTextEdit->append(QStringLiteral("[sys] 已订阅：%1").arg(ui->topicEdit->text()));
            }
        }
    });
    connect(ui->topicEdit2, &QLineEdit::editingFinished, this, [this] {
        saveIotConfig();
        if (m_client->state() == QMqttClient::Connected) {
            if (m_sub2) {
                delete m_sub2;
                m_sub2 = nullptr;
            }
            if (!ui->topicEdit2->text().isEmpty()) {
                m_sub2 = m_client->subscribe(QMqttTopicFilter(ui->topicEdit2->text()));
                if (!m_sub2) {
                    ui->recvTextEdit->append(QStringLiteral("[sys] 订阅失败：%1").arg(ui->topicEdit2->text()));
                } else {
                    ui->recvTextEdit->append(QStringLiteral("[sys] 已订阅：%1").arg(ui->topicEdit2->text()));
                }
            }
        }
    });

    ui->configTableView->setModel(&m_model);
    ui->configTableView->setItemDelegate(new MqttDelegate(this));
    ui->configTableView->verticalHeader()->setVisible(false);
    ui->configTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->configTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->configTableView->horizontalHeader()->setStretchLastSection(true);
    ui->configTableView->setAlternatingRowColors(true);
    ui->configTableView->setColumnWidth(0, 100);
    ui->configTableView->setColumnWidth(1, 100);
    ui->configTableView->setColumnWidth(2, 220);
    ui->configTableView->setColumnWidth(3, 100);
    ui->configTableView->setColumnWidth(4, 80);
    ui->configTableView->setColumnWidth(5, 100);
    ui->paramsTableWidget->verticalHeader()->setVisible(false);
    ui->paramsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->paramsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->paramsTableWidget->setShowGrid(false);
    ui->paramsTableWidget->setAlternatingRowColors(true);
    ui->paramsTableWidget->setWordWrap(false);
    ui->paramsTableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->paramsTableWidget->setColumnWidth(0, 36);
    ui->paramsTableWidget->setColumnWidth(1, 110);
    ui->paramsTableWidget->setColumnWidth(2, 110);
    ui->paramsTableWidget->setColumnWidth(4, 90);

    auto *pHeader = ui->paramsTableWidget->horizontalHeader();
    pHeader->setSectionResizeMode(0, QHeaderView::Fixed);
    pHeader->setSectionResizeMode(1, QHeaderView::Fixed);
    pHeader->setSectionResizeMode(2, QHeaderView::Fixed);
    pHeader->setSectionResizeMode(3, QHeaderView::Stretch);
    pHeader->setSectionResizeMode(4, QHeaderView::Fixed);
    pHeader->setSectionResizeMode(5, QHeaderView::Stretch);

    connect(ui->deviceConnectBtn, &QPushButton::clicked, this, &MainWindow::onDeviceConnectClicked);
    connect(ui->deviceTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onDeviceTypeChanged);
    connect(ui->zmcConnCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onZmcConnTypeChanged);
    // connect(ui->mbIpEdit, &QLineEdit::editingFinished, this, &MainWindow::saveDeviceConfig);
    // connect(ui->mbIpEdit, &QLineEdit::textChanged, this, [this](const QString &) { saveDeviceConfig(); });
    //connect(ui->mbPortSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int) { saveDeviceConfig(); });
    //connect(ui->zmcIpEdit, &QLineEdit::editingFinished, this, &MainWindow::saveDeviceConfig);
    //connect(ui->zmcIpEdit, &QLineEdit::textChanged, this, [this](const QString &) { saveDeviceConfig(); });
    //connect(ui->zmcComSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int) { saveDeviceConfig(); });

    ui->splitterLeft->setSizes(ui->splitterRight->sizes());
    connect(ui->splitterRight, &QSplitter::splitterMoved, this, [this](int, int) {
        if (m_syncingSplitters)
            return;
        m_syncingSplitters = true;
        ui->splitterLeft->setSizes(ui->splitterRight->sizes());
        m_syncingSplitters = false;
    });
    connect(ui->splitterLeft, &QSplitter::splitterMoved, this, [this](int, int) {
        if (m_syncingSplitters)
            return;
        m_syncingSplitters = true;
        ui->splitterRight->setSizes(ui->splitterLeft->sizes());
        m_syncingSplitters = false;
    });

    ui->splitterMsg->setStretchFactor(0, 1);
    ui->splitterMsg->setStretchFactor(1, 1);
    QTimer::singleShot(0, this, [this] {
        const int w = ui->splitterMsg->width();
        if (w > 0) {
            ui->splitterMsg->setSizes({w / 2, w - (w / 2)});
        }
    });

    m_devicePollTimer->setInterval(1000);
    connect(m_devicePollTimer, &QTimer::timeout, this, &MainWindow::onDevicePollTick);

    m_autoSendTimer->setInterval(1000);
    connect(m_autoSendTimer, &QTimer::timeout, this, &MainWindow::onAutoSendTick);
    connect(ui->autoSendCheckBox, &QCheckBox::toggled, this, [this](bool on) {
        ui->setValueBtn->setEnabled(!on);
        ui->sendBtn->setEnabled(!on);
        if (on) {
            if (!m_autoSendTimer->isActive()) {
                m_autoSendTimer->start();
            }
            onAutoSendTick();
        } else {
            m_autoSendTimer->stop();
        }
    });
    {
        const bool on = ui->autoSendCheckBox->isChecked();
        ui->setValueBtn->setEnabled(!on);
        ui->sendBtn->setEnabled(!on);
        if (on) {
            if (!m_autoSendTimer->isActive()) {
                m_autoSendTimer->start();
            }
            onAutoSendTick();
        } else {
            m_autoSendTimer->stop();
        }
    }

    connect(ui->sendClearBtn, &QPushButton::clicked, this, [this] {
        ui->messTextEdit->clear();
        m_sendMessages.clear();
    });
    connect(ui->recvClearBtn, &QPushButton::clicked, this, [this] {
        ui->recvTextEdit->clear();
        m_recvMessages.clear();
    });

    connect(m_modbusSocket, &QTcpSocket::connected, this, [this] {
        updateDeviceStatus(QStringLiteral("Modbus TCP 已连接"));
        ui->deviceConnectBtn->setText(QStringLiteral("断开下位机"));
        m_reconnectTry = 0;
        m_autoReconnecting = false;
        startDevicePolling();
    });
    connect(m_modbusSocket, &QTcpSocket::disconnected, this, [this] {
        updateDeviceStatus(QStringLiteral("未连接"));
        ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
        stopDevicePolling();
        if (m_userDisconnecting) {
            m_userDisconnecting = false;
            return;
        }
        beginDeviceReconnect();
    });
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(m_modbusSocket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
            updateDeviceStatus(QStringLiteral("Modbus TCP 连接失败"));
            ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
            stopDevicePolling();
            if (!m_userDisconnecting) {
                beginDeviceReconnect();
            }
    });
#else
    connect(m_modbusSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, [this](QAbstractSocket::SocketError) {
            updateDeviceStatus(QStringLiteral("Modbus TCP 连接失败"));
            ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
            stopDevicePolling();
            if (!m_userDisconnecting) {
                beginDeviceReconnect();
            }
    });
#endif

    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(ui->restoreButton, &QPushButton::clicked, this, &MainWindow::onRestoreClicked);
    connect(ui->importButton, &QPushButton::clicked, this, &MainWindow::onImportClicked);
    connect(ui->configTableView, &QTableView::clicked, this, &MainWindow::onConfigCellClicked);

    if (QFile::exists(m_configFilePath)) {
        m_model.loadFromFile(m_configFilePath);
    } else {
        m_model.addItem();
    }


    loadDeviceConfig();
    loadIotConfig();
    updateDeviceUi();
    reloadConfigItems();

    QTimer::singleShot(0, this, [this] {
        autoConnectIot();
        autoConnectDevice();
    });
}

MainWindow::~MainWindow()
{
    // 停止定时器
    if (m_devicePollTimer->isActive()) {
        m_devicePollTimer->stop();
    }
    if (m_autoSendTimer->isActive()) {
        m_autoSendTimer->stop();
    }
    
    // 清理订阅对象
    if (m_sub) {
        delete m_sub;
        m_sub = nullptr;
    }
    if (m_sub2) {
        delete m_sub2;
        m_sub2 = nullptr;
    }
    
    // 断开MQTT连接
    if (m_client->state() != QMqttClient::Disconnected) {
        m_client->disconnectFromHost();
    }
    
    // 断开设备连接
    if (m_zmc.isConnected()) {
        m_zmc.close();
    }
    if (m_modbusSocket->state() != QAbstractSocket::UnconnectedState) {
        m_modbusSocket->disconnectFromHost();
    }
    
    delete ui;
}

void MainWindow::setupSystemTray()
{
    // 检查系统是否支持系统托盘
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("系统不支持系统托盘功能！"));
        return;
    }
    
    // 创建系统托盘图标
    m_trayIcon = new QSystemTrayIcon(this);
    
    // 尝试使用不同的图标方式
    QIcon icon;
    // 尝试从资源加载图标
    if (QFile::exists("./icon.ico")) {
        icon = QIcon("./icon.ico");
    } else {
        // 使用内置图标
        icon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
    }
    
    m_trayIcon->setIcon(icon);
    m_trayIcon->setToolTip(windowTitle());
    
    // 创建托盘菜单
    m_trayMenu = new QMenu(this);
    
    // 显示窗口动作
    QAction *showAction = new QAction(QStringLiteral("显示窗口"), this);
    connect(showAction, &QAction::triggered, this, [this]() {
        showNormal();
        raise();
        activateWindow();
    });
    m_trayMenu->addAction(showAction);
    
    // 退出动作
    QAction *exitAction = new QAction(QStringLiteral("退出"), this);
    connect(exitAction, &QAction::triggered, this, [this]() {
        // 真正退出程序
        QApplication::quit();
    });
    m_trayMenu->addAction(exitAction);
    
    // 设置托盘菜单
    m_trayIcon->setContextMenu(m_trayMenu);
    
    // 显示托盘图标
    m_trayIcon->show();
    
    // 点击托盘图标时显示/隐藏窗口
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            if (isVisible()) {
                hide();
                m_trayIcon->showMessage(windowTitle(), QStringLiteral("程序已最小化到托盘"), QSystemTrayIcon::Information, 2000);
            } else {
                showNormal();
                raise();
                activateWindow();
            }
        }
    });
    
    // 测试消息
    m_trayIcon->showMessage(windowTitle(), QStringLiteral("程序已启动，最小化时会缩小到托盘"), QSystemTrayIcon::Information, 3000);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 检查系统托盘是否可用且托盘图标是否可见
    if (QSystemTrayIcon::isSystemTrayAvailable() && m_trayIcon && m_trayIcon->isVisible()) {
        // 最小化到托盘而不是关闭
        hide();
        m_trayIcon->showMessage(windowTitle(), QStringLiteral("程序已最小化到托盘，点击托盘图标恢复"), QSystemTrayIcon::Information, 2000);
        event->ignore();
    } else {
        // 系统托盘不可用，正常关闭程序
        // 保存配置
        // saveIotConfig();
        // saveDeviceConfig();
        QMainWindow::closeEvent(event);
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    
    if (event->type() == QEvent::WindowStateChange) {
        // 当窗口最小化时，隐藏窗口并显示在托盘
        if (windowState() & Qt::WindowMinimized) {
            if (QSystemTrayIcon::isSystemTrayAvailable() && m_trayIcon && m_trayIcon->isVisible()) {
                hide();
                m_trayIcon->showMessage(windowTitle(), QStringLiteral("程序已最小化到托盘，点击托盘图标恢复"), QSystemTrayIcon::Information, 2000);
            }
        }
    }
}

void MainWindow::startDevicePolling()
{
    if (!m_devicePollTimer->isActive()) {
        m_devicePollTimer->start();
    }
}

void MainWindow::stopDevicePolling()
{
    if (m_devicePollTimer->isActive()) {
        m_devicePollTimer->stop();
    }
}

void MainWindow::beginDeviceReconnect()
{
    if (m_autoReconnecting)
        return;
    m_autoReconnecting = true;
    m_reconnectTry = 0;
    attemptDeviceReconnect();
}

void MainWindow::attemptDeviceReconnect()
{
    if (!m_autoReconnecting)
        return;

    if (m_reconnectTry >= 3) {
        m_autoReconnecting = false;
        updateDeviceStatus(QStringLiteral("断开"));
        ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
        stopDevicePolling();
        return;
    }

    m_reconnectTry++;
    updateDeviceStatus(QStringLiteral("重连中(%1/3)...").arg(m_reconnectTry));

    const int typeIndex = ui->deviceTypeCombo->currentIndex();
    if (typeIndex == 0) {
        if (m_modbusSocket->state() == QAbstractSocket::ConnectedState) {
            m_reconnectTry = 0;
            m_autoReconnecting = false;
            updateDeviceStatus(QStringLiteral("Modbus TCP 已连接"));
            ui->deviceConnectBtn->setText(QStringLiteral("断开下位机"));
            startDevicePolling();
            return;
        }
        const QString ip = ui->mbIpEdit->text().trimmed();
        const int port = ui->mbPortSpin->value();
        m_modbusSocket->abort();
        m_modbusSocket->connectToHost(ip, static_cast<quint16>(port));
        QTimer::singleShot(1000, this, &MainWindow::attemptDeviceReconnect);
        return;
    }

    if (m_zmc.isConnected()) {
        m_zmc.forceDisconnect();
    }

    const int connIndex = ui->zmcConnCombo->currentIndex();
    int rc = -1;
    if (connIndex == 0) {
        rc = m_zmc.openEthernet(ui->zmcIpEdit->text().trimmed());
    } else {
        rc = m_zmc.openSerial(static_cast<quint32>(ui->zmcComSpin->value()));
    }

    if (rc == 0) {
        m_reconnectTry = 0;
        m_autoReconnecting = false;
        updateDeviceStatus(QStringLiteral("ZMC 已连接"));
        ui->deviceConnectBtn->setText(QStringLiteral("断开下位机"));
        startDevicePolling();
        readDeviceValues();
        return;
    }

    QTimer::singleShot(1000, this, &MainWindow::attemptDeviceReconnect);
}

void MainWindow::onDevicePollTick()
{
    const int typeIndex = ui->deviceTypeCombo->currentIndex();
    if (typeIndex == 0) {
        if (m_modbusSocket->state() != QAbstractSocket::ConnectedState) {
            stopDevicePolling();
            beginDeviceReconnect();
        }
        return;
    }

    if (!m_zmc.isConnected()) {
        stopDevicePolling();
        beginDeviceReconnect();
        return;
    }

    if (!readDeviceValues()) {
        stopDevicePolling();
        m_zmc.forceDisconnect();
        beginDeviceReconnect();
    }
}

void MainWindow::onAutoSendTick()
{
    if (!ui->autoSendCheckBox->isChecked())
        return;
    if (m_client->state() != QMqttClient::Connected)
        return;
    if (ui->topicEdit->text().isEmpty())
        return;

    // 先获取当前要发送的内容
    QJsonObject root;
    root["id"] = "admin";
    root["type"] = "variant_data";
    root["version"] = "1.0";

    QJsonObject params;
    for (int r = 0; r < ui->paramsTableWidget->rowCount(); ++r) {
        auto *addrItem = ui->paramsTableWidget->item(r, 1);
        auto *valItem = ui->paramsTableWidget->item(r, 5);
        if (!addrItem || !valItem)
            continue;
        const QString key = addrItem->text().trimmed();
        const QString text = valItem->text().trimmed();
        bool ok = false;
        const double v = text.toDouble(&ok);
        if (ok) {
            params[key] = v;
        } else {
            params[key] = text;
        }
    }
    root["params"] = params;

    QJsonDocument doc(root);
    const QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString message = QStringLiteral("[%1]\n%2").arg(timestamp).arg(QString::fromUtf8(doc.toJson(QJsonDocument::Indented)));
    
    // 发送消息
    m_client->publish(QMqttTopicName(ui->topicEdit->text()), doc.toJson(QJsonDocument::Indented));
    
    // 发送后添加到发送消息列表的开头，使最新消息显示在最上方
    m_sendMessages.prepend(message);
    
    // 限制只保留最近20次发送的消息
    if (m_sendMessages.size() > 20) {
        m_sendMessages.removeLast();
    }
    
    // 更新发送文本框
    ui->messTextEdit->setText(m_sendMessages.join("\n\n"));
}

void MainWindow::loadDeviceConfig()
{
    QString configPath = appConfigPath();
    QSettings s(configPath, QSettings::IniFormat);
    s.beginGroup("device");
    ui->deviceTypeCombo->setCurrentIndex(s.value("typeIndex", 0).toInt());
    
    QString modbusIp = s.value("modbus/ip", "192.168.0.10").toString().trimmed();
    if (modbusIp.isEmpty()) {
        modbusIp = "192.168.0.10";
    }
    ui->mbIpEdit->setText(modbusIp);
    
    ui->mbPortSpin->setValue(s.value("modbus/port", 502).toInt());
    ui->zmcConnCombo->setCurrentIndex(s.value("zmc/connIndex", 0).toInt());
    
    QString zmcIp = s.value("zmc/ip", "192.168.0.11").toString().trimmed();
    if (zmcIp.isEmpty()) {
        zmcIp = "192.168.0.11";
    }
    ui->zmcIpEdit->setText(zmcIp);
    
    ui->zmcComSpin->setValue(s.value("zmc/comId", 3).toInt());
    const QString libPath = s.value("zmc/libPath", "zmotion").toString();
    m_zmc.setLibraryPath(libPath);
    s.endGroup();
    updateDeviceStatus(QStringLiteral("未连接"));
    ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
    qDebug() << "Device config loaded from:" << configPath;
    qDebug() << "Loaded Modbus IP:" << ui->mbIpEdit->text();
    qDebug() << "Loaded ZMC IP:" << ui->zmcIpEdit->text();
}

void MainWindow::saveDeviceConfig()
{
    QString configPath = appConfigPath();
    QSettings s(configPath, QSettings::IniFormat);
    s.beginGroup("device");
    s.setValue("typeIndex", ui->deviceTypeCombo->currentIndex());
    s.setValue("modbus/ip", ui->mbIpEdit->text().trimmed());
    s.setValue("modbus/port", ui->mbPortSpin->value());
    s.setValue("zmc/connIndex", ui->zmcConnCombo->currentIndex());
    s.setValue("zmc/ip", ui->zmcIpEdit->text().trimmed());
    s.setValue("zmc/comId", ui->zmcComSpin->value());
    s.setValue("zmc/libPath", "zmotion");
    s.setValue("autoConnect", true);
    s.endGroup();
    qDebug() << "Device config saved to:" << configPath;
    qDebug() << "Modbus IP:" << ui->mbIpEdit->text().trimmed();
    qDebug() << "ZMC IP:" << ui->zmcIpEdit->text().trimmed();
}

void MainWindow::loadIotConfig()
{
    QSettings s(appConfigPath(), QSettings::IniFormat);
    s.beginGroup("iot");
    ui->hostEdit->setText(s.value("host", "www.sukon-cloud.com").toString());
    ui->portEdit->setText(s.value("port", "9006").toString());
    ui->topicEdit->setText(s.value("topic", "sys/device/eewcqJPjw9w&SKSJ-XD-0001/variant_data").toString());
    ui->topicEdit2->setText(s.value("topic2", "").toString());
    ui->userNameEdit->setText(s.value("username", "eewcqJPjw9w&SKSJ-XD-0001").toString());
    ui->PassWordEdit->setText(s.value("password", "7244c7ed-6a").toString());
    s.endGroup();
}

void MainWindow::saveIotConfig()
{
    QSettings s(appConfigPath(), QSettings::IniFormat);
    s.beginGroup("iot");
    s.setValue("host", ui->hostEdit->text().trimmed());
    s.setValue("port", ui->portEdit->text().trimmed());
    s.setValue("topic", ui->topicEdit->text().trimmed());
    s.setValue("topic2", ui->topicEdit2->text().trimmed());
    s.setValue("username", ui->userNameEdit->text().trimmed());
    s.setValue("password", ui->PassWordEdit->text());
    s.setValue("autoConnect", true);
    s.endGroup();
}

void MainWindow::autoConnectIot()
{
    if (m_client->state() != QMqttClient::Disconnected)
        return;
    QSettings s(appConfigPath(), QSettings::IniFormat);
    s.beginGroup("iot");
    const bool autoConnect = s.value("autoConnect", true).toBool();
    s.endGroup();
    if (!autoConnect)
        return;
    on_connectBtn_clicked();
}

void MainWindow::autoConnectDevice()
{
    const int typeIndex = ui->deviceTypeCombo->currentIndex();
    bool already = false;
    if (typeIndex == 0) {
        already = (m_modbusSocket->state() == QAbstractSocket::ConnectedState);
    } else {
        already = m_zmc.isConnected();
    }
    if (already)
        return;

    QSettings s(appConfigPath(), QSettings::IniFormat);
    s.beginGroup("device");
    const bool autoConnect = s.value("autoConnect", true).toBool();
    s.endGroup();
    if (!autoConnect)
        return;

    onDeviceConnectClicked();
}

void MainWindow::updateDeviceUi()
{
    const int typeIndex = ui->deviceTypeCombo->currentIndex();
    ui->deviceStack->setCurrentIndex(typeIndex);
    if (typeIndex == 1) {
        ui->zmcStack->setCurrentIndex(ui->zmcConnCombo->currentIndex());
    }
}

void MainWindow::updateDeviceStatus(const QString &text)
{
    ui->deviceStatusLabel->setText(text);
    if (text.contains(QStringLiteral("已连接"))) {
        ui->deviceStatusLabel->setStyleSheet(QStringLiteral("color:#7fb3ff;"));
    } else if (text.contains(QStringLiteral("失败"))) {
        ui->deviceStatusLabel->setStyleSheet(QStringLiteral("color:#ff7f7f;"));
    } else {
        ui->deviceStatusLabel->setStyleSheet(QStringLiteral("color:#c6cdd6;"));
    }
}

void MainWindow::onDeviceTypeChanged(int)
{
    stopDevicePolling();
    m_autoReconnecting = false;
    m_reconnectTry = 0;
    if (m_zmc.isConnected()) {
        m_zmc.close();
    }
    if (m_modbusSocket->state() != QAbstractSocket::UnconnectedState) {
        m_modbusSocket->disconnectFromHost();
    }
    updateDeviceStatus(QStringLiteral("未连接"));
    ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
    updateDeviceUi();
    //saveDeviceConfig();
}

void MainWindow::onZmcConnTypeChanged(int)
{
    stopDevicePolling();
    m_autoReconnecting = false;
    m_reconnectTry = 0;
    if (m_zmc.isConnected()) {
        m_zmc.close();
        updateDeviceStatus(QStringLiteral("未连接"));
        ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
    }
    updateDeviceUi();
    saveDeviceConfig();
}

void MainWindow::onDeviceConnectClicked()
{
    saveDeviceConfig();
    stopDevicePolling();
    m_autoReconnecting = false;
    m_reconnectTry = 0;

    const int typeIndex = ui->deviceTypeCombo->currentIndex();
    if (typeIndex == 0) {
        if (m_zmc.isConnected()) {
            m_zmc.close();
        }
        if (m_modbusSocket->state() == QAbstractSocket::ConnectedState) {
            m_userDisconnecting = true;
            m_modbusSocket->disconnectFromHost();
            return;
        }
        const QString ip = ui->mbIpEdit->text().trimmed();
        const int port = ui->mbPortSpin->value();
        updateDeviceStatus(QStringLiteral("Modbus TCP 连接中..."));
        ui->deviceConnectBtn->setText(QStringLiteral("断开下位机"));
        m_modbusSocket->connectToHost(ip, static_cast<quint16>(port));
        return;
    }

    if (m_modbusSocket->state() != QAbstractSocket::UnconnectedState) {
        m_userDisconnecting = true;
        m_modbusSocket->disconnectFromHost();
    }

    if (m_zmc.isConnected()) {
        m_userDisconnecting = true;
        m_zmc.close();
        updateDeviceStatus(QStringLiteral("未连接"));
        ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
        m_userDisconnecting = false;
        return;
    }

    updateDeviceStatus(QStringLiteral("ZMC 连接中..."));
    const int connIndex = ui->zmcConnCombo->currentIndex();
    int rc = -1;
    if (connIndex == 0) {
        rc = m_zmc.openEthernet(ui->zmcIpEdit->text().trimmed());
    } else {
        rc = m_zmc.openSerial(static_cast<quint32>(ui->zmcComSpin->value()));
    }

    if (rc == 0) {
        updateDeviceStatus(QStringLiteral("ZMC 已连接"));
        ui->deviceConnectBtn->setText(QStringLiteral("断开下位机"));
        startDevicePolling();
        readDeviceValues();
    } else {
        updateDeviceStatus(QStringLiteral("ZMC 连接失败"));
        ui->deviceConnectBtn->setText(QStringLiteral("连接下位机"));
        QMessageBox::warning(this, QStringLiteral("提示"), m_zmc.lastErrorString());
    }
}

bool MainWindow::readZmcValue(int address, const QString &typeText, QString *outValue)
{
    const QString t = typeText.trimmed().toUpper();

    if (address < 0 || address > 65535) {
        return false;
    }

    const quint16 start = static_cast<quint16>(address);

    if (t == "00_X") {
        QByteArray bits;
        const int rc = m_zmc.modbusGet0x(start, 1, &bits);
        if (rc != 0 || bits.isEmpty())
            return false;
        const int v = (bits.at(0) & 0x01) ? 1 : 0;
        if (outValue) *outValue = QString::number(v);
        return true;
    }

    if (t == "1REG") {
        QVector<quint16> regs;
        const int rc = m_zmc.modbusGet4x(start, 1, &regs);
        if (rc != 0 || regs.isEmpty())
            return false;
        if (outValue) *outValue = QString::number(regs.at(0));
        return true;
    }

    if (t == "2IEEE" || t == "2") {
        QVector<float> values;
        const int rc = m_zmc.modbusGet4xFloat(start, 1, &values);
        if (rc != 0 || values.isEmpty())
            return false;
        if (outValue) *outValue = QString::number(values.at(0), 'f', 3);
        return true;
    }

    if (t == "3LONG") {
        QVector<qint32> values;
        const int rc = m_zmc.modbusGet4xLong(start, 1, &values);
        if (rc != 0 || values.isEmpty())
            return false;
        if (outValue) *outValue = QString::number(values.at(0));
        return true;
    }

    if (t == "4STRING") {
        QByteArray buf;
        const quint16 len = 64;
        const int rc = m_zmc.modbusGet4xString(start, len, &buf);
        if (rc != 0)
            return false;
        const int end = buf.indexOf('\0') >= 0 ? buf.indexOf('\0') : buf.size();
        QString s = QString::fromLocal8Bit(buf.left(end)).trimmed();
        s.remove(QChar('\r'));
        s.remove(QChar('\n'));
        if (outValue) *outValue = s.trimmed();
        return true;
    }

    return false;
}

bool MainWindow::readDeviceValues()
{
    const int typeIndex = ui->deviceTypeCombo->currentIndex();
    if (typeIndex != 1) {
        return true;
    }
    if (!m_zmc.isConnected()) {
        return false;
    }

    QSignalBlocker b(ui->paramsTableWidget);
    bool okAll = true;
    for (int r = 0; r < ui->paramsTableWidget->rowCount(); ++r) {
        auto *addrItem = ui->paramsTableWidget->item(r, 1);
        auto *typeItem = ui->paramsTableWidget->item(r, 2);
        auto *valItem = ui->paramsTableWidget->item(r, 5);
        if (!addrItem || !typeItem || !valItem)
            continue;

        bool ok = false;
        const int addr = addrItem->text().toInt(&ok);
        if (!ok)
            continue;

        QString value;
        const bool got = readZmcValue(addr, typeItem->text(), &value);
        if (got) {
            valItem->setText(value);
        } else {
            okAll = false;
        }
    }
    return okAll;
}

void MainWindow::reloadConfigItems()
{
    QHash<int, QString> oldValues;
    for (int r = 0; r < ui->paramsTableWidget->rowCount(); ++r) {
        auto *addrItem = ui->paramsTableWidget->item(r, 1);
        auto *valItem = ui->paramsTableWidget->item(r, 5);
        if (!addrItem || !valItem)
            continue;
        bool ok = false;
        const int addr = addrItem->text().toInt(&ok);
        if (ok) {
            oldValues.insert(addr, valItem->text());
        }
    }

    ui->paramsTableWidget->setRowCount(0);

    const auto items = m_model.enabledItems();
    ui->paramsTableWidget->setRowCount(items.size());
    for (int i = 0; i < items.size(); ++i) {
        const auto &item = items.at(i);
        const QString regName = item.mqttName.trimmed().isEmpty() ? QStringLiteral("-") : item.mqttName.trimmed();
        const QString addrText = QString::number(item.registerAddress);
        const QString typeText = MqttModel::registerTypeText(item.registerType);
        const QString rwText = MqttModel::writeTypeText(item.writeType);

        auto *idxItem = new QTableWidgetItem(QString::number(i + 1));
        idxItem->setFlags(idxItem->flags() & ~Qt::ItemIsEditable);
        idxItem->setTextAlignment(Qt::AlignCenter);
        ui->paramsTableWidget->setItem(i, 0, idxItem);

        auto *addrItem = new QTableWidgetItem(addrText);
        addrItem->setFlags(addrItem->flags() & ~Qt::ItemIsEditable);
        addrItem->setTextAlignment(Qt::AlignCenter);
        ui->paramsTableWidget->setItem(i, 1, addrItem);

        auto *typeItem = new QTableWidgetItem(typeText);
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        ui->paramsTableWidget->setItem(i, 2, typeItem);

        auto *nameItem = new QTableWidgetItem(regName);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        ui->paramsTableWidget->setItem(i, 3, nameItem);

        auto *rwItem = new QTableWidgetItem(rwText);
        rwItem->setFlags(rwItem->flags() & ~Qt::ItemIsEditable);
        ui->paramsTableWidget->setItem(i, 4, rwItem);

        auto *valItem = new QTableWidgetItem(oldValues.value(item.registerAddress, "0"));
        valItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->paramsTableWidget->setItem(i, 5, valItem);
    }
}

void MainWindow::on_connectBtn_clicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        m_client->setHostname(ui->hostEdit->text());
        m_client->setPort(ui->portEdit->text().toInt());
        m_client->setClientId(ui->userNameEdit->text());
        m_client->setUsername(ui->userNameEdit->text());
        m_client->setPassword(ui->PassWordEdit->text());

        ui->connectBtn->setText(QStringLiteral("断开MQTT"));
        m_client->connectToHost();
    } else {
        ui->connectBtn->setText(QStringLiteral("连接MQTT"));
        m_client->disconnectFromHost();
    }
}

void MainWindow::on_sendBtn_clicked()
{
    // 提取纯消息内容（移除时间戳）
    QString rawMessage = ui->messTextEdit->toPlainText();
    // 发送原始消息
    m_client->publish(QMqttTopicName(ui->topicEdit->text()), rawMessage.toUtf8());
    
    // 添加时间戳后保存
    const QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString messageWithTimestamp = QStringLiteral("[%1]\n%2").arg(timestamp).arg(rawMessage);
    
    // 添加到发送消息列表的开头，使最新消息显示在最上方
    m_sendMessages.prepend(messageWithTimestamp);
    
    // 限制只保留最近20次发送的消息
    if (m_sendMessages.size() > 20) {
        m_sendMessages.removeLast();
    }
    
    // 更新发送文本框
    ui->messTextEdit->setText(m_sendMessages.join("\n\n"));
}

void MainWindow::on_setValueBtn_clicked()
{
    QJsonObject root;
    root["id"] = "admin";
    root["type"] = "variant_data";
    root["version"] = "1.0";

    QJsonObject params;
    for (int r = 0; r < ui->paramsTableWidget->rowCount(); ++r) {
        auto *addrItem = ui->paramsTableWidget->item(r, 1);
        auto *valItem = ui->paramsTableWidget->item(r, 5);
        if (!addrItem || !valItem)
            continue;
        const QString key = addrItem->text().trimmed();
        const QString text = valItem->text().trimmed();
        bool ok = false;
        const double v = text.toDouble(&ok);
        if (ok) {
            params[key] = v;
        } else {
            params[key] = text;
        }
    }
    root["params"] = params;

    QJsonDocument doc(root);
    QString currentMessage = QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
    
    // 如果消息列表为空，添加当前消息；否则替换第一条消息的内容（保留时间戳）
    if (m_sendMessages.isEmpty()) {
        const QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        m_sendMessages.prepend(QStringLiteral("[%1]\n%2").arg(timestamp).arg(currentMessage));
    } else {
        // 保留第一条消息的时间戳，只更新内容
        QString firstMessage = m_sendMessages.first();
        int timestampEnd = firstMessage.indexOf("\n");
        if (timestampEnd > 0) {
            QString timestamp = firstMessage.left(timestampEnd + 1);
            m_sendMessages.first() = timestamp + currentMessage;
        } else {
            // 如果没有时间戳，添加一个
            const QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            m_sendMessages.first() = QStringLiteral("[%1]\n%2").arg(timestamp).arg(currentMessage);
        }
    }
    
    // 更新发送文本框
    ui->messTextEdit->setText(m_sendMessages.join("\n\n"));
}

void MainWindow::onSaveClicked()
{
    if (m_model.saveToFile(m_configFilePath)) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("保存成功！"));
        reloadConfigItems();
    } else {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("保存失败！"));
    }
}

void MainWindow::onRestoreClicked()
{
    if (QFile::exists(m_configFilePath)) {
        m_model.loadFromFile(m_configFilePath);
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("恢复成功！"));
        reloadConfigItems();
    } else {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("配置文件不存在！"));
    }
}

void MainWindow::onImportClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择Excel文件"), "", "Excel Files (*.xls *.xlsx)");
    if (filePath.isEmpty())
        return;

    QProgressDialog *progress = new QProgressDialog(QStringLiteral("正在导入Excel数据..."), QStringLiteral("取消"), 0, 100, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->setMinimumDuration(0);
    progress->setValue(0);
    progress->setAttribute(Qt::WA_DeleteOnClose);
    progress->show();
    QCoreApplication::processEvents();

    auto conn = connect(&m_model, &MqttModel::importProgress, this, [progress](int current, int total) {
        progress->setMaximum(total);
        progress->setValue(current);
        QCoreApplication::processEvents();
    });

    bool success = m_model.importFromExcel(filePath);

    disconnect(conn);
    progress->close();

    if (success) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("导入成功！"));
        reloadConfigItems();
    } else {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("导入失败，请检查文件格式或是否安装了Excel。"));
    }
}

void MainWindow::onConfigCellClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    if (index.column() != 5)
        return;

    if (m_model.isAddRow(index)) {
        m_model.addItem();
        return;
    }

    if (m_model.rowCount() > 2) {
        m_model.removeRow(index.row());
    } else {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("至少需要保留一行数据！"));
    }
}
