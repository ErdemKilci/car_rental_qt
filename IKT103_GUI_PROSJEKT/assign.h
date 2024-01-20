#ifndef ASSIGN_H
#define ASSIGN_H

#include <QWidget>
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
#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QHeaderView>
#include <QShowEvent>
#include <QWidget>
#include <QtSql>

namespace Ui {
class assign;
}

class assign : public QWidget
{
    Q_OBJECT

public:
    explicit assign(QWidget *parent = nullptr);
    ~assign();

    QSqlDatabase db;

    void load_sqlite_database(QTableView* tableView, const QString& tableName, const QString& querySearch = QString());

private slots:
    void on_pushButtonAssignCarToCustomer_clicked();

    void on_pushButtonUnassignAgreement_clicked();

    void on_pushButtonSearchCar_clicked();

    void on_pushButtonSearchCustomer_clicked();

    void on_pushButtonRefreshCarTable_clicked();

    void on_pushButtonRefreshCustomerTable_clicked();

private:
    Ui::assign *ui;
    assign *assignWidget;

    bool connect_sqlite_database();

};

#endif // ASSIGN_H
