#include <math.h>
#include <time.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "ImageLoader.cpp"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <glut.h>

# define M_PI 3.14159265358979323846

GLuint _cielo;

//Fuentes de luz
//Consultado en: https://www.glprogramming.com/red/chapter05.html#name4
GLfloat light_position1[] = { -3, 2, 2, 0 };
GLfloat light_position2[] = { 3, -2, -2, 0 };
GLfloat light_ambient[] = { 0, 0, 0, 1};
GLfloat light_diffuse[] = { 1, 1, 1, 1};

//Tipo de dato coord (coordenadas x,y,z)
typedef struct {
	float x, y, z; 
} coord;

//Tipo de dato objeto (Automoviles y gallina)
typedef struct {
	coord posicionInicial, posicion, velocidad;
	float ultimoMov;
	bool haChocado;
}objeto;

objeto gallina;
objeto autos[8];

typedef struct {
	bool saltando;
	float tiempo;
	float angulo;
	float velocidad;
	float rotacion;
	int ladosPoligono;
	bool btnDerecho;
	int puntuacion;
	bool perdiste;
} control;

control variables = {false, 0, M_PI / 4, 2.0, M_PI / 2, 16, false, 0, false };

typedef struct {
	float anguloX;
	float anguloY;
	float anguloZ;
	float anteriorX;
	float anteriorY;
	float deltaX;
	float deltaY;
	float zoom;
	float x;
	float y;
} camera_t;

camera_t camara = { 90.0, 0.0, 40.0, 0, 0, 0, 0, -1, 0.0, 0.0 };

//COLORES RGB
GLfloat verde[] = { 0.0, 1.0, 0.0 };
GLfloat cafe[] = { 0.5, 0.3, 0.1 };
GLfloat rojo[] = { 1.0, 0.0, 0.0 };
GLfloat blanco[] = { 1.0, 1.0, 1.0 };
GLfloat gris[] = { 0.8, 0.8, 0.8 };
GLfloat azul[] = { 0.0, 0.0, 1.0 };
GLfloat negro[] = { 0.0, 0.0, 0.0 };
GLfloat azulCielo[] = { 0.6, 0.8, 1.0 };
GLfloat morado[] = { 1.0, 0.0, 1.0 };
GLfloat naranja[] = { 1.0, 0.6, 0.0 };

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

void dibujaCielo() {
	float textureSize = 512.0;
	float width, height, length;
	width = 6;
	height = 6;
	length = 6;
	float texture0 = 1.0f / textureSize;
	float texture1 = (textureSize - 1) / textureSize;
	cargarTextura(_cielo);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(width, height, length);
	glTexCoord2d(texture0, texture1); glVertex3f(-width, height, length);
	glTexCoord2d(texture1, texture1); glVertex3f(-width, height, -length);
	glTexCoord2d(texture1, texture0); glVertex3f(width, height, -length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(-width, -height, length);
	glTexCoord2d(texture1, texture0); glVertex3f(width, -height, length);
	glTexCoord2d(texture1, texture1); glVertex3f(width, height, length);
	glTexCoord2d(texture0, texture1); glVertex3f(-width, height, length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(width, -height, -length);
	glTexCoord2d(texture1, texture0); glVertex3f(-width, -height, -length);
	glTexCoord2d(texture1, texture1); glVertex3f(-width, height, -length);
	glTexCoord2d(texture0, texture1); glVertex3f(width, height, -length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(-width, -height, -length);
	glTexCoord2d(texture1, texture0); glVertex3f(-width, -height, length);
	glTexCoord2d(texture1, texture1); glVertex3f(-width, height, length);
	glTexCoord2d(texture0, texture1); glVertex3f(-width, height, -length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, -1.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(width, -height, length);
	glTexCoord2d(texture1, texture0); glVertex3f(width, -height, -length);
	glTexCoord2d(texture1, texture1); glVertex3f(width, height, -length);
	glTexCoord2d(texture0, texture1); glVertex3f(width, height, length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, _cielo);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, -1.0f);
	glTexCoord2d(texture0, texture0); glVertex3f(width, -height, length);
	glTexCoord2d(texture1, texture0); glVertex3f(width, -height, -length);
	glTexCoord2d(texture1, texture1); glVertex3f(width, height, -length);
	glTexCoord2d(texture0, texture1); glVertex3f(width, height, length);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void reiniciarPosicion() {
	gallina.posicionInicial.x = gallina.posicion.x = -1.8;
	gallina.posicionInicial.y = gallina.posicion.y = 0;
	gallina.posicionInicial.z = gallina.posicion.z = 0;
}

void retrocedeTantitoDer() {
	gallina.posicion.x = gallina.posicionInicial.x + 0;
	gallina.posicion.y = gallina.posicionInicial.y + 0;
	gallina.posicion.z = gallina.posicionInicial.z - 0.3;
}

void retrocedeTantitoIzq() {
	gallina.posicion.x = gallina.posicion.x + 0;
	gallina.posicion.y = gallina.posicion.y + 0;
	gallina.posicion.z = gallina.posicion.z + 0.3;
}

void retrocedeTantitoAtr() {
	gallina.posicion.x = gallina.posicion.x + 0.2;
	gallina.posicion.y = gallina.posicion.y + 0;
	gallina.posicion.z = gallina.posicion.z + 0;
}

void revivir() {
	reiniciarPosicion();
}

//Revisar colisión
bool revisarColision() {

	for (int i = 0; i < 8; i++) {
		if ((gallina.posicion.x - autos[i].posicion.x) * (gallina.posicion.x - autos[i].posicion.x)
			+ (gallina.posicion.z - autos[i].posicion.z) * (gallina.posicion.z - autos[i].posicion.z) <= 0.10 * 0.2) { //era .25 .25
			autos[i].haChocado = true;
			return true;
		}
		else {
			autos[i].haChocado = false;
		}
	}
	return false;
}

void dibujaTerreno(float a, float b, float separacion, GLfloat color[]) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(a, separacion, -2);
	glNormal3f(0, 1, 0);
	glVertex3f(a, separacion, 2);
	glNormal3f(0, 1, 0);
	glVertex3f(b, separacion, 2);
	glNormal3f(0, 1, 0);
	glVertex3f(b, separacion, -2);
	glEnd();
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
	camara.deltaX = camara.anteriorX - x;
	camara.deltaY = camara.anteriorY - y;

	camara.anteriorX = x;
	camara.anteriorY = y;

	if (variables.btnDerecho == 1)
		camara.zoom -= camara.deltaY / 50.0;
	else {
		camara.anguloX -= camara.deltaX;
		camara.anguloY -= camara.deltaY;
		camara.anguloZ -= camara.deltaY;
	}

	glutPostRedisplay();
}

void movimientoRatonBtn(int boton, int objeto, int x, int y) {
	if (boton == GLUT_LEFT_BUTTON) {
		variables.btnDerecho = 0;
	}
	if (boton == GLUT_RIGHT_BUTTON) {
		variables.btnDerecho = 1;
	}
	camara.anteriorX = x;
	camara.anteriorY = y;
	glutPostRedisplay();
}

//Dibujar la gallina
void dibujarGallina() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blanco);
	glPushMatrix();
	glTranslatef(gallina.posicion.x, gallina.posicion.y + 0.055, gallina.posicion.z); //0.055 es ajuste al suelo para que las patas no estén abajo del mapa, falta arreglar
	glScalef(0.01, 0.01, 0.01);

	glRotatef(variables.rotacion * 180 / M_PI - 90, 0, 1, 0); //Rotación inicial de la gallina (no se grafica en la dirección correcta xd)
	
	dibujaCubo(2, 5.0, 2.2);
	dibujaCubo(3, 2.0, 3.2);

	//Dibuja las alas del pollito	
	glPushMatrix();
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
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, naranja);
	glPushMatrix();
	glTranslatef(2.0, 3, 0.0);
	dibujaCubo(2.0, 0.5, 1.0);
	glPopMatrix();

	//Dibuja Las patas del pollito
	glPushMatrix();
	glTranslatef(0.0, -5.0, 0.0);
	dibujaCubo(3.0, 0.4, 1.0);
	glPopMatrix();

	//Dibuja los ojos del pollito
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, negro);
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

	//Dibuja la cresta del pollito	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rojo);
	glPushMatrix();
	glTranslatef(0.0, 5, 0.0);
	glRotatef(10, 0, 0, 1);
	dibujaCubo(1.50, 1, 1.0); 
	glPopMatrix();
	
	glPopMatrix();
	
}

void dibujaLlantas() {

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, negro);
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
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, azul);
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

//Recalcula el salto de la gallina, dependiendo la dirección en la que voltea
void calculaSalto() {
	gallina.velocidad.x = sin(variables.rotacion) * cos(variables.angulo) * variables.velocidad;
	gallina.velocidad.y = sin(variables.angulo) * variables.velocidad;
	gallina.velocidad.z = cos(variables.angulo) * cos(variables.rotacion)* variables.velocidad;
}

void setPosicion() {
	gallina.ultimoMov = variables.tiempo;
	gallina.posicion.y = 0;
	gallina.posicionInicial.x = gallina.posicion.x;
	gallina.posicionInicial.y = gallina.posicion.y;
	gallina.posicionInicial.z = gallina.posicion.z;
}

//Crea el carro en el carril i
void iniciaAutomovil(int i) {
	if (i > 3) {
		autos[i].posicion.x = -0.8 + 0.5 * i;
	}
	else
	{
		autos[i].posicion.x = -1.2 + 0.5 * i;
	}
	
}

void actualizaPosGallina(float dt)
{
	// Posicion
	gallina.posicion.x += gallina.velocidad.x * dt;
	gallina.posicion.y += gallina.velocidad.y * dt;
	gallina.posicion.z += gallina.velocidad.z * dt;

	// Velocidad
	gallina.velocidad.y += -12 * dt;
}

void verificaColision(float dt) {
	gallina.haChocado = revisarColision();

	if (gallina.haChocado) {
		gallina.haChocado = false;
		printf("Te atropellaron, intenta nuevamente...\n");
		printf("=====================\n");
		revivir();
	}
}

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
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2i(w / 2 - 20, h - 50);
	snprintf(buffer, sizeof buffer, "PUNTOS: %d", variables.puntuacion);
	for (bufp = buffer; *bufp; bufp++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *bufp);

	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);


	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);


	glPopAttrib();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//transformaciones camara
	glTranslatef(0, 0, camara.zoom);
	glRotatef(camara.anguloY, 1.0, 0.0, 0.0);
	glRotatef(camara.anguloX, 0.0, 1.0, 0.0); //rotacion interactiva
	glRotatef(camara.anguloZ, 0.0, 0.0, 1.0);
	glTranslatef(-gallina.posicion.x, -gallina.posicion.y, -gallina.posicion.z); //camara sigue gallina

	glDisable(GL_LIGHTING);
	//Cielo
	dibujaCielo();
	displayOSD();
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1); 
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glEnable(GL_LIGHT1);
	
	//carretera 1
	dibujaTerreno(-1.5, 0.5, 0.001, gris);

	//carretera 2
	dibujaTerreno(1, 3, 0.001, gris);

	dibujarGallina();

	//pasto
	dibujaTerreno(-2, 4, 0, verde);

	//autos
	for (int i = 0; i < 8; i++) {
		glPushMatrix();
		glTranslatef(autos[i].posicion.x, 0, autos[i].posicion.z);
		dibujaCarro();
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void update(void) {

	float t, dt; //tiempo y diferenciaTiempo

	t = glutGet(GLUT_ELAPSED_TIME) / (float)1000; //Obtiene el tiempo que ha transcurrido

	dt = t - autos[0].ultimoMov; //Obtiene el tiempo transcurrido desde el último movimiento de autos

	for (int i = 0; i < 8; i++) {
		autos[i].posicion.z += dt * autos[i].velocidad.x;//Calcula la nueva posición (tiempo transcurrido desde el último movimiento * velocidad de movimiento)
		if (autos[i].posicion.z > 2) { //Si el auto llega al final de la carretera
			autos[i].posicion.z = -2; //Lo regresa al principio
			iniciaAutomovil(i);
			autos[i].velocidad.x = rand() / (RAND_MAX / 3)+1; //Establece la velocidad en X de los autos
		}
	}

	verificaColision(dt);

	//Revisa si el jugador llegó a la meta
	if (gallina.posicion.x > 3.5) {
		printf("Llegaste a la meta!\n");
		printf("Puntos: %d\n",variables.puntuacion+1);
		printf("=====================\n");
		variables.puntuacion++;
		reiniciarPosicion();
	}

	//Revisa si el jugador llegó al limite por la derecha
	if (gallina.posicion.z > 2.0) {
		printf("Llegaste al limite por la derecha!\n");
		printf("=====================\n");
		retrocedeTantitoDer();
	}

	//Revisa si el jugador llegó al limite por la izquierda
	if (gallina.posicion.z < -2.0) {
		printf("Llegaste al limite por la izquierda!\n");
		printf("=====================\n");
		retrocedeTantitoIzq();
	}

	//Revisa si el jugador llegó al limite por detrás
	if (gallina.posicion.x < -2.0) {
		printf("Llegaste al limite por detras!\n");
		printf("=====================\n");
		retrocedeTantitoAtr();
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
	autos[0].ultimoMov = variables.tiempo;
	glEnable(GL_NORMALIZE);

	gallina.posicionInicial.x = -1.8;
	gallina.posicion.x = -1.8;

	calculaSalto();
	srand(time(NULL));
	for (int i = 0; i < 8; i++) {
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
			variables.tiempo = glutGet(GLUT_ELAPSED_TIME) / (float)1000;
			calculaSalto();
			gallina.posicion.y = 0;
			variables.saltando = true;
			setPosicion();
			calculaSalto();
		}
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
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(300, 100);
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