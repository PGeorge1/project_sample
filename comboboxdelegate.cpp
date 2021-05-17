#include "comboboxdelegate.h"
#include <QComboBox>


comboboxdelegate::comboboxdelegate (QObject *parent, const std::set<QString> &options) : QStyledItemDelegate (parent), options (options)
{

}

QWidget *comboboxdelegate::createEditor (QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QComboBox *cb = new QComboBox (parent);
    cb->addItems (QList (options.begin(), options.end ()));
    return cb;
}

void comboboxdelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *> (editor);
    if (!cb)
        return;
    auto current_value = index.data (Qt::EditRole).toString ();
    int i = cb->findText (current_value);
    if (i >= 0)
        cb->setCurrentIndex (i);
}


void comboboxdelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox *cb = qobject_cast<QComboBox *> (editor);
  model->setData (index, cb->currentText ());
}
