#include "customer.h"
#include "ui_customer.h"
#include "widget.h"


customer::customer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::customer)
{
    ui->setupUi(this);

    if (connect_sqlite_database()) {
        load_sqlite_database(ui->tableViewCustomer, "customer");
    }
    else{
        QMessageBox::information(this, "Error", "Database can not be opened!");
    }
}

customer::~customer()
{
    delete ui;
}

bool customer::connect_sqlite_database() {
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

void customer::load_sqlite_database(QTableView *tableView, const QString &tableName) {

    QSqlQueryModel *reflecting_model = new QSqlQueryModel();
    reflecting_model->setQuery(QString("SELECT * FROM %1").arg(tableName));
    tableView->setModel(reflecting_model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->verticalHeader()->setVisible(false);
}


void customer::on_pushButtonAddCustomer_clicked()
{

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("database");

    if(db.open()){
            QString customer_id = ui->lineEditCustomerId->text();
            QString  customer_name = ui->lineEditCustomerName->text();
            QString  customer_last_name = ui->lineEditCustomerLastName->text();
            QString c_phoneNumber = ui->lineEditPhone->text();
            QString c_email = ui->lineEditEmail->text();

        QSqlQuery query(db);

        query.prepare("INSERT INTO customer (id, name, lastName, phoneNumber, email) VALUES (:id,:name, :lastName, :phoneNumber, :email)");
        query.bindValue(":id", customer_id);
        query.bindValue(":name", customer_name);
        query.bindValue(":lastName", customer_last_name);
        query.bindValue(":phoneNumber", c_phoneNumber);
        query.bindValue(":email", c_email);

        if(!query.exec()){
            qDebug() << "Query Error: " << query.lastError().text();
            qDebug() << "Last Query: " << query.lastQuery();
            ui->labelResultCustomer->setText("Data is not added, customer id is already registered in the system. ");
        }
        else {

            ui->labelResultCustomer->setText("Customer is added successfully");
        }
    }
    else{
        ui->labelResultCustomer->setText("Database is not connected");
        qWarning() << "Error: " << db.lastError().text();
        return;
    }

    qDebug() << "Customer added succesfully";

    //loading the table just after added customer
    load_sqlite_database(ui->tableViewCustomer, "customer");

}


void customer::on_pushButtonEditCustomer_clicked()
{
    if (db.isOpen()) {
        QString customer_id = ui->lineEditCustomerId->text();
        QString customer_name = ui->lineEditCustomerName->text();
        QString customer_last_name = ui->lineEditCustomerLastName->text();
        QString phone_number = ui->lineEditPhone->text();
        QString email = ui->lineEditEmail->text();

        QSqlQuery query(db);
        query.prepare("SELECT * FROM customer WHERE id = :customer_id");
        query.bindValue(":customer_id", customer_id);

        if (query.exec() && query.next()) {
            QString db_customer_name = query.value("name").toString();
            QString db_customer_last_name = query.value("lastName").toString();
            QString db_phone_number = query.value("phoneNumber").toString();
            QString db_email = query.value("email").toString();

            if (customer_name.isEmpty()) {
                customer_name = db_customer_name;
            }
            if (customer_last_name.isEmpty()) {
                customer_last_name = db_customer_last_name;
            }
            if (phone_number.isEmpty()) {
                phone_number = db_phone_number;
            }
            if (email.isEmpty()) {
                email = db_email;
            }

            query.prepare("UPDATE customer SET name = :name, lastName = :last_name, phoneNumber = :phone_number, email = :email WHERE id = :customer_id");
            query.bindValue(":name", customer_name);
            query.bindValue(":last_name", customer_last_name);
            query.bindValue(":phone_number", phone_number);
            query.bindValue(":email", email);
            query.bindValue(":customer_id", customer_id);

            if (query.exec()) {
                ui->labelResultCustomer->setText("Data is edited successfully");
                load_sqlite_database(ui->tableViewCustomer, "customer");
            } else {
                ui->labelResultCustomer->setText("Data can not be edited");
                qDebug() << "Query Error: " << query.lastError().text();
            }
        }
    } else {
        ui->labelResultCustomer->setText("Database is not connected");
    }

}

void customer::on_pushButtonRemoveCustomer_clicked()
{

    if (db.isOpen()) {
        QString id = ui->lineEditCustomerId->text();
        QSqlQuery query(db);

        query.prepare("DELETE FROM customer WHERE id = :customer_id");
        query.bindValue(":customer_id", id);

        if (query.exec()) {
            ui->labelResultCustomer->setText("Customer removed successfully");
            load_sqlite_database(ui->tableViewCustomer, "customer");
        } else {
            ui->labelResultCustomer->setText("Customer can not be removed");
            qDebug() << "Query Error: " << query.lastError().text();
        }
    } else {
        ui->labelResultCustomer->setText("Database is not connected");
    }
}



