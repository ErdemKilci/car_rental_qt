#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLCDNumber>
#include <QComboBox>
#include <QSlider>
#include <QFontComboBox>
#include <QTextEdit>
#include <QTableWidgetItem>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QHeaderView>


namespace Ui {
class customer;
}

class customer : public QWidget
{
    Q_OBJECT

public:
    explicit customer(QWidget *parent = nullptr);
    ~customer();

    QSqlDatabase db;

    void load_sqlite_database(QTableView *tableView, const QString &tableName);


private slots:

    void on_pushButtonAddCustomer_clicked();

    void on_pushButtonEditCustomer_clicked();

    void on_pushButtonRemoveCustomer_clicked();


private:
    Ui::customer *ui;
    customer *customerWidget;

    bool connect_sqlite_database();

};

#endif // CUSTOMER_H
