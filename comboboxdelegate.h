#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H
#include <QStyledItemDelegate>
#include <set>

class comboboxdelegate  : public QStyledItemDelegate
{
    Q_OBJECT
public:
    comboboxdelegate (QObject *parent, const std::set<QString> &options);
    QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    std::set<QString> options;

};

#endif // COMBOBOXDELEGATE_H
