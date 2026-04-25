#ifndef MQTTCONFIGWINDOW_H
#define MQTTCONFIGWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class MqttConfigWindow; }
QT_END_NAMESPACE

class MqttModel;

class MqttConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MqttConfigWindow(QWidget *parent = nullptr);
    ~MqttConfigWindow();

private slots:
    void onSaveClicked();
    void onRestoreClicked();
    void onImportClicked();
    void onButtonClicked(const QModelIndex &index);

private:
    Ui::MqttConfigWindow *ui;
    MqttModel *m_model;
    QString m_filePath;
};

#endif // MQTTCONFIGWINDOW_H

