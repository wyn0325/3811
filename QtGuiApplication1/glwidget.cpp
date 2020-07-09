//glwidget.cpp
#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include <glut.h>
#include "glwidget.h"
#define ROT_DELTA   0.5f
#define PI 3.14
float year = 0, day = 0;
GLWidget::GLWidget(QWidget *parent) :
	QGLWidget(parent)
{

	xRot = 0;
	yRot = 0;
	zRot = 0;

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(alwaysRotate()));
	timer->start(70);

	QTimer *timer2 = new QTimer(this);
	connect(timer2, SIGNAL(timeout()), this, SLOT(starRotate()));
	timer2->start(70);
}

void GLWidget::initializeGL()
{
	glEnable(GL_DEPTH);
	glClearColor(0.3, 0.3, 0.3, 0.0);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	loadGLTextures();
}

void GLWidget::paintGL()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	QMetaObject::invokeMethod(this, "updateGL", Qt::QueuedConnection);
	glPushMatrix();
	cube();
	sphere();
	star();
	glPopMatrix();
}

void GLWidget::resizeGL(int w, int h)
{
	int side = qMin(w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	GLfloat light_pos[] = { 0., 0., 0., 10. };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glFrustum(-1.2, 1.2, -1.2, 1.2, 5.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -50.0);
	//	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

void GLWidget::loadGLTextures()
{
	QImage tex1, buf1;
	QImage tex2, buf2;
	if (!buf1.load("Moi.jpg"))
	{
		qWarning("Could not read image file!");
		QImage dummy(128, 128, QImage::Format_RGB32);
		dummy.fill(Qt::green);
		buf1 = dummy;
	}

	if (!buf2.load("earth.jpg"))
	{
		qWarning("Could not read image file!");
		QImage dummy(128, 128, QImage::Format_RGB32);
		dummy.fill(Qt::green);
		buf2 = dummy;
	}

	tex1 = QGLWidget::convertToGLFormat(buf1);
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex1.width(), tex1.height(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex1.bits());

	tex2 = QGLWidget::convertToGLFormat(buf2);
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex2.width(), tex2.height(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex2.bits());
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		(xRot + 4 * dx);
		setYRotation(yRot + 4 * dy);
	}
	else if (event->buttons() & Qt::RightButton) {
		(xRot + 4 * dy);
		setZRotation(zRot + 4 * dx);
	}

	lastPos = event->pos();
}

void GLWidget::cube()
{
	glPushMatrix();
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat lmodel_ambient[] = { 1.0,1.0,1.0,1.0 };
	GLfloat white_light[] = { 1.0, 1.0,1.0, 1.0 };


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glColor3f(1.0, 1.0, 0.0);
	glRotatef((GLfloat)year, 1.0, 0.0, 0.0);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(xRot, 1.0, 0.0, 0.0);
	glRotatef(yRot, 0.0, 1.0, 0.0);
	glRotatef(zRot, 0.0, 0.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glEnd();
	xRot += ROT_DELTA;
	if (xRot > 360) xRot = 0;
	yRot += ROT_DELTA;
	if (yRot > 360) yRot = 0;
	zRot += ROT_DELTA;
	if (zRot > 360) zRot = 0;
	glPopMatrix();
}

void GLWidget::normalizeAngle(int *angle)
{
	while (*angle < 0)
		angle += 360 * 16;
	while (*angle > 360 * 16)
		angle -= 360 * 16;
}

void GLWidget::sphere()
{
	glPushMatrix();
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat lmodel_ambient[] = { 1.0,1.0,1.0,1.0 };
	GLfloat white_light[] = { 1.0, 1.0,1.0, 1.0 };


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);


	glColor3f(1.0, 1.0, 1.0);
	glRotatef((GLfloat)year, 1, 1, 0);
	glTranslatef(0.0, 3.0, 0.0);
	glRotatef(xRot, 1.0, 0.0, 0.0);
	glRotatef(yRot, 0.0, 1.0, 0.0);
	glRotatef(zRot, 0.0, 0.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glEnd();
	xRot += ROT_DELTA;
	if (xRot > 360) xRot = 0;
	yRot += ROT_DELTA;
	if (yRot > 360) yRot = 0;
	zRot += ROT_DELTA;
	if (zRot > 360) zRot = 0;
	glPopMatrix();

}

void GLWidget::star()
{
	glPushMatrix();
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat lmodel_ambient[] = { 0.8,0.7,0.0,1.0 };
	GLfloat white_light[] = { 1.0, 1.0,1.0, 1.0 };


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glRotatef((GLfloat)year, 0, 1, 0);
	glTranslatef(5, 0, 0);
	//	glRotatef((GLfloat)day, 0, 0, 1);
	glutSolidSphere(2, 10, 10);
	glPopMatrix();
	glPushMatrix();
	GLfloat normals[][3] = { {1., 0. ,0.}, {-1., 0., 0.}, {0., 0., 1.}, {0., 0., -1.} };
	glRotatef((GLfloat)year, 0, 1, 0);
	glTranslatef(2.5, 0, 0);
	glRotatef(xRot, 1.0, 0.0, 0.0);
	glRotatef(yRot, 0.0, 1.0, 0.0);
	glRotatef(zRot, 0.0, 0.0, 1.0);
	glNormal3fv(normals[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glEnd();


	glNormal3fv(normals[3]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glEnd();

	glNormal3fv(normals[2]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	glNormal3fv(normals[1]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();
	glPopMatrix();
}

void GLWidget::setXRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != xRot) {
		xRot = angle;
		emit xRotationChanged(angle);
		updateGL();
	}
}

void GLWidget::setYRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != yRot) {
		yRot = angle;
		emit yRotationChanged(angle);
		updateGL();
	}
}

void GLWidget::setZRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != zRot) {
		zRot = angle;
		emit zRotationChanged(angle);
		updateGL();
	}
}

void GLWidget::alwaysRotate()
{
	zRot += 2;
	emit zRotationChanged(zRot);
	updateGL();
}

void GLWidget::starRotate()
{
	year += 10;
	if (year >= 360)
		year -= 360;
}