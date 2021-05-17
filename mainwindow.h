#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QAbstractTableModel>
#include <QMainWindow>
#include <QTableView>
#include <QGridLayout>
#include "car.h"

class CarModel : public QAbstractTableModel
{

public:
  std::vector<Car> m_data;
  CarModel (QObject *parent, const std::vector<Car> &data);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
};



class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QTableView *m_view;
    CarModel model;
    ~MainWindow();
};
#endif // MAINWINDOW_H
