#include "debug_dialog.h"

DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent),
    m_textEdit(new QPlainTextEdit(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_textEdit->setReadOnly(true);
    layout->addWidget(m_textEdit);

    QPushButton *clearButton = new QPushButton(tr("Clear"), this);
    layout->addWidget(clearButton);

    connect(clearButton, &QPushButton::clicked, m_textEdit, &QPlainTextEdit::clear);
}

DebugDialog::~DebugDialog()
{
}

void DebugDialog::appendDebugMessage(const QString &msg)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);
    m_textEdit->appendPlainText(msg);
}

