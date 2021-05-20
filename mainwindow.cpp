#include "mainwindow.h"
#include "comboboxdelegate.h"
#include <QMenuBar>
#include <QItemSelectionModel>
#include <unordered_set>

std::vector<Car> read_data()
{
  std::vector<Car> result;
  result.push_back (Car ({QDate(2012, 5, 1), "Volvo", "102AE", 200, "new"}));
  result.push_back (Car ({QDate(2013, 5, 1), "Nissan", "230OA", 200, "old"}));
  return result;
}

// QtableView
// Model
CarModel::CarModel (QObject *parent, const std::vector<Car> &data, const QTableView *m_view) :
    QAbstractTableModel (parent), m_data (data), m_view (m_view)
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

tableWidget::tableWidget (QWidget *parent) : QWidget(parent)
{
    m_view = new QTableView (parent);
    model = new CarModel(parent, read_data (), m_view);

    m_view->setModel (model);
    std::set<QString> options = {"new", "old"};
    comboboxdelegate *cb_delegate = new comboboxdelegate (this, options);
    m_view->setItemDelegateForColumn ((int) car_fields::state, cb_delegate);


    QGridLayout *layout = new QGridLayout (this);
    setLayout(layout);
    layout->addWidget (m_view);
    layout->addWidget(&add_row);
    connect (&add_row,  &QPushButton::clicked, model, &CarModel::add_row_func);
    layout ->addWidget(&delete_rows);
    connect (&delete_rows,  &QPushButton::clicked, model, &CarModel::delete_rows_func);
}



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    centralwidget = new tableWidget (this);
    setCentralWidget (centralwidget);
    resize (1200, 600);

    auto file_menu = menuBar ()->addMenu ("File");

    auto newAction = new QAction (QIcon (style()->standardIcon(QStyle::SP_DirOpenIcon)), "Open");
    connect(newAction, &QAction::triggered, this, &MainWindow::load_function);
    newAction->setShortcut(QKeySequence(Qt::CTRL, Qt::Key_O));
    file_menu->addAction (newAction);

}

MainWindow::~MainWindow()
{
}

