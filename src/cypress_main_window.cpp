#include "cypress_main_window.h"
#include "ui_cypress_main_window.h"


#include <QVBoxLayout>
#include <QDebug>

CypressMainWindow::CypressMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CypressMainWindow)
{

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    m_button = new QPushButton("Click Me!", this);
    connect(m_button, &QPushButton::clicked, this, &CypressMainWindow::onButtonClick);

    layout->addWidget(m_button);
    centralWidget->setLayout(layout);

    setCentralWidget(centralWidget);
    setWindowTitle("Your Main Window");
    resize(400, 300);

    ui->setupUi(this);
}

CypressMainWindow::~CypressMainWindow()
{
    delete ui;
}


void CypressMainWindow::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}

void CypressMainWindow::onButtonClick()
{
    qDebug() << "button clicked";
}
