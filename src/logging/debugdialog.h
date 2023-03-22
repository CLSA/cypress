#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMetaObject>
#include <QDebug>
#include <QMutex>

class DebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DebugDialog(QWidget *parent = nullptr);
    ~DebugDialog();

public slots:
    void appendDebugMessage(const QString &msg);

private:
    QPlainTextEdit *m_textEdit;
};

#endif // DEBUGDIALOG_H
