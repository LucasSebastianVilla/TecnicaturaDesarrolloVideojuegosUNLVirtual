#include <iostream> // cout
#include <cstdlib> // exit
#include <cmath> // fabs
#include <GL/glut.h>
#include "Teclado.h"
#include "uglyfont.h"
#include <string>

/**
-----------------------------------------------
-----Juego Top Gun ::: Trabajo Practico II-----
-----------------------------------------------
Alumno: Lucas Sebastian Villa
Profesor: Walter Sotil - Emmanuel Rojas Fredini
Materia: Manipulacion de Objetos en 2D
Año:2021
**/


using namespace std;

// variables globales
int
  w=800,h=600; // tamanio inicial de la ventana

double 
  AvionX=400, 
  AvionY=300, 
  AvionAng=0, 
  ArmaAng=0,
  ArmaTamanio=0;

const double PI=4*atan(1.0);
bool cl_info=true; // informa por la linea de comandos
//teclas simultaneas
double vel=0, ang=0;//velocidad del "personaje"
Teclado teclado(' ', 'w', 's', 'a', 'd', 'o', 'p'); // para manejar la entrada por teclado
//============================================================
// Renderiza texto en pantalla usando fuentes raster
void print_text(string cadena, int x, int y, int espacio=5){
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glColor4f(0.1,0.9,0.1,0.5);
  for(unsigned int i=0; i < cadena.size(); i++) {
    glRasterPos2i(x + i*espacio, y);// espaciado en x entre cada caracter
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)cadena[i]);
  }
  glPopAttrib();
}
  
void DibujarCabina() {
  glColor3d(0.3,0.1,0.3);
  glBegin( GL_TRIANGLE_FAN );
  glVertex2d(0,	0);
  for(double r=0; r<PI*2; r+=0.1)
    glVertex2d(cos(r), sin(r));
  glVertex2d(1.0,0.0);
  glEnd();
}

void DibujarCuerpo() {
  glColor3d(0.9,0.1,0.9);
  glBegin( GL_TRIANGLE_FAN );
  glVertex2d(0.0,	0.0);
  glVertex2d(0.0,	70.0);
  glVertex2d(-8, 35.0);
  glVertex2d(-10,	-30.0);
  glVertex2d(0.0,	-15.0);
  glVertex2d(10, -30.0);
  glVertex2d(8,	35.0);
  glVertex2d(0.0,	70.0);
  glEnd();
}

void DibujarMetralleta() {//Dibujo la metralleta sobre el ala
  glColor3d(0.9,0.1,0.1);
  glBegin( GL_TRIANGLE_FAN );
  glVertex2d(47.0, 0.0);
  glVertex2d(52.0, 0.0);
  glVertex2d(52.0, 20.0);
  glVertex2d(47.0, 20.0);
  glVertex2d(47.0, 0.0);
  glEnd();
}

void DibujarMotor() {//Dibujo la metralleta sobre el ala
  glColor3d(0.7,0.7,0.3);
  glBegin( GL_TRIANGLE_FAN );
  glVertex2d(-10.0, -15.0);
  glVertex2d(10.0, -15.0);
  glVertex2d(10.0, -30.0);
  glVertex2d(0.0, -35.0);
  glVertex2d(-10.0, -30.0);
  glVertex2d(-10.0, -15.0);
  glEnd();
}


void DibujarAla() {
  glColor3d(0.1,0.9,0.1);
  glBegin( GL_TRIANGLE_FAN );
  glVertex2d(35,10);
  glVertex2d(0.0,20.0); 
  glVertex2d(0.0,0.0); 
  glVertex2d(35,4);
  glVertex2d(50.0,0.0);
  glEnd();
}

void DibujarAvion() {
  glPushMatrix();// inicio push

  // Posiciona y rota el Avion en el modelo
  glTranslated(AvionX,AvionY,0);
  glRotated(AvionAng,0,0,1);  
  
  //Dibujamos las distintas partes de la nave, aplicando las transformaciones necesarias

  //Motor derecho
  glPushMatrix();
  DibujarMotor();
  glPopMatrix();

  
  //Ala derecha
  glPushMatrix();
  DibujarAla();
  glPopMatrix();
  
  //Metralleta derecha
  glPushMatrix();
  DibujarMetralleta();
  glPopMatrix();
  
  //Ala izquierda
  glPushMatrix();
  glScaled(-1,1,1); //Con este escalamiento logramos reflejar (x = -AnchoAla * x)  
  DibujarAla();
  glPopMatrix();

  //Metralleta izquierda
  glPushMatrix();
  glScaled(-1,1,1); //Con este escalamiento logramos reflejar (x = -AnchoAla * x)  
  DibujarMetralleta();
  glPopMatrix();
  
  //Cuerpo
  DibujarCuerpo();
  
  //Cabina
  glPushMatrix();
  glScaled(6,12,1);
  DibujarCabina();
  glPopMatrix();
  
  
  // Luego de aplicar la traslacion (AvionX,AvionY) y la rotacion (AvionAng) inicial 
  // dibuja un punto en la posición 0,0 (es solo para ayuda)
  glColor3f(0.0f,0.0f,0.0f);
  glPointSize(5.0);
  glBegin(GL_POINTS);
  glVertex2d(0.0,0.0);
  glEnd();
  
  
  glPopMatrix();// fin push1
}

void DibujarPared() {
  glColor3f(0.9f,0.9f,0.9f);
  glLineWidth(5.0);
  glBegin(GL_LINES);
  glVertex2i(325,400); glVertex2i(325,200);
  glVertex2i(325,200); glVertex2i(475,200);
  glVertex2i(475,200); glVertex2i(475,400);
  glEnd();
}

//============================================================
// callbacks

//------------------------------------------------------------

// arma un un nuevo buffer (back) y reemplaza el framebuffer
void Display_cb() {
  static int i=10, j=470;
  // arma el back-buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);// rellena con color de fondo
  print_text("Top Gun MOD2D-TDVJ", i, j, 17);
  if(i>=640){ 
    i=10;
    if(j<=10) j=470;
    else j-=10;
  }
  else i+=1;
  DibujarPared();
  DibujarAvion();
  glutSwapBuffers(); // lo manda al monitor
  
  // chequea errores
  int errornum=glGetError();
  while(errornum!=GL_NO_ERROR){
    if (cl_info){
      if(errornum==GL_INVALID_ENUM)
        cout << "GL_INVALID_ENUM" << endl;
      else if(errornum==GL_INVALID_VALUE)
        cout << "GL_INVALID_VALUE" << endl;
      else if (errornum==GL_INVALID_OPERATION)
        cout << "GL_INVALID_OPERATION" << endl;
      else if (errornum==GL_STACK_OVERFLOW)
        cout << "GL_STACK_OVERFLOW" << endl;
      else if (errornum==GL_STACK_UNDERFLOW)
        cout << "GL_STACK_UNDERFLOW" << endl;
      else if (errornum==GL_OUT_OF_MEMORY)
        cout << "GL_OUT_OF_MEMORY" << endl;
    }
    errornum=glGetError();
  }
}

//------------------------------------------------------------
// Maneja cambios de ancho y alto de la ventana
void Reshape_cb(int width, int height){
  if (!width||!height) return; // minimizado ==> nada
  w=width; h=height;
  glViewport(0,0,w,h); // región donde se dibuja (toda la ventana)
  // rehace la matriz de proyección (la porcion de espacio visible)
  glMatrixMode(GL_PROJECTION);  glLoadIdentity();
  glOrtho(0,w,0,h,1,3); // unidades = pixeles
  // las operaciones subsiguientes se aplican a la matriz de modelado GL_MODELVIEW
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0,0,2, 0,0,0, 0,1,0);// ubica la camara
  glutPostRedisplay(); // avisa que se debe redibujar
}

//-----------------------------------------------------------

// Special keys (non-ASCII)
// teclas de funcion, flechas, page up/dn, home/end, insert
void Special_cb(int key,int xm=0,int ym=0) {
  if (key==GLUT_KEY_F4 && glutGetModifiers()==GLUT_ACTIVE_ALT) // alt+f4 => exit
    exit(EXIT_SUCCESS);
}

//------------------------------------------------------------
void idle_cb() {
  static unsigned int lt=0;
  //double ang=AvionAng*PI/180.0;
  int dt = glutGet(GLUT_ELAPSED_TIME) - lt;
  if(dt > 60 ) {
    // Primero miramos el estado del teclado y realizamos los cambios en la direccion y la velocidad del 'personaje'
    double ang=AvionAng*PI/180;
    if( teclado.Salir() )
      exit( EXIT_SUCCESS );
    if( teclado.Adelante() )
      vel -= 2;
    if( teclado.Atras() )
      vel += 2;
    if( teclado.Izquierda() )
      AvionAng += 2;
    if( teclado.Derecha() )
      AvionAng -= 2;
    
    // Actualizamos la posicion del 'personaje'
    AvionX += vel*sin(ang);
    AvionY -= vel*cos(ang);
    // Disminuimos la velocidad 
    vel *= 0.9;
    
    // controlamos que no salga de la pantalla
    if( AvionX < 0) AvionX = 0;
    if( AvionX > w) AvionX = w;
    if( AvionY < 0) AvionY = 0;
    if(AvionY > h) AvionY = h;
    
    if(dt > 60) {
      lt = glutGet(GLUT_ELAPSED_TIME);
      Display_cb();
    }
    glutPostRedisplay();
  }
}

void inicializa() {
  // GLUT
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);// pide color RGB y double buffering
  glutInitWindowSize(w,h); glutInitWindowPosition(10,10);
  glutCreateWindow("Top Gun - Villa, Lucas Sebastian"); // crea el main window

  // declara los callbacks, los que (aun) no se usan (aun) no se declaran
  glutDisplayFunc(Display_cb);
  glutReshapeFunc(Reshape_cb);

  glutSpecialFunc(Special_cb);
  glutIdleFunc(idle_cb);

  //teclas simultaneas
  teclado.Iniciar();
  
  // OpenGL
  glClearColor(0.01f,0.01f,0.01f,1.f);// color de fondo
  
  // Activo el zbuffer
  glDepthFunc( GL_LEQUAL ); 
  
  // las operaciones subsiguientes se aplican a la matriz de modelado GL_MODELVIEW
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

//------------------------------------------------------------
// main
int main(int argc,char** argv) {
  // teclas a utilizar
  cout << "Teclas a utilizar:" << endl;
  cout << "w: avanza" << endl;
  cout << "s: retrocede" << endl;
  cout << "d: gira en sentido horario" << endl;
  cout << "a: gira en sentido antihorario" << endl;
  
  glutInit(&argc,argv); // inicialización interna de GLUT
  inicializa(); // define el estado inicial de GLUT y OpenGL
  glutMainLoop(); // entra en loop de reconocimiento de eventos
  return 0; // nunca se llega acá, es sólo para evitar un warning
}
