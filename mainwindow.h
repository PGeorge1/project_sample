#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QAbstractTableModel>
#include <QMainWindow>
#include <QTableView>
#include <QGridLayout>
#include <QPushButton>
#include "car.h"

class CarModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  std::vector<Car> m_data;
  const QTableView *m_view;
  CarModel (QObject *parent, const std::vector<Car> &data, const QTableView *m_view);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
    void add_row_func();
    void delete_rows_func();

};


class tableWidget : public QWidget
{
    Q_OBJECT
public:
    tableWidget (QWidget *parent = nullptr);
    QTableView *m_view;
    CarModel *model;
    QPushButton add_row {"Add Row"};
    QPushButton delete_rows {"Delete Rows"};
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    tableWidget *centralwidget;

    ~MainWindow();
public slots:
    void load_function () {}
};
#endif // MAINWINDOW_H
