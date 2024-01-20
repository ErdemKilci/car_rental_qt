
#ifndef WIDGET_H
#define WIDGET_H

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
#include <QMessageBox>
#include <QShowEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsVideoItem>
#include <QVideoSink>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTableView>
#include <QString>
#include <QSqlQueryModel>
#include <QHeaderView>

#include "customer.h"
#include "car.h"
#include "assign.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Widget;}
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QSqlDatabase db;
    QTableWidget *QTableWidget;

private slots:

    void on_pushButtonCustomer_clicked();

    void on_pushButtonCars_clicked();

    void on_pushButtonAssign_clicked();

    void on_pushButtonExport_clicked();

    void on_pushButtonImport_clicked();


private:
    Ui::Widget *ui;

    customer *customerWidget;
    car *carWidget;
    assign *assignWidget;

    QJsonArray collect_car();

    QJsonArray collect_customer();

    void export_json_file(const QString &json_file_name);

    void import_json_file(const QString &json_file_name);

    void insert_car_json(const QJsonArray &car_array);

    void insert_customer_json(const QJsonArray &customer_array);

    void load_table_with_jsondata(const QJsonArray &car_array, const QJsonArray &customer_array);

};

#endif // WIDGET_H
