#include "assign.h"
#include "ui_assign.h"
#include "widget.h"

#include <QShowEvent>
#include <QStandardItemModel>

assign::assign(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::assign)
{
    ui->setupUi(this);

    if (connect_sqlite_database()) {

        load_sqlite_database(ui->tableViewSearchCar,"car");
        load_sqlite_database(ui->tableViewSearchCustomer, "customer");
        load_sqlite_database(ui->tableViewAssign, "rent");

    }
    else{
        QMessageBox::information(this, "Error", "Database can not be opened!");
    }
}

assign::~assign()
{
    delete ui;
}

bool assign::connect_sqlite_database() {

    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName("database");
    }

    else {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }

    if (!db.open()) {
        qDebug() << "Database connection problem! ";
        qDebug() << "Error: " << db.lastError().text();
        return false;
    }

    return true;
}


//loading the database is slightly different from loading in customer and car table. Query is used here for searching purposes
void assign::load_sqlite_database(QTableView* tableView, const QString &tableName, const QString &querySearch) {

    QSqlQuery query;

    if (querySearch.isEmpty()) {
        query.exec(QString("SELECT * FROM %1").arg(tableName));
    } else {
        query.exec(querySearch);
    }

    int row_count{0};

    QStandardItemModel* reflection_model = new QStandardItemModel();
    reflection_model->setColumnCount(query.record().count());

    while (query.next()) {
        QList<QStandardItem*> row_data;
        for (int column{0}; column < query.record().count(); ++column) {
            QStandardItem* item = new QStandardItem(query.value(column).toString());
            row_data.append(item);
        }
        reflection_model->appendRow(row_data);
        row_count++;
    }

    tableView->setModel(reflection_model);
    tableView->verticalHeader()->setVisible(false);

    QStringList columnLabels;
    if (tableName == "car") {
        columnLabels = {"Id", "Regist. Number", "Make", "Model", "Color", "Transmission", "Production Year", "Fuel Type"};
    } else if (tableName == "customer") {
        columnLabels = {"Customer Id", "Name", "Last Name", "Phone Number", "E-mail"};
    } else if (tableName == "rent") {
        columnLabels = {"Rent Id", "Customer Id", "Car Id", "Start Date", "End Date"};
    }

    for (int column{0}; column < qMin(query.record().count(), columnLabels.size()); ++column) {
        reflection_model->setHeaderData(column, Qt::Horizontal, columnLabels[column]);
    }
}

void assign::on_pushButtonAssignCarToCustomer_clicked()
{

    if(db.open()){

        QString  customer_id = ui->lineEditAssignCustomerId->text();
        QString car_id = ui->lineEditAssignCarId->text();
        QDate start_date = ui->dateEditRentStarts->date();
        QDate end_date = ui->dateEditRentEnds->date();

        // Checking the car is already rented in the specified date
        QSqlQuery check_query;
        check_query.prepare("SELECT * FROM rent WHERE carId = :car_id AND ((startDate BETWEEN :start_date AND :end_date) OR (endDate BETWEEN :start_date AND :end_date) OR (startDate < :start_date AND endDate > :end_date))");
        check_query.bindValue(":car_id", car_id);
        check_query.bindValue(":start_date", start_date.toString("yyyy-MM-dd"));
        check_query.bindValue(":end_date", end_date.toString("yyyy-MM-dd"));

        if (check_query.exec()) {
            if (check_query.next()) {
                ui->labelResultAssign->setText("This car is already rented in the selected date.");
                return;
            }
        } else {
            ui->labelResultAssign->setText("Error while checking car");
            qDebug() << "Error: " << check_query.lastError().text();
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO rent (rentId, customerId, carId, startDate, endDate) VALUES (null, :customer_id, :car_id , :start_date, :end_date)");

        query.bindValue(":customer_id", customer_id );
        query.bindValue(":car_id", car_id);
        query.bindValue(":start_date", start_date.toString("yyyy-MM-dd"));
        query.bindValue(":end_date", end_date.toString("yyyy-MM-dd"));

        if(query.exec()){
            ui->labelResultAssign->setText("Assignment is performed succesfully! ");
        }
        else{
            ui->labelResultAssign->setText("Assignment can not be completed");
            qDebug() << "Error: " << query.lastError().text();
        }
    }
    else{
        ui->labelResultAssign->setText("Database is not connected");
    }
// load the rent table in the window
    load_sqlite_database(ui->tableViewAssign, "rent");
}


void assign::on_pushButtonUnassignAgreement_clicked()
{

    if(db.open()){

        QString id = ui->lineEditRentId->text();
        QSqlQuery query;

        if(query.exec("DELETE FROM rent WHERE rentId = " + id)){
            ui->labelResultAssign->setText("Assignment is unassigned successfully");
        }
        else{
            ui->labelResultAssign->setText("Unassignment can not be completed");
        }
    }

    else{
        ui->labelResultAssign->setText("Database is not connected");
    }

    //loading rent table in window after changes
    load_sqlite_database(ui->tableViewAssign, "rent");
}


void assign::on_pushButtonSearchCar_clicked()
{
    QString registration_number = ui->lineEditSearchRegistrationNumber->text();
    QSqlQuery query;

    QString querySearch = "SELECT * FROM car WHERE registration_number = '" + registration_number + "'";


    load_sqlite_database(ui->tableViewSearchCar, "car", querySearch);

}


void assign::on_pushButtonSearchCustomer_clicked()
{
    QString customer_name = ui->lineEditSearchCustomerName->text();
    QSqlQuery query;
    QString querySearch = "SELECT * FROM customer WHERE name = '" + customer_name + "'";

    if(query.exec(querySearch)){

        ui->labelResultAssign->setText("Customer is found successfully! ");

        load_sqlite_database(ui->tableViewSearchCustomer, "customer", querySearch);
    }
    else{
        ui->labelResultAssign->setText("Customer can not be found ");
    }
}


void assign::on_pushButtonRefreshCarTable_clicked()
{
    load_sqlite_database(ui->tableViewSearchCar, "car");
}


void assign::on_pushButtonRefreshCustomerTable_clicked()
{
    load_sqlite_database(ui->tableViewSearchCustomer, "customer");
}

