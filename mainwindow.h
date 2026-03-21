#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MqttModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSaveClicked();
    void onRestoreClicked();
    void onImportClicked();
    void onButtonClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    MqttModel *m_model;
    QString m_filePath;
};

#endif // MAINWINDOW_H
