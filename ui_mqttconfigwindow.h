/********************************************************************************
** Form generated from reading UI file 'mqttconfigwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MQTTCONFIGWINDOW_H
#define UI_MQTTCONFIGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MqttConfigWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *importButton;
    QPushButton *saveButton;
    QPushButton *restoreButton;

    void setupUi(QMainWindow *MqttConfigWindow)
    {
        if (MqttConfigWindow->objectName().isEmpty())
            MqttConfigWindow->setObjectName("MqttConfigWindow");
        MqttConfigWindow->resize(800, 600);
        centralwidget = new QWidget(MqttConfigWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setGridStyle(Qt::DashLine);

        verticalLayout->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        importButton = new QPushButton(centralwidget);
        importButton->setObjectName("importButton");

        horizontalLayout->addWidget(importButton);

        saveButton = new QPushButton(centralwidget);
        saveButton->setObjectName("saveButton");

        horizontalLayout->addWidget(saveButton);

        restoreButton = new QPushButton(centralwidget);
        restoreButton->setObjectName("restoreButton");

        horizontalLayout->addWidget(restoreButton);


        verticalLayout->addLayout(horizontalLayout);

        MqttConfigWindow->setCentralWidget(centralwidget);

        retranslateUi(MqttConfigWindow);

        QMetaObject::connectSlotsByName(MqttConfigWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MqttConfigWindow)
    {
        MqttConfigWindow->setWindowTitle(QCoreApplication::translate("MqttConfigWindow", "MQTT\345\257\204\345\255\230\345\231\250\351\205\215\347\275\256", nullptr));
        importButton->setText(QCoreApplication::translate("MqttConfigWindow", "\345\257\274\345\205\245Excel", nullptr));
        saveButton->setText(QCoreApplication::translate("MqttConfigWindow", "\344\277\235\345\255\230", nullptr));
        restoreButton->setText(QCoreApplication::translate("MqttConfigWindow", "\346\201\242\345\244\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MqttConfigWindow: public Ui_MqttConfigWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MQTTCONFIGWINDOW_H
