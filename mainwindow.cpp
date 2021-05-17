#include "mainwindow.h"
#include "comboboxdelegate.h"
#include <QMenuBar>

std::vector<Car> read_data()
{
  std::vector<Car> result;
  result.push_back (Car ({QDate(2012, 5, 1), "Volvo", "102AE", 200, "new"}));
  result.push_back (Car ({QDate(2013, 5, 1), "Nissan", "230OA", 200, "old"}));
  return result;
}

// QtableView
// Model
CarModel::CarModel (QObject *parent, const std::vector<Car> &data) :
    QAbstractTableModel (parent), m_data (data) {}


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
        if (index.column() == (int)car_fields::model)
            return QString("model: ") + car.data[index.column()].toString();
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
    return {Qt::ItemIsEnabled, Qt::ItemIsEditable};
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      model (parent, read_data ())
{
    m_view = new QTableView (parent);
    m_view->setModel (&model);

    std::set<QString> options = {"new", "old"};
    comboboxdelegate *cb_delegate = new comboboxdelegate (this, options);
    m_view->setItemDelegateForColumn ((int) car_fields::state, cb_delegate);

    QGridLayout *layout = new QGridLayout (this);
    setLayout(layout);
    layout->addWidget (m_view);

    resize (m_view->width (), height ());
//    auto file_menu = menuBar ()->addMenu ("File");
//    // Actions
//    auto newAction = new QAction ("Load");
//    connect(newAction, &QAction::triggered, this, QLOT (load_function));
//    file_menu.addAction (newAction);

}

MainWindow::~MainWindow()
{
}

