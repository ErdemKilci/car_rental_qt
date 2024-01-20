#ifndef CAR_H
#define CAR_H

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
#include <QTableView>


namespace Ui {
class car;
}

class car : public QWidget
{
    Q_OBJECT

public:
    explicit car(QWidget *parent = nullptr);
    ~car();

    void load_sqlite_database(QTableView *tableView, const QString &tableName);

private slots:

    void on_pushButtonAddCar_clicked();
    void on_pushButtonEditCar_clicked();
    void on_pushButtonRemoveCar_clicked();

private:
    Ui::car *ui;
    car *carWidget;
    QSqlDatabase db;

    bool connect_sqlite_database();

};

#endif // CAR_H
