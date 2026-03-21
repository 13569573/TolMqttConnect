#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mqttmodel.h"
#include "mqttdelegate.h"
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableView>
#include <QFileDialog>
#include <QProgressDialog>
#include <QAbstractItemView>
#include <QMetaObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_model(new MqttModel(this))
{
    ui->setupUi(this);
    resize(800, 600);

    // QString appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // QDir dir(appDir);
    // if (!dir.exists()) {
    //     dir.mkpath(".");
    // }
    // m_filePath = appDir + "/mqtt_config.json";
    m_filePath =  "./mqtt_config.json";

    ui->tableView->setModel(m_model);
    ui->tableView->setItemDelegate(new MqttDelegate(this));
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 100);

    ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(ui->restoreButton, &QPushButton::clicked, this, &MainWindow::onRestoreClicked);
    connect(ui->importButton, &QPushButton::clicked, this, &MainWindow::onImportClicked);

    connect(ui->tableView, &QTableView::clicked, this, [this](const QModelIndex &index) {
        if (index.column() == 5) {
            onButtonClicked(index);
        }
    });

    if (QFile::exists(m_filePath)) {
        m_model->loadFromFile(m_filePath);
    } else {
        m_model->addItem();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSaveClicked()
{
    if (m_model->saveToFile(m_filePath)) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("保存成功！"));
    } else {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("保存失败！"));
    }
}

void MainWindow::onRestoreClicked()
{
    if (QFile::exists(m_filePath)) {
        m_model->loadFromFile(m_filePath);
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("恢复成功！"));
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

    auto conn = connect(m_model, &MqttModel::importProgress, this, [progress](int current, int total) {
        progress->setMaximum(total);
        progress->setValue(current);
        QCoreApplication::processEvents();
    });

    bool success = m_model->importFromExcel(filePath);
    
    disconnect(conn);
    progress->close();

    if (success) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("导入成功！"));
    } else {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("导入失败，请检查文件格式或是否安装了Excel。"));
    }
}

void MainWindow::onButtonClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    if (m_model->isAddRow(index)) {
        m_model->addItem();
    } else {
        if (m_model->rowCount() > 2) {
            m_model->removeRow(index.row());
        } else {
            QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("至少需要保留一行数据！"));
        }
    }
}
