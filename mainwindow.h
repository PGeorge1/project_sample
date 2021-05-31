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
    QSortFilterProxyModel *filter_model;
    QLineEdit filter;
    QPushButton apply_filter {"Apply"};
    QPushButton add_row {"Add Row"};
    QPushButton delete_rows {"Delete Rows"};
public slots:
    void filter_data ();
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
