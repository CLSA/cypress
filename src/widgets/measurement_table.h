#ifndef MEASUREMENT_TABLE_H
#define MEASUREMENT_TABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QComboBox>
#include <QStandardItemModel>

#include <QStyledItemDelegate>
#include <QItemDelegate>

#include <QSpinBox>
#include <QDoubleSpinBox>

#include <QLineEdit>
#include <QRegExpValidator>

#include "data/TestBase.h"
#include "data/measurement_base.h"

namespace Ui {
class MeasurementTable;
}


#include <QStyledItemDelegate>
#include <QComboBox>

class ComboBoxDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    QStringList items;
    bool isEditable;
    bool required;
    bool readOnly;
    QString placeholderText;

    ComboBoxDelegate(const QStringList &items, bool isEditable = false, bool required = false, bool readOnly = false, const QString &placeholderText = QString(), QObject *parent = nullptr)
        : QStyledItemDelegate(parent), items(items), isEditable(isEditable), required(required), readOnly(readOnly), placeholderText(placeholderText) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(items);
        editor->setEditable(false);
        if (isEditable && !placeholderText.isEmpty()) {
            editor->setPlaceholderText(placeholderText);
        }
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int itemIndex = comboBox->findText(value);
        if (itemIndex >= 0) {
            comboBox->setCurrentIndex(itemIndex);
        } else if (isEditable) {
            comboBox->setEditText(value);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        if (isEditable) {
            model->setData(index, comboBox->currentText(), Qt::EditRole);
        } else {
            model->setData(index, comboBox->currentData(), Qt::EditRole);
        }
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        editor->setGeometry(option.rect);
    }
};


class NumberDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    double min, max;
    bool required;
    bool readOnly;
    bool allowEmpty;
    int decimals;

    NumberDelegate(double min = -10000, double max = 10000, bool required = true, bool readOnly = false, bool allowEmpty = true, int decimals = 2, QObject *parent = nullptr)
        : QStyledItemDelegate(parent), min(min), max(max), required(required), readOnly(readOnly), allowEmpty(allowEmpty), decimals(decimals) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (decimals == 0) {  // Integer
            QSpinBox *editor = new QSpinBox(parent);
            editor->setMinimum(min);
            editor->setMaximum(max);
            editor->setReadOnly(readOnly);
            return editor;
        } else {  // Float
            QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
            editor->setMinimum(min);
            editor->setMaximum(max);
            editor->setDecimals(decimals);
            editor->setReadOnly(readOnly);
            return editor;
        }
    };

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        if (decimals == 0) {
            int value = index.model()->data(index, Qt::EditRole).toInt();
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            spinBox->setValue(value);
        } else {
            double value = index.model()->data(index, Qt::EditRole).toDouble();
            QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
            spinBox->setValue(value);
        }
    };

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        if (decimals == 0) {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            model->setData(index, spinBox->value(), Qt::EditRole);
        } else {
            QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
            spinBox->interpretText();
            if (allowEmpty && spinBox->text().isEmpty() && !required) {
                model->setData(index, QVariant(), Qt::EditRole);
            } else {
                model->setData(index, spinBox->value(), Qt::EditRole);
            }
        }
    };

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        editor->setGeometry(option.rect);
    };
};



class TextDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    QString placeholderText;
    QRegExp regexPattern;

    bool readOnly;
    bool required;

    TextDelegate(const QString &placeholderText = QString(), const QRegExp &regexPattern = QRegExp(), bool readOnly = false, bool required = false, QObject *parent = nullptr)
        : QStyledItemDelegate(parent), placeholderText(placeholderText), regexPattern(regexPattern), readOnly(readOnly), required(required) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QLineEdit *editor = new QLineEdit(parent);

        if (!placeholderText.isEmpty()) {
            editor->setPlaceholderText(placeholderText);
        }

        if (!regexPattern.isEmpty()) {
            editor->setValidator(new QRegExpValidator(regexPattern, editor));
        }

        if (readOnly)
        {
            editor->setReadOnly(true);
        }

        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        model->setData(index, lineEdit->text(), Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        editor->setGeometry(option.rect);
    }
};

class TableColumn {
public:
    TableColumn(const QString& key, const QString& displayName, QStyledItemDelegate* delegate):
        key(key), displayName(displayName), delegate(delegate) {}
    QString key;
    QString displayName;
    QStyledItemDelegate* delegate;
};

class MeasurementTable : public QWidget
{
    Q_OBJECT

public:
    explicit MeasurementTable(QWidget *parent = nullptr);
    ~MeasurementTable();

    void initializeModel(QList<TableColumn> columns);
    void updateModel(TestBase<Measurement>* test);

    void enableMeasureButton();
    void disableMeasureButton();

    void enableFinishButton();
    void disableFinishButton();

    void setColumnDelegate(int col, QItemDelegate* itemDelegate);
    void toggleManualEntry(bool saveChanges);

signals:
    void measure();
    void finish();
    void addMeasurement();

public slots:
    void handleTestUpdate(TestBase<Measurement>* test);

    void addManualMeasurement();
    void removeManualMeasurement();

private slots:
    void handleChange(int row, int col);

private:
    void updateRowIds();
    void setTableColumns();
    void saveManualChanges();

    bool manualEditsMade { false };
    bool manualEntryMode { false };

    Ui::MeasurementTable *ui;
    TestBase<Measurement>* m_test;
    QList<TableColumn> m_columns;
};

#endif // MEASUREMENT_TABLE_H
