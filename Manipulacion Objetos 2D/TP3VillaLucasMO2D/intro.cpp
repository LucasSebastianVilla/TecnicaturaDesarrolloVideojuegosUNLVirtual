#include <iostream> // cout
#include <cstdlib> // exit
#include <cmath> // fabs
#include <GL/glut.h>
#include "Teclado.h"
#include "uglyfont.h"
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

// variables globales
int 
  w=1000,//ancho de la ventana
  h=800,//alto de la ventana
  condicion=0, //informa si gano o perdio - 0:continua, 1:gano, 2:perdio
  tiempo,//tiempo desde el inicio
  milisegundos,//variable para tomar el tiempo
  EnemigoX=50,//posicion inicial del enemigo en x
  EnemigoY=50,//posicion inicial del enemigo en y
  Energia=100;//vida inicial del enemigo

double 
  AvionX=500, 
  AvionY=400, 
  AvionAng=0, 
  ArmaAng=0,
  ArmaTamanio=0;

const double PI=4*atan(1.0);
bool
  turbina=true,//muestro la turbina del avion encendida
  cl_info=true;//informa por la linea de comandos

static int AngLineaRadar=0;//angulo de rotacion de linea del radar

//teclas simultaneas
double 
  vel=0, 
  ang=0;//velocidad del avion

Teclado teclado(' ', 'w', 's', 'a', 'd', 'q', 'p'); //manejar la entrada por teclado
GLuint texid[3];
/**============================================================*/
// Textura 1D
#define stripeImageWidth 32
GLubyte stripeImage[stripeImageWidth][4];
void makeStripeImage(void){
  int j, rojo,verde,azul,alpha;
  for (j = 0; j < stripeImageWidth; j++){
    alpha=255;
    if(j<16) {rojo=64;verde=32;azul=0;}else{rojo=32;verde=16;azul=0;}
    stripeImage[j][0] = (GLubyte) rojo;
    stripeImage[j][1] = (GLubyte) verde;
    stripeImage[j][2] = (GLubyte) azul;
    stripeImage[j][3] = (GLubyte) alpha;
  }
}
// Renderiza texto en pantalla usando fuentes raster
void print_text(string cadena, int x, int y, int espacio=5, float r=1.0, float g=1.0, float b=1.0, float a=1.0){
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glColor4f(r,g,b,a);//color de la fuente
  for(unsigned int i=0; i < cadena.size(); i++) {
    glRasterPos2i(x + i*espacio, y);// espaciado en x entre cada caracter
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)cadena[i]);
  }
  glPopAttrib();
}

// Carga una imagen en formato ppm, crea el canal alpha (ya que las imagenes ppm 
// no guardan este canal) , crea los mipmaps y deja la textura lista para usar en nuestro programa
bool mipmap_ppm(const char *ifile){
  // Se declaran algunas variables para usar despues y se abre el archivo que contiene la imagen
  char dummy; int maxc,wt,ht;
  ifstream fileinput(ifile, ios::binary);
  // Si no se pudo abrir el archivo se escribe por consola que hubo un error y se anula la operacion
  if (!fileinput.is_open()) {cerr<<"El archivo no se encuentra"<<endl; return false;}
  fileinput.get(dummy);
  // Se leen dos caracteres llamados "números mágicos" que indican que el archivo es una 
  // imagen en formato ppm. Estos caracteres son una 'P' seguida de un '6'
  // Si no se encuentran estos caracteres significa que el archivo no es una 
  // imagen ppm por lo que se anula el proceso
  if (dummy!='P') {cerr<<"No es un archivo P6 PPM"<<endl; return false;}
  fileinput.get(dummy);
  if (dummy!='6') {cerr<<"No es un archivo P6 PPM"<<endl; return false;}  
  // Se leen algunos caracteres de relleno, que no tienen ningun significado util, se leen solo 
  // para saltearlos. Primero se lee un espacio, luego, el metodo peek lee un caracter sin 
  // adelantar la posicion, si este es un '#' se continua leyendo hasta que se encuentra 
  // un salto de linea (un enter)  
  fileinput.get(dummy);
  dummy=fileinput.peek();
  if (dummy=='#') do {
    fileinput.get(dummy);
  } while (dummy!=10);
  // Se lee el ancho y alto de la imagen, y el ultimo indica la cantidad de colores de la imagen
  fileinput >> wt >> ht;
  fileinput >> maxc;
  // Se saltea un espacio en blanco
  fileinput.get(dummy);  
  // Se reserva memoria para wt*xt pixels. Como tenemos un byte para cada canal (rgb), 
  // son 3 bytes por pixel. Por ultimo se cierra en archivo    
  unsigned char *img=new unsigned char[3*wt*ht];
  fileinput.read((char *)img, 3*wt*ht);
  fileinput.close();
  // gluBuild2DMipmaps(GL_TEXTURE_2D, 3, wt, ht,  GL_RGB, GL_UNSIGNED_BYTE, img);
  // Conversion a rgba alpha=255-(r+g+b)/3 (blanco=transparente, negro=opaco)
  // Las imagenes en formato ppm no pueden guardar el canal alpha, por lo que lo creamos 
  // a partir de los canales rgb. Primero reservamos memoria para wt*ht pixels, esta vez 
  // con 4 bytes por pixel (rgba)
  // Recorremos la imagen leida del archivo, copiamos los canales r, g y b. El canal 
  // alpha lo definimos como trasparente si el color es blanco (si r=g=b=255, 
  // entonces r+g+b=765). Sino alpha es completamente opaco (=255)
  unsigned char *imga=new unsigned char[4*wt*ht];
  unsigned char r,g,b;
  for (int i=0;i<wt*ht;i++){
    r=imga[4*i+0]=img[3*i+0];
    g=imga[4*i+1]=img[3*i+1];
    b=imga[4*i+2]=img[3*i+2];
    imga[4*i+3]=((r+g+b==765)? 0: 255);
  }
  // Por ultimo le pedimos a glut que cree un mipmap a partir de la imagen rgba y 
  // liberamos la memoria reservada. Al creer el mipmap la textura queda lista para usarse
  // Cualquier primitiva de dibujo que aparezca a continuacion, si esta habilitada la 
  // generacion de texturas 2D (con glEnable(GL_TEXTURE_2D)) hara uso de esta textura
  delete[] img;
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, wt, ht,  GL_RGBA, GL_UNSIGNED_BYTE, imga);
  delete[] imga;
  return true;
}
  class Bala {
  private:
    double x;
    double y;
    double incrementox;
    double incrementoy;
  public:
    Bala(double posX, double posY, double incX, double incY) : x(posX), y(posY), incrementox(incX), incrementoy(incY) {}
    bool Update() {
      x+=incrementox;
      y+=incrementoy;
      //Me fijo si hay colision de la bala con una torre enemiga
          if( fabs(x-EnemigoX)+fabs(y-EnemigoY) < 50 ) {
            Energia-=10;
            cout << "Energia del enemigo: " << Energia << endl;
      return true;
    }
    //Si esta fuera de la pantalla, elimino la bala
    return ( x > w || x < 0 || y > h || y < 0 );
  }
  void Draw() {
    glVertex2d(x,y);
  }
};
list<Bala> proyectil;
  
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
void DibujarFuegoTur() {
  glColor3d(0.9,0.5,0.0);
  glBegin( GL_TRIANGLE_FAN );
  glVertex2d(-33,4.0);
  glVertex2d(-25.0,-30.0); 
  glVertex2d(-19.0,4.0); 
  glEnd();
}


void DibujarAvion(bool activaTurbina) {
  glPushMatrix();// inicio push

  // Posiciona y rota el Avion en el modelo
  glTranslated(AvionX,AvionY,0);
  glRotated(AvionAng,0,0,1);  
  
  //Dibujamos las distintas partes de la nave, aplicando las transformaciones necesarias

  //Motor derecho
  glPushMatrix();
  DibujarMotor();
  glPopMatrix();

  if(activaTurbina){//si esta verdadero muestra la turbina, sino no
    //fuego turbina derecha
    glPushMatrix();
    DibujarFuegoTur();
    glPopMatrix();
    
    //fuego turbina izq
    glPushMatrix();
    glScaled(-1,1,1); //Con este escalamiento logramos reflejar (x = -AnchoAla * x)  
    DibujarFuegoTur();
    glPopMatrix();  
  }
  
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
  glVertex2i(425,500); glVertex2i(425,300);
  glVertex2i(425,300); glVertex2i(575,300);
  glVertex2i(575,300); glVertex2i(575,500);
  glEnd();
}
void DibujarPiso(){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texid[0]); //selecciono la textura a utilizar
  glBegin(GL_QUADS);//
  glTexCoord2f(0, 0); glVertex2i(0,0); 
  glTexCoord2f(3, 0); glVertex2i(1000,0);
  glTexCoord2f(3, 3); glVertex2i(1000,800);
  glTexCoord2f(0, 3); glVertex2i(0,800);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
void DibujarRadar() {  
  glPushMatrix();
  glTranslatef (900, 700, 0.0);//posicion del cirulo
  
  //circulo contorno
  glColor4f(0.8f,0.2f,0.6f,0.7f);//color
  glPointSize(1);
  GLUquadricObj *q=gluNewQuadric();
  gluQuadricDrawStyle(q,GLU_FILL);//GLU_POINT//GLU_FILL
  gluDisk(q,65,70,30,30);//propiedades de la linea 
  
  //circulo radar
  glColor4f(0,0.1f,0.1f,0.5f);
  gluQuadricDrawStyle(q,GLU_FILL);//GLU_POINT//GLU_FILL
  gluDisk(q,0,70,30,30);//gluDisk(q,0,baseRadius,slices,stacks);  
  gluDeleteQuadric(q);
  
  glPopMatrix();
  
  //linea deteccion radar
  glPushMatrix();
  glTranslatef (900, 700, 0.0);//posicion del lector del radar
  glRotatef (AngLineaRadar, 0.0, 0.0, -1.0);
  glColor3f(1.0f,0.0f,0.0f);//glColor3ub(1,245,0);
  glLineWidth(3);//glLineWidth(1.0);
  glBegin(GL_LINES);
  glVertex2i(0,0); glVertex2i(68,0);//glVertex2f(0.f,0.f); glVertex2f(0.f,50.f);
  glEnd();
  glPopMatrix();
}
void DibujarTecho(){
  // Aplicacion de textura 2D
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texid[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.00,0.67); glColor3f(0,1,0); glVertex3f(425,500,0);
  glTexCoord2f(0.00,0.99); glColor3f(0,1,1); glVertex3f(425,300,0);
  glTexCoord2f(0.25,0.99); glColor3f(1,1,1); glVertex3f(575,300,0);
  glTexCoord2f(0.25,0.67); glColor3f(1,1,0); glVertex3f(575,500,0);  
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
/**============================================================**/
//callbacks
// arma un un nuevo buffer (back) y reemplaza el framebuffer
void Display_cb() {
  switch (condicion){
    case 0:
      {
      int variacionX, variacionY;
      static int i=10, j=470;
      // arma el back-buffer
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);// rellena con color de fondo
      DibujarPiso();//dibujo el piso antes que todo sino tapa al resto de los actores
      
      print_text("Top Gun MOD2D-TDVJ", i, 760, 17, 0.792f, 0.231f, 0.929f, 0.9f);
      
      char st3[20] = "Tiempo Restante: ";
      char st4[10] = "";
      sprintf(st4, "%d", 60-tiempo);
      strcat(st3, st4);
      print_text(st3, 20, 730, 15.0, 0.341f, 0.349f, 0.274f, 0.9f);
      
      //Controlo los mensajes segun el tiempo
      if (tiempo < 10) {print_text("En 60 segundos debes derrotar al enemigo", 20, 670, 15, 0.054f, 0.403f, 0.066f, 0.9f);}
      else if(tiempo > 50) {print_text("Se esta terminando tu tiempo, Rapido!!", 20, 670, 15, 0.721f, 0.039f, 0.066f, 0.9f);}
      
      char st5[20]="Enemigo Defensas: ";
      char st6[10] = "";
      sprintf(st6,"%d", Energia);
      strcat(st5, st6);
      print_text(st5, 20, 700, 15, 0.71f, 0.03f, 0.06f, 0.9f);
      if(i>=640){ 
        i=10;
        if(j<=10) j=470;
        else j-=10;
      }
      else i+=1;
      DibujarRadar();
      DibujarPared();
      DibujarAvion(turbina);
      DibujarTecho();
      
      //Enemigo
      glPushMatrix();
      if(EnemigoX-50<w || EnemigoY-50<h){//Movimiento del enemigo
        variacionX = (rand() % 5) - 1;
        variacionY = (rand() % 5) - 1;
        glTranslated(EnemigoX,EnemigoY, 0 );
        EnemigoY+=variacionY;
        EnemigoX+=variacionX;
        if(EnemigoX>w){EnemigoX=0;}else if (EnemigoX<0){EnemigoX=1000;}
        if(EnemigoY>h){EnemigoY=0;}else if (EnemigoY<0){EnemigoX=800;}
      }
      
      glColor3f(0.9,0.1,0.9);  
      GLdouble baseRadius=25;
      int lod=30;
      GLint slices=lod, stacks=lod;  
      GLUquadricObj *q=gluNewQuadric();
      gluQuadricDrawStyle(q,GLU_FILL);
      gluDisk(q,0,baseRadius,slices,stacks);  
      gluDeleteQuadric(q);  
      glColor3f(0.1,0.9,0.1); 
      glPointSize(4);
      glBegin(GL_POINTS);
      for(double x=0; x<2*PI; x+=0.4) 
        glVertex2d(40.0*0.8*cos(x),40.0*0.8*sin(x));
      glEnd(); 
      glPopMatrix();  
      
      //proyectil  
      list<Bala>::iterator p=proyectil.begin();
      glPointSize(8);
      glColor3b(10,50,10);
      glBegin(GL_POINTS);
      while( p != proyectil.end() ) {
        p->Draw();
        p++;
      }
      glEnd();
      
      //HUD de control
      glColor3f(1.0f,0.0f,0.0f);
      glLineWidth(2);
      glPushMatrix();
      glTranslated(20,640,0);
      glScaled(12,12,1);
      YsDrawUglyFont("Coraza: %100",0);
      glTranslated(0,-2,0);
      YsDrawUglyFont("Motor:  %100",0);
      glTranslated(0,-2,0);
      YsDrawUglyFont("Armas:  %100",0);
      glTranslated(0,-2,0);
      glPopMatrix();
    }
      break;
    case 1:
      {print_text("GANADOR!!!!!", 450, 400, 15, 0.039f, 0.145f, 0.721f, 0.9f);}
      break;
    case 2:
      {print_text("PERDEDOR!!!!!", 450, 400, 15, 0.721f, 0.039f, 0.509f, 0.5f);}
      break;
  }
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
    lt = glutGet(GLUT_ELAPSED_TIME);
    AngLineaRadar=(AngLineaRadar+2)%360;
    
    //Actualizamos las posiciones de los proyectiles
    list<Bala>::iterator p = proyectil.begin();
    while( p != proyectil.end() ) {
      //Si esta fuera del mapa o impacta con el enemigo eliminamos el proyectil
      if( p->Update() )
        p = proyectil.erase(p);
      else
        p++;
    }
    // si hubo 10 impactos al enemigo se sale del juego
    if( Energia<1 ) condicion=1;
    
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
    if(teclado.Disparar())
      proyectil.push_back( Bala( (AvionX), (AvionY), -30*sin(ang), 30*cos(ang)) );//la bala sale desde la base del arma
    if( teclado.Activar() )
      turbina=!turbina;
    
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
  //Controlo el tiempo del juego
  milisegundos = glutGet(GLUT_ELAPSED_TIME);
  if (milisegundos%1000 == 0) {
    tiempo = milisegundos / 1000;
    if(tiempo == 60){
      condicion=2;
    }
  }
}
void inicializa() {
  // GLUT
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);// pide color RGBA y double buffering
  glutInitWindowSize(w,h); glutInitWindowPosition(10,10);
  glutCreateWindow("Top Gun Recargado - Villa, Lucas Sebastian"); // crea el main window

  // declara los callbacks, los que (aun) no se usan (aun) no se declaran
  glutDisplayFunc(Display_cb);
  glutReshapeFunc(Reshape_cb);

  glutSpecialFunc(Special_cb);
  glutIdleFunc(idle_cb);

  //teclas simultaneas
  teclado.Iniciar();
  
  // OpenGL
  glClearColor(0.23f,0.20f,0.01f,1.f);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);  
  
  //Textura 1: inicializacion
  //textura 2D
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(3,texid);
  glBindTexture(GL_TEXTURE_2D, texid[0]);
  mipmap_ppm("nubes.ppm");
  //Utilizamos Repeat en lugar de clamp para hacer que la textura se repita
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  
  //Textura 2
  //textura 2D
  glBindTexture(GL_TEXTURE_2D, texid[1]);
  mipmap_ppm("cubo.ppm");
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  
  
  //Textura 3
  //textura 1D
  makeStripeImage();
  glBindTexture(GL_TEXTURE_1D, texid[2]);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_REPEAT//GL_CLAMP
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);//GL_LINEAR
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);//GL_LINEAR
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);//GL_DECAL//GL_REPLACE
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, stripeImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);
  // ========================  
  
  // Activo el zbuffer
  glDepthFunc( GL_LEQUAL ); 
  
  // las operaciones subsiguientes se aplican a la matriz de modelado GL_MODELVIEW
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

/**
-----------------------------------------------------
--Juego Top Gun Recargado :: Trabajo Practico Final--
-----------------------------------------------------
Alumno: Lucas Sebastian Villa
Profesor: Walter Sotil - Emmanuel Rojas Fredini
Materia: Manipulacion de Objetos en 2D
Año:2021
**/

int main(int argc,char** argv) {
  // teclas a utilizar
  srand(time(NULL));
  cout << "Teclas a utilizar:" << endl;
  cout << "-------------------------------" << endl;
  cout << "w: Avanza" << endl;
  cout << "s: Retrocede" << endl;
  cout << "d: Gira en sentido horario" << endl;
  cout << "a: Gira en sentido antihorario" << endl;
  cout << "q: Prende o apaga las turbinas" << endl;
  cout << "espacio: Dispara arma principal" << endl;
  cout << "-------------------------------" << endl;
  
  glutInit(&argc,argv); // inicialización interna de GLUT
  inicializa(); // define el estado inicial de GLUT y OpenGL
  glutMainLoop(); // entra en loop de reconocimiento de eventos
  return 0; // nunca se llega acá, es sólo para evitar un warning
}
