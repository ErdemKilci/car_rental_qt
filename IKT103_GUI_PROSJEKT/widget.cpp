
#include "widget.h"
#include "./ui_widget.h"

#include "customer.h"
#include "car.h"

#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QMenu>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTime>
#include <QTimer>
#include <QIcon>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QListWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QSqlQueryModel>
#include <QPushButton>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTableWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QMovie>
#include <QPixmap>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsVideoItem>
#include <QVideoSink>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtMultimediaWidgets/QVideoWidget>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Displaying opening car image
    QLabel *opening_image_label = new QLabel(this);
    QPixmap pixmap(":/bilder/opening.png");
    opening_image_label->setPixmap(pixmap);
    opening_image_label->setScaledContents(true);
    opening_image_label->resize(this->size());
    opening_image_label->show();

    //Counter for 3 seconds for the opening image

    QTimer *counter = new QTimer(this);
    counter->setSingleShot(true);
    connect(counter, &QTimer::timeout, [opening_image_label]() {
        opening_image_label->hide();
        delete opening_image_label;
    });

    counter->start(3000);


    customerWidget = new customer();
    carWidget = new car();
    assignWidget = new assign();

    //Car pushbutton logo
    QPixmap car_pixmap(":/bilder/sedan.png");
    ui->image_car->setPixmap(car_pixmap);
    ui->image_car->setScaledContents(true);
    ui->image_car->setFixedWidth(100);
    ui->image_car->setFixedHeight(85);
    ui->image_car->setAutoFillBackground(false);

    //Customer pushbutton logo
    QPixmap customer_pixmap(":/bilder/customer.png");
    ui->image_customer->setPixmap(customer_pixmap);
    ui->image_customer->setScaledContents(true);
    ui->image_customer->setFixedWidth(65);
    ui->image_customer->setFixedHeight(55);
    ui->image_customer->setAutoFillBackground(false);

    //Assign pushbutton logo
    QPixmap assign_pixmap(":/bilder/paper.png");
    ui->image_assign->setPixmap(assign_pixmap);
    ui->image_assign->setScaledContents(true);
    ui->image_assign->setFixedWidth(65);
    ui->image_assign->setFixedHeight(55);
    ui->image_assign->setAutoFillBackground(false);
}

Widget::~Widget()
{
    delete ui;    
}


QJsonArray Widget::collect_car()
{
    QJsonArray car_array;
    QSqlQuery query("SELECT * FROM car");
    while (query.next())
    {
        QJsonObject car;

        car["id"] = query.value("id").toInt();
        car["registration_number"] = query.value("registration_number").toString();
        car["make"] = query.value("make").toString();
        car["model"] = query.value("model").toString();
        car["color"] = query.value("color").toString();
        car["transmission"] = query.value("transmission").toString();
        car["production_year"] = query.value("production_year").toString();
        car["fuel_type"] = query.value("fuel_type").toString();

        car_array.append(car);
    }
    return car_array;
}

QJsonArray Widget::collect_customer()
{
    QJsonArray customer_array;
    QSqlQuery query("SELECT * FROM customer");
    while (query.next())
    {
        QJsonObject customer;

        customer["id"] = query.value("id").toInt();
        customer["name"] = query.value("name").toString();
        customer["lastName"] = query.value("lastName").toString();
        customer["phoneNumber"] = query.value("phoneNumber").toString();
        customer["email"] = query.value("email").toString();
        customer_array.append(customer);
    }
    return customer_array;
}

void Widget::insert_car_json(const QJsonArray &car_array)
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO car (id, registration_number, make, model, color, transmission, production_year, fuel_type) VALUES (:id, :registration_number, :make, :model, :color, :transmission, :production_year, :fuel_type) ON DUPLICATE KEY UPDATE registration_number = :registration_number, make = :make, model = :model, color = :color, transmission = :transmission, production_year = :production_year, fuel_type = :fuel_type");
    for (const QJsonValue &value_car : car_array)
    {
        QJsonObject car = value_car.toObject();
        query.bindValue(":id", car["id"].toInt());
        query.bindValue(":registration_number", car["registration_number"].toString());
        query.bindValue(":make", car["make"].toString());
        query.bindValue(":model", car["model"].toString());
        query.bindValue(":color", car["color"].toString());
        query.bindValue(":transmission", car["transmission"].toString());
        query.bindValue(":production_year", car["production_year"].toString());
        query.bindValue(":fuel_type", car["fuel_type"].toString());
        if (!query.exec())
        {
            qDebug() << "Error inserting car:" << query.lastError();
        }
    }
    QSqlDatabase::database().commit();
}

void Widget::insert_customer_json(const QJsonArray &customer_array)
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO customer (id, name, lastName, phoneNumber, email) VALUES (:id, :name, :lastName, :phoneNumber, :email) ON DUPLICATE KEY UPDATE name = :name, lastName = :lastName, phoneNumber = :phoneNumber, email = :email");
    for (const QJsonValue &value_customer : customer_array)
    {
        QJsonObject customer = value_customer.toObject();
        query.bindValue(":id", customer["id"].toInt());
        query.bindValue(":name", customer["name"].toString());
        query.bindValue(":lastName", customer["lastName"].toString());
        query.bindValue(":phoneNumber", customer["phoneNumber"].toString());
        query.bindValue(":email", customer["email"].toString());
        if (!query.exec())
        {
            qDebug() << "Error inserting customer:" << query.lastError();
        }
    }
    QSqlDatabase::database().commit();
}

void Widget::export_json_file(const QString &json_file_name)
{
    // Creating json object
    QJsonObject root;

    // Exporting the data

    QJsonArray car_array = collect_car();

    QJsonArray customer_array = collect_customer();

    root["cars"] = car_array;
    root["customers"] = customer_array;

    // Write json data to a file
    QFile file(json_file_name);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

void Widget::import_json_file(const QString &json_file_name)
{
    QFile file(json_file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument json_document = QJsonDocument::fromJson(data);
    QJsonObject root = json_document.object();

    // Importing the data

    QJsonArray car_array = root["cars"].toArray();
    insert_car_json(car_array);

    QJsonArray customer_array = root["customers"].toArray();
    insert_customer_json(customer_array);


    //Hiding the .JSON label on the table
    ui->labelJson->hide();

    // Reflecting the json data to the table
    load_table_with_jsondata(car_array, customer_array);
}



void Widget::load_table_with_jsondata(const QJsonArray &car_array, const QJsonArray &customer_array)
{
    ui->tableWidget->clear();

    // Column count and putting header label
    ui->tableWidget->setColumnCount(13);
    QStringList headers = {"Car ID", "Regist. Number", "Make", "Model", "Color", "Transmission", "Production Year", "Fuel Type", "Customer ID", "Name", "Last Name", "Phone Number", "E-mail"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Row count
    int row_count = std::max(car_array.size(), customer_array.size());
    ui->tableWidget->setRowCount(row_count);

    //hiding row number if needed
    ui->tableWidget->verticalHeader()->setVisible(false);

    // Loading cars to table
    for (int i{0}; i < car_array.size(); ++i)
    {
        QJsonObject car = car_array[i].toObject();
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(car["id"].toInt())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(car["registration_number"].toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(car["make"].toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(car["model"].toString()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(car["color"].toString()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(car["transmission"].toString()));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(car["production_year"].toString()));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(car["fuel_type"].toString()));
    }

    // Loading customers to table
    for (int i{0}; i < customer_array.size(); ++i)
    {
        QJsonObject customer = customer_array[i].toObject();
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(QString::number(customer["id"].toInt())));
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(customer["name"].toString()));
        ui->tableWidget->setItem(i, 10, new QTableWidgetItem(customer["lastName"].toString()));
        ui->tableWidget->setItem(i, 11, new QTableWidgetItem(customer["phoneNumber"].toString()));
        ui->tableWidget->setItem(i, 12, new QTableWidgetItem(customer["email"].toString()));
    }
}


void Widget::on_pushButtonCustomer_clicked()
{
    qDebug() << "Customer button clicked";
    customerWidget->show();
}


void Widget::on_pushButtonCars_clicked()
{
    qDebug() << " Cars button clicked";
    carWidget->show();

}


void Widget::on_pushButtonAssign_clicked()
{
    qDebug() << "Assign button clicked";
    assignWidget->show();
}


void Widget::on_pushButtonExport_clicked()
{
    QString json_file_name = QFileDialog::getSaveFileName(this, tr("Export Data"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (!json_file_name.isEmpty())
    {
        export_json_file(json_file_name);
    }
}


void Widget::on_pushButtonImport_clicked()
{
    QString json_file_name = QFileDialog::getOpenFileName(this, tr("Import Data"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (!json_file_name.isEmpty())
    {
        import_json_file(json_file_name);
    }
}


