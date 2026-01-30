#include "mainwindow.h"
#include <QVBoxLayout>
#include "openglwidget.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

	OpenGLWidget* widget = new OpenGLWidget();

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setSpacing(0);  // 设置子部件之间的间距
	layout->setContentsMargins(0, 0, 0, 0);  // 设置布局与父部件的内边距（左、上、右、下）
	layout->addWidget(widget);

	ui->centralWidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
	delete ui;
}



