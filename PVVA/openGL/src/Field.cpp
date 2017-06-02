#include "openGL/include/Field.h"

Field::Field()
{
	init();
}

Field::Field(const Field & _field)
	:para(_field.para), graphTransPara(_field.graphTransPara), max(_field.max),
	min(_field.min), content(_field.content), N_width(_field.N_width),
	M_depth(_field.M_depth), ds(_field.ds), isSource(_field.isSource)
{
	Ex = NULL;
	Plane = NULL;
	if (_field.Ex != NULL)
	{
		allocateMemory();
		for (int i = 0; i < N_width; i++)
			for (int j = 0; j < M_depth; j++)
			{
				Ex[i][j] = _field.Ex[i][j];
				Ey[i][j] = _field.Ey[i][j];
				drawData[i][j] = _field.drawData[i][j];
				if (!isSource)
				{
					Ez[i][j] = _field.Ez[i][j];
					Hx[i][j] = _field.Hx[i][j];
					Hy[i][j] = _field.Hy[i][j];
					Hz[i][j] = _field.Hz[i][j];
				}
			}
	}
	if (_field.Plane != NULL)
	{
		Plane = Allocate_2D(Plane, N_width, M_depth);
		for (int i = 0; i < N_width; i++)     // 更新平面位置
			for (int j = 0; j < M_depth; j++)
			{
				Plane[i][j] = _field.Plane[i][j];
			}
	}
}

Field::Field(Gaussian * const _gaussian)
{
	init();
	ds = _gaussian->getDs();
	para = _gaussian->getPara();
	graphTransPara = _gaussian->getGraphTransPara();
	N_width = int(para[0] / ds) + 1;
	M_depth = int(para[1] / ds) + 1;
	isSource = true;
	allocateMemory();
	_gaussian->CreateSource(Ex, Ey, N_width, M_depth);
}

Field::~Field()
{
	freeMemory();
	if (Plane != NULL)
	{
		Free_2D(Plane);
		Plane = NULL;
	}
}

void Field::init()
{
	Ex = NULL; Ey = NULL; Ez = NULL;	// 场分量
	Hx = NULL; Hy = NULL; Hz = NULL;

	Plane = NULL;

	N_width = 0; M_depth = 0; 

	ds = 0;

	isSource = false;
}

void Field::freeMemory()
{
	if (Ex != NULL)
	{
		Free_2D(Ex);
		Free_2D(Ey);

		Free_2D(drawData);
		if (!isSource)
		{
			Free_2D(Ez);
			Free_2D(Hx);
			Free_2D(Hy);
			Free_2D(Hz);

			Ez = NULL;	// 场分量
			Hx = NULL; 
			Hy = NULL; 
			Hz = NULL;
		}
		
		Ex = NULL; Ey = NULL;
	}
}

void Field::allocateMemory()
{
	freeMemory();
	Ex = Allocate_2D(Ex, N_width, M_depth);
	Ey = Allocate_2D(Ey, N_width, M_depth);
	drawData = Allocate_2D(drawData, N_width, M_depth);
	if (!isSource) // 不是源才分配其他的场分量
	{
		Ez = Allocate_2D(Ez, N_width, M_depth);
		Hx = Allocate_2D(Hx, N_width, M_depth);
		Hy = Allocate_2D(Hy, N_width, M_depth);
		Hz = Allocate_2D(Hz, N_width, M_depth);
	}
	
}

void Field::setNM(int N, int M)
{
	N_width = N;
	M_depth = M;
}

void Field::setPlane(Vector3 ** _Plane, double factor)
{
	Plane = Allocate_2D(Plane, N_width, M_depth);
	for (int i = 0; i < N_width; i++)     // 更新平面位置
		for (int j = 0; j < M_depth; j++)
		{
			Plane[i][j] = _Plane[i][j] * (1 / factor);
		}
}

void Field::setField(complex<double>** _Ex, complex<double>** _Ey, complex<double>** _Ez,
	complex<double>** _Hx, complex<double>** _Hy, complex<double>** _Hz)
{
	allocateMemory();
	for (int i = 0; i < N_width; i++)    
		for (int j = 0; j < M_depth; j++)
		{
			Ex[i][j] = _Ex[i][j];
			Ey[i][j] = _Ey[i][j];
			Ez[i][j] = _Ez[i][j];
			Hx[i][j] = _Hx[i][j];
			Hy[i][j] = _Hy[i][j];
			Hz[i][j] = _Hz[i][j];
		}
}

complex<double>** Field::getEx() const
{
	return Ex;
}

complex<double>** Field::getEy() const
{
	return Ey;
}

void Field::getSourcePara(GraphTrans & _graphTransPara, int & _N_width, 
	int & _M_depth, double & _ds) const
{
	_graphTransPara = graphTransPara;
	_N_width = N_width;
	_M_depth = M_depth;
	_ds = ds;
}

void Field::setContent(int _content)
{
	content = _content;
	switch (content)
	{
	case 0:
		setDrawData(Ex);
		break;
	case 1:
		setDrawData(Ey);
		break;
	case 2:
		setDrawData(Ez);
		break;

	default:
		break;
	}
}

void Field::draw() const
{
	if (isSource)
	{
		glPushMatrix();

		glTranslated(graphTransPara.getTrans_x(), graphTransPara.getTrans_y(), graphTransPara.getTrans_z());
		glRotated(graphTransPara.getRotate_theta(), graphTransPara.getRotate_x(),
			graphTransPara.getRotate_y(), graphTransPara.getRotate_z());
		//glPointSize(1.5);

		for (int i = 0; i < N_width; i++)
			for (int j = 0; j < M_depth; j++)
			{
				//setRGB(drawData[i][j]);
				float temp1 = (drawData[i][j] - min) / (max - min);

				if ((temp1 >= 0) && (temp1 < 0.2))
					glColor3f(0.0f, 0.0f, 1.0f);
				else if ((temp1 >= 0.2) && (temp1 < 0.4))
					glColor3f(0.0f, 1.0f, 1.0f);
				else if ((temp1 >= 0.4) && (temp1 < 0.6))
					glColor3f(0.0f, 1.0f, 0.0f);
				else if ((temp1 >= 0.6) && (temp1 < 0.8))
					glColor3f(1.0f, 1.0f, 0.0f);
				else
					glColor3f(1.0f, 0.0f, 0.0f);

				glBegin(GL_POINTS);
				glVertex3f((i - (N_width - 1) / 2) * ds, (j - (M_depth - 1) / 2) * ds, 0);
				glEnd();
			}
		glColor3f(1.0f, 0.0f, 0.0f);

		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 500);
		glEnd();

		glPopMatrix();
	}
	else
	{
		for (int i = 0; i < N_width; i++)
			for (int j = 0; j < M_depth; j++)
			{
				//setRGB(drawData[i][j]);
				float temp1 = (drawData[i][j] - min) / (max - min);

				if ((temp1 >= 0) && (temp1 < 0.2))
					glColor3f(0.0f, 0.0f, 1.0f);
				else if ((temp1 >= 0.2) && (temp1 < 0.4))
					glColor3f(0.0f, 1.0f, 1.0f);
				else if ((temp1 >= 0.4) && (temp1 < 0.6))
					glColor3f(0.0f, 1.0f, 0.0f);
				else if ((temp1 >= 0.6) && (temp1 < 0.8))
					glColor3f(1.0f, 1.0f, 0.0f);
				else
					glColor3f(1.0f, 0.0f, 0.0f);

				glBegin(GL_POINTS);
				glVertex3f(Plane[i][j].x, Plane[i][j].y, Plane[i][j].z);
				glEnd();
			}
	}
}

void Field::setDrawData(complex<double>** data)
{
	float max_back = 0, min_back = 0;
	for(int i = 0; i < N_width; i++)
		for (int j = 0; j < M_depth; j++)
		{
			drawData[i][j] = pow((data[i][j].real() * data[i][j].real() +
				data[i][j].imag() * data[i][j].imag()), 0.5);

			if (max_back < drawData[i][j])
				max_back = drawData[i][j];
			if (min_back > drawData[i][j])
				min_back = drawData[i][j];
		}
	max = max_back;
	min = min_back;
}

void Field::setRGB(float var) const 
{
	float temp1 = (var - min) / (max - min);

	if ((temp1 >= 0) && (temp1 < 0.2))
		glColor3f(0.0f, 0.0f, 1.0f);
	else if ((temp1 >= 0.2) && (temp1 < 0.4))
		glColor3f(0.0f, 1.0f, 1.0f);
	else if ((temp1 >= 0.4) && (temp1 < 0.6))
		glColor3f(0.0f, 1.0f, 0.0f);
	else if ((temp1 >= 0.6) && (temp1 < 0.8))
		glColor3f(1.0f, 1.0f, 0.0f);
	else
		glColor3f(1.0f, 0.0f, 0.0f);
}
