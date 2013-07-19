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
static float Eye_x = 0.0, Eye_y = 0.0, Eye_z = 0.0;		//�۾�λ��
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
static GLfloat leftPersMatrix[16] = {1.0, 0.0, 0.0, 0.0,			//����λ��
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
//Ҫ��ת��
//n=0;
leftMatrix[12] = n;
rightMatrix[12] = -n;

//��������۵���ĻΪһ�ף�����Ϊ��λ
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
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);					   //���ʵľ��淴����ɫΪmat_specular:����ɫ
glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);					//���ʾ��淴��ָ��Ϊmat_shininess:50
glLightfv(GL_LIGHT0, GL_POSITION, light_position);						//allocate zero light source position:
glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow_light);//�������ɫ				//ָ����Դ��ɫ�����ɫ
glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);//�߹����ɫ			   //ָ����Դ���淴������ɫ
//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);				   //ָ������ģʽ��ȫ�ֹ��գ��������嶼�ᱻ����
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
//glDepthFunc(GL_LEQUAL);				//��������ڵ�����
glEnable(GL_DEPTH_TEST);

}
void display(void)
{	 
//init();		//��ʱ�����������۾���


//��������ͼ��
glDrawBuffer(GL_BACK_RIGHT);
glEnable(GL_DEPTH_TEST);
glColorMask(1.0, 1.0,1.0,1.0);
glClearColor(0.0,0.0,0.0,1.0);
glClearDepth(1.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	   
sprintf(Teapot_buf, "Distance to eyes:%f(meter)",Eye_z - zlength);
sprintf(zbuf, "Distance to z-plane:%f(meter)",zlength); 

glColor3f(1.0, 1.0, 1.0);
// ������
glMatrixMode(GL_PROJECTION);	 //���õ�ǰ����ģʽ��ͶӰ����
glPushMatrix();						  //��λ����ջ
//glEnable(GL_FOG);
float mat[16];
glGetFloatv(GL_PROJECTION_MATRIX,mat); //����ͶӰ���������ƶ�ֻ��ı�λ�ã�������״����ı䣩 
glLoadIdentity();
glMultMatrixf(leftPersMatrix);		   
glMultMatrixf(mat);

glMatrixMode(GL_MODELVIEW);			//����ģ�;���(�����ƶ���ı���գ�Ȼ���ı�������״) ���Ҽ��1m
glPushMatrix();
glGetFloatv(GL_MODELVIEW_MATRIX,mat);	
glLoadIdentity();
glMultMatrixf(leftMatrix);
glMultMatrixf(mat);

//glPushMatrix();						 //����һ����·
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

//glPushMatrix();					   //��һ������
////glDisable(GL_LIGHTING);
//glColor3f(1.0, 1.0, 1.0);
//glColorMask(1.0, 1.0, 1.0, 1.0);
//gluLookAt(0, 0, R, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glTranslatef(-4.0, -3.0, -10.0);
//glutSolidTorus(0.5, 1.0, 200, 200);
//glPopMatrix();
//
//glPushMatrix();					 //����һ������ʮ����
//glEnable(GL_LIGHTING);
//glColorMask(1.0, 1.0,0.0,1.0);
//gluLookAt(0, 0, R, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glTranslatef(1,1, 3.0);												 
////glutSolidSphere(1.0,3000,3000);
//glutSolidIcosahedron();
//glPopMatrix();
//gluLookAt(0, 0, 1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

gluLookAt(0, -2, 4, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );	
glPushMatrix();						  //����һ�������
glColorMask(1.0, 1.0, 0.0, 1.0);
glBegin(GL_QUADS);					 //����
glVertex3f(2.5, 2.0, 0.0);
glVertex3f(-2.5, 2.0, 0.0);
glVertex3f(-2.5, -2.0, 0.0);
glVertex3f(2.5, -2.0, 0.0);
glEnd();
glDisable(GL_LIGHTING);
glColor3f(1.0, 1.0, 1.0);
glBegin(GL_QUADS);
glVertex3f(2.5, 2.0, 0.0);						  //ǽ��
glVertex3f(2.5, 2.0, 4.0);
glVertex3f(-2.5, -2.0, 0.0);
glVertex3f(-2.5, -2.0, 4.0);
glEnd();
glPopMatrix();

glPushMatrix();				   //����һ�����
glEnable(GL_LIGHTING);
glColorMask(1.0, 1.0,0.0,1.0);
		  
//glTranslatef(0.0, 0.0, 5.0);	//��z������5��
glTranslatef((GLfloat) xlength, 0.0, 0.0);
glTranslatef(0.0,(GLfloat) ylength, 0.0);
glTranslatef(0.0, 0.0, (GLfloat) zlength);

glRotatef(90, 1.0, 0.0 ,0.0);				//��x����ת90��
glRotatef((GLfloat) big, 0.0, 1.0, 0.0);
glRotatef(rotate_y, 0.0, 1.0, 0.0);
glRotatef(height, 1.0, 0.0 ,0.0);
//gluLookAt(R*cos(ref*c), height, R*sin(ref*c), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
glutSolidTeapot(0.5);
glPopMatrix();
glPopMatrix();						//ģ�;����ջ

//glDisable(GL_DEPTH_TEST);					   //��ʾ�������z��0��ľ���
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

glPopMatrix();						//ͶӰ�����ջ
glFlush();



////������
//glDrawBuffer(GL_BACK_LEFT);		  //�������,��ֹ����ͼ�����������ͼ��
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
//glMatrixMode(GL_PROJECTION);						//����͸�Ӿ���
//
//glPushMatrix();
//glGetFloatv(GL_PROJECTION_MATRIX,mat);
//glLoadIdentity();
//glMultMatrixf(rightPersMatrix);
//glMultMatrixf(mat);
//
//glMatrixMode(GL_MODELVIEW);						 //����ģ�;���
//glPushMatrix();
//glGetFloatv(GL_MODELVIEW_MATRIX,mat);
//glLoadIdentity();
//glMultMatrixf(rightMatrix);
//glMultMatrixf(mat);
//
//glPushMatrix();						 //����һ����·
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
//glPushMatrix();					   //��һ������
//glColorMask(1.0, 1.0, 1.0, 1.0);
//gluLookAt(0, 0, R, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glTranslatef(-4.0, -3.0, -10.0);
//glutSolidTorus(0.5, 1.0, 200, 200);
//glPopMatrix();
//
//glPushMatrix();					  //����һ������ʮ����
//glColorMask(1.0, 1.0,0.0,1.0);
//gluLookAt(0, 0, R, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
//glTranslatef(1,1, 3.0);
//////glutSolidSphere(1.0,3000,3000);
//glutSolidIcosahedron();
//glPopMatrix();
//
//glPushMatrix();							   //����һ�����
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
case 27: // ��ESC��ʱ�˳�����
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
	printf("��ʼ����ֱͬ��\n");
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
printf("�ж��Ƿ�ֱͬ�������ر�־λ\n");
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
		printf("���ñ�־λΪ1���򿪴�ֱͬ��\n");
	}
	else 
	{
		wglSwapIntervalEXT(0);
		printf("���ñ�־λΪ0���رմ�ֱͬ��\n");
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
	printf("���ر�־λΪ1\n");
	SetVSyncState(false);
}
else
{	printf("���ر�־λΪ0\n");
	SetVSyncState(false);
}

glutMainLoop();
return 0;
} 