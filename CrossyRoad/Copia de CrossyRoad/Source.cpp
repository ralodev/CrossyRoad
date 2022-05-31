#include <math.h>
#include <time.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "ImageLoader.cpp"
/*
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <glut.h>*/
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/GLUT.h>

GLuint _cielo;

GLuint loadTexture(Image* image) {
	GLuint idtextura;
	glGenTextures(1, &idtextura);
	glBindTexture(GL_TEXTURE_2D, idtextura);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return idtextura;
}

void initRendering() {	
	Image* sky = loadBMP("skya.bmp");
	_cielo = loadTexture(sky);	
	delete sky;
}

void cargarTextura(GLuint _textura) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


//Tipo de dato KeyFrame (LearnOpenGL)
typedef struct{
	float time;
	float value;
} Keyframe;

//Tipo de dato coord (coordenadas x,y,z)
typedef struct {
	float x, y, z; 
} coord;

//Tipo de dato objeto (Automoviles y gallina)
typedef struct {
	coord posicionInicial, posicion, velocidad;
	float ultimoMov;//?
	float radius;// Hitbox?
	bool haChocado;
}objeto;

objeto gallina;
objeto autos[4];

const float g = -9.8;
const int milliseconds = 1000;

typedef struct {
	bool saltando;
	float startTime;//?
	int frames;//?
	float frameRate;//?
	float frameRateInterval;//?
	float lastFrameRateT;//?
	float angulo;
	float velocidad;
	float rotacion;
	int ladosPoligono;
	bool rightbtn;
	int puntuacion;
	bool murio;
	bool ganaste;
	bool perdiste;
} control;

control variables = {false, 0, 0, 0, 0.2, 0, M_PI / 4, 2.0, M_PI / 2, 5, false, 0, false, false, false };

typedef struct {
	float angleX;
	float angleY;
	float angleZ;
	float lastX;
	float lastY;
	float deltaX;
	float deltaY;
	float zoomScale;
	float x;
	float y;
} camera_t;

//static GLuint texturaCalle;
static GLuint texturaPasto;
static GLuint skybox[6];
//static GLuint texturaCielo;

camera_t camara = { 90.0, 0.0, 30.0, 0, 0, 0, 0, -0.7, 0.0, 0.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0};

//COLORES RGB
GLfloat green[] = { 0.0, 1.0, 0.0 };
GLfloat brown[] = { 0.5, 0.3, 0.1 };
GLfloat red[] = { 1.0, 0.0, 0.0 };
GLfloat white[] = { 1.0, 1.0, 1.0 };
GLfloat gray[] = { 0.8, 0.8, 0.8 };
GLfloat blue[] = { 0.0, 0.0, 1.0 };
GLfloat black[] = { 0.0, 0.0, 0.0 };
GLfloat lightBlue[] = { 0.6, 0.8, 1.0 };
GLfloat purple[] = { 1.0, 0.0, 1.0 };
GLfloat orange[] = { 1.0, 0.6, 0.0 };

void reiniciarPosicion() {
	gallina.posicionInicial.x = gallina.posicion.x = -1.8;
	gallina.posicionInicial.y = gallina.posicion.y = 0;
	gallina.posicionInicial.z = gallina.posicion.z = 0;
}

void revivir() {
	reiniciarPosicion();
	variables.murio = 1;
}

/**
*Detecta la colisión del pollito con los coches
*/
bool revisarColision() {

	for (int i = 0; i < 4; i++) {
		if ((gallina.posicion.x - autos[i].posicion.x) * (gallina.posicion.x - autos[i].posicion.x)
			+ (gallina.posicion.z - autos[i].posicion.z) * (gallina.posicion.z - autos[i].posicion.z) <= 0.25 * 0.25) {
			autos[i].haChocado = true;
			return true;
		}
		else {
			autos[i].haChocado = false;

		}
	}
	return false;
}

void dibujaCuadrado(float gridX, float gridZ, float gridWidth, float profundidad, GLuint texture) {


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBegin(GL_QUADS);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(gridX, profundidad, gridZ);

	glNormal3f(0, 1, 0);
	glTexCoord2f(1, 0);
	glVertex3f(gridX + gridWidth, profundidad, gridZ);

	glNormal3f(0, 1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(gridX + gridWidth, profundidad, gridZ - gridWidth);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(gridX, profundidad, gridZ - gridWidth);
	glEnd();


	glDisable(GL_TEXTURE_2D);
}


void dibujaCielo() {

	float textureSize = 300.0;
	float width, height, length;
	width = 5;
	height = 3;
	length = 5;

	float texture0 = 1.0f / textureSize;
	float texture1 = (textureSize - 1) / textureSize;
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cielo);
	//cargarTextura();
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(width, height, length);
	glTexCoord2d(texture0, texture1); glVertex3f(-width, height, length);
	glTexCoord2d(texture1, texture1); glVertex3f(-width, height, -length);
	glTexCoord2d(texture1, texture0); glVertex3f(width, height, -length);
	glEnd();

	glBindTexture(GL_TEXTURE_CUBE_MAP, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(-width, -height, length);
	glTexCoord2d(texture1, texture0); glVertex3f(width, -height, length);
	glTexCoord2d(texture1, texture1); glVertex3f(width, height, length);
	glTexCoord2d(texture0, texture1); glVertex3f(-width, height, length);
	glEnd();

	glBindTexture(GL_TEXTURE_CUBE_MAP, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(width, -height, -length);
	glTexCoord2d(texture1, texture0); glVertex3f(-width, -height, -length);
	glTexCoord2d(texture1, texture1); glVertex3f(-width, height, -length);
	glTexCoord2d(texture0, texture1); glVertex3f(width, height, -length);
	glEnd();

	glBindTexture(GL_TEXTURE_CUBE_MAP, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(-width, -height, -length);
	glTexCoord2d(texture1, texture0); glVertex3f(-width, -height, length);
	glTexCoord2d(texture1, texture1); glVertex3f(-width, height, length);
	glTexCoord2d(texture0, texture1); glVertex3f(-width, height, -length);
	glEnd();

	glBindTexture(GL_TEXTURE_CUBE_MAP, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, -1.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(width, -height, length);
	glTexCoord2d(texture1, texture0); glVertex3f(width, -height, -length);
	glTexCoord2d(texture1, texture1); glVertex3f(width, height, -length);
	glTexCoord2d(texture0, texture1); glVertex3f(width, height, length);
	glEnd();

	glDisable( GL_TEXTURE_CUBE_MAP);
}


void dibujaCarretera(float a, float b, float height, GLfloat color[]) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(a, height, -2);
	glNormal3f(0, 1, 0);
	glVertex3f(a, height, 2);
	glNormal3f(0, 1, 0);
	glVertex3f(b, height, 2);
	glNormal3f(0, 1, 0);
	glVertex3f(b, height, -2);
	glEnd();
}
/*Ayuda a dibujar el pasto*/
void dibujaGrid(int n) {
	float gridWidth = 0.5;
	float gridX = -(gridWidth * n / 2.0);
	float gridZ = gridWidth * n / 2.0;
	float profundidad = 0;
	for (int i = 0; i < n; i++) { //for each column
		for (int j = 0; j < n; j++) { //one line
			dibujaCuadrado(gridX, gridZ, gridWidth, profundidad, texturaPasto);
			gridX += gridWidth;
		}
		gridZ -= gridWidth;
		gridX = -(gridWidth * n / 2.0);
	}
}

void dibujaCirculo(float r, float posZ) {
	float theta;
	float x1, y1, x2, y2;

	for (int i = 0; i <= variables.ladosPoligono; i++) {
		theta = i / (float)variables.ladosPoligono * 2.0 * M_PI;
		x1 = r * cosf(theta);
		y1 = r * sinf(theta);
		theta = (i + 1) / (float)variables.ladosPoligono * 2.0 * M_PI;
		x2 = r * cosf(theta);
		y2 = r * sinf(theta);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(x1, y1, posZ);
		glVertex3f(x2, y2, posZ);
		glVertex3f(0, 0, posZ);
		glEnd();
	}
}

void dibujaCilindro(float r, float length) {
	float theta;
	float x, y;

	dibujaCirculo(r, 0);

	dibujaCirculo(r, -length);

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= variables.ladosPoligono; i++) {
		theta = i / (float)variables.ladosPoligono * 2.0 * M_PI;
		x = r * cosf(theta);
		y = r * sinf(theta);
		glTexCoord2f(0, 0);
		glVertex3f(x, y, 0);
		glTexCoord2f(1, 0);
		glVertex3f(x, y, -length);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param z 
 */
void dibujaCubo(float x, float y, float z) {
	glBegin(GL_QUADS);

	// arriba
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-x, y, z);
	glVertex3f(x, y, z);
	glVertex3f(x, y, -z);
	glVertex3f(-x, y, -z);

	// abajo
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-x, -y, z);
	glVertex3f(x, -y, z);
	glVertex3f(x, -y, -z);
	glVertex3f(-x, -y, -z);


	// enfrente
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(x, -y, z);
	glVertex3f(x, y, z);
	glVertex3f(-x, y, z);
	glVertex3f(-x, -y, z);


	// atras
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(x, -y, -z);
	glVertex3f(x, y, -z);
	glVertex3f(-x, y, -z);
	glVertex3f(-x, -y, -z);

	// derechg
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x, y, -z);
	glVertex3f(x, y, z);
	glVertex3f(x, -y, z);
	glVertex3f(x, -y, -z);

	// izq
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-x, y, -z);
	glVertex3f(-x, y, z);
	glVertex3f(-x, -y, z);
	glVertex3f(-x, -y, -z);

	glEnd();
}

void movimientoRaton(int x, int y) {
	camara.deltaX = camara.lastX - x;
	camara.deltaY = camara.lastY - y;

	camara.lastX = x;
	camara.lastY = y;


	if (variables.rightbtn == 1)
		camara.zoomScale -= camara.deltaY / 50.0;
	else {
		camara.angleX -= camara.deltaX;
		camara.angleY -= camara.deltaY;
		camara.angleZ -= camara.deltaY;
	}

	glutPostRedisplay();
}

void movimientoRatonBtn(int boton, int objeto, int x, int y) {


	if (boton == GLUT_LEFT_BUTTON) {
		variables.rightbtn = 0;
	}

	if (boton == GLUT_RIGHT_BUTTON) {
		variables.rightbtn = 1;
	}
	camara.lastX = x;
	camara.lastY = y;
	glutPostRedisplay();
}

//Dibujar la gallina
void dibujarGallina() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glPushMatrix();
	glTranslatef(gallina.posicion.x, gallina.posicion.y + 0.055, gallina.posicion.z); //0.055 es ajuste al suelo para que las patas no estén abajo del mapa, falta arreglar
	glScalef(0.01, 0.01, 0.01);

	glRotatef(variables.rotacion * 180 / M_PI - 90, 0, 1, 0); //Rotación inicial de la gallina (no se grafica en la dirección correcta xd)
	//Dibuja el cuerpo
	dibujaCubo(2, 5.0, 2.2);
	dibujaCubo(3, 2.0, 3.2);
	glPushMatrix();
	//Dibuja las alas del pollito	
	
	glTranslatef(0.0, 0, 4.0); 
	glRotatef(10, 0, 0, 1);
	dibujaCubo(2.0, 1, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0, -4.0); 
	glRotatef(10, 0, 0, 1);
	dibujaCubo(2.0, 1, 1.0); 
	glPopMatrix();

	//Dibuja el pico del pollito
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, orange);
	glPushMatrix();
	glTranslatef(2.0, 3, 0.0);
	dibujaCubo(2.0, 0.5, 1.0);
	glPopMatrix();
	//Dibuja los ojos del pollito
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(1.0, 4, 2.5); 
	glRotatef(10, 0, 0, 1);
	dibujaCubo(0.5, 0.5, 0);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(1.0, 4, -2.5); 
	glRotatef(10, 0, 0, 1);
	dibujaCubo(0.5, 0.5, 0); 
	glPopMatrix();

	//Dibuja Las patas del pollito
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, orange);
	glPushMatrix();
	glTranslatef(0.0, -5.0, 0.0);
	dibujaCubo(3.0, 0.4, 1.0);
	glPopMatrix();

	//Dibuja la cresta del pollito	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
	glPushMatrix();
	glTranslatef(0.0, 5, 0.0);
	glRotatef(10, 0, 0, 1);
	dibujaCubo(1.50, 1, 1.0); 
	glPopMatrix();
	
	glPopMatrix();
	
}

void dibujaLlantas() {

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
	glPushMatrix();
	glTranslatef(-0.35, -0.18, 0.26);
	dibujaCilindro(0.1, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, -0.18, 0.26);
	dibujaCilindro(0.1, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.35, -0.18, -0.26);
	dibujaCilindro(0.1, 0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, -0.18, -0.26);
	dibujaCilindro(0.1, 0.05);
	glPopMatrix();
	glPopMatrix();

}

//Dibuja los carros
void dibujaCarro() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
	glPushMatrix();
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(0, 0.2, 0);
	glRotatef(90, 0, 1, 0);
	//centro del carro
	dibujaCubo(0.5, 0.1, 0.25);
	glPushMatrix();
	glTranslatef(0, 0.1, 0);

	//cajuela y cofre del carro
	dibujaCubo(0.25, 0.15, 0.25);
	glPushMatrix();
	glPopMatrix();

	//dibuja las llantas
	dibujaLlantas();
	glPopMatrix();
}
/**
*Actualiza la velocidad de
*/
void calculaSalto() {
	gallina.velocidad.x = sin(variables.rotacion) * cos(variables.angulo) * variables.velocidad;
	gallina.velocidad.y = sin(variables.angulo) * variables.velocidad;
	gallina.velocidad.z = cos(variables.angulo) * cos(variables.rotacion)* variables.velocidad;
}

void setPosicion() {
	gallina.ultimoMov = variables.startTime;
	gallina.posicion.y = 0;
	gallina.posicionInicial.x = gallina.posicion.x;
	gallina.posicionInicial.y = gallina.posicion.y;
	gallina.posicionInicial.z = gallina.posicion.z;
}

//Crea el carro en el carril i
void iniciaAutomovil(int i) {
	autos[i].posicion.x = -1.2 + 0.5 * i;
}

void actualizaPosGallina(float dt)
{
	// Posicion
	gallina.posicion.x += gallina.velocidad.x * dt;
	gallina.posicion.y += gallina.velocidad.y * dt;
	gallina.posicion.z += gallina.velocidad.z * dt;

	// Velocidad
	gallina.velocidad.y += g * dt;
}

void verificaColision(float dt) {
	gallina.haChocado = revisarColision();

	if (gallina.haChocado) {
		gallina.haChocado = false;
		revivir();
	}
}
/**
 * @brief 
 *Sirve para imprimir los letreros 
 */
void displayOSD() {
	char buffer[30];
	char *bufp;
	int w, h;

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();


	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/* Puntos */
	glColor3f(0.0, 1.0, 0.0);
	glRasterPos2i(w / 2 - 20, h - 50);
	snprintf(buffer, sizeof buffer, "PUNTOS: %d", variables.puntuacion);
	for (bufp = buffer; *bufp; bufp++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

	/* Mensaje al ganar */
	if (variables.ganaste) {
		glColor3f(0.0, 1.0, 0.0);
		glRasterPos2i(w / 2 - 20, h / 2 - 50);
		snprintf(buffer, sizeof buffer, "GANASTE");
		for (bufp = buffer; *bufp; bufp++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *bufp);
	}

	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);


	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);


	glPopAttrib();
}
/**
 * @brief 
 * Dibuja los compoenentes en la pantalla
 */
void display(void) {
	GLenum err;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//transformaciones camara
	glTranslatef(0, 0, camara.zoomScale);
	glRotatef(camara.angleY, 1.0, 0.0, 0.0);
	glRotatef(camara.angleX, 0.0, 1.0, 0.0); //rotacion interactiva
	glRotatef(camara.angleZ, 0.0, 0.0, 1.0);
	glTranslatef(-gallina.posicion.x, -gallina.posicion.y, -gallina.posicion.z); //camara sigue esfera


	glDisable(GL_LIGHTING);
	dibujaCielo();
	displayOSD();
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glTranslatef(gallina.posicionInicial.x, gallina.posicionInicial.y, gallina.posicionInicial.z);
	glRotatef(variables.rotacion * 180 / M_PI - 90, 0.0, 1.0, 0.0);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); //after the camara transformations!
	
	
	//carretera
	dibujaCarretera(-1.5, 0.5, 0.01, gray);
	dibujarGallina();

	//pasto
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
 	dibujaGrid(8);

	 //
	 glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lightBlue);
	dibujaCielo();

	//autos
	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glTranslatef(autos[i].posicion.x, 0, autos[i].posicion.z);
		if (autos[i].haChocado) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
		}
		else {
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
		}
		dibujaCarro();
		glPopMatrix();
	}
	glutSwapBuffers();

	variables.frames++;
}

void update(void) {

	/*Pequeña pausa para mostrar mensaje al ganar
	if (variables.ganaste)
	{
		Sleep(500);
		variables.ganaste = false;
	}*/
	

	float t, dt; //tiempo y diferenciaTiempo

	t = glutGet(GLUT_ELAPSED_TIME) / (float)milliseconds; //Obtiene el tiempo que ha transcurrido

	dt = t - autos[0].ultimoMov; //Obtiene el tiempo transcurrido desde el último movimiento de autos

	for (int i = 0; i < 4; i++) {
		autos[i].posicion.z += dt * autos[i].velocidad.x;//Calcula la nueva posición (tiempo transcurrido desde el último movimiento * velocidad de movimiento)
		if (autos[i].posicion.z > 2) { //Si el auto llega al final de la carretera
			autos[i].posicion.z = -2; //Lo regresa al principio
			iniciaAutomovil(i);
			autos[i].velocidad.x = rand() / (RAND_MAX / 5)+0.5; //Establece la velocidad en X de los autos
		}
	}


	verificaColision(dt);

	//Revisa si el jugador llegó a la meta
	if (gallina.posicion.x > 2) {
		printf("Llegaste a la meta\n");
		variables.ganaste = true;
		variables.puntuacion++;
		reiniciarPosicion();
	}

	//Actualiza momento de último movimiento
	autos[0].ultimoMov = t;
	glutPostRedisplay();
	dt = t - gallina.ultimoMov;
	gallina.ultimoMov = t;

	if (gallina.posicion.y < 0) {
		setPosicion();
		calculaSalto();
		display();
		variables.saltando = false;
	}

	if (!variables.saltando) {
		return;
	}
	actualizaPosGallina(dt);
	
	glutPostRedisplay();
}

void myinit(void)
{
	autos[0].ultimoMov = variables.startTime;
	glEnable(GL_NORMALIZE);

	gallina.posicionInicial.x = -1.8;
	gallina.posicion.x = -1.8;


	gallina.radius = 0.05;
	calculaSalto();
	srand(time(NULL));
	for (int i = 0; i < 4; i++) {
		iniciaAutomovil(i);
		autos[i].velocidad.x = (float)rand() / (float)(RAND_MAX / 0.5) + 1;
	}
}

void keyboardCB(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: //ESCAPE
		exit(EXIT_SUCCESS);
		break;
	case ' ':
		if (!variables.saltando && !variables.perdiste) {
			variables.startTime = glutGet(GLUT_ELAPSED_TIME) / (float)milliseconds;
			calculaSalto();
			gallina.posicion.y = 0;
			variables.saltando = true;
			setPosicion();
			calculaSalto();
		}
		break;
	case 13: //ENTER
		variables.perdiste = false;
		break;
	case 'a':
		if (!variables.saltando) {
			variables.rotacion = M_PI;
			calculaSalto();
			display();
		}
		break;
	case 'w':
		if (!variables.saltando) {
			variables.rotacion = M_PI / 2;
			calculaSalto();
			display();
		}
		break;
	case 's':
		if (!variables.saltando) {
			variables.rotacion = (M_PI / 2) + M_PI;
			calculaSalto();
			display();
		}
		break;
	case 'd':
		if (!variables.saltando) {
			variables.rotacion = 0;
			calculaSalto();
			display();
		}
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();		
	gluPerspective(75.0, (float)w / (float)h, 0.01, 100);		
	glMatrixMode(GL_MODELVIEW);	
}

/* Ciclo principal
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(500, 500);	
	glutCreateWindow("CrossyRoad: Equipo graficacion");
	glutKeyboardFunc(keyboardCB);
	initRendering();
	glutMotionFunc(movimientoRaton);
	glutMouseFunc(movimientoRatonBtn);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(update);

	myinit();

	glutMainLoop();

	return 0;
}