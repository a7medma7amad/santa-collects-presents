#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <string>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

GLdouble rotAngle = 0;
//GLdouble rotX = 0;
GLdouble rotY = 1;
//GLdouble rotZ = 0;
GLdouble transX = 0;
GLdouble transZ = 0;
bool present1Flag = true;
bool present2Flag = true;
bool present3Flag = true;
bool animTree = false;
bool treeScalingUp = true;
bool animLanterns = false;
bool animSnowMan = false;
bool animFences = false;
bool animPresents = false;
bool fenceScalingUp = true;
bool snowmanFly = true;

GLdouble fenceColR= 0.588;
GLdouble fenceColG=0.294;
GLdouble fenceColB=0;
GLdouble fenceScale = 0;


GLdouble lanternsRot = 0;
GLdouble lanternsTrans = 0;
GLdouble treesScaling = 1;

GLdouble snowmanX=0;
GLdouble snowmanY = 0;
GLdouble snowmanZ = 0;

GLdouble presentsScale = 0;

int currentTime = 0;
int timeOut = 30000;
bool gameOn = true;
bool changeFenceCol = false;
class Vector3f {
	
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.492079f, float eyeY = 0.882731f, float eyeZ = 1.45444f, float centerX = 0.495573f, float centerY = 0.309848f, float centerZ = 0.634812f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;

	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
	void look(float eyeX , float eyeY , float eyeZ , float centerX , float centerY , float centerZ , float upX , float upY  , float upZ ) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{	
	int timeInc = 100;
	currentTime += timeInc;
	if (currentTime >= timeOut) {
		gameOn = false;
		presentsScale += 0.01;
	}
	if (gameOn) {
		if (changeFenceCol) {
			fenceColR = 1;
			fenceColG = 0;
			fenceColB = 0;
			changeFenceCol = false;
		}
		else {
			fenceColR = 0.588;
			fenceColG = 0.294;
			fenceColB = 0;
			changeFenceCol = true;
		}

		//trees
		if (animTree) {

			if (treesScaling <= 2 && treeScalingUp) {
				if (treesScaling > 1.9)
					treeScalingUp = false;
				else
					treesScaling += 0.1;
			}
			else if (treesScaling >= 1 && !treeScalingUp)
			{
				if (treesScaling == 1)
					treeScalingUp = true;
				else
					treesScaling -= 0.1;
			}
		}
		else {
			treesScaling = 1;
			treeScalingUp = true;
		}
		//========================

		//lanterns
		if (animLanterns) {
			lanternsTrans = 0.4;
			lanternsRot += 5;

		}
		else {
			lanternsRot = 0;
			lanternsTrans = 0;
		}


		//===================
		//fences

		
		if (animFences) {

			if (fenceScale <= 2 && fenceScalingUp) {
				if (fenceScale > 1.9)
					fenceScalingUp = false;
				else
					fenceScale += 0.1;
			}
			else if (fenceScale >= 1 && !fenceScalingUp)
			{
				if (fenceScale <= 1)
					fenceScalingUp = true;
				else
					fenceScale -= 0.1;
			}
		}
		else {
			fenceScale = 1;
			fenceScalingUp = true;
		}


		//=======================
		//snowman
		bool f1 = false;
		bool f2 = false;
		bool f3 = false;

		bool f4 = false;
		bool f5 = false;
		bool f6 = false;
		if (animSnowMan) {
			if (snowmanFly) {
				if (snowmanX <= 0.2)
					snowmanX += 0.05;
				else
					f1 = true;
				if (snowmanY <= 0.5)
					snowmanY += 0.05;
				else
					f2 = true;
				if (snowmanZ <= 0.6)
					snowmanZ += 0.05;
				else
					f3 = true;
				if (f1 && f2 && f3)
					snowmanFly = false;

			}
			else {
				if (snowmanX > 0)
					snowmanX -= 0.05;
				else
					f4 = true;
				if (snowmanY > 0)
					snowmanY -= 0.05;
				else
					f5 = true;
				if (snowmanZ > 0)
					snowmanZ -= 0.05;
				else
					f6 = true;
				if (f4 && f5 && f6)
					snowmanFly = true;
			}
		}
		else {
			snowmanX = 0;
			snowmanY = 0;
			snowmanZ = 0;
		}


		//============

	}
	glutPostRedisplay();								
	glutTimerFunc(timeInc, time, 0);					
}

void drawWall(double thickness) {
	glPushMatrix();
	//glColor3d(1.0, 1.0, 1.0);
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawFence(double thickness, char rotAxis) {

	
	for (size_t i = 0; i < 21; i++)
	{
		glPushMatrix();
		
		glColor3d(fenceColR,fenceColG,fenceColB);
		
		if (rotAxis == 'x') {
			glTranslated(0.05 * i, 0.5 * thickness, 0);
			glScaled(0.02, thickness, 0.2);
		}
		else if (rotAxis == 'z') {
			glTranslated(0, 0.5 * thickness,0.05 * i);
			glScaled(0.2, thickness, 0.02);
		}
		glutSolidCube(1);
		glPopMatrix();
	}
	
	
}
void drawFenceHolder() {
	
	glPushMatrix();
	glColor3d(0.588, 0.294, 0);

	glPushMatrix();
	glScaled(0.3, 0.05, 1.0);
	glTranslated(0, 1.5, 0);
	glRotated(90, 0, 0, 1.0);
	drawWall(0.002);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 0.05, 1.0);
	glTranslated(1.0, 1.5, 0.0);
	glPushMatrix();
	glRotated(90, 0, 0, 1.0);
	drawWall(0.002);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	
	glTranslated(0, 0.07, 0);
	glScaled(1, 0.05, 0.3);
	glRotated(-90, 1.0, 0.0, 0.0);
	
	drawWall(0.002);
	glPopMatrix();

	glPopMatrix();
}
void drawPresent() {

	glPushMatrix();
	glColor3d(1.0, 0, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.9 , 1 , 0.2);
	glTranslated(0, 0, 0.05);
	glScaled(0.03, 0.1, 0.01);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.9, 1, 0.2);
	glRotated(180, 0, 1, 0);
	glTranslated(0, 0, 0.05);
	glScaled(0.03, 0.1, 0.01);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.9, 1, 0.2);
	
	glTranslated(0, 0.05, 0);
	glRotated(-90, 1, 0, 0);
	glScaled(0.03, 0.1, 0.01);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0,1,0);
	glTranslated(0, 0.05, 0);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();
}
void drawLocation() {
	
	
	glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);
	drawWall(0.02);

	glPushMatrix();
	glScaled(1, fenceScale, 1);

	glPushMatrix();
	glTranslated(0.0, 0.47, 0.0);
	glTranslated(0, -0.35, 0);
	glRotated(90, 0, 0, 1.0);
	drawFence(0.002, 'z');
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.47, 0.0);
	glTranslated(0, -0.35, 0);
	glTranslated(1.0, 0.0, 0.0);
	glPushMatrix();

	glRotated(90, 0, 0, 1.0);
	drawFence(0.002, 'z');
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.47, 0.0);
	glTranslated(0, -0.35, 0);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawFence(0.002, 'x');
	glPopMatrix();

	

	drawFenceHolder();

	glPopMatrix();

	glPopMatrix();
}
void drawTableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawJackPart() {
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}

void drawSnowman() {

	glPushMatrix();
	glColor3d(1,1,1);
	glutSolidSphere(0.1, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslated(0, 0.1, 0);
	glutSolidSphere(0.08, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslated(0, 0.2, 0);
	glutSolidSphere(0.05, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.5f);
	glTranslated(0, 0.2, 0.05);
	glutSolidCone(0.005f, 0.1f, 10, 2);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0, 0.0, 0.0);
	glTranslated(-0.02, 0.22, 0.04);
	glutSolidSphere(0.01, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0, 0.0, 0.0);
	glTranslated(0.02, 0.22, 0.04);
	glutSolidSphere(0.01, 100, 100);
	glPopMatrix();

}

void drawTree() {
	glPushMatrix();
	glColor3d(0,1,0);
	glTranslated(0, 0, 0);
	glRotated(-90, 1.0, 0, 0);
	glutSolidCone(0.4, 0.3, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0, 1, 0);
	glTranslated(0, 0.2, 0);
	glRotated(-90, 1.0, 0, 0);
	glutSolidCone(0.3, 0.3, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0, 1, 0);
	glTranslated(0, 0.4, 0);
	glRotated(-90, 1.0, 0, 0);
	glutSolidCone(0.2, 0.3, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.8 , 0 , 0.4);
	glTranslated(0, 0.35,0.17);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.8 , 0.9 , 0.1);
	glTranslated(0.15, 0.35, 0);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1 , 0.6 , 0);
	glTranslated(0, 0.1, 0.3);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1 , 0.6 , 1);
	glTranslated(0.3, 0.1, 0);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();
	
	glPushMatrix();
	glColor3d(0.6 , 0.2 , 0.9);
	glTranslated(0, 0.55, 0.1);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glRotated(180, 0, 1, 0);

	glPushMatrix();
	glColor3d(0.8, 0, 0.4);
	glTranslated(0, 0.35, 0.17);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.8, 0.9, 0.1);
	glTranslated(0.15, 0.35, 0);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 0.6, 0);
	glTranslated(0, 0.1, 0.3);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 0.6, 1);
	glTranslated(0.3, 0.1, 0);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.6, 0.2, 0.9);
	glTranslated(0, 0.55, 0.1);
	glutSolidSphere(0.03, 100, 100);
	glPopMatrix();

	glPopMatrix();
}
void drawMessi() {
	
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(0.1, -0.3, 0.03);
	//glRotated(45, 0.0, 1.0, 0.0);
	glScaled(0.1, 0.05, 0.15);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(-0.1, -0.3, 0.03);
	//glRotated(45, 0.0, 1.0, 0.0);
	glScaled(0.1, 0.05, 0.15);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslated(0.1, 0.0, 0.0);
	GLUquadricObj* quadratic;
	quadratic = gluNewQuadric();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadratic, 0.05f, 0.05f, 0.3f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 1, 1);
	GLUquadricObj* quadratic1;
	quadratic1 = gluNewQuadric();
	glTranslated(-0.1, 0.0, 0.0);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadratic1, 0.05f, 0.05f, 0.3f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 0.0, 0.0);
	glRotated(-90, 1.0, 0, 0);
	glutSolidCone(0.2, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);
	glTranslated(0, 0.35, 0);
	glutSolidSphere(0.1, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 0, 0);
	GLUquadricObj* quadratic2;
	quadratic2 = gluNewQuadric();
	glTranslated(0, 0.23, 0.0);
	glRotated(35, 0.0f, 0.0f, 1.0f);
	glRotated(-90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic2, 0.03f, 0.03f, 0.35f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 0, 0);
	GLUquadricObj* quadratic3;
	quadratic3 = gluNewQuadric();
	glTranslated(0, 0.23, 0.0);
	glRotated(-35, 0.0f, 0.0f, 1.0f);
	glRotated(90, 0.0f, 1.0f, 0.0f);
	gluCylinder(quadratic3, 0.03f, 0.03f, 0.35f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0, 0, 0);
	
	glRotated(-35, 0.0f, 0.0f, 1.0f);
	glRotated(90, 0.0f, 1.0f, 0.0f);
	glTranslated(0.0, 0.19, 0.2);
	glScaled(0.07, 0.07, 0.07);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0, 0, 0);

	glRotated(35, 0.0f, 0.0f, 1.0f);
	glRotated(-90, 0.0f, 1.0f, 0.0f);
	glTranslated(0.0, 0.19, 0.2);
	glScaled(0.07, 0.07, 0.07);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0, 0.0, 0.0);
	glTranslated(-0.03, 0.4, 0.07);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0, 0.0, 0.0);
	glTranslated(0.03, 0.4, 0.07);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 0.0, 0.0);
	glTranslated(0, 0.25, 0);
	glRotated(-90, 1.0, 0, 0);
	glutSolidCone(0.1, 0.4, 20, 20);
	glPopMatrix();

	glFlush();
}

void drawGreenLantern() {
	glPushMatrix();
	glColor3d(0.0, 1.0, 0.0);
	//glTranslated(0.03, 0.4, 0.07);
	glScaled(1.5, 1, 1);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0, 0.0, 0.0);
	glTranslated(0.0, 0.2,0);
	glScaled(1.5, 1, 1);
	glutSolidSphere(0.06, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0, 0.0, 0.0);
	glRotated(180, 1, 0, 0);
	glTranslated(0.0, 0.2, 0);
	glScaled(1.5, 1, 1);
	glutSolidSphere(0.06, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0, 1.0, 0.0);
	glRotated(90, 1, 0, 0);
	glTranslated(0.0, 0.2, -0.1);
	glScaled(1.5, 1, 1);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(0.0, 1.0, 0.0);
	glRotated(-90, 1, 0, 0);
	glTranslated(0.0, 0.2, -0.1);
	glScaled(1.5, 1, 1);
	glutSolidSphere(0.02, 100, 100);
	glPopMatrix();

}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void Display() {
	setupCamera();
	setupLights();

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//-----------------------------------------------------------
	
	
	drawLocation();
	
	//==============
	//trees
	glPushMatrix();
	//glScaled(treesScaling, treesScaling, treesScaling);

	glPushMatrix();
	glTranslated(0.2, 0, 0.2);
	glScaled(treesScaling, treesScaling, treesScaling);
	glScaled(0.4, 0.6, 0.4);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.8, 0, 0.2);
	glScaled(treesScaling, treesScaling, treesScaling);
	glScaled(0.4, 0.6, 0.4);
	drawTree();
	glPopMatrix();

	glPopMatrix();
	//=================
	//snowman
	glPushMatrix();
	glTranslated(snowmanX, snowmanY, snowmanZ);
	glTranslated(0.4, 0.05, 0.2);
	glScaled(0.6, 0.6, 0.6);
	drawSnowman();
	glPopMatrix();

	//==============
	//lanterns
	
	glPushMatrix();
	glRotated(lanternsRot, 0, 1, 0);
	glTranslated(0, lanternsTrans, 0);

	glPushMatrix();
	glTranslated(0.7, 0.1, 0.7);
	glScaled(0.3, 0.3, 0.3);
	drawGreenLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.2, 0.1, 0.9);
	glScaled(0.3, 0.3, 0.3);
	drawGreenLantern();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.8, 0.1, 0.5);
	glScaled(0.3, 0.3, 0.3);
	drawGreenLantern();
	glPopMatrix();

	glPopMatrix();

	//=========================
	 //presents
	glPushMatrix();
	glTranslated(0, presentsScale, 0);

	if (present1Flag) {
		glPushMatrix();
		glTranslated(0.2, 0.05, 0.6);
		drawPresent();
		glPopMatrix();
	}

	if (present2Flag) {
		glPushMatrix();
		glTranslated(0.5, 0.05, 0.8);
		drawPresent();
		glPopMatrix();
	}

	if (present3Flag) {
		glPushMatrix();
		glTranslated(0.8, 0.05, 0.9);
		drawPresent();
		glPopMatrix();
	}
	glPopMatrix();
	//======================

	//messi
	glPushMatrix();
	glTranslated(0.5+transX, 0.15, 0.5+transZ);
	glRotated(rotAngle, 0, rotY, 0);
	glScaled(0.4, 0.4, 0.4);
	drawMessi();
	glPopMatrix();
	
	//=====================
	//print
	if (!gameOn) {
		glColor3f(1, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "Game Over");
		print(500, 300, (char*)p0s);
	}

	//==========
	
	
	//-----------------------------------------------------------
	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;
	if ((0.5 + transX > 0.2 -0.05 && 0.5 + transX < 0.2 + 0.05)&&  (0.5 + transZ > 0.6 -0.05 && 0.5 + transZ < 0.6 + 0.05)) {
		present1Flag = false;
	}
	else if ((0.5 + transX > 0.5 - 0.05 && 0.5 + transX < 0.5 + 0.05) && (0.5 + transZ > 0.8 - 0.05 && 0.5 + transZ < 0.8 + 0.05)) {
		present2Flag = false;
	}
	else if ((0.5 + transX > 0.8 - 0.05 && 0.5 + transX < 0.8 + 0.05) && (0.5 + transZ > 0.9 - 0.05 && 0.5 + transZ < 0.9 + 0.05)) {
		present3Flag = false;
	}

	switch (key) {
		//camera views
		
		//top
		case't':
			
			camera.look(0.495210,1.265493, 0.720206,  0.495868,  0.277496,  0.565732, 0.004212,  0.154476, -0.987988);
			break;
		//front
		case'y':
			camera.look(0.492079f,0.882731f, 1.45444f, 0.495573f,  0.309848f,  0.634812f,  0.0f, 1.0f,  0.0f);
			break;
		
		//side
		case'u':
			camera.look(1.480090,  0.329268,  0.487120,  0.484874, 0.231999, 0.477926, -0.097367, 0.995189,  0.010927);
			break;

		case 'w':
			camera.moveY(d);
			break;
		case 's':
			camera.moveY(-d);
			break;
		case 'a':
			camera.moveX(d);
			break;
		case 'd':
			camera.moveX(-d);
			break;
		case 'q':
			camera.moveZ(d);
			break;
		case 'e':
			camera.moveZ(-d);
			break;

			
	}
	if (gameOn) {
		switch (key) {
		
		
			//messi movement
			//up
		case 'i':
			rotAngle = 180;
			if (transZ + 0.5 > 0.1)
				transZ -= 0.01;
			break;
			//down
		case 'k':
			rotAngle = 0;
			if (transZ + 0.5 < 0.9)
				transZ += 0.01;
			break;
			//left
		case 'j':
			rotAngle = -90;
			if (transX + 0.5 > 0.1)
				transX -= 0.01;
			break;
			//right
		case 'l':
			rotAngle = 90;
			if (transX + 0.5 < 0.9)
				transX += 0.01;
			break;

			//-----------------
			//animations

			//trees
		case 'z':
			animTree = !animTree;

			break;
		case 'x':
			animLanterns = !animLanterns;

			break;
		case 'c':
			animSnowMan = !animSnowMan;

			break;
		case 'v':
			animPresents = !animPresents;

			break;
		case 'b':
			animFences = !animFences;

			break;


		case GLUT_KEY_ESCAPE:
			exit(EXIT_SUCCESS);
		}
	}
	printf("eyeX=%f, eyeY=%f, eyeZ=%f, centerX=%f, centerY=%f, centerZ=%f, upX=%f, upY=%f, upZ=%f \n", camera.eye.x, camera.eye.y, camera.eye.z, camera.center.x, camera.center.y, camera.center.z, camera.up.x, camera.up.y, camera.up.z);
	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Lab 5");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutTimerFunc(0, time, 0);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
