#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
#include<math.h>
#include<time.h>

#define WIDTH 1200
#define HEIGHT 1000
#define SLICES 40
#define STACKS 40

#define SPEED 0.2
#define JUMP_SPEED 0.07
#define JUMP_LIMIT 1
#define ROTATION_SPEED 20
#define PLATFORM_SIZE 20
#define ROAD_WIDTH 1.8
#define NB_OB 5

struct Game { 
  bool win;
  bool over;
};

struct Camera {
  float eyeX;
  float eyeY;
  float eyeZ;
  float centerX;
  float centerY;
  float centerZ;
  float upX;
  float upY;
  float upZ;
};

struct Rabbit {
  float x;
  float y;
  float z;
  float rotation;
  float colorR;
  float colorG;
  float colorB;
  int state; // 0 = Idle ,1 = Happy,2 = Sad
  int isJumping; // 0 = not jumping , 1 = up , 2 = down
  int mistakes;
};

struct Obstacle {
  float x;
  float y;
  float z;
  float height;
  float width;
};

Game game;
Rabbit rabbit;
Camera camera;
Obstacle obstacles[NB_OB];

/*void reshapeEvent(int width, int height) {
    const float x = WIDTH / HEIGHT;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(x,-x,x,-x,0,10);
    glMatrixMode(GL_MODELVIEW);
}*/
time_t start,end;

void init();

void startTimer(){
  start = time(NULL);
}

void stopTimer(){
  end = time(NULL);
}

time_t getTime(){
  return end - start;
}

void jump(int isJumping){
  if(isJumping == 0){
    return;
  }else if(isJumping == 1){
    rabbit.y += JUMP_SPEED;
    rabbit.z += JUMP_SPEED;
    if( rabbit.y >= JUMP_LIMIT){
      rabbit.isJumping = 2;
    }
  }else if (isJumping == 2){
    rabbit.y -= JUMP_SPEED;
    rabbit.z += JUMP_SPEED;
    if( rabbit.y <= 0){
      rabbit.isJumping = 0;
      rabbit.y = 0;
    } 
  }
  glutTimerFunc(5,jump,rabbit.isJumping);
}

void smile(int n){
  glColor3f(0,0,0);
  glPointSize(7);
  glBegin(GL_POINTS);
    for(int i= sqrt(n/2); i<n - sqrt(n/2);i++){ //sqrt bich nrod smile mtkamalich chtar dora kamla 
      glVertex3f(cos(-M_PI * i/n)/10,sin(-M_PI * i /n)/10,0);
    }
  glEnd();
}

void sad(int n){
  glColor3f(0,0,0);
  glPointSize(7);
  glBegin(GL_POINTS);
    for(int i= sqrt(n/2); i<n - sqrt(n/2);i++){ //sqrt bich nrod smile mtkamalich chtar dora kamla 
      glVertex3f(cos(M_PI * i/n)/10,sin(M_PI * i /n)/10,0);
    }
  glEnd();
}

void idle(float size){
  glColor3f(0,0,0);
  glPointSize(7);
  glBegin(GL_POINTS);
    for(float i= -size; i<size;i+=0.001){
      glVertex3f(i,0,0);
    }
  glEnd();
}

void drawText(char * text,float x,float y){
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1,0,0);
    glRasterPos2f(x,y);
    for (char * i = text; *i != '\0' ; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }
    glPopMatrix();
}

void drawMustache(){
  glPushMatrix();
    glTranslatef(0,0.57,-0.065);
    glPushMatrix();
      glTranslatef(-0.09,0,0);
      glRotatef(0,1,0,0);
      glScalef(15,1,1);
      glutSolidCube(0.005);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-0.09,-0.015,0);
      glRotatef(30,0,0,1);
      glScalef(15,1,1);
      glutSolidCube(0.005);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-0.09,0.015,0);
      glRotatef(-30,0,0,1);
      glScalef(15,1,1);
      glutSolidCube(0.005);
    glPopMatrix();
    glTranslatef(0.09,0,0);
    glRotatef(180,0,1,0);
    glPushMatrix();
      glRotatef(0,1,0,0);
      glScalef(15,1,1);
      glutSolidCube(0.005);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0,-0.015,0);
      glRotatef(30,0,0,1);
      glScalef(15,1,1);
      glutSolidCube(0.005);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0,0.015,0);
      glRotatef(-30,0,0,1);
      glScalef(15,1,1);
      glutSolidCube(0.005);
    glPopMatrix();
  glPopMatrix();
}

void drawRabbit(){
  glPushMatrix();
    glTranslatef(rabbit.x,rabbit.y,rabbit.z);
    glRotatef(rabbit.rotation,0,1,0);
    //left eye
    glPushMatrix();
      glColor3f(0.9,0.9,0.9);
      glTranslatef(0.035,0.63,-0.05);
      glutSolidSphere(0.03,SLICES,STACKS);
    glPopMatrix();
    //right eye
    glPushMatrix();
      glTranslatef(-0.035,0.63,-0.05);
      glutSolidSphere(0.03,SLICES,STACKS);
    glPopMatrix();
    // mouth
    glPushMatrix();
      if(rabbit.state == 0){ //Idle
        glTranslatef(0,0.58,-0.105);
        idle(0.04);
      }else if (rabbit.state == 1){ // Happy
        glTranslatef(0,0.6,-0.1);
        glScalef(0.3,0.3,0.3);
        smile(100);
      }else { //Sad
        glTranslatef(0,0.55,-0.11);
        glScalef(0.3,0.3,0.3);
        sad(100);
      }
    glPopMatrix();
    //inner body
    glPushMatrix();
      glColor3f(0.7,0.7,0.7);
      glTranslatef(0,0,-0.07);
      glScalef(1,1.86,1);
      glutSolidSphere(0.22,SLICES,STACKS);
    glPopMatrix();
    //left ear
    glColor3f(0.7,0.7,0.7);
    glPushMatrix();
      glTranslatef(0.045,0.65,0);
      glScalef(1,1,1);
      glRotatef(-90,1,0,0);
      glRotatef(40,0,1,0);
      glutSolidCone(0.03,0.08,SLICES,STACKS);
    glPopMatrix();
    // right ear
    glPushMatrix();
      glTranslatef(-0.045,0.65,0);
      glScalef(1,1,1);
      glRotatef(-90,1,0,0);
      glRotatef(-40,0,1,0);
      glutSolidCone(0.03,0.08,SLICES,STACKS);
    glPopMatrix();
    // mustache
    drawMustache();
    glColor3f(rabbit.colorR,rabbit.colorG,rabbit.colorB);
    //head
    glPushMatrix();
      glTranslatef(0,0.58,0);
      glutSolidSphere(0.1,SLICES,STACKS);
    glPopMatrix();
    //body
    glPushMatrix();
      glTranslatef(0,0,0);
      glScalef(1,1.86,1);
      glutSolidSphere(0.25,SLICES,STACKS);
    glPopMatrix();
    //left leg
    glPushMatrix();
      glTranslatef(-0.2,-0.49,0);
      glRotatef(-5,0,0,1);
      glScalef(1,5,1);
      glutSolidCube(0.1);
    glPopMatrix();
    //right leg
    glPushMatrix();
      glTranslatef(0.2,-0.49,0);
      glRotatef(5,0,0,1);
      glScalef(1,5,1);
      glutSolidCube(0.1);
    glPopMatrix();
    //left hand
    glPushMatrix();
      glTranslatef(-0.28,0.15,0);
      glRotatef(-50,0,0,1);
      glScalef(1,3.5,1);
      glutSolidCube(0.08);
    glPopMatrix();
    //right hand
    glPushMatrix();
      glTranslatef(0.28,0.15,0);
      glRotatef(50,0,0,1);
      glScalef(1,3.5,1);
      glutSolidCube(0.08);
    glPopMatrix();
  glPopMatrix();
}

void drawTree(float x,float z) {
  glPushMatrix();
    glTranslatef(x,0.6,z);
    glPushMatrix();
      glColor3f(0.54f, 0.31f, 0.23f);
      glScalef(0.2f, 0.8f, 0.2f);
      glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.0f, 0.8f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);
      glScalef(0.6f, 0.4f, 0.6f);
      glutSolidSphere(1.0f, SLICES, STACKS);
    glPopMatrix();
  glPopMatrix();
}

void drawLine(float width){
  glColor3f(1,1,1);
  glBegin(GL_QUADS);
    glVertex3f(-width,0,0);
    glVertex3f(width,0,0);
    glVertex3f(width,0,0.5);
    glVertex3f(-width,0,0.5);
  glEnd();
}

void drawRoad(float size,float width){
  glPushMatrix();
    glTranslatef(0,0.001,0);
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
      glVertex3f(-width,0,0);
      glVertex3f(width,0,0);
      glVertex3f(width,0,size*2);
      glVertex3f(-width,0,size*2);
    glEnd();
    glColor3f(1,1,1);
    glTranslatef(0,0.001,0);
    for(float i = 0; i<size*2 ; i += 1){
      glBegin(GL_QUADS);
        glVertex3f(-width/20,0,i);
        glVertex3f(width/20,0,i);
        glVertex3f(width/20,0,i+0.5);
        glVertex3f(-width/20,0,i+0.5);
      glEnd();
    }
    glPushMatrix();
      drawLine(width);
      glTranslatef(0,0,size*2);
      drawLine(width);
    glPopMatrix();
    //obstacle 
    glColor3f(0.8,0.8,0.8);
    for(int i = 0; i<NB_OB;i++){
      Obstacle ob = obstacles[i];
      glPushMatrix();
        glTranslatef(ob.x,ob.y,ob.z);
        glScalef(ob.width,ob.height,1);
        glutSolidCube(0.5);
      glPopMatrix();
    }
  glPopMatrix();
}

void drawCloud(float x,float y,float z){
  glPushMatrix();
    glTranslatef(x,y,z);
    glutSolidSphere(0.1,SLICES,STACKS);
  glPopMatrix();
}

void drawForest(){
  drawTree(2,3);
  drawTree(-2,3);
  for (float i = 5; i< 13; i += 3)
    for (float j = 5; j< 13; j += 3){
      drawTree(i,j);
    }
}

void drawPlatform(float size){
  glColor3f(0.2,1,0.2);
  glPushMatrix();
    glTranslatef(0,-0.75,0);
    //ground
    glBegin(GL_QUADS);
      glVertex3f(-size,0,-size/3);
      glVertex3f(size,0,-size/3);
      glVertex3f(size,0,size*3);
      glVertex3f(-size,0,size*3);
    glEnd();
    drawRoad(size,ROAD_WIDTH);
    drawForest();
  glPopMatrix();
}

void display(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-10,10,-10,10,0,5);
  glFrustum(-1,1,-1,1,1,50);
  gluLookAt(camera.eyeX + rabbit.x ,camera.eyeY + rabbit.y ,camera.eyeZ + rabbit.z,
            camera.centerX + rabbit.x , camera.centerY + rabbit.y , camera.centerZ + rabbit.z ,
            camera.upX , camera.upY ,camera.upZ);
  /*gluLookAt(0 ,1.1 ,-1.1,
            0,0,0,
            0,1,0);*/
  if(game.over){
    drawText("press x to play again",0,0.5+rabbit.y);
    if(game.win == false){
      drawText("you lost",0,0.55+rabbit.y);
    }else {
      char text[30];
      sprintf(text,"you win in %lu seconds",getTime());
      drawText(text,0,0.55+rabbit.y);
    }
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  drawPlatform(PLATFORM_SIZE);
  drawRabbit();
  glutSwapBuffers();
}

void mouseEvent(int button,int state,int x,int y){
  switch(button){
    case GLUT_LEFT_BUTTON:
      if(state == GLUT_DOWN){
        rabbit.rotation += ROTATION_SPEED;
      }
      break;
    case GLUT_RIGHT_BUTTON:
      if(state == GLUT_DOWN){
        rabbit.rotation -= ROTATION_SPEED;
      }
      break;
  }
}

void keyboardEvent(unsigned char key,int x,int y){
  switch(key) {
    case 32:
      if(!game.over && rabbit.isJumping == 0){
        rabbit.isJumping = 1;
        glutTimerFunc(10,jump,rabbit.isJumping);
      }
      break;
    case 'X':
    case 'x':
      init();
      break;
    case 'U':
    case 'u':
      camera.eyeX = 0;
      camera.eyeY = 1.1;
      camera.eyeZ = 1.1;
      break;
    case 'D':
    case 'd':
      camera.eyeX = 0;
      camera.eyeY = 1.1;
      camera.eyeZ = -1.1;
      break;
    case 'P':
    case 'p':
      camera.eyeX = 1.1;
      camera.eyeY = 1.1;
      camera.eyeZ = 0;
      break;
    case 'N':
    case 'n':
      camera.eyeX = -1.1;
      camera.eyeY = 1.1;
      camera.eyeZ = 0;
      break;
    case 'M':
    case 'm':
      camera.eyeX = 0;
      camera.eyeY = 3;
      camera.eyeZ = 0.1;
      break;
    case 'L':
    case 'l':
      camera.eyeX = 0;
      camera.eyeY = -3;
      camera.eyeZ = 0.1;
      break;
    case 'b':
    case 'B':
      rabbit.colorR = 0;
      rabbit.colorG = 0;
      rabbit.colorB = 1;
      break;
    case 'r':
    case 'R':
      rabbit.colorR = 1;
      rabbit.colorG = 0;
      rabbit.colorB = 0;
      break;
    case 'g':
    case 'G':
      rabbit.colorR = 0;
      rabbit.colorG = 1;
      rabbit.colorB = 0;
      break;
    case 'S':
    case 's':
      rabbit.state = 1;
      break;
    case 'T':
    case 't':
      rabbit.state = 2;
      break;
    case 'I':
    case 'i':
      rabbit.state = 0;
      break;
  }
}

bool isRabbitCollided(){
  for(int i = 0;i < NB_OB; i++){
    Obstacle ob = obstacles[i];
    if((ob.x <= rabbit.x + 1.4) && (ob.x >= rabbit.x - 1.4) && (ob.z <= rabbit.z + 0.5) && (ob.z >= rabbit.z - 0.5)){
      return true;
    }
  }
  return false;
}

bool isRabbitAtTheFinishLine(){
  const float pos_z = PLATFORM_SIZE * 2 + 0.25;
  return (-ROAD_WIDTH - 0.5 <= rabbit.x) && (ROAD_WIDTH + 0.5 >= rabbit.x) && (pos_z - 0.25 <= rabbit.z) && (pos_z +0.25 >= rabbit.z);
}

void specialKeyboardEvent(int key,int x,int y){
  if(!game.over){
    if(!isRabbitCollided()){
      switch(key){
        case GLUT_KEY_UP:
          if(rabbit.z < PLATFORM_SIZE*3 - 0.5){
            rabbit.z += SPEED;
            rabbit.rotation = 180;
          }
          break;
        case GLUT_KEY_DOWN:
          if(rabbit.z > -PLATFORM_SIZE/3){
            rabbit.z -= SPEED;
            rabbit.rotation = 0;
          }
          break;
        case GLUT_KEY_LEFT:
          if(rabbit.x < PLATFORM_SIZE - 0.5){
            rabbit.x += SPEED;
            rabbit.rotation = -90;
          }
          break;
        case GLUT_KEY_RIGHT:
          if(rabbit.x > -PLATFORM_SIZE + 0.5){
            rabbit.x -= SPEED;
            rabbit.rotation = 90;
          }
          break;
        }

        if(isRabbitAtTheFinishLine()){
          stopTimer();
          printf("%lu sec\n", getTime());
          game.over = true;
          game.win = true;
        }
    } else {
      rabbit.mistakes += 1;
      if(rabbit.mistakes == 3){
        game.over = true;
      }
      switch(key){
        case GLUT_KEY_UP:
          rabbit.z -= SPEED * 3;
          break;
        case GLUT_KEY_DOWN:
          rabbit.z += SPEED * 3;
        break;
        case GLUT_KEY_LEFT:
          rabbit.x -= SPEED * 3;
          break;
        case GLUT_KEY_RIGHT:
          rabbit.x += SPEED * 3;
          break;
        }
    }
  }
}


void idle(){
  glutPostRedisplay();
}

void initOb(){
  for(int i= 1 ; i < NB_OB+1 ; i++ ){
    obstacles[i-1].x=0;
    obstacles[i-1].y=0;
    obstacles[i-1].z=i * ((PLATFORM_SIZE * 2 / NB_OB)) - 5;
    obstacles[i-1].height = 3;
    obstacles[i-1].width = 4;
  }
}

void init(){
  srand(time(NULL));
  //rabbit init
  rabbit.x=0;
  rabbit.y=0;
  rabbit.z=0;
  rabbit.rotation=180;
  rabbit.colorR = 0.5;
  rabbit.colorG = 0.5;
  rabbit.colorB = 0.5;
  rabbit.state = 0;
  rabbit.mistakes = 0;
  //camera init
  camera.eyeX = 0;
  camera.eyeY = 1.1;
  camera.eyeZ = -1.1;
  camera.centerX = 0;
  camera.centerY = 0;
  camera.centerZ = 0;
  camera.upX = 0;
  camera.upY = 1;
  camera.upZ = 0;

  game.over = false;
  game.win = false;
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.7,0.7,1,1);
  startTimer();
}

void initLight(){
// Define material and light properties
const GLfloat light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {1.0f, 5.0f, -3.0f, 0.0f};
const GLfloat spot_direction[] = {0.0f,0.0f,0.0f};

const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};
glEnable(GL_LIGHT0);
glEnable(GL_NORMALIZE);
glEnable(GL_COLOR_MATERIAL);
glEnable(GL_LIGHTING);

glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

/*glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);*/
}


int main(int argc,char ** argv){
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WIDTH,HEIGHT);
  glutCreateWindow("Race");

  glutDisplayFunc(display);
  glutMouseFunc(mouseEvent);
  glutKeyboardFunc(keyboardEvent);
  glutSpecialFunc(specialKeyboardEvent);
  glutIdleFunc(idle);
  //glutReshapeFunc(reshapeEvent);

  init();
  initOb();
  initLight();
  glutMainLoop();
  return 0;
}