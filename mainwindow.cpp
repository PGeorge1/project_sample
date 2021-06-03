#include "mainwindow.h"
#include "comboboxdelegate.h"
#include <QMenuBar>
#include <QItemSelectionModel>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QSplitter>
#include "qcustomplot.h"

std::vector<Car> read_data(QString path)
{
  std::ifstream input (path.toStdString().c_str ());
  std::string line;
  std::vector<Car> result;
  while (std::getline(input, line))
  {
    std::stringstream ss (line);
    std::string token;
    std::vector<QVariant> line_data;
    while(std::getline(ss, token, ','))
    {
      line_data.push_back(QString (token.c_str ()));
    }
    result.push_back(Car(line_data));
  }
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
        return enum_to_string (field);
    }
    return {};
}

Qt::ItemFlags CarModel::flags (const QModelIndex &/*index*/) const
{
    return {Qt::ItemIsEnabled, Qt::ItemIsEditable, Qt::ItemIsSelectable};
}

void CarModel::add_row_func ()
{
    beginResetModel ();
    std::vector<QVariant> input_data ((size_t)car_fields::COUNT);
    auto car = Car (input_data);
    m_data.push_back (car);
    endResetModel ();

    emit layoutChanged ();
}

// Egor(26/05)
void CarModel::delete_rows_func ()
{
    beginResetModel ();
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
    endResetModel ();

    emit layoutChanged ();
}

tableWidget::tableWidget (QWidget *parent, CarModel *model) : QWidget(parent),
    model (model)
{
    m_view = new QTableView (parent);
    if (model)
      model->set_view (m_view);

//    comboboxdelegate *cb_delegate = new comboboxdelegate (this, {"new", "old"});
//    m_view->setItemDelegateForColumn ((int) car_fields::state, cb_delegate);

    filter_model = new FilterModel (this, this);
    filter_model->setSourceModel (model);

    m_view->setModel (filter_model);

    QGridLayout *layout = new QGridLayout (this);
    setLayout(layout);
    QSplitter *splitter = new QSplitter (this);
    logo_widget = new logoWidget (this);

    QHBoxLayout *logo_layout = new QHBoxLayout ();
    QLabel *label = new QLabel ("Working for future");

    logo_widget->setFixedSize(30, 30);
    label->setFixedHeight (30);
    logo_layout->addWidget (logo_widget);
    logo_layout->addWidget (label);
    logo_layout->addStretch();
    layout->addLayout (logo_layout, 0, 0, 1, 2);


    layout->addWidget (splitter, 1, 0, 1, 5);

    QGridLayout *filters_layout = new QGridLayout ();
    modelfilter = new QLineEdit ();
    minPrice = new QDoubleSpinBox ();
    maxPrice = new QDoubleSpinBox ();

    maxPrice->setRange (0, 1e30);
    maxPrice->setValue (1e30);

    minPrice->setRange (0, 1e30);
    minPrice->setValue (0);

    filters_layout->addWidget (new QLabel ("Model:"), 0, 0);
    filters_layout->addWidget (modelfilter, 0, 1);
    modelfilter->setMaximumWidth(100);

    filters_layout->addWidget (new QLabel ("Min Price:"), 1, 0);
    filters_layout->addWidget (minPrice, 1, 1);
    minPrice->setMaximumWidth(100);

    filters_layout->addWidget (new QLabel ("Max Price:"), 2, 0);
    filters_layout->addWidget (maxPrice, 2, 1);
    maxPrice->setMaximumWidth(100);

    filters_layout->addWidget (&apply_filter, 3, 0);
    apply_filter.setMaximumWidth(50);
    layout->addLayout (filters_layout, 3, 0, 1, 1);

    connect (&apply_filter,  &QPushButton::clicked, this, &tableWidget::filter_data);

    splitter->addWidget (m_view);


    // todo
    QCustomPlot *plot = new QCustomPlot (this);
    plot->setInteractions (QCP::Interaction::iRangeDrag | QCP::Interaction::iRangeZoom);
    plot->addGraph ();
    splitter->addWidget(plot);
}

void tableWidget::filter_data ()
{
  filter_model->set_filter ();
}



logoWidget::logoWidget (QWidget *parent) : QWidget (parent)
{
   setMaximumSize (QSize (20, 20));
}

void logoWidget::paintEvent (QPaintEvent */*event*/)
{
    QPainter painter (this);

    painter.setBackground(QBrush (Qt::white));
    // QColor

    painter.setPen (QPen (QBrush (Qt::blue), 7));
    painter.setBrush (QBrush (Qt::red));

    QPoint center_widget (width () / 2, height () / 2);
    int radius = std::min (width (), height ()) / 4;
    painter.drawEllipse (center_widget, radius, radius);

    double triangle_height = height () / 2;

    painter.setPen (Qt::green);
    painter.setBrush (QBrush (Qt::white));
    QList<QPoint> points = {center_widget - QPoint (0, 2 * triangle_height / 3),
                            center_widget + QPoint (triangle_height / sqrt (3), triangle_height / 3),
                            center_widget + QPoint (-triangle_height / sqrt (3), triangle_height / 3)};

    QPolygon polygon (points);
    painter.drawPolygon (polygon);
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


//Nikolay
FilterModel::FilterModel(QObject *parent, const tableWidget *w) : QSortFilterProxyModel(parent), table_widget (w)
{
}


void FilterModel::set_filter()
{
    min_price = table_widget->minPrice->value();
    max_price = table_widget->maxPrice->value();
    model     = table_widget->modelfilter->text();

    invalidate ();
}

bool FilterModel::filterAcceptsRow (int source_row, const QModelIndex &source_parent) const
{
    const Car& car = table_widget->model->m_data[source_row];
    double price = car.data[(int)car_fields::price].toDouble();
    QString car_model = car.data[(int)car_fields::model].toString();

    // price
    if (!(price >= min_price && price <= max_price))
        return false;

   //model
   if (car_model != model && model.size ())
       return false;


    return true;
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

    newAction = new QAction ("Refresh");
    connect(newAction, &QAction::triggered, this, &MainWindow::re_function);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    file_menu->addAction (newAction);

    newAction = new QAction ("Save");
    connect(newAction, &QAction::triggered, this, &MainWindow::save);
    newAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    file_menu->addAction (newAction);

    newAction = new QAction ("Save as");
    connect(newAction, &QAction::triggered, this, &MainWindow::save_as);
    file_menu->addAction (newAction);

    auto edit_menu = menuBar ()->addMenu ("Edit");

    newAction = new QAction ("Add Row");
    connect(newAction, &QAction::triggered, &model, &CarModel::add_row_func);
    edit_menu->addAction (newAction);

    newAction = new QAction ("Delete Rows");
    connect(newAction, &QAction::triggered, &model, &CarModel::delete_rows_func);
    newAction->setShortcut(QKeySequence(Qt::Key_Delete));
    edit_menu->addAction (newAction);

}

MainWindow::~MainWindow()
{
}
