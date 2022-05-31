
#include <GLUT/GLUT.h>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
/*Esteblece las configuraciones del juego*/
typedef struct {	
	bool go;
	float startTime;
	int frames;
	float frameRate;
	float frameRateInterval;
	float lastFrameRateT;
	float angle;
	float speed;
	float rotation;
	int circleNum;
	bool normalsOn;
	bool rightbtn;
	bool lightsOn;
	bool wireframe;
	int logColided;
	int lives;
	int score;
	bool died;
	bool won;
	bool gameOver;
} configuraciones;

configuraciones global = {false, 0, 0, 0, 0.2, 0, M_PI / 4, 2.0, M_PI / 2, 16, false, false, true, false, -1, 5, 0, 0, 0 };
 
void updateVelocity() {
	projectile.v.x = sin(global.rotation) * cos(global.angle) * global.speed;
	projectile.v.y = sin(global.angle) * global.speed;
	projectile.v.z = cos(global.angle) * cos(global.rotation)* global.speed;
}
//Eventos del teclado
void keyboardCB(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
	case 'q':
		exit(EXIT_SUCCESS);
		break;	
	case 'l':
		global.lightsOn = !global.lightsOn;
		break;	
	case ' ':
		if (!global.go && !global.gameOver) {
			printf("************************El pollo salto***********************************\n"); //Accion para el salto del pollo
			global.startTime = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
			printf("start time: %f\n", global.startTime);
			updateVelocity();
			projectile.position.y = 0; 
			global.go = true;
			setPosition();
			updateVelocity();
		}
		break;
	case 13: // presiona enter para reiniciar
		global.gameOver = false;
		break;
	/*	
	case 'n':
		global.normalsOn = !global.normalsOn;
		break;*/
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(500, 500);
	glutCreateWindow("CrossyRoad: Equipo graficaci�n");
	glutKeyboardFunc(keyboardCB); //Establece los eventos del teclado
	glutSpecialFunc(handleSpecialKeypress);

	glutMotionFunc(mouseMotion);//
	glutMouseFunc(mouseMotionBtn);//Establece del mouse
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display);//Dibuja en la pantalla
	glutIdleFunc(update);

	myinit();

	glutMainLoop();

	return 0;
}