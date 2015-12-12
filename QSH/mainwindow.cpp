#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(read_uart()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serial;
}

int MainWindow::toggle_serial_port()
{
    if (serial->isOpen())
    {
        serial->close();
        ui->toggle->setText("Open");
        ui->statusBar->showMessage("Closed", 3000);
        return 0;
    }

    QString port = ui->port->text();
    int baud = ui->baud->text().toInt();
    serial->setBaudRate(baud);
    serial->setPortName(port);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        ui->statusBar->showMessage("Connected");
        ui->toggle->setText("Close");

    } else {
        ui->toggle->setText("Open");
        ui->statusBar->showMessage("Error", 3000);
    }

    return 0;
}

int MainWindow::send_uart()
{
    QByteArray data = ui->editor->toPlainText().toUtf8();
    serial->write(data);
    return 0;
}

void MainWindow::read_uart()
{
    ui->browser->append(serial->readAll());
}
