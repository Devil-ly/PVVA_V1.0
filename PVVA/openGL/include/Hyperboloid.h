#ifndef HYPERBOLOID_H
#define HYPERBOLOID_H
#include <gl/GLUT.H>
#include <cmath>

class Hyperboloid
{
public:
	Hyperboloid()
		:a(1.0), b(0.5), c(0.4), r(1.0), x(0.0), y(0.0), z(0.0), rotate_x(1.0), rotate_y(0.0), rotate_z(0.0), rotate_theta(0.0)
	{
		initial();
	};

	Hyperboloid(float _a, float _b, float _c, float _r, float _x, float _y, float _z,
		float _rotate_x, float _rotate_y, float _rotate_z, float _rotate_theta)
		:a(_a), b(_b), c(_c), r(_r), x(_x), y(_y), z(_z), rotate_x(_rotate_x), rotate_y(_rotate_y), rotate_z(_rotate_z), rotate_theta(_rotate_theta)
	{
		initial();
	};

	//~Hyperboloid();

	void initial()
	{
		n = 40;
		m = 20;
		Pi =  3.1415926;
	}

	void draw()
	{
		glColor3f(0.0f, 0.0f, 1.0f);
		glPushMatrix();

		glTranslated(x, y, z);
		glRotated(rotate_theta, rotate_x, rotate_y, rotate_z);

		drawCircle(r);
		drawHyperbola(0);
		drawHyperbola(Pi);
		drawHyperbola(Pi/2);
		drawHyperbola(3*Pi/2);
		glPopMatrix();
	};

private:
	float a, b, c; //双曲线参数
	float r; // 与x轴的距离
	float x, y, z; //c点相对于世界坐标的位置
	float rotate_x, rotate_y, rotate_z; // 旋转轴
	float rotate_theta; // 旋转角度 右手定则
	//int rotate_num; // 旋转次数
	int n, m;
	float Pi;

	// 画圆
	void drawCircle(float _u)
	{
		float theta = 2 * Pi / n;
		glBegin(GL_LINE_LOOP);
		float temp;
		for (int i = 0; i < n; i++)
		{
			temp = pow(((_u * _u / a / a) + 1), 0.5);
			glVertex3d(_u * sin(i * theta), _u / a * b* cos(i * theta), temp * c);
		}
		glEnd();
	}

	void drawHyperbola(float _v)
	{
		float tempL = r / ( m - 1 );
		float temp;
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < m; i++)
		{
			temp = pow(((tempL * tempL * i * i / a / a) + 1), 0.5);
			glVertex3d(tempL * i * sin(_v), tempL * i / a * b * cos(_v), temp * c);
		}
		glEnd();
	}

};


#endif