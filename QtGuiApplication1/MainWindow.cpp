//mainwindow.cpp
#include <QtOpenGL>
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QSlider>
#include <QScrollArea>
#include <QMenuBar>
#include <QApplication>


#include "mainwindow.h"
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	centralWidget = new QWidget;
	setCentralWidget(centralWidget);

	glWidget = new GLWidget;
	pixmapLabel = new QLabel;

	glWidgetArea = new QScrollArea;
	glWidgetArea->setWidget(glWidget);

	glWidgetArea->setWidgetResizable(true);
	glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	glWidgetArea->setMinimumSize(50, 50);

	pixmapLabelArea = new QScrollArea;
	pixmapLabelArea->setWidget(pixmapLabel);
	pixmapLabelArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	pixmapLabelArea->setMinimumSize(50, 50);



	QGridLayout *centralLayout = new QGridLayout;
	centralLayout->addWidget(glWidgetArea, 0, 0);
	centralLayout->addWidget(pixmapLabelArea, 0, 1);
	centralWidget->setLayout(centralLayout);

	createActions();
	createMenus();

	//	xSlider->setValue(15 * 16);
	//	ySlider->setValue(345 * 16);
	//	zSlider->setValue(0 * 16);

	setWindowTitle(tr("Sc16y2w"));
	resize(1000, 600);
}

void MainWindow::setPixmap(const QPixmap &pixmap)
{
	pixmapLabel->setPixmap(pixmap);
	QSize size = pixmap.size();
	if (size - QSize(1, 0) == pixmapLabelArea->maximumViewportSize())
		size -= QSize(1, 0);
	pixmapLabel->resize(size);
}

QSize MainWindow::getSize()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Sc16y2w"),
		tr("Enter Pixmap Size:"),
		QLineEdit::Normal,
		tr("%1 x %2").arg(glWidget->width())
		.arg(glWidget->height()),
		&ok);
	if (!ok)
		return QSize();

	QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
	if (regExp.exactMatch(text)) {
		int width = regExp.cap(1).toInt();
		int height = regExp.cap(2).toInt();
		if (width > 0 && width < 2048 && height > 0 && height < 2048)
			return QSize(width, height);
	}
	return glWidget->size();
}

void MainWindow::renderIntoPixmap()
{
	QSize size = getSize();
	if (size.isValid()) {
		QPixmap pixmap = glWidget->renderPixmap(size.width(), size.height());
		setPixmap(pixmap);
	}
}

void MainWindow::grabFrameBuffer()
{
	QImage image = glWidget->grabFrameBuffer();
	setPixmap(QPixmap::fromImage(image));
}

void MainWindow::clearPixmap()
{
	setPixmap(QPixmap());
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("sc16y2w"),
		tr("This design is used to show the perspective"
			"of different geometric objects from different angles"));
}

void MainWindow::createActions()
{
	renderIntoPixmapAction = new QAction(tr("&Render into Pixmap..."), this);
	renderIntoPixmapAction->setShortcut(tr("Ctrl+R"));
	renderIntoPixmapAction->setToolTip(tr("yes, triggerd it"));
	connect(renderIntoPixmapAction, SIGNAL(triggered()),
		this, SLOT(renderIntoPixmap()));

	grabFrameBufferAction = new QAction(tr("&Grab Frame Buffer"), this);
	grabFrameBufferAction->setShortcut(tr("Ctrl+G"));
	connect(grabFrameBufferAction, SIGNAL(triggered()),
		this, SLOT(grabFrameBuffer()));

	clearPixmapAction = new QAction(tr("&Clear Pixmap"), this);
	clearPixmapAction->setShortcut(tr("Ctrl+L"));
	connect(clearPixmapAction, SIGNAL(triggered()), this, SLOT(clearPixmap()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcuts(QKeySequence::Quit);
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	aboutAction = new QAction(tr("&About"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(tr("About &Qt"), this);
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(renderIntoPixmapAction);
	fileMenu->addAction(grabFrameBufferAction);
	fileMenu->addAction(clearPixmapAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

MainWindow::~MainWindow()
{
}