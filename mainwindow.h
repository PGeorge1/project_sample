#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QAbstractTableModel>
#include <QMainWindow>
#include <QTableView>
#include <QGridLayout>
#include <QPushButton>
#include "car.h"
#include <QFileDialog>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QDoubleSpinBox>

class FilterModel;

class CarModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  std::vector<Car> m_data;
  const QTableView *m_view;
  CarModel (QObject *parent);
  void set_view (const QTableView *view) {m_view = view;}
  void set_data (std::vector<Car> &data)
  {
      m_data = std::move (data);
      emit layoutChanged ();
  }

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


class logoWidget : public QWidget
{
    Q_OBJECT
public:
    logoWidget (QWidget *parent);
protected:
    void paintEvent (QPaintEvent *event) override;
};


class tableWidget : public QWidget
{
    Q_OBJECT
public:
    tableWidget (QWidget *parent = nullptr, CarModel *model = nullptr);
    QTableView *m_view;
    QTableView *m_filter_view;
    logoWidget *logo_widget;

    CarModel *model;
    FilterModel *filter_model;
    QLineEdit *modelfilter;
    QDoubleSpinBox *minPrice;
    QDoubleSpinBox *maxPrice;

    QPushButton apply_filter {"Apply"};
public slots:
    void filter_data ();
};




class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FilterModel (QObject *parent, const tableWidget *w);
    void set_filter ();

protected:
    virtual bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const override;

private:
    double min_price = 0,
           max_price = 1e30;
    QString model;
    const tableWidget* table_widget;
};




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    tableWidget *centralwidget;
    CarModel model;
    void save_file(QString path);

    ~MainWindow();
public slots:
    void load_function ();
    void save_as();
    void save();
    void re_function();

private:
    QString dir;


};
#endif // MAINWINDOW_H
