/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayoutRoot;
    QTabWidget *tabWidget;
    QWidget *tabSend;
    QHBoxLayout *horizontalLayoutSend;
    QSplitter *splitterMain;
    QSplitter *splitterLeft;
    QGroupBox *groupMqttConn;
    QVBoxLayout *verticalLayoutMqtt;
    QFormLayout *formLayoutConn;
    QLabel *labelHost;
    QLineEdit *hostEdit;
    QLabel *labelPort;
    QLineEdit *portEdit;
    QLabel *labelTopic;
    QLineEdit *topicEdit;
    QLabel *labelTopic2;
    QLineEdit *topicEdit2;
    QLabel *labelUser;
    QLineEdit *userNameEdit;
    QLabel *labelPwd;
    QLineEdit *PassWordEdit;
    QHBoxLayout *horizontalLayoutMqttStatus;
    QPushButton *connectBtn;
    QLabel *mqttStatusLabel;
    QSpacerItem *horizontalSpacerMqttStatus;
    QGroupBox *groupDeviceConn;
    QVBoxLayout *verticalLayoutDeviceConn;
    QFormLayout *formLayoutDevice;
    QLabel *labelDeviceType;
    QComboBox *deviceTypeCombo;
    QStackedWidget *deviceStack;
    QWidget *pageModbusTcp;
    QFormLayout *formLayoutModbusTcp;
    QLabel *labelMbIp;
    QLineEdit *mbIpEdit;
    QLabel *labelMbPort;
    QSpinBox *mbPortSpin;
    QWidget *pageZmc;
    QVBoxLayout *verticalLayoutZmc;
    QFormLayout *formLayoutZmcTop;
    QLabel *labelZmcConn;
    QComboBox *zmcConnCombo;
    QStackedWidget *zmcStack;
    QWidget *pageZmcEth;
    QFormLayout *formLayoutZmcEth;
    QLabel *labelZmcIp;
    QLineEdit *zmcIpEdit;
    QWidget *pageZmcCom;
    QFormLayout *formLayoutZmcCom;
    QLabel *labelZmcComId;
    QSpinBox *zmcComSpin;
    QHBoxLayout *horizontalLayoutDeviceStatus;
    QPushButton *deviceConnectBtn;
    QLabel *deviceStatusLabel;
    QSpacerItem *horizontalSpacerDeviceStatus;
    QSplitter *splitterRight;
    QSplitter *splitterMsg;
    QGroupBox *groupSendMsg;
    QVBoxLayout *verticalLayoutSendMsg;
    QTextEdit *messTextEdit;
    QHBoxLayout *horizontalLayoutMsgBtns;
    QCheckBox *autoSendCheckBox;
    QSpacerItem *horizontalSpacerMsgLeft;
    QSpacerItem *horizontalSpacerMsg;
    QPushButton *sendClearBtn;
    QPushButton *setValueBtn;
    QPushButton *sendBtn;
    QGroupBox *groupRecvMsg;
    QVBoxLayout *verticalLayoutRecvMsg;
    QTextEdit *recvTextEdit;
    QHBoxLayout *horizontalLayoutRecvBtns;
    QSpacerItem *horizontalSpacerRecv;
    QPushButton *recvClearBtn;
    QGroupBox *groupParams;
    QVBoxLayout *verticalLayoutParams;
    QTableWidget *paramsTableWidget;
    QWidget *tabConfig;
    QVBoxLayout *verticalLayoutConfig;
    QTableView *configTableView;
    QHBoxLayout *horizontalLayoutConfigBtns;
    QSpacerItem *horizontalSpacerConfig;
    QPushButton *importButton;
    QPushButton *saveButton;
    QPushButton *restoreButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutRoot = new QVBoxLayout(centralwidget);
        verticalLayoutRoot->setObjectName("verticalLayoutRoot");
        verticalLayoutRoot->setContentsMargins(10, 10, 10, 10);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setDocumentMode(true);
        tabWidget->setTabPosition(QTabWidget::North);
        tabSend = new QWidget();
        tabSend->setObjectName("tabSend");
        horizontalLayoutSend = new QHBoxLayout(tabSend);
        horizontalLayoutSend->setSpacing(10);
        horizontalLayoutSend->setObjectName("horizontalLayoutSend");
        splitterMain = new QSplitter(tabSend);
        splitterMain->setObjectName("splitterMain");
        splitterMain->setOrientation(Qt::Horizontal);
        splitterLeft = new QSplitter(splitterMain);
        splitterLeft->setObjectName("splitterLeft");
        splitterLeft->setOrientation(Qt::Vertical);
        splitterLeft->setChildrenCollapsible(false);
        groupMqttConn = new QGroupBox(splitterLeft);
        groupMqttConn->setObjectName("groupMqttConn");
        groupMqttConn->setMinimumSize(QSize(250, 0));
        verticalLayoutMqtt = new QVBoxLayout(groupMqttConn);
        verticalLayoutMqtt->setObjectName("verticalLayoutMqtt");
        formLayoutConn = new QFormLayout();
        formLayoutConn->setObjectName("formLayoutConn");
        formLayoutConn->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelHost = new QLabel(groupMqttConn);
        labelHost->setObjectName("labelHost");

        formLayoutConn->setWidget(0, QFormLayout::LabelRole, labelHost);

        hostEdit = new QLineEdit(groupMqttConn);
        hostEdit->setObjectName("hostEdit");

        formLayoutConn->setWidget(0, QFormLayout::FieldRole, hostEdit);

        labelPort = new QLabel(groupMqttConn);
        labelPort->setObjectName("labelPort");

        formLayoutConn->setWidget(1, QFormLayout::LabelRole, labelPort);

        portEdit = new QLineEdit(groupMqttConn);
        portEdit->setObjectName("portEdit");

        formLayoutConn->setWidget(1, QFormLayout::FieldRole, portEdit);

        labelTopic = new QLabel(groupMqttConn);
        labelTopic->setObjectName("labelTopic");

        formLayoutConn->setWidget(2, QFormLayout::LabelRole, labelTopic);

        topicEdit = new QLineEdit(groupMqttConn);
        topicEdit->setObjectName("topicEdit");

        formLayoutConn->setWidget(2, QFormLayout::FieldRole, topicEdit);

        labelTopic2 = new QLabel(groupMqttConn);
        labelTopic2->setObjectName("labelTopic2");

        formLayoutConn->setWidget(3, QFormLayout::LabelRole, labelTopic2);

        topicEdit2 = new QLineEdit(groupMqttConn);
        topicEdit2->setObjectName("topicEdit2");

        formLayoutConn->setWidget(3, QFormLayout::FieldRole, topicEdit2);

        labelUser = new QLabel(groupMqttConn);
        labelUser->setObjectName("labelUser");

        formLayoutConn->setWidget(4, QFormLayout::LabelRole, labelUser);

        userNameEdit = new QLineEdit(groupMqttConn);
        userNameEdit->setObjectName("userNameEdit");

        formLayoutConn->setWidget(4, QFormLayout::FieldRole, userNameEdit);

        labelPwd = new QLabel(groupMqttConn);
        labelPwd->setObjectName("labelPwd");

        formLayoutConn->setWidget(5, QFormLayout::LabelRole, labelPwd);

        PassWordEdit = new QLineEdit(groupMqttConn);
        PassWordEdit->setObjectName("PassWordEdit");
        PassWordEdit->setEchoMode(QLineEdit::Password);

        formLayoutConn->setWidget(5, QFormLayout::FieldRole, PassWordEdit);


        verticalLayoutMqtt->addLayout(formLayoutConn);

        horizontalLayoutMqttStatus = new QHBoxLayout();
        horizontalLayoutMqttStatus->setObjectName("horizontalLayoutMqttStatus");
        connectBtn = new QPushButton(groupMqttConn);
        connectBtn->setObjectName("connectBtn");
        connectBtn->setMinimumHeight(32);

        horizontalLayoutMqttStatus->addWidget(connectBtn);

        mqttStatusLabel = new QLabel(groupMqttConn);
        mqttStatusLabel->setObjectName("mqttStatusLabel");

        horizontalLayoutMqttStatus->addWidget(mqttStatusLabel);

        horizontalSpacerMqttStatus = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutMqttStatus->addItem(horizontalSpacerMqttStatus);


        verticalLayoutMqtt->addLayout(horizontalLayoutMqttStatus);

        splitterLeft->addWidget(groupMqttConn);
        groupDeviceConn = new QGroupBox(splitterLeft);
        groupDeviceConn->setObjectName("groupDeviceConn");
        verticalLayoutDeviceConn = new QVBoxLayout(groupDeviceConn);
        verticalLayoutDeviceConn->setObjectName("verticalLayoutDeviceConn");
        formLayoutDevice = new QFormLayout();
        formLayoutDevice->setObjectName("formLayoutDevice");
        formLayoutDevice->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelDeviceType = new QLabel(groupDeviceConn);
        labelDeviceType->setObjectName("labelDeviceType");

        formLayoutDevice->setWidget(0, QFormLayout::LabelRole, labelDeviceType);

        deviceTypeCombo = new QComboBox(groupDeviceConn);
        deviceTypeCombo->addItem(QString());
        deviceTypeCombo->addItem(QString());
        deviceTypeCombo->setObjectName("deviceTypeCombo");

        formLayoutDevice->setWidget(0, QFormLayout::FieldRole, deviceTypeCombo);


        verticalLayoutDeviceConn->addLayout(formLayoutDevice);

        deviceStack = new QStackedWidget(groupDeviceConn);
        deviceStack->setObjectName("deviceStack");
        pageModbusTcp = new QWidget();
        pageModbusTcp->setObjectName("pageModbusTcp");
        formLayoutModbusTcp = new QFormLayout(pageModbusTcp);
        formLayoutModbusTcp->setObjectName("formLayoutModbusTcp");
        formLayoutModbusTcp->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelMbIp = new QLabel(pageModbusTcp);
        labelMbIp->setObjectName("labelMbIp");

        formLayoutModbusTcp->setWidget(0, QFormLayout::LabelRole, labelMbIp);

        mbIpEdit = new QLineEdit(pageModbusTcp);
        mbIpEdit->setObjectName("mbIpEdit");

        formLayoutModbusTcp->setWidget(0, QFormLayout::FieldRole, mbIpEdit);

        labelMbPort = new QLabel(pageModbusTcp);
        labelMbPort->setObjectName("labelMbPort");

        formLayoutModbusTcp->setWidget(1, QFormLayout::LabelRole, labelMbPort);

        mbPortSpin = new QSpinBox(pageModbusTcp);
        mbPortSpin->setObjectName("mbPortSpin");
        mbPortSpin->setMaximum(65535);
        mbPortSpin->setValue(502);

        formLayoutModbusTcp->setWidget(1, QFormLayout::FieldRole, mbPortSpin);

        deviceStack->addWidget(pageModbusTcp);
        pageZmc = new QWidget();
        pageZmc->setObjectName("pageZmc");
        verticalLayoutZmc = new QVBoxLayout(pageZmc);
        verticalLayoutZmc->setObjectName("verticalLayoutZmc");
        formLayoutZmcTop = new QFormLayout();
        formLayoutZmcTop->setObjectName("formLayoutZmcTop");
        formLayoutZmcTop->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelZmcConn = new QLabel(pageZmc);
        labelZmcConn->setObjectName("labelZmcConn");

        formLayoutZmcTop->setWidget(0, QFormLayout::LabelRole, labelZmcConn);

        zmcConnCombo = new QComboBox(pageZmc);
        zmcConnCombo->addItem(QString());
        zmcConnCombo->addItem(QString());
        zmcConnCombo->setObjectName("zmcConnCombo");

        formLayoutZmcTop->setWidget(0, QFormLayout::FieldRole, zmcConnCombo);


        verticalLayoutZmc->addLayout(formLayoutZmcTop);

        zmcStack = new QStackedWidget(pageZmc);
        zmcStack->setObjectName("zmcStack");
        pageZmcEth = new QWidget();
        pageZmcEth->setObjectName("pageZmcEth");
        formLayoutZmcEth = new QFormLayout(pageZmcEth);
        formLayoutZmcEth->setObjectName("formLayoutZmcEth");
        formLayoutZmcEth->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelZmcIp = new QLabel(pageZmcEth);
        labelZmcIp->setObjectName("labelZmcIp");

        formLayoutZmcEth->setWidget(0, QFormLayout::LabelRole, labelZmcIp);

        zmcIpEdit = new QLineEdit(pageZmcEth);
        zmcIpEdit->setObjectName("zmcIpEdit");

        formLayoutZmcEth->setWidget(0, QFormLayout::FieldRole, zmcIpEdit);

        zmcStack->addWidget(pageZmcEth);
        pageZmcCom = new QWidget();
        pageZmcCom->setObjectName("pageZmcCom");
        formLayoutZmcCom = new QFormLayout(pageZmcCom);
        formLayoutZmcCom->setObjectName("formLayoutZmcCom");
        formLayoutZmcCom->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelZmcComId = new QLabel(pageZmcCom);
        labelZmcComId->setObjectName("labelZmcComId");

        formLayoutZmcCom->setWidget(0, QFormLayout::LabelRole, labelZmcComId);

        zmcComSpin = new QSpinBox(pageZmcCom);
        zmcComSpin->setObjectName("zmcComSpin");
        zmcComSpin->setMaximum(256);
        zmcComSpin->setMinimum(1);
        zmcComSpin->setValue(3);

        formLayoutZmcCom->setWidget(0, QFormLayout::FieldRole, zmcComSpin);

        zmcStack->addWidget(pageZmcCom);

        verticalLayoutZmc->addWidget(zmcStack);

        deviceStack->addWidget(pageZmc);

        verticalLayoutDeviceConn->addWidget(deviceStack);

        horizontalLayoutDeviceStatus = new QHBoxLayout();
        horizontalLayoutDeviceStatus->setObjectName("horizontalLayoutDeviceStatus");
        deviceConnectBtn = new QPushButton(groupDeviceConn);
        deviceConnectBtn->setObjectName("deviceConnectBtn");
        deviceConnectBtn->setMinimumHeight(32);

        horizontalLayoutDeviceStatus->addWidget(deviceConnectBtn);

        deviceStatusLabel = new QLabel(groupDeviceConn);
        deviceStatusLabel->setObjectName("deviceStatusLabel");

        horizontalLayoutDeviceStatus->addWidget(deviceStatusLabel);

        horizontalSpacerDeviceStatus = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutDeviceStatus->addItem(horizontalSpacerDeviceStatus);


        verticalLayoutDeviceConn->addLayout(horizontalLayoutDeviceStatus);

        splitterLeft->addWidget(groupDeviceConn);
        splitterMain->addWidget(splitterLeft);
        splitterRight = new QSplitter(splitterMain);
        splitterRight->setObjectName("splitterRight");
        splitterRight->setOrientation(Qt::Vertical);
        splitterMsg = new QSplitter(splitterRight);
        splitterMsg->setObjectName("splitterMsg");
        splitterMsg->setOrientation(Qt::Horizontal);
        groupSendMsg = new QGroupBox(splitterMsg);
        groupSendMsg->setObjectName("groupSendMsg");
        verticalLayoutSendMsg = new QVBoxLayout(groupSendMsg);
        verticalLayoutSendMsg->setObjectName("verticalLayoutSendMsg");
        messTextEdit = new QTextEdit(groupSendMsg);
        messTextEdit->setObjectName("messTextEdit");

        verticalLayoutSendMsg->addWidget(messTextEdit);

        horizontalLayoutMsgBtns = new QHBoxLayout();
        horizontalLayoutMsgBtns->setObjectName("horizontalLayoutMsgBtns");
        autoSendCheckBox = new QCheckBox(groupSendMsg);
        autoSendCheckBox->setObjectName("autoSendCheckBox");
        autoSendCheckBox->setChecked(true);

        horizontalLayoutMsgBtns->addWidget(autoSendCheckBox);

        horizontalSpacerMsgLeft = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutMsgBtns->addItem(horizontalSpacerMsgLeft);

        horizontalSpacerMsg = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutMsgBtns->addItem(horizontalSpacerMsg);

        sendClearBtn = new QPushButton(groupSendMsg);
        sendClearBtn->setObjectName("sendClearBtn");
        sendClearBtn->setMinimumHeight(32);

        horizontalLayoutMsgBtns->addWidget(sendClearBtn);

        setValueBtn = new QPushButton(groupSendMsg);
        setValueBtn->setObjectName("setValueBtn");
        setValueBtn->setMinimumHeight(32);

        horizontalLayoutMsgBtns->addWidget(setValueBtn);

        sendBtn = new QPushButton(groupSendMsg);
        sendBtn->setObjectName("sendBtn");
        sendBtn->setMinimumHeight(32);

        horizontalLayoutMsgBtns->addWidget(sendBtn);


        verticalLayoutSendMsg->addLayout(horizontalLayoutMsgBtns);

        splitterMsg->addWidget(groupSendMsg);
        groupRecvMsg = new QGroupBox(splitterMsg);
        groupRecvMsg->setObjectName("groupRecvMsg");
        verticalLayoutRecvMsg = new QVBoxLayout(groupRecvMsg);
        verticalLayoutRecvMsg->setObjectName("verticalLayoutRecvMsg");
        recvTextEdit = new QTextEdit(groupRecvMsg);
        recvTextEdit->setObjectName("recvTextEdit");
        recvTextEdit->setReadOnly(true);

        verticalLayoutRecvMsg->addWidget(recvTextEdit);

        horizontalLayoutRecvBtns = new QHBoxLayout();
        horizontalLayoutRecvBtns->setObjectName("horizontalLayoutRecvBtns");
        horizontalSpacerRecv = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutRecvBtns->addItem(horizontalSpacerRecv);

        recvClearBtn = new QPushButton(groupRecvMsg);
        recvClearBtn->setObjectName("recvClearBtn");
        recvClearBtn->setMinimumHeight(32);

        horizontalLayoutRecvBtns->addWidget(recvClearBtn);


        verticalLayoutRecvMsg->addLayout(horizontalLayoutRecvBtns);

        splitterMsg->addWidget(groupRecvMsg);
        splitterRight->addWidget(splitterMsg);
        groupParams = new QGroupBox(splitterRight);
        groupParams->setObjectName("groupParams");
        verticalLayoutParams = new QVBoxLayout(groupParams);
        verticalLayoutParams->setObjectName("verticalLayoutParams");
        paramsTableWidget = new QTableWidget(groupParams);
        if (paramsTableWidget->columnCount() < 6)
            paramsTableWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        paramsTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        paramsTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        paramsTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        paramsTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        paramsTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        paramsTableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        paramsTableWidget->setObjectName("paramsTableWidget");
        paramsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        paramsTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        paramsTableWidget->setAlternatingRowColors(true);
        paramsTableWidget->setColumnCount(6);
        paramsTableWidget->setRowCount(0);

        verticalLayoutParams->addWidget(paramsTableWidget);

        splitterRight->addWidget(groupParams);
        splitterMain->addWidget(splitterRight);

        horizontalLayoutSend->addWidget(splitterMain);

        tabWidget->addTab(tabSend, QString());
        tabConfig = new QWidget();
        tabConfig->setObjectName("tabConfig");
        verticalLayoutConfig = new QVBoxLayout(tabConfig);
        verticalLayoutConfig->setObjectName("verticalLayoutConfig");
        configTableView = new QTableView(tabConfig);
        configTableView->setObjectName("configTableView");
        configTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        configTableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);

        verticalLayoutConfig->addWidget(configTableView);

        horizontalLayoutConfigBtns = new QHBoxLayout();
        horizontalLayoutConfigBtns->setObjectName("horizontalLayoutConfigBtns");
        horizontalSpacerConfig = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutConfigBtns->addItem(horizontalSpacerConfig);

        importButton = new QPushButton(tabConfig);
        importButton->setObjectName("importButton");
        importButton->setMinimumHeight(32);

        horizontalLayoutConfigBtns->addWidget(importButton);

        saveButton = new QPushButton(tabConfig);
        saveButton->setObjectName("saveButton");
        saveButton->setMinimumHeight(32);

        horizontalLayoutConfigBtns->addWidget(saveButton);

        restoreButton = new QPushButton(tabConfig);
        restoreButton->setObjectName("restoreButton");
        restoreButton->setMinimumHeight(32);

        horizontalLayoutConfigBtns->addWidget(restoreButton);


        verticalLayoutConfig->addLayout(horizontalLayoutConfigBtns);

        tabWidget->addTab(tabConfig, QString());

        verticalLayoutRoot->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupMqttConn->setTitle(QCoreApplication::translate("MainWindow", "\347\211\251\350\201\224\347\275\221\351\205\215\347\275\256", nullptr));
        labelHost->setText(QCoreApplication::translate("MainWindow", "Host", nullptr));
        labelPort->setText(QCoreApplication::translate("MainWindow", "Port", nullptr));
        labelTopic->setText(QCoreApplication::translate("MainWindow", "Topic", nullptr));
        labelTopic2->setText(QCoreApplication::translate("MainWindow", "Topic 2", nullptr));
        labelUser->setText(QCoreApplication::translate("MainWindow", "User", nullptr));
        labelPwd->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        connectBtn->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245MQTT", nullptr));
        mqttStatusLabel->setText(QCoreApplication::translate("MainWindow", "\346\234\252\350\277\236\346\216\245", nullptr));
        groupDeviceConn->setTitle(QCoreApplication::translate("MainWindow", "\344\270\213\344\275\215\346\234\272\351\205\215\347\275\256", nullptr));
        labelDeviceType->setText(QCoreApplication::translate("MainWindow", "\351\200\232\350\256\257\347\261\273\345\236\213", nullptr));
        deviceTypeCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Modbus TCP", nullptr));
        deviceTypeCombo->setItemText(1, QCoreApplication::translate("MainWindow", "ZMC", nullptr));

        labelMbIp->setText(QCoreApplication::translate("MainWindow", "IP", nullptr));
        labelMbPort->setText(QCoreApplication::translate("MainWindow", "\347\253\257\345\217\243", nullptr));
        labelZmcConn->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245\346\226\271\345\274\217", nullptr));
        zmcConnCombo->setItemText(0, QCoreApplication::translate("MainWindow", "Ethernet", nullptr));
        zmcConnCombo->setItemText(1, QCoreApplication::translate("MainWindow", "COM", nullptr));

        labelZmcIp->setText(QCoreApplication::translate("MainWindow", "IP", nullptr));
        labelZmcComId->setText(QCoreApplication::translate("MainWindow", "COM\345\217\267", nullptr));
        deviceConnectBtn->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245\344\270\213\344\275\215\346\234\272", nullptr));
        deviceStatusLabel->setText(QCoreApplication::translate("MainWindow", "\346\234\252\350\277\236\346\216\245", nullptr));
        groupSendMsg->setTitle(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\212\245\346\226\207", nullptr));
        autoSendCheckBox->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250", nullptr));
        sendClearBtn->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        setValueBtn->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\346\212\245\346\226\207", nullptr));
        sendBtn->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        groupRecvMsg->setTitle(QCoreApplication::translate("MainWindow", "\346\216\245\346\224\266\346\212\245\346\226\207", nullptr));
        recvClearBtn->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        groupParams->setTitle(QCoreApplication::translate("MainWindow", "\345\217\202\346\225\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem = paramsTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "#", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = paramsTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\345\257\204\345\255\230\345\231\250\345\234\260\345\235\200", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = paramsTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "\345\257\204\345\255\230\345\231\250\347\261\273\345\236\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = paramsTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "\345\257\204\345\255\230\345\231\250\345\220\215\347\247\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = paramsTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\350\257\273\345\206\231\347\261\273\345\236\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = paramsTableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "\345\200\274", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSend), QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        importButton->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245Excel", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        restoreButton->setText(QCoreApplication::translate("MainWindow", "\346\201\242\345\244\215", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabConfig), QCoreApplication::translate("MainWindow", "\351\205\215\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
