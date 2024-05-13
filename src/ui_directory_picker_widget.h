/********************************************************************************
** Form generated from reading UI file 'directory_picker_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIRECTORY_PICKER_WIDGET_H
#define UI_DIRECTORY_PICKER_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DirectoryPickerWidget
{
public:

    void setupUi(QWidget *DirectoryPickerWidget)
    {
        if (DirectoryPickerWidget->objectName().isEmpty())
            DirectoryPickerWidget->setObjectName(QString::fromUtf8("DirectoryPickerWidget"));
        DirectoryPickerWidget->resize(400, 300);

        retranslateUi(DirectoryPickerWidget);

        QMetaObject::connectSlotsByName(DirectoryPickerWidget);
    } // setupUi

    void retranslateUi(QWidget *DirectoryPickerWidget)
    {
        DirectoryPickerWidget->setWindowTitle(QCoreApplication::translate("DirectoryPickerWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DirectoryPickerWidget: public Ui_DirectoryPickerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRECTORY_PICKER_WIDGET_H
