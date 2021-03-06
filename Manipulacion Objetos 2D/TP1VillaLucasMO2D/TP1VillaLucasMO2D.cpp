#include <iostream> // cout
#include <cstdlib> // exit
#include <cmath> // fabs
#include <GL/glut.h>

using namespace std;

//------------------------------------------------------------
// variables globales
int
  w=480,h=360, // tama?o inicial de la ventana
  npuntos=0, // cantidad de puntos
  pt[6], // los puntos (hasta 3: x0,y0,x1,y1,x2,y2)
  ep=-1; // ?ndice del punto a editar

//============================================================
// callbacks

//------------------------------------------------------------

// arma un un nuevo buffer (back) y reemplaza el framebuffer
void Display_cb() {
  //static int counter=0; cout << "display: " << counter++ << endl;

  // arma el back-buffer
  glClear(GL_COLOR_BUFFER_BIT);// rellena con color de fondo
  /**Dibujo borde punteado en el triangulo**/
  glEnable(GL_LINE_STIPPLE);   
  GLint factor=1;   
  GLushort pattern=0x5555;   
  glLineStipple(factor,pattern);

  if (!npuntos){ // no hay nada
    glutSwapBuffers(); // manda al monitor la pantalla vac?a
    return;
  }

  // dibuja
  int i;
  // tri?ngulo (s?lo si ya est?n los tres puntos)
  if (npuntos==3) {
    glColor3f(.8f,.1f,.5f);
    glBegin(GL_TRIANGLES);
      for(i=0;i<npuntos;i++) glVertex2i(pt[2*i],pt[2*i+1]);
    glEnd();
	
	/** **/
	glLineWidth(3);
	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < 3; j++) { glVertex2i(pt[2 * j], pt[2 * j + 1]); }
	glEnd();
  }
  
  glDisable(GL_LINE_STIPPLE);
  
  // puntos (despu?s del tri?ngulo, para que se vean encima)
  glColor3f(.1f,.2f,.3f);
  glPointSize(5.0); // punto grueso
  glBegin(GL_POINTS);
    for(i=0;i<npuntos;i++) glVertex2i(pt[2*i],pt[2*i+1]);
  glEnd();

  glutSwapBuffers(); // lo manda al monitor
}

//------------------------------------------------------------
// Maneja cambios de ancho y alto de la ventana
void Reshape_cb(int width, int height){
  if (!width||!height) return; // minimizado ==> nada
  w=width; h=height;
  glViewport(0,0,w,h); // regi?n donde se dibuja (toda la ventana)
  // rehace la matriz de proyecci?n (la porcion de espacio visible)
  glMatrixMode(GL_PROJECTION);  glLoadIdentity();
  glOrtho(0,w,0,h,-1,1); // unidades = pixeles
  glutPostRedisplay(); // avisa que se debe redibujar
}

//------------------------------------------------------------
// Mouse

// Seguimiento del cursor
void PassiveMotion_cb(int x, int y){
  y=h-y; // el 0 est? arriba
  cout << x << "," << y << "                    \r" << flush;
}

// Drag (movimiento con algun boton apretado)
void Motion_cb(int x, int y){
  y=h-y; // el 0 est? arriba

  pt[2*ep]=x;pt[2*ep+1]=y; // fija el punto editado en x,y
  /**Evito que el punto escape de la pantalla al ser arrastrado**/
  if((x<w && x>0)&&(y<h && y>0)){
	  glutPostRedisplay(); // avisa que se debe redibujar
  }
  
}

// Botones picados o soltados
void Mouse_cb(int button, int state, int x, int y){
  
  cout << x << "," << y << "," << button << endl;
  if (button==GLUT_LEFT_BUTTON){ // boton izquierdo
    if (state==GLUT_DOWN) { // clickeado
      y=h-y; // el 0 est? arriba
      ep=-1;
      // verifica si pic? a menos de 5 pixeles de alg?n punto previo
      for (int i=0;i<npuntos;i++){
        /**Metrica del rombo**/
		int d = abs(x - pt[2 * i]) + abs(y - pt[2 * i + 1]);
		if (d>5) continue; // lejos
        ep=i; // edita el punto i
        glutMotionFunc(Motion_cb); // define el callback para los drags
        return;
      }
      // no pico cerca de otro
      if (npuntos==3) return; // ya hay 3 ==> no hace nada
      // agrega un punto
      pt[2*npuntos]=x; pt[2*npuntos+1]=y; npuntos++;
      Display_cb(); // Redibuja
      // y queda listo para editarlo hasta que suelte el bot?n izquierdo
      ep=npuntos-1; // edita el ultimo punto
      glutMotionFunc(Motion_cb); // define el callback para los drags
    } // fin clickeado
    else if (state==GLUT_UP) // soltado
      glutMotionFunc(0); // anula el callback para los drags
  } // fin bot?n izquierdo
}

//------------------------------------------------------------
// Teclado

// Maneja pulsaciones del teclado (ASCII keys)
// x,y posicion del mouse cuando se teclea (aqui no importan)
void Keyboard_cb(unsigned char key,int x,int y) {
  if (key==127||key==8){ // DEL o Backspace ==> borra el punto ep (el editable)
    if (ep==-1||ep==npuntos) return;
    // corre los siguientes hacia atras
    for(int i=ep;i<npuntos-1;i++) {pt[2*i]=pt[2*i+2];pt[2*i+1]=pt[2*i+3];}
    npuntos--;
    glutPostRedisplay(); // avisa que se debe redibujar
    // Backspace borra para atras, DEL borra para adelante
    if (key==8) ep--; // ep pasa a ser el anterior
  }
  if (key==27){
    npuntos=0;
    glutPostRedisplay();
  }
}

// Special keys (non-ASCII)
// teclas de funcion, flechas, page up/dn, home/end, insert
void Special_cb(int key,int xm=0,int ym=0) {
  if (key==GLUT_KEY_F4 && glutGetModifiers()==GLUT_ACTIVE_ALT) // alt+f4 => exit
    exit(EXIT_SUCCESS);
}

//------------------------------------------------------------
void inicializa() {
  // GLUT
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);// pide color RGB y double buffering
  glutInitWindowSize(w,h); glutInitWindowPosition(50,50);
  glutCreateWindow("Ejemplo 4"); // crea el main window

  //declara los callbacks, los que (aun) no se usan (aun) no se declaran
  glutDisplayFunc(Display_cb);
  glutReshapeFunc(Reshape_cb);
  glutKeyboardFunc(Keyboard_cb);
  glutSpecialFunc(Special_cb);
  glutMouseFunc(Mouse_cb);

  // OpenGL
  glClearColor(0.85f,0.9f,0.95f,1.f); // color de fondo
  glMatrixMode(GL_MODELVIEW); glLoadIdentity(); // constante
}

//------------------------------------------------------------
// main
int main(int argc,char** argv) {
  glutInit(&argc,argv); // inicializaci?n interna de GLUT
  inicializa(); // define el estado inicial de GLUT y OpenGL
  glutMainLoop(); // entra en loop de reconocimiento de eventos
  return 0; // nunca se llega ac?, es s?lo para evitar un warning
}
