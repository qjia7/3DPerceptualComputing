//#include "stdafx.h"
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib")
#include "gl/glut.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
#include "windows.h"
//#include <GL/glext.h>

#pragma comment(lib, "OPENGL32.LIB")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib,"glut32.lib")

typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);
PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = NULL;
PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT = NULL;

static float distance = 1.0;
static float pers_distance = 1.0;
static int big = 0;
static float xlength = 0.0;
static float ylength = 0.0;
static float zlength = 0.0;
static char Teapot_buf[80];
static char zbuf[80];
static float interval = 0.0;
//GLint interval = 1;
static float ref = 90.0;
static float OldMx = -1.0;
static float OldMy = -1.0;
static float rotate_y = 0.0;
static float r = 2.5, height = 0.0;
static float Eye_x = 0.0, Eye_y = 0.0, Eye_z = 0.0;		//眼睛位置
#define M 3.1415
static float c = M/180.0;
//WGL_EXT_extensions_string;
static bool isLeftEye = false;
#define PI 3.1415926
const GLfloat R = 8.0;
static GLfloat leftMatrix[16] = {1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0};
static GLfloat rightMatrix[16] = {1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0};
static GLfloat leftPersMatrix[16] = {1.0, 0.0, 0.0, 0.0,			//左眼位置
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0};
static GLfloat rightPersMatrix[16] = {1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0};
static void printstring(void *font, char *string)
{
	int len,i;
	len = (int)strlen(string);
	for(i = 0; i < len; i++)
	{
		glutBitmapCharacter(font,string[i]);
	}
}

void init(void)
{

const GLfloat SD = 0.06;
GLfloat n = distance*SD*R/2.0;
//printf("n=%f\n",n);

//GLboolean wglSwapIntervalEXT(1);	
//要是转秩
//n=0;
leftMatrix[12] = n;
rightMatrix[12] = -n;

//这里假设眼到屏幕为一米，以米为单位
GLfloat p = pers_distance*SD/(2*1*tan(PI/6)*1);
//p = 0.0;
//printf("p=%f\n",p);
leftPersMatrix[12] = -p;
rightPersMatrix[12] = p;
GLfloat mat_specular[] = {0.8, 0.8, 0.0, 1.0};
GLfloat mat_shininess[] = {50.0};
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat yellow_light[] = {1.0, 1.0, 0.0, 1.0};
GLfloat lmodel_ambient[] = {0.0, 0.7, 0.5, 1.0};
glClearColor(1.0, 1.0, .0, 0.0);
glShadeModel(GL_SMOOTH);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);					   //材质的镜面反射颜色为mat_specular:暗黄色
glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);					//材质镜面反射指数为mat_shininess:50
glLightfv(GL_LIGHT0, GL_POSITION, light_position);						//allocate zero light source position:
glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow_light);//主体的颜色				//指定光源漫色光的颜色
glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);//高光的颜色			   //指定光源镜面反射光的颜色
//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);				   //指定光照模式：全局光照，所有物体都会被照亮
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
//glDepthFunc(GL_LEQUAL);				//启用深度遮挡测试
glEnable(GL_DEPTH_TEST);

}
void display(void)
{	 
//init();		//暂时不控制左右眼距离


//绘制右眼图像
glDrawBuffer(GL_BACK_RIGHT);
glEnable(GL_DEPTH_TEST);
glColorMask(1.0, 1.0,1.0,1.0);
glClearColor(0.0,0.0,0.0,1.0);
glClearDepth(1.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	   
sprintf(Teapot_buf, "Distance to eyes:%f(meter)",Eye_z - zlength);
sprintf(zbuf, "Distance to z-plane:%f(meter)",zlength); 

glColor3f(1.0, 1.0, 1.0);
// 画右眼
glMatrixMode(GL_PROJECTION);	 //设置当前矩阵模式：投影矩阵
glPushMatrix();						  //单位阵入栈
//glEnable(GL_FOG);
float mat[16];
glGetFloatv(GL_PROJECTION_MATRIX,mat); //绘制投影矩阵（左右移动只会改变位置，物体形状不会改变） 
glLoadIdentity();
glMultMatrixf(leftPersMatrix);		   
glMultMatrixf(mat);

glMatrixMode(GL_MODELVIEW);			//绘制模型矩阵(左右移动会改变光照，然后会改变物体形状) 左右间距1m
glPushMatrix();
glGetFloatv(GL_MODELVIEW_MATRIX,mat);	
glLoadIdentity();
glMultMatrixf(leftMatrix);
glMultMatrixf(mat);

//glPushMatrix();						 //绘制一条道路
//glColorMask(1.0, 1.0, 1.0, 1.0);
//gluLookAt(0, 50, 3, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0 );
////glTranslatef(0.0, 0.0, -10.0);
// glBegin(GL_QUADS);
//glVertex3f(-8.0, 100.0, -5.0);
//glVertex3f(8.0, 100.0, -5.0);
//glVertex3f(-8.0, -100.0, -5.0);
//glVertex3f(8.0, -100.0, -5.0);
//glEnd();
//glPopMatrix();

//glPushMatrix();					   //画一个花托
////glDisable(GL_LIGHTING);
//glColor3f(1.0, 1.0, 1.0);
//glColorMask(1.0, 1.0, 1.0, 1.0);
//gluLookAt(0, 0, R, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glTranslatef(-4.0, -3.0, -10.0);
//glutSolidTorus(0.5, 1.0, 200, 200);
//glPopMatrix();
//
//glPushMatrix();					 //绘制一个正二十面体
//glEnable(GL_LIGHTING);
//glColorMask(1.0, 1.0,0.0,1.0);
//gluLookAt(0, 0, R, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glTranslatef(1,1, 3.0);												 
////glutSolidSphere(1.0,3000,3000);
//glutSolidIcosahedron();
//glPopMatrix();
//gluLookAt(0, 0, 1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

gluLookAt(0, -2, 4, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );	
glPushMatrix();						  //绘制一个房间脚
glColorMask(1.0, 1.0, 0.0, 1.0);
glBegin(GL_QUADS);					 //地面
glVertex3f(2.5, 2.0, 0.0);
glVertex3f(-2.5, 2.0, 0.0);
glVertex3f(-2.5, -2.0, 0.0);
glVertex3f(2.5, -2.0, 0.0);
glEnd();
glDisable(GL_LIGHTING);
glColor3f(1.0, 1.0, 1.0);
glBegin(GL_QUADS);
glVertex3f(2.5, 2.0, 0.0);						  //墙壁
glVertex3f(2.5, 2.0, 4.0);
glVertex3f(-2.5, -2.0, 0.0);
glVertex3f(-2.5, -2.0, 4.0);
glEnd();
glPopMatrix();

glPushMatrix();				   //绘制一个茶壶
glEnable(GL_LIGHTING);
glColorMask(1.0, 1.0,0.0,1.0);
		  
//glTranslatef(0.0, 0.0, 5.0);	//绕z轴上移5米
glTranslatef((GLfloat) xlength, 0.0, 0.0);
glTranslatef(0.0,(GLfloat) ylength, 0.0);
glTranslatef(0.0, 0.0, (GLfloat) zlength);

glRotatef(90, 1.0, 0.0 ,0.0);				//绕x轴旋转90度
glRotatef((GLfloat) big, 0.0, 1.0, 0.0);
glRotatef(rotate_y, 0.0, 1.0, 0.0);
glRotatef(height, 1.0, 0.0 ,0.0);
//gluLookAt(R*cos(ref*c), height, R*sin(ref*c), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
glutSolidTeapot(0.5);
glPopMatrix();
glPopMatrix();						//模型矩阵出栈

//glDisable(GL_DEPTH_TEST);					   //显示茶壶距离z轴0点的距离
//glDisable(GL_ALPHA_TEST);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-0.5,639.5,-0.5,479.5,-1.0,1.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glRasterPos2i(5,465);							 
printstring(GLUT_BITMAP_HELVETICA_18,zbuf);
glRasterPos2i(5,450);							 
printstring(GLUT_BITMAP_HELVETICA_18,Teapot_buf);
glMatrixMode(GL_PROJECTION);

glPopMatrix();						//投影矩阵出栈
glFlush();



////画左眼
//glDrawBuffer(GL_BACK_LEFT);		  //清除缓存,防止左眼图像叠加在右眼图像
//glEnable(GL_DEPTH_TEST);
////glEnable(GL_FOG);
//glColorMask(1.0, 1.0,1.0,1.0);
//glClearColor(0.0,0.0,0.0,1.0);
//glClearDepth(1.0);
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//glColor3f(1.0, 1.0, 1.0);
//sprintf(Teapot_buf, "Distance to eyes:%f(meter)",(R - zlength)/10);
//sprintf(zbuf, "Distance to z-plane:%f(meter)",zlength); 
//
////glClearDepth(1.0);
////glClear(GL_DEPTH_BUFFER_BIT);
//glMatrixMode(GL_PROJECTION);						//绘制透视矩阵
//
//glPushMatrix();
//glGetFloatv(GL_PROJECTION_MATRIX,mat);
//glLoadIdentity();
//glMultMatrixf(rightPersMatrix);
//glMultMatrixf(mat);
//
//glMatrixMode(GL_MODELVIEW);						 //绘制模型矩阵
//glPushMatrix();
//glGetFloatv(GL_MODELVIEW_MATRIX,mat);
//glLoadIdentity();
//glMultMatrixf(rightMatrix);
//glMultMatrixf(mat);
//
//glPushMatrix();						 //绘制一条道路
//glColorMask(1.0, 1.0, 1.0, 1.0);
//gluLookAt(0, 50, 3, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0 );
////glTranslatef(0.0, 0.0, -10.0);
// glBegin(GL_TRIANGLE_STRIP);
//glVertex3f(-8.0, 100.0, -5.0);
//glVertex3f(8.0, 100.0, -5.0);
//glVertex3f(-8.0, -100.0, -5.0);
//glVertex3f(8.0, -100.0, -5.0);
//glEnd();
//glPopMatrix();
//
//glPushMatrix();					   //画一个花托
//glColorMask(1.0, 1.0, 1.0, 1.0);
//gluLookAt(0, 0, R, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glTranslatef(-4.0, -3.0, -10.0);
//glutSolidTorus(0.5, 1.0, 200, 200);
//glPopMatrix();
//
//glPushMatrix();					  //绘制一个正二十面体
//glColorMask(1.0, 1.0,0.0,1.0);
//gluLookAt(0, 0, R, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glTranslatef(1,1, 3.0);
//////glutSolidSphere(1.0,3000,3000);
//glutSolidIcosahedron();
//glPopMatrix();
//
//glPushMatrix();							   //绘制一个茶壶
//glColorMask(0.0, 1.0,1.0,1.0);
//glRotatef((GLfloat) big, 0.0, 1.0, 0.0);
//glTranslatef((GLfloat) xlength, 0.0, 0.0);
//glTranslatef(0.0,(GLfloat) ylength, 0.0);
//glTranslatef(0.0, 0.0, (GLfloat) zlength);
//
//gluLookAt(R*cos(ref*c) , height, R*sin(ref*c), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glutSolidTeapot(2.0);
//glPopMatrix();
////glTranslatef(-1.5,-1.5, 3.0);
////glutSolidSphere(1.0,3000,3000);
//
//glPopMatrix(); 
//glMatrixMode(GL_PROJECTION);
//
//glLoadIdentity();
//glOrtho(-0.5,639.5,-0.5,479.5,-1.0,1.0);
//glMatrixMode(GL_MODELVIEW);
//glLoadIdentity();
//glRasterPos2i(5,465);							 
//printstring(GLUT_BITMAP_HELVETICA_18,zbuf);
//glRasterPos2i(5,450);							 
//printstring(GLUT_BITMAP_HELVETICA_18,Teapot_buf);
//glMatrixMode(GL_PROJECTION);
//glPopMatrix();
//glFlush();

glutSwapBuffers();

/////////////////////////////////////////////////////////////////////
//glPopMatrix();
//if(isLeftEye)
//{ 
// glMatrixMode(GL_PROJECTION);
// glMultMatrixf(leftPersMatrix);
// glMatrixMode(GL_MODELVIEW);
// glMultMatrixf(leftMatrix);
// glColorMask(1.0, 0.0,0.0,1.0);
// 
// 
// 
// isLeftEye = false;
//}else
//{ 
// 
// glMatrixMode(GL_PROJECTION);
// glMultMatrixf(rightPersMatrix);
// glMatrixMode(GL_MODELVIEW);
// glMultMatrixf(rightMatrix);
// glColorMask(0.0, 1.0,1.0,1.0); 
// isLeftEye = true;
//}
//glRotatef((GLfloat) big, 0.0, 1.0, 0.0);
//glutSolidTeapot(1.0);
//glRotatef((GLfloat) big, 0.0, 1.0, 0.0);
//glTranslatef(3.0, 0.0, 0.0);
//glutSolidTeapot(0.5);
///////////////////////////////////////////////////////////////////////////


}
void reshape(int w, int h)
{  	  
glViewport(0, 0, (GLsizei) w, (GLsizei) h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(60, (GLfloat)w/(GLfloat)h, 0.1, 500.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
//gluLookAt(R*cos(ref*c), height, R*sin(ref*c), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}
void keyboard(unsigned char key, int x, int y)
{		 
switch (key)
{
case 'b':
	big = (big + 1) % 360;
	glutPostRedisplay();
	break;
case 'B':
	big = (big - 1) % 360;
	glutPostRedisplay();
	break;
case 'x':
	xlength = xlength + 0.1;
	glutPostRedisplay();
	break;
case 'X':
	xlength = xlength - 0.1;
	glutPostRedisplay();
	break;
case 'y':
	ylength = ylength + 0.1;
	glutPostRedisplay();
	break;
case 'Y':
	ylength = ylength - 0.1;
	glutPostRedisplay();
	break;
case 'z':
	zlength = zlength + 0.1;
	glutPostRedisplay();
	break;
case 'Z':
	zlength = zlength - 0.1;
	glutPostRedisplay();
	break;
case 'w':
	pers_distance = pers_distance + 0.2;
	glutPostRedisplay();
	break;
case 'W':
	pers_distance = pers_distance - 0.2;
	glutPostRedisplay();
	break;
case 27: // 按ESC键时退出程序
	exit (0);
	break;
default:
	break;
}
}
void mouse(int button,int state, int x, int y)
{	//printf("we are in the mouse\n");
	if(state = GLUT_DOWN)
		OldMx = x, OldMy = y;
	

};
void onMouseMove(int x,int y)
{
	
	rotate_y += 1*(x - OldMx);
	height += (y - OldMy);
	/*if(height>8.0f) 
		height = 8.0f;
	else if(height<-8.0f)
		height = -8.0f;*/
	OldMx = x;
	OldMy = y;
	
}
//////////////////////////////////////////////////////
//void spinDisplay(void)
//{
//big = (big + 1) % 360;
//glutPostRedisplay();
//}
//////////////////////////////////////////////////

bool InitVSync()
{
	printf("初始化垂直同步\n");
char* extensions = (char*)glGetString(GL_EXTENSIONS);
if (strstr(extensions,"WGL_EXT_swap_control"))
	{
	wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
	return true;
	}

	return false;
}
bool IsVSyncEnabled()
{
printf("判断是否垂直同步，返回标志位\n");
if(wglGetSwapIntervalEXT() > 0)
	return true;
else
	return false;
}
void SetVSyncState(bool enable)
{
	if (enable)
	{			
		wglSwapIntervalEXT(1);
		printf("设置标志位为1，打开垂直同步\n");
	}
	else 
	{
		wglSwapIntervalEXT(0);
		printf("设置标志位为0，关闭垂直同步\n");
	}
}
int main (int argc, char** argv)
{
glutInit(&argc, argv); 
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STEREO);	   //Display mode must have GLUT_STEREO,it is the key to resolve flash on the screen
glutInitWindowSize(800, 600);
glutInitWindowPosition(100, 100);
glutCreateWindow(argv[0]);

glutReshapeFunc(reshape);
glutDisplayFunc(display);
glutIdleFunc(display);
glutKeyboardFunc(keyboard);
glutMouseFunc(mouse);
glutMotionFunc(onMouseMove);

init();

//glEnable(GL_STEREO);
GLboolean isStereo = GL_FALSE;
glGetBooleanv(GL_STEREO,&isStereo);
if(!isStereo)
	printf("FAIL! GPU don't support stereo\n");
else
	printf("SUCCESS! GPU support stereo\n");


InitVSync();
bool isOk = IsVSyncEnabled(); 
if (isOk) 
{
	printf("返回标志位为1\n");
	SetVSyncState(false);
}
else
{	printf("返回标志位为0\n");
	SetVSyncState(false);
}

glutMainLoop();
return 0;
} 