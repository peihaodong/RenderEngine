#include "mainwindow.h"
#include <QVBoxLayout>
#include "openglwidgettest.h"
#include <openglwidget.h>
#include <geometry.h>
#include <material.h>
#include <object3d.h>
#include <camera.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

	OpenGLWidgetTest* widget = new OpenGLWidgetTest();
	//QWidget* widget = Test();

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

QWidget* MainWindow::Test()
{
	std::vector<float> positions = {
		-0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	std::vector<float> colors = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	std::vector<uint32_t> indices = {
		0, 1, 2
	};

	//创建几何对象
	PGeometry geometry = Geometry::New();
	geometry->SetAttribute(EAttributeType::EAT_Position, Attributef::New(positions, 3));
	geometry->SetAttribute(EAttributeType::EAT_Color, Attributef::New(colors, 3));
	geometry->SetIndexAttribute(Attributei::New(indices, 1));
	//创建材质对象
	PMaterial material = Material::New();
	//创建网格对象
	PMesh mesh = Mesh::New(geometry, material);
	//创建场景对象
	PScene scene = Scene::New();
	scene->AddChild(mesh);
	//创建相机对象
	std::shared_ptr<PerspectiveCamera> camera = std::make_shared<PerspectiveCamera>(45.0f, 0.1, 1000);
	camera->SetPosition(0, 0, 1.0f);
	//创建相机控制器对象
	std::shared_ptr<GameCameraControl> camera_control = std::make_shared<GameCameraControl>();
	//创建窗口对象
	OpenGLWidget* widget = new OpenGLWidget();
	widget->SetScene(scene);
	widget->SetCamera(camera);
	widget->SetCameraControl(camera_control);
	
	widget->SetClearColor(0.94, 1.0, 0.94, 1.0);

	return widget;
}


