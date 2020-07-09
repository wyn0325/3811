//glwidget.h
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	explicit GLWidget(QWidget *parent = 0);

	int xRotation() const { return xRot; }
	int yRotation() const { return yRot; }
	int zRotation() const { return zRot; }

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

	void starRotate();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void loadGLTextures();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	void cube();
	void star();

	void sphere();

private slots:
	void alwaysRotate();


private:
	void normalizeAngle(int *angle);


	int xRot;
	int yRot;
	int zRot;


	QPoint lastPos;

	GLuint texture[2];
};

#endif // GLWIDGET_H