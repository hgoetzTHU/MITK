#include "QmitkUSZoneManagementColorDialogDelegate.h"

#include <QEvent>
#include <QColorDialog>

QmitkUSZoneManagementColorDialogDelegate::QmitkUSZoneManagementColorDialogDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{
}

QmitkUSZoneManagementColorDialogDelegate::~QmitkUSZoneManagementColorDialogDelegate()
{
}

bool QmitkUSZoneManagementColorDialogDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem& option, const QModelIndex &index)
{
  if( event->type() == QEvent::MouseButtonDblClick )
  {
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QColor color = QColorDialog::getColor(QColor(value));
    if (color.isValid()) { model->setData(index, QVariant(color)); }

    return true;
  }
  else
  {
    return QStyledItemDelegate::editorEvent(event, model, option, index);
  }
}
