#include "mainwindow.h"
#include "comboboxdelegate.h"
#include <QMenuBar>
#include <QItemSelectionModel>
#include <unordered_set>
#include <iostream>
#include <fstream>

std::vector<Car> read_data(QString /*path*/)
{
  std::vector<Car> result;
  result.push_back (Car ({QDate(2012, 5, 1), "Volvo", "102AE", 200, "new"}));
  result.push_back (Car ({QDate(2013, 5, 1), "Nissan", "230OA", 200, "old"}));
  return result;
}

// QtableView
// Model
CarModel::CarModel (QObject *parent) : QAbstractTableModel (parent)
{}


int CarModel::rowCount(const QModelIndex &/*parent*/) const
{
  return m_data.size ();
}

int CarModel::columnCount(const QModelIndex &/*parent*/) const
{
  return (int)car_fields::COUNT;
}

QVariant CarModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        const Car &car = m_data[index.row ()];
        return car.data[index.column()];
    }
    return {};
}

bool CarModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
   if (role == Qt::EditRole)
   {
       Car &car = m_data[index.row ()];
       car.data[index.column()] = value;
       return true;
   }
   return false;
}


QVariant CarModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        car_fields field = (car_fields)section;
        switch (field)
        {
        case car_fields::date              :
            return "Date";
        case car_fields::model             :
            return  "Car Model";
        case car_fields::registration_number:
            return "Registration Number";
        case car_fields::horsepower        :
            return "Horse Power";
        case car_fields::state             :
            return "State";
        case car_fields::COUNT:
            return {};
        }
    }
    return {};
}

Qt::ItemFlags CarModel::flags (const QModelIndex &/*index*/) const
{
    return {Qt::ItemIsEnabled, Qt::ItemIsEditable, Qt::ItemIsSelectable};
}

void CarModel::add_row_func ()
{
    auto car = Car ({QDate(1900, 0, 0), "", "", 0, "old"});
    m_data.push_back (car);
    emit layoutChanged ();
}

// Egor(26/05)
void CarModel::delete_rows_func ()
{
  std::vector<Car> new_data;
  std::unordered_set<size_t> rows_to_delete;

  for (const auto &row_index : m_view->selectionModel()->selectedRows())
    rows_to_delete.insert (row_index.row());

  for (size_t index = 0; index < m_data.size(); index++)
  {
    if (rows_to_delete.find (index) == rows_to_delete.end ())
    {
        new_data.push_back(m_data[index]);
    }
  }
  m_data = new_data;
  emit layoutChanged ();
}

tableWidget::tableWidget (QWidget *parent, CarModel *model) : QWidget(parent),
    model (model)
{
    m_view = new QTableView (parent);
    if (model)
      model->set_view (m_view);

    m_view->setModel (model);
    comboboxdelegate *cb_delegate = new comboboxdelegate (this, {"new", "old"});
    m_view->setItemDelegateForColumn ((int) car_fields::state, cb_delegate);
    filter_model = new QSortFilterProxyModel (this);
    filter_model->setSourceModel (model);

    m_filter_view = new QTableView(parent);
    m_filter_view->setModel(filter_model);

    QGridLayout *layout = new QGridLayout (this);
    setLayout(layout);
    layout->addWidget (m_view, 0, 0, 1, 3);
    layout->addWidget (m_filter_view, 0, 3, 1, 2);
    layout->addWidget (&filter, 1, 3);
    layout->addWidget (&apply_filter, 1, 4);

    layout->addWidget(&add_row, 1, 0);
    layout->addWidget(&delete_rows, 1, 1);

    connect (&add_row,  &QPushButton::clicked, model, &CarModel::add_row_func);
    connect (&delete_rows,  &QPushButton::clicked, model, &CarModel::delete_rows_func);
    connect (&apply_filter,  &QPushButton::clicked, this, &tableWidget::filter_data);
}

void tableWidget::filter_data ()
{
  auto filter_text = filter.text ();
  filter_model->setFilterKeyColumn ((int)car_fields::model);
  filter_model->setFilterFixedString (filter_text);
}


void MainWindow::load_function ()
{
    dir = QFileDialog::getOpenFileName (this, "Open File", QDir::currentPath ());
    auto data = read_data (dir);
    model.set_data (data);
}

// Kirill
void MainWindow::re_function ()
{
    auto data = read_data (dir);
    model.set_data (data);
}


void MainWindow::save_file(QString path)
{
  std::ofstream out(path.toStdString ().c_str ());

  for (auto &car : model.m_data)
  {
    for (auto &token : car.data)
    {

      out << token.toString ().toStdString ();
      if (token != car.data[car.data.size() - 1])
        out << ',';
      else
        out << '\n';
    }
  }
}

// Nikolay
void MainWindow::save_as()
{
  auto path = QFileDialog::getSaveFileName(this, "Save as", QDir::currentPath());
  save_file(path);
}

// Nikolay
void MainWindow::save()
{
  if (dir.size ())
    save_file(dir);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), model (parent)
{
    centralwidget = new tableWidget (this, &model);
    setCentralWidget (centralwidget);
    resize (1200, 600);

    auto file_menu = menuBar ()->addMenu ("File");

    auto newAction = new QAction (QIcon (style()->standardIcon(QStyle::SP_DirOpenIcon)), "Open");
    connect(newAction, &QAction::triggered, this, &MainWindow::load_function);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    file_menu->addAction (newAction);

    newAction = new QAction (QIcon (style()->standardIcon(QStyle::SP_DirOpenIcon)), "Refresh");
    connect(newAction, &QAction::triggered, this, &MainWindow::re_function);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    file_menu->addAction (newAction);

    newAction = new QAction (QIcon (style()->standardIcon(QStyle::SP_DirOpenIcon)), "Save");
    connect(newAction, &QAction::triggered, this, &MainWindow::save);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    file_menu->addAction (newAction);

    newAction = new QAction (QIcon (style()->standardIcon(QStyle::SP_DirOpenIcon)), "Save as");
    connect(newAction, &QAction::triggered, this, &MainWindow::save_as);
    file_menu->addAction (newAction);
}

MainWindow::~MainWindow()
{
}
