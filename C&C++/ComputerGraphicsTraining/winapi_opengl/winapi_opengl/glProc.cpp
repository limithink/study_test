#include "stdafx.h"
#include "glProc.h"

namespace mydef
{

void Init()
{
	//glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearDepth(1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(110.0, 118.0, 30.0, 38.0, -1.0, 1.0);
}
void SceneProc(int cmd, int width, int height, mydef::Map *MapData, mydef::pSP SetParam)
{
	using namespace mydef;
	static Map *save = NULL;
	static SIP Param;
	//flag
	int DrawFlag = NULL;
	int PolyCmdFlag = NULL;
	int RollFlag = NULL;
	int PitchFlag = NULL;
	int YawFlag = NULL;
	int FovFlag = NULL;
	int cmdmax;
	if (MapData) save = MapData;
	//
	for (cmdmax = 1; cmdmax < cmd; cmdmax <<= 1);
	if (cmdmax != cmd)
	{
		PolyCmdFlag = 1;
		cmdmax >>= 1;
	}
	for (int ctr = 1; ctr <= cmdmax; ctr <<= 1)
	{
		int temp = cmd;
		if (PolyCmdFlag)
			temp &= ctr;
		switch (temp)
		{
		case CMD_EXEC_RESIZE:
			{
				if (width && height)
					Param.scale = width * 1.0 / height;
				glViewport(0, 0, width, height);
				FovFlag = 1;
			}
			break;
		case CMD_EXEC_DISP:
			{
				DrawFlag = 1;
			}
			break;
		case CMD_EXEC_ZOOMIN:
			{
				Param.fov -= Param.dzoom;
				FovFlag = 1;
			}
			break;
		case CMD_EXEC_ZOOMOUT:
			{
				Param.fov += Param.dzoom;
				FovFlag = 1;
			}
			break;
		case CMD_EXEC_UP:
			{
				Param.centery += Param.dmove;
				DrawFlag = 1;
			}
			break;
		case CMD_EXEC_DOWN:
			{
				Param.centery -= Param.dmove;
				DrawFlag = 1;
			}
			break;
		case CMD_EXEC_LEFT:
			{
				Param.centerx -= Param.dmove;
				DrawFlag = 1;
			}
			break;
		case CMD_EXEC_RIGHT:
			{
				Param.centerx += Param.dmove;
				DrawFlag = 1;
			}
			break;
		case CMD_EXEC_ROLLLEFT:
			{
				Param.roll += Param.drotate;
				RollFlag = 1;
			}
			break;
		case CMD_EXEC_ROLLRIGHT:
			{
				Param.roll -= Param.drotate;
				RollFlag = 1;
			}
			break;
		case CMD_EXEC_PITCHUP:
			{
				Param.pitch += Param.drotate;
				PitchFlag = 1;
			}
			break;
		case CMD_EXEC_PITCHDOWN:
			{
				Param.pitch -= Param.drotate;
				PitchFlag = -1;
			}
			break;
		case CMD_EXEC_YAWLEFT:
			{
				Param.yaw -= Param.drotate;
				YawFlag = -1;
			}
			break;
		case CMD_EXEC_YAWRIGHT:
			{
				Param.yaw += Param.drotate;
				YawFlag = 1;
			}
			break;
		case CMD_EXEC_INIT:
			{
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glEnable(GL_DEPTH_TEST);
			}
			break;
		case CMD_EXEC_SET:
			{
				Param.centerx = SetParam->centerx;
				Param.centery = SetParam->centery;
				Param.fov = SetParam->fov;
				//
				Param.dmove = SetParam->dmove;
				Param.dzoom = SetParam->dzoom;
				Param.Near = SetParam->Near;
				Param.Far = SetParam->Far;
			}
			break;
		case CMD_EXEC_RESET_ALL:
			{
				Param.centerx = INIT_CENTERX;
				Param.centery = INIT_CENTERY;
				Param.centerz = INIT_CENTERZ;
				Param.eyex = INIT_EYEX;
				Param.eyey = INIT_EYEY;
				Param.eyez = INIT_EYEZ;
				Param.roll = INIT_ROLL;
				Param.pitch = INIT_PITCH;
				Param.yaw = INIT_YAW;
				//
				Param.fov = INIT_FOV;
				Param.scale = INIT_SCALE;
				Param.dmove = INIT_DMOVE;
				Param.dzoom = INIT_DZOOM;
				Param.drotate = INIT_DROTATE;

				Param.Near = INIT_NEAR;
				Param.Far = INIT_FAR;
				//
				Param.upx = INIT_UPX;
				Param.upy = INIT_UPY;
				Param.upz = INIT_UPZ;
				Param.distance = std::sqrt(std::pow(Param.eyex - Param.centerx, 2) + std::pow(Param.eyey - Param.centery, 2) + std::pow(Param.eyez - Param.centerz, 2));
				FovFlag = 1;
			}
			break;
		case CMD_EXEC_INVERSE:
			{
				Param.dmove *= -1.0;
			}
			break;
		case CMD_DEF_ORTHO:
			{
				Param.projf = PROJ_ORTHO;
				DrawFlag = 1;
			}
			break;
		case CMD_DEF_FRUSTUM:
			{
				Param.projf = PROJ_FRUSTUM;
				DrawFlag = 1;
			}
			break;
		default:
			break;
		}
		if (!PolyCmdFlag)
			break;
	}
	if (PitchFlag)
	{
		GLdouble dAngle, temp_sin, temp_cos, temp_x, temp_y;
		dAngle = PitchFlag * Param.drotate;
		temp_sin = std::sin(dAngle * PI);
		temp_cos = std::cos(dAngle * PI);
		//eye coord
		temp_x = Param.eyey;
		temp_y = Param.eyez;
		Param.eyey = temp_x * temp_cos - temp_y * temp_sin;
		Param.eyez = temp_x * temp_sin + temp_y * temp_cos;
		//up coord
		temp_x = Param.upy;
		temp_y = Param.upz;
		Param.upy = temp_x * temp_cos - temp_y * temp_sin;
		Param.upz = temp_x * temp_sin + temp_y * temp_cos;
		//
		FovFlag = 1;
		DrawFlag = 1;
	}
	if (YawFlag)
	{
		GLdouble dAngle, temp_sin, temp_cos, temp_x, temp_y;
		dAngle = YawFlag * Param.drotate;
		temp_sin = std::sin(dAngle * PI);
		temp_cos = std::cos(dAngle * PI);
		//eye coord
		temp_x = Param.eyex;
		temp_y = Param.eyez;
		Param.eyex = temp_x * temp_cos - temp_y * temp_sin;
		Param.eyez = temp_x * temp_sin + temp_y * temp_cos;
		//
		FovFlag = 1;
		DrawFlag = 1;
	}
	if (RollFlag)
	{
		GLdouble dAngle, temp_sin, temp_cos, temp_x, temp_y;
		dAngle = RollFlag * Param.drotate;
		temp_sin = std::sin(dAngle * PI);
		temp_cos = std::cos(dAngle * PI);
		//up coord
		temp_x = Param.upx;
		temp_y = Param.upy;
		Param.upx = temp_x * temp_cos - temp_y * temp_sin;
		Param.upy = temp_x * temp_sin + temp_y * temp_cos;
		//
		FovFlag = 1;
		DrawFlag = 1;
	}
	if (FovFlag)
	{
		GLdouble tempx, tempy, diag;
		diag = std::tan(Param.fov / 2 * PI) * Param.distance;
		tempy = std::sqrt((diag * diag) / (Param.scale * Param.scale + 1));
		tempx = Param.scale * tempy;
		Param.Left = -tempx;
		Param.Right = tempx;
		Param.Bottom = -tempy;
		Param.Top = tempy;
		//
		DrawFlag = 1;
	}
	if (DrawFlag)
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//proj transform
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (Param.projf == PROJ_ORTHO)
			glOrtho(Param.Left, Param.Right, Param.Bottom, Param.Top, Param.Near, Param.Far);//ortho
		if (Param.projf == PROJ_FRUSTUM)
			glFrustum(Param.Left, Param.Right, Param.Bottom, Param.Top, Param.Near, Param.Far);//perspective
		//model transform
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(Param.eyex, Param.eyey, Param.eyez, Param.centerx, Param.centery, Param.centerz, Param.upx, Param.upy, Param.upz);
		glColor3f(0.0f, 1.0f, 1.0f);
		if (save) DrawMap(save);
		
	}
}

void DrawCircle(GLenum mode, GLfloat x, GLfloat y, GLfloat r, int TotalSample)
{
	double dRad, Rad;
	int Sample;
	glBegin(mode);
	dRad = 2 * PI / TotalSample;
	for (Sample = 0, Rad = 0.0f; Sample < TotalSample; Sample++, Rad += dRad)
	{
		glVertex2f(x + r * cos(Rad), y + r * sin(Rad));
		//glFlush();
	}
	glEnd();
}

void DrawPolygon(mydef::Polygon *pg)
{
	using namespace mydef;
	//HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	//char strTemp[100];
	//sprintf(strTemp, "%d\n", pg->TotalPoint);
	//WriteConsoleA(hstdout, strTemp, lstrlenA(strTemp), NULL, NULL);
	LinkList<pMP> *cur = pg->head;
	//
	glBegin(GL_LINE_LOOP);
	for (int ctr = 0; ctr < pg->TotalPoint; ctr++)
	{
		//sprintf(strTemp, "%f  %f\n", cur->obj->lon, cur->obj->lat);
		//WriteConsoleA(hstdout, strTemp, lstrlenA(strTemp), NULL, NULL);
		glVertex3f(cur->obj->lon, cur->obj->lat, 0.0);
		cur = cur->next;
	}
	glEnd();
}

void DrawMap(mydef::Map *MapData)
{
	using namespace mydef;
	LinkList<Polygon*> *cur = MapData->head;
	for (int ctr = 0; ctr < MapData->TotalPolygon; ctr++)
	{
		DrawPolygon(cur->obj);
		cur = cur->next;
	}
	glFlush();
}

Polygon::Polygon(int TotalPoint)
{
	using namespace mydef;
	this->TotalPoint = TotalPoint;
	head = NULL;
	LinkList<pMP> **cur = &head;
	for (int ctr = 0; ctr < TotalPoint; ctr++)
	{
		*cur = new LinkList<pMP>;
		(*cur)->obj = new MP;
		(*cur)->next = NULL;
		cur = &((*cur)->next);
	}
}

Polygon::~Polygon()
{
	using namespace mydef;
	LinkList<pMP> *cur, *prev;
	cur = head;
	for (int ctr = 0; ctr < TotalPoint; ctr++)
	{
		prev = cur;
		cur = cur->next;
		delete prev->obj;
		delete prev;
	}
}

Map::Map()
{
}

Map::~Map()
{
	using namespace mydef;
	LinkList<Polygon*> *cur, *prev;
	cur = head;
	for (int ctr = 0; ctr < TotalPolygon; ctr++)
	{
		prev = cur;
		cur = cur->next;
		delete prev->obj;
		delete prev;
	}
}

}
