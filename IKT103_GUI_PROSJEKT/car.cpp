#include "car.h"
#include "ui_car.h"
#include "widget.h"

#include <QSqlQueryModel>

car::car(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::car)
{
    ui->setupUi(this);

    if (connect_sqlite_database()) {

        load_sqlite_database(ui->tableViewCar , "car");
    }
    else{
        QMessageBox::information(this, "Error", "Database can not be opened!");
    }

}

car::~car()
{
    delete ui;
}


bool car::connect_sqlite_database() {
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName("database");
    } else {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }

    if (!db.open()) {
        qDebug() << "Database connection problem!";
        qDebug() << "Error: " << db.lastError().text();
        return false;
    }

    return true;
}

void car::load_sqlite_database(QTableView *tableView, const QString &tableName) {

    QSqlQueryModel *reflecting_model = new QSqlQueryModel();
    reflecting_model->setQuery(QString("SELECT * FROM %1").arg(tableName));
    tableView->setModel(reflecting_model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->verticalHeader()->setVisible(false);
}



void car::on_pushButtonAddCar_clicked()
{

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        db.setDatabaseName("database");

    if(db.open()){
        QString id = ui->lineEditCarId->text();
        QString registration_number = ui->lineEditRegistrationNumber->text();
        QString make = ui->lineEditMake->text();
        QString model = ui->lineEditModel->text();
        QString color = ui->lineEditColor->text();
        QString transmission = ui->lineEditTransmission->text();
        QString production_year = ui->lineEditProductionYear->text();
        QString fuel_type = ui->lineEditFuelType->text();

        QSqlQuery query(db);
         qDebug() << "Query Error1: " << query.lastError().text();
        query.prepare("INSERT INTO car (id, registration_number, make, model, color, transmission, production_year, fuel_type) "
                       "VALUES (:id, :registration_number, :make, :model, :color, :transmission, :production_year, :fuel_type)");

        query.bindValue(":id", id);
        query.bindValue(":registration_number", registration_number);
        query.bindValue(":make", make);
        query.bindValue(":model", model);
        query.bindValue(":color", color);
        query.bindValue(":transmission", transmission);
        query.bindValue(":production_year", production_year);
        query.bindValue(":fuel_type", fuel_type);


        if(!query.exec()){
            qDebug() << "Query Error: " << query.lastError().text();
            qDebug() << "Last Query: " << query.lastQuery();
            ui->labelResult->setText("Car is not added, registration number or car id is already registered in the system. ");
        }
        else {

            ui->labelResult->setText("Car is added successfully");
        }
    }
    else{
        ui->labelResult->setText("Database is not connected");
        qWarning() << "Error: " << db.lastError().text();
        return;
    }


//loading the car table after changes
    load_sqlite_database(ui->tableViewCar, "car");
}

void car::on_pushButtonEditCar_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        db.setDatabaseName("database");

    if(db.open()){
        QString id = ui->lineEditCarId->text();
        QString  registration_number = ui->lineEditRegistrationNumber->text();
        QString make = ui->lineEditMake->text();
        QString model = ui->lineEditModel->text();
        QString color = ui->lineEditColor->text();
        QString transmission = ui->lineEditTransmission->text();
        QString production_year = ui->lineEditProductionYear->text();
        QString fuel_type = ui->lineEditFuelType->text();

        QSqlQuery query;
        query.prepare("SELECT * FROM car WHERE id = :car_id");
        query.bindValue(":car_id", id);

        if (query.exec() && query.next()) {
            QString db_registration_number = query.value("registration_number").toString();
            QString db_make = query.value("make").toString();
            QString db_model = query.value("model").toString();
            QString db_color = query.value("color").toString();
            QString db_transmission = query.value("transmission").toString();
            QString db_production_year = query.value("production_year").toString();
            QString db_fuel_type = query.value("fuel_type").toString();

            if (registration_number.isEmpty()) {
                registration_number = db_registration_number;
            }
            if (make.isEmpty()) {
                make = db_make;
            }
            if (model.isEmpty()) {
                model = db_model;
            }
            if (color.isEmpty()) {
                color = db_color;
            }
            if (color.isEmpty()) {
                color = db_transmission;
            }
            if (color.isEmpty()) {
                color = db_production_year;
            }
            if (color.isEmpty()) {
                color = db_fuel_type;
            }

        if(query.exec("UPDATE car SET registration_number = '" + registration_number + "',make  = '" + make + "',model = '" + model + "',color = '" + color
                           + "',transmission = '" + transmission + "',production_year = '" + production_year + "',fuel_type = '" + fuel_type + "' WHERE id = " + id)){

            ui->labelResult->setText("Car is edited successfully");
        }
        else{
            ui->labelResult->setText("Car can not be edited");
        }
    }
    }
    else{
        ui->labelResult->setText("Database is not connected");
    }

    //load the car data after editing car
    load_sqlite_database(ui->tableViewCar, "car");
}


void car::on_pushButtonRemoveCar_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        db.setDatabaseName("database");

    if(db.open()){

    QString id = ui->lineEditCarId->text();
    QSqlQuery query;

    if(query.exec("DELETE FROM car WHERE id = " + id)){
        ui->labelResult->setText("Car is removed successfully ");
    }
    else{
        ui->labelResult->setText("Car can not be removed");
    }
    }

else{
    ui->labelResult->setText("Database is not connected");
}

//loading data after removal of a car
load_sqlite_database(ui->tableViewCar, "car");
}





