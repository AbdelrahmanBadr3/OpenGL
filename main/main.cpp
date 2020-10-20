#include <math.h>
#include <stdio.h>
#include <string.h>
#include <GLUT/glut.h>
//#include <irrklang/irrKlang.h>
//using namespace irrklang;

//ISoundEngine *SoundEngine = createIrrKlangDevice();

int p0[2];
int p1[2];
int p2[2];
int p3[2];
int p4[2];


int turn = 0;

//curve issues
bool mouseCliked = false;
bool mouseReleased = false;
bool inProcess = false;
bool gameOver = false;
bool isReached = false;
bool isHit = false;
bool isDefended = false;
bool isWallHitted=false;
bool start= false;

int powerValue = 0;
int powerBar1 = 0;
int powerBar2 = 0;
int healthbar1 = 5;
int healthbar2 = 5;


int player1Pos[2];
int player2Pos[2];
int endPoint[2];
int ballPos[2];
int WallHitPos[2];
int ballIndex = -1;



int windowYSize = 900;
int windowXSize = 1200;
int transationPLayer2X = 4700;
int transationPLayer2Y = 350;
double xTransfer = -10;
float translatioCLoud = 0;
float scaling = 0.2;
float colorStatusColor = 0;
float wallHitTime=0;
float translateMoon=0;
int translateMoon1=0;

int d=0;
int xDaynamic=0;
void drawCircle(double radius, float x1, float y1) {
    float  x2, y2;
    float angle;
    y1 = windowYSize - y1;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);

    for (angle = 1.0f; angle < 361.0f; angle += 0.2)
    {
        x2 = x1 + sin(angle) * radius;
        y2 = y1 + cos(angle) * radius;
        glVertex2f(x2, y2);
    }

    glEnd();
}
void drawCircleHole( float r,float cx, float cy){
    int     num_segments=100;
   glBegin(GL_LINE_LOOP);
   for(int ii = 0; ii < num_segments; ii++)
   {
       float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

       float x = r * cosf(theta);//calculate the x component
       float y = r * sinf(theta);//calculate the y component

       glVertex2f(x + cx, y + cy);//output vertex

   }
   glEnd();
}
void drawCircle1(double radius, float x1, float y1) {
    float  x2, y2;
    float angle;
    y1 =  y1;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);

    for (angle = 1.0f; angle < 361.0f; angle += 0.2)
    {
        x2 = x1 + sin(angle) * radius;
        y2 = y1 + cos(angle) * radius;
        glVertex2f(x2, y2);
    }

    glEnd();
}
void convertor() {
    int shiftYUP = 50;
    if (turn % 2 == 0) {

        p3[0] = player2Pos[0];
        p3[1] = player2Pos[1];

        p0[0] = p3[0] - powerValue;
        p0[1] = shiftYUP;

        p1[0] = p0[0];
        p1[1] = shiftYUP + 0.5 * (p3[0] - p0[0]);

        if (0.75 * (p3[0] - p0[0]) <= 50) shiftYUP = 100;
        p2[0] = p3[0] - (4 + 8 * ((powerValue) / 63));
        p2[1] = shiftYUP + 0.75 * (p3[0] - p0[0]);

    }
    else {


        p0[0] = player1Pos[0];
        p0[1] = player1Pos[1];

        p3[0] = p0[0] + powerValue;
        p3[1] = shiftYUP;

        p2[0] = p3[0];
        p2[1] = shiftYUP + 0.5 * (p3[0] - p0[0]);

        if (0.75 * (p3[0] - p0[0]) <= 50) shiftYUP = 100;
        p1[0] = p0[0] + (4 + 8 * ((powerValue) / 63));
        p1[1] = shiftYUP + 0.75 * (p3[0] - p0[0]);
    }
}
void powerUP() {
    if (turn % 2 == 0) powerBar1 += 3;
    else powerBar2 += 3;
}
void mouseListener(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (!inProcess)
        {
            powerUP();
            mouseCliked = true;
        }

    }
    else
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        {
            if (mouseCliked)
            {
                mouseReleased = true;
                mouseCliked = false;
                if (turn % 2 == 0)
                {
                    powerValue = powerBar1;

                    powerBar1 = 0;
                }
                else
                {
                    powerValue = powerBar2;
                    powerBar2 = 0;
                }

                turn++;
                convertor();
                inProcess = true;

            }
        }

    }

    glutPostRedisplay();
}
int* bezier(float t, int* p0, int* p1, int* p2, int* p3)
{
    int res[2];
    res[0] = pow((1 - t), 3) * p0[0] + 3 * t * pow((1 - t), 2) * p1[0] + 3 * pow(t, 2) * (1 - t) * p2[0] + pow(t, 3) * p3[0];
    res[1] = pow((1 - t), 3) * p0[1] + 3 * t * pow((1 - t), 2) * p1[1] + 3 * pow(t, 2) * (1 - t) * p2[1] + pow(t, 3) * p3[1];
    return res;
}
void Anim()
{
    
    if(start){
    translateMoon+=.3;
        if(translateMoon<1 && translateMoon>-1)
            start=false;
    if(translateMoon>900)
        translateMoon=-300;
       
        
    }else{
        translateMoon1++;
        if(translateMoon1==200){
            start=true;
            translateMoon1=0;
        }
    }
    if (!gameOver) {
        
        xDaynamic++;
        if(xDaynamic%30==0)
            d++;
        if (inProcess)
        {
            int XWallMin=565;
             int XWallMax=600;
             int YWallMin=0;
             int YWallMax=420;
            float Xmin = translatioCLoud;
            float Xmax = 220 + translatioCLoud;
            float Ymin = 450;
            float Ymax = 590;
            printf("p0={%d ,%d} p1={%d ,%d} p2={%d ,%d} p3={%d ,%d} ,ballIndex=%d \n", p0[0], p0[1], p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], ballIndex);
            if (ballIndex == -1) ballIndex = 0;
            if (ballIndex < 200)
            {
                if (turn % 2 == 1) {
                    float    t = ((float)ballIndex) / 200;
                    int* p = bezier(t, p0, p1, p2, p3);
                    ballPos[0] = p[0];
                    ballPos[1] = p[1];
         
                    if (!isHit && ballPos[0] >= 960 && ballPos[0] <= 1070 && ballPos[1] >= 50 && ballPos[1] <= 200) {
                        isHit = true;
                        healthbar2--;
                    }
                    if (healthbar2 == 0)  gameOver = true;
                }
                else {
                    float    t = 1 - ((float)ballIndex) / 150;
                    int* p = bezier(t, p0, p1, p2, p3);
                    ballPos[0] = p[0];
                    ballPos[1] = p[1];
                    
                    if (!isHit && ballPos[0] >= 70 && ballPos[0] <= 180 && ballPos[1] >= 50 && ballPos[1] <= 200) {
                        isHit = true;
                        healthbar1--;
                    }
                    if (healthbar1 == 0) gameOver=true;
                }

                if (ballPos[0] >= Xmin && ballPos[0] <= Xmax && ballPos[1] >= Ymin && ballPos[1] <= Ymax) {
                    isDefended = true;
                    ballIndex = 400;
                    ballPos[0]=-1000;
                    ballPos[1]=-1000;
                }
                else{
                if (ballPos[0] >= XWallMin && ballPos[0] <= XWallMax && ballPos[1] >= YWallMin && ballPos[1] <= YWallMax) {
                    isWallHitted = true;
                    ballIndex = 400;
                    WallHitPos[0]=ballPos[0];
                    WallHitPos[1]=ballPos[1];
                    ballPos[0]=-1000;
                    ballPos[1]=-1000;
                }
                }
                ballIndex++;
                if (ballIndex == 150) {
                    ballPos[0]=-1000;
                    ballPos[1]=-1000;
                inProcess = false;
                ballIndex = -1;
                isHit = false;

                }
            }
            else {
                ballIndex = -1;
                inProcess = false;
                isHit = false;

            }
        }
        if (mouseCliked) powerUP();

        translatioCLoud += 0.5;
        if (translatioCLoud > 1300) translatioCLoud = -300;
        if (xTransfer > 10) isReached = true;
        if (xTransfer < -10) isReached = false;

        if (!isReached) xTransfer += .0001;
        else xTransfer -= .0001;

            
        if (xTransfer > 10) isReached = true;
        if (xTransfer < -10) isReached = false;

        if (!isReached) xTransfer += .5;
        else xTransfer -= .5;
        //change
        colorStatusColor += 0.004;
        if (colorStatusColor >= 1) {
            colorStatusColor = 0;
            isDefended = false;
        }
        //change
        if(isWallHitted)
        wallHitTime += 0.08;
               if (wallHitTime >= 1) {
                   printf("Here={%d ,%d}",WallHitPos[0],WallHitPos[1]);
                   wallHitTime = 0;
                  isWallHitted = false;
               }
    }
    glutPostRedisplay();
}
void drawPowerBar(bool direction) {

    glPushMatrix();
    glScalef(.9, .8, 1);
    glTranslatef(1150, -480, 0);

    if (direction) {
        glTranslatef(-130, 0, 0);

    }
    else {
        glTranslatef(-900, 0, 0);
        glScalef(-1, 1, 1);
    }


    if (powerBar2 > 0 && direction) {
        glColor3f(1, 0.933, 0.549);
        glBegin(GL_TRIANGLES);
        glVertex3f(125.43, windowYSize - 59.65, 0);
        glVertex3f(204.27, windowYSize - 96.82, 0);
        glVertex3f(112.59, windowYSize - 19.41, 0);
        glEnd();

    }
    else {
        if (powerBar1 > 0 && !direction) {
            glColor3f(0.58, 0, 0.062);
            glBegin(GL_TRIANGLES);
            glVertex3f(125.43, windowYSize - 59.65, 0);
            glVertex3f(204.27, windowYSize - 96.82, 0);
            glVertex3f(112.59, windowYSize - 19.41, 0);
            glEnd();
        }
    }
    if (powerBar2 > 300 && direction) {
        glColor3f(1, 0.933, 0.549);
        glBegin(GL_POLYGON);
        glVertex3f(125.43, windowYSize - 59.65, 0);
        glVertex3f(112.59, windowYSize - 19.41, 0);
        glVertex3f(99.57, windowYSize - 48.64, 0);
        glVertex3f(109.75, windowYSize - 95.52, 0);
        glEnd();

    }
    else {
        if (powerBar1 > 300 && !direction) {
            glColor3f(0.58, 0, 0.062);
            glBegin(GL_POLYGON);
            glVertex3f(125.43, windowYSize - 59.65, 0);
            glVertex3f(112.59, windowYSize - 19.41, 0);
            glVertex3f(99.57, windowYSize - 48.64, 0);
            glVertex3f(109.75, windowYSize - 95.52, 0);
            glEnd();
        }
    }

    if (powerBar2 > 500 && direction) {
        glColor3f(1, 0.933, 0.549);
        glBegin(GL_POLYGON);
        glVertex3f(99.57, windowYSize - 48.64, 0);
        glVertex3f(109.75, windowYSize - 95.52, 0);
        glVertex3f(81.38, windowYSize - 61.65, 0);

        glVertex3f(67.61, windowYSize - 21.27, 0);
        glEnd();

    }
    else {
        if (powerBar1 > 500 && !direction) {
            glBegin(GL_POLYGON);
            glColor3f(0.58, 0, 0.062);
            glVertex3f(99.57, windowYSize - 48.64, 0);
            glVertex3f(109.75, windowYSize - 95.52, 0);
            glVertex3f(81.38, windowYSize - 61.65, 0);

            glVertex3f(67.61, windowYSize - 21.27, 0);
            glEnd();
        }
    }

    if (powerBar2 > 700 && direction) {
        glColor3f(1, 0.933, 0.549);
        glBegin(GL_POLYGON);
        glVertex3f(81.38, windowYSize - 61.65, 0);
        glVertex3f(67.61, windowYSize - 21.27, 0);
        glVertex3f(55.98, windowYSize - 51.64, 0);
        glVertex3f(70.66, windowYSize - 96.82, 0);
        glEnd();

    }
    else {
        if (powerBar1 > 700 && !direction) {
            glColor3f(0.58, 0, 0.062);
            glBegin(GL_POLYGON);
            glVertex3f(81.38, windowYSize - 61.65, 0);
            glVertex3f(67.61, windowYSize - 21.27, 0);
            glVertex3f(55.98, windowYSize - 51.64, 0);
            glVertex3f(70.66, windowYSize - 96.82, 0);
            glEnd();
        }
    }
    if (powerBar2 > 950 && direction) {
        glColor3f(1, 0.933, 0.549);
        glBegin(GL_TRIANGLES);
        glVertex3f(5.05, windowYSize - 12, 0);
        glVertex3f(70.66, windowYSize - 96.82, 0);
        glVertex3f(55.98, windowYSize - 51.64, 0);
        glEnd();

    }
    else {
        if (powerBar1 > 950 && !direction) {
            glColor3f(0.58, 0, 0.062);
            glBegin(GL_TRIANGLES);
            glVertex3f(5.05, windowYSize - 12, 0);
            glVertex3f(70.66, windowYSize - 96.82, 0);
            glVertex3f(55.98, windowYSize - 51.64, 0);
            glEnd();
        }
    }
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);

    glVertex3f(5.05, windowYSize - 12, 0);
    glVertex3f(70.66, windowYSize - 96.82, 0);
    glVertex3f(81.38, windowYSize - 61.65, 0);
    glVertex3f(109.75, windowYSize - 95.52, 0);
    glVertex3f(125.43, windowYSize - 59.65, 0);
    glVertex3f(204.27, windowYSize - 96.82, 0);

    glVertex3f(112.59, windowYSize - 19.41, 0);
    glVertex3f(99.57, windowYSize - 48.64, 0);

    glVertex3f(67.61, windowYSize - 21.27, 0);

    glVertex3f(55.98, windowYSize - 51.64, 0);
    glEnd();

    glPopMatrix();

    //

}
void drawCloud(bool status ,bool s2) {
    if(status){
        float cloudColor = 1;
          if(isDefended&& turn%2==0){
             
              float rg=1-(0.342)*colorStatusColor;
               float b=1-(0.33)*colorStatusColor;
              glColor3f(rg , rg, b);
              //printf("%1.2f %1.4f", rg , b);
          }
          else {
              if (isDefended && turn % 2 == 1){
                  
                  float r=1-(0.851)*colorStatusColor;
                  float g=1-(0.325)*colorStatusColor;
                  printf("color =%1.4f %1.4f %1.4f \n ", r , g, colorStatusColor);
                  glColor3f(r, g, 1);
              }
              else glColor3f(cloudColor, cloudColor, cloudColor);

          }
          //glLineWidth(15);
          glPushMatrix();
          
          glTranslatef(translatioCLoud, -300, 0);
          glBegin(GL_POLYGON);

          glVertex3f(35.79, windowYSize - 58.27, 0);
          glVertex3f(42.21, windowYSize - 45.52, 0);
          glVertex3f(49.1, windowYSize - 39.81, 0);
          glVertex3f(57.02, windowYSize - 36.47, 0);
          glVertex3f(68.37, windowYSize - 36.47, 0);
          glVertex3f(77.89, windowYSize - 39.81, 0);

          glVertex3f(79.84, windowYSize - 31.2, 0);
          glVertex3f(83.51, windowYSize - 26.61, 0);
          glVertex3f(95.44, windowYSize - 17.36, 0);
          glVertex3f(105.43, windowYSize - 12.94, 0);
          glVertex3f(119.79, windowYSize - 9.6, 0);
          glVertex3f(140.07, windowYSize - 10.27, 0);

          glVertex3f(150.14, windowYSize - 12.94, 0);
          glVertex3f(159.69, windowYSize - 17.36, 0);
          glVertex3f(175.13, windowYSize - 29.21, 0);
          glVertex3f(180.49, windowYSize - 36.98, 0);
          glVertex3f(186.4, windowYSize - 45.52, 0);
          glVertex3f(188.06, windowYSize - 60.66, 0);
          glVertex3f(203.92, windowYSize - 67.61, 0);
          glVertex3f(217.78, windowYSize - 83.21, 0);

          glVertex3f(220.62, windowYSize - 99.07, 0);
          glVertex3f(217.46, windowYSize - 111.05, 0);
          glVertex3f(214.94, windowYSize - 120.61, 0);
          glVertex3f(207.45, windowYSize - 129.59, 0);
          glVertex3f(195.99, windowYSize - 134.54, 0);
          glVertex3f(177.63, windowYSize - 134.54, 0);
          glVertex3f(169.92, windowYSize - 142.62, 0);
          glVertex3f(158.69, windowYSize - 146.4, 0);
          glVertex3f(146.8, windowYSize - 147.53, 0);
          glVertex3f(132.98, windowYSize - 143.06, 0);

          glVertex3f(126.21, windowYSize - 149.73, 0);
          glVertex3f(119.79, windowYSize - 153.24, 0);
          glVertex3f(105.43, windowYSize - 156.46, 0);
          glVertex3f(89.93, windowYSize - 156.46, 0);


          glVertex3f(77.89, windowYSize - 153.24, 0);
          glVertex3f(63.89, windowYSize - 147.32, 0);
          glVertex3f(53.69, windowYSize - 139.55, 0);
          glVertex3f(44.5, windowYSize - 132.04, 0);
          glVertex3f(31.5, windowYSize - 135.04, 0);
          glVertex3f(20.3, windowYSize - 132.04, 0);
          glVertex3f(6.53, windowYSize - 120.61, 0);
          glVertex3f(2.77, windowYSize - 111.96, 0);
          glVertex3f(2.77, windowYSize - 103.66, 0);
          glVertex3f(2.77, windowYSize - 91.31, 0);
          glVertex3f(6.53, windowYSize - 83.21, 0);
          glVertex3f(20.3, windowYSize - 71.53, 0);
          glVertex3f(26.98, windowYSize - 69.44, 0);
          glVertex3f(33.49, windowYSize - 69.44, 0);
          glEnd();
          if (isDefended){
              float gray=1-colorStatusColor;
              glColor3f(gray,gray, gray);
          }    else
               glColor3f(cloudColor, cloudColor, cloudColor);

          
          glBegin(GL_LINE_LOOP);

          glVertex3f(35.79, windowYSize - 58.27, 0);
          glVertex3f(42.21, windowYSize - 45.52, 0);
          glVertex3f(49.1, windowYSize - 39.81, 0);
          glVertex3f(57.02, windowYSize - 36.47, 0);
          glVertex3f(68.37, windowYSize - 36.47, 0);
          glVertex3f(77.89, windowYSize - 39.81, 0);

          glVertex3f(79.84, windowYSize - 31.2, 0);
          glVertex3f(83.51, windowYSize - 26.61, 0);
          glVertex3f(95.44, windowYSize - 17.36, 0);
          glVertex3f(105.43, windowYSize - 12.94, 0);
          glVertex3f(119.79, windowYSize - 9.6, 0);
          glVertex3f(140.07, windowYSize - 10.27, 0);

          glVertex3f(150.14, windowYSize - 12.94, 0);
          glVertex3f(159.69, windowYSize - 17.36, 0);
          glVertex3f(175.13, windowYSize - 29.21, 0);
          glVertex3f(180.49, windowYSize - 36.98, 0);
          glVertex3f(186.4, windowYSize - 45.52, 0);
          glVertex3f(188.06, windowYSize - 60.66, 0);
          glVertex3f(203.92, windowYSize - 67.61, 0);
          glVertex3f(217.78, windowYSize - 83.21, 0);

          glVertex3f(220.62, windowYSize - 99.07, 0);
          glVertex3f(217.46, windowYSize - 111.05, 0);
          glVertex3f(214.94, windowYSize - 120.61, 0);
          glVertex3f(207.45, windowYSize - 129.59, 0);
          glVertex3f(195.99, windowYSize - 134.54, 0);
          glVertex3f(177.63, windowYSize - 134.54, 0);
          glVertex3f(169.92, windowYSize - 142.62, 0);
          glVertex3f(158.69, windowYSize - 146.4, 0);
          glVertex3f(146.8, windowYSize - 147.53, 0);
          glVertex3f(132.98, windowYSize - 143.06, 0);

          glVertex3f(126.21, windowYSize - 149.73, 0);
          glVertex3f(119.79, windowYSize - 153.24, 0);
          glVertex3f(105.43, windowYSize - 156.46, 0);
          glVertex3f(89.93, windowYSize - 156.46, 0);


          glVertex3f(77.89, windowYSize - 153.24, 0);
          glVertex3f(63.89, windowYSize - 147.32, 0);
          glVertex3f(53.69, windowYSize - 139.55, 0);
          glVertex3f(44.5, windowYSize - 132.04, 0);
          glVertex3f(31.5, windowYSize - 135.04, 0);
          glVertex3f(20.3, windowYSize - 132.04, 0);
          glVertex3f(6.53, windowYSize - 120.61, 0);
          glVertex3f(2.77, windowYSize - 111.96, 0);
          glVertex3f(2.77, windowYSize - 103.66, 0);
          glVertex3f(2.77, windowYSize - 91.31, 0);
          glVertex3f(6.53, windowYSize - 83.21, 0);
          glVertex3f(20.3, windowYSize - 71.53, 0);
          glVertex3f(26.98, windowYSize - 69.44, 0);
          glVertex3f(33.49, windowYSize - 69.44, 0);
          glEnd();
          glPopMatrix();
    }else{
        float cloudColor = 1;
         
        glColor3f(cloudColor, cloudColor, cloudColor);

         
          //glLineWidth(15);
          glPushMatrix();
        if(s2){
            glScalef(2, 2, 1);
            glTranslatef(0, -450, 0);
        }else{
            glScalef(2, 1, 1);
            glTranslatef(300, -150, 0);
        }
        
            
          glBegin(GL_POLYGON);

          glVertex3f(35.79, windowYSize - 58.27, 0);
          glVertex3f(42.21, windowYSize - 45.52, 0);
          glVertex3f(49.1, windowYSize - 39.81, 0);
          glVertex3f(57.02, windowYSize - 36.47, 0);
          glVertex3f(68.37, windowYSize - 36.47, 0);
          glVertex3f(77.89, windowYSize - 39.81, 0);

          glVertex3f(79.84, windowYSize - 31.2, 0);
          glVertex3f(83.51, windowYSize - 26.61, 0);
          glVertex3f(95.44, windowYSize - 17.36, 0);
          glVertex3f(105.43, windowYSize - 12.94, 0);
          glVertex3f(119.79, windowYSize - 9.6, 0);
          glVertex3f(140.07, windowYSize - 10.27, 0);

          glVertex3f(150.14, windowYSize - 12.94, 0);
          glVertex3f(159.69, windowYSize - 17.36, 0);
          glVertex3f(175.13, windowYSize - 29.21, 0);
          glVertex3f(180.49, windowYSize - 36.98, 0);
          glVertex3f(186.4, windowYSize - 45.52, 0);
          glVertex3f(188.06, windowYSize - 60.66, 0);
          glVertex3f(203.92, windowYSize - 67.61, 0);
          glVertex3f(217.78, windowYSize - 83.21, 0);

          glVertex3f(220.62, windowYSize - 99.07, 0);
          glVertex3f(217.46, windowYSize - 111.05, 0);
          glVertex3f(214.94, windowYSize - 120.61, 0);
          glVertex3f(207.45, windowYSize - 129.59, 0);
          glVertex3f(195.99, windowYSize - 134.54, 0);
          glVertex3f(177.63, windowYSize - 134.54, 0);
          glVertex3f(169.92, windowYSize - 142.62, 0);
          glVertex3f(158.69, windowYSize - 146.4, 0);
          glVertex3f(146.8, windowYSize - 147.53, 0);
          glVertex3f(132.98, windowYSize - 143.06, 0);

          glVertex3f(126.21, windowYSize - 149.73, 0);
          glVertex3f(119.79, windowYSize - 153.24, 0);
          glVertex3f(105.43, windowYSize - 156.46, 0);
          glVertex3f(89.93, windowYSize - 156.46, 0);


          glVertex3f(77.89, windowYSize - 153.24, 0);
          glVertex3f(63.89, windowYSize - 147.32, 0);
          glVertex3f(53.69, windowYSize - 139.55, 0);
          glVertex3f(44.5, windowYSize - 132.04, 0);
          glVertex3f(31.5, windowYSize - 135.04, 0);
          glVertex3f(20.3, windowYSize - 132.04, 0);
          glVertex3f(6.53, windowYSize - 120.61, 0);
          glVertex3f(2.77, windowYSize - 111.96, 0);
          glVertex3f(2.77, windowYSize - 103.66, 0);
          glVertex3f(2.77, windowYSize - 91.31, 0);
          glVertex3f(6.53, windowYSize - 83.21, 0);
          glVertex3f(20.3, windowYSize - 71.53, 0);
          glVertex3f(26.98, windowYSize - 69.44, 0);
          glVertex3f(33.49, windowYSize - 69.44, 0);
          glEnd();
        
        glColor3f(cloudColor, cloudColor, cloudColor);

          
          glBegin(GL_LINE_LOOP);

          glVertex3f(35.79, windowYSize - 58.27, 0);
          glVertex3f(42.21, windowYSize - 45.52, 0);
          glVertex3f(49.1, windowYSize - 39.81, 0);
          glVertex3f(57.02, windowYSize - 36.47, 0);
          glVertex3f(68.37, windowYSize - 36.47, 0);
          glVertex3f(77.89, windowYSize - 39.81, 0);

          glVertex3f(79.84, windowYSize - 31.2, 0);
          glVertex3f(83.51, windowYSize - 26.61, 0);
          glVertex3f(95.44, windowYSize - 17.36, 0);
          glVertex3f(105.43, windowYSize - 12.94, 0);
          glVertex3f(119.79, windowYSize - 9.6, 0);
          glVertex3f(140.07, windowYSize - 10.27, 0);

          glVertex3f(150.14, windowYSize - 12.94, 0);
          glVertex3f(159.69, windowYSize - 17.36, 0);
          glVertex3f(175.13, windowYSize - 29.21, 0);
          glVertex3f(180.49, windowYSize - 36.98, 0);
          glVertex3f(186.4, windowYSize - 45.52, 0);
          glVertex3f(188.06, windowYSize - 60.66, 0);
          glVertex3f(203.92, windowYSize - 67.61, 0);
          glVertex3f(217.78, windowYSize - 83.21, 0);

          glVertex3f(220.62, windowYSize - 99.07, 0);
          glVertex3f(217.46, windowYSize - 111.05, 0);
          glVertex3f(214.94, windowYSize - 120.61, 0);
          glVertex3f(207.45, windowYSize - 129.59, 0);
          glVertex3f(195.99, windowYSize - 134.54, 0);
          glVertex3f(177.63, windowYSize - 134.54, 0);
          glVertex3f(169.92, windowYSize - 142.62, 0);
          glVertex3f(158.69, windowYSize - 146.4, 0);
          glVertex3f(146.8, windowYSize - 147.53, 0);
          glVertex3f(132.98, windowYSize - 143.06, 0);

          glVertex3f(126.21, windowYSize - 149.73, 0);
          glVertex3f(119.79, windowYSize - 153.24, 0);
          glVertex3f(105.43, windowYSize - 156.46, 0);
          glVertex3f(89.93, windowYSize - 156.46, 0);


          glVertex3f(77.89, windowYSize - 153.24, 0);
          glVertex3f(63.89, windowYSize - 147.32, 0);
          glVertex3f(53.69, windowYSize - 139.55, 0);
          glVertex3f(44.5, windowYSize - 132.04, 0);
          glVertex3f(31.5, windowYSize - 135.04, 0);
          glVertex3f(20.3, windowYSize - 132.04, 0);
          glVertex3f(6.53, windowYSize - 120.61, 0);
          glVertex3f(2.77, windowYSize - 111.96, 0);
          glVertex3f(2.77, windowYSize - 103.66, 0);
          glVertex3f(2.77, windowYSize - 91.31, 0);
          glVertex3f(6.53, windowYSize - 83.21, 0);
          glVertex3f(20.3, windowYSize - 71.53, 0);
          glVertex3f(26.98, windowYSize - 69.44, 0);
          glVertex3f(33.49, windowYSize - 69.44, 0);
          glEnd();
          glPopMatrix();
    }
  
}

void drawHeart(float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);
    drawCircle(10, x, y);
    drawCircle(10, (x + 12), y);
    glBegin(GL_TRIANGLES);
    glVertex3f(x - 11, windowYSize - (y + 2), 0);
    glVertex3f(x + 23, windowYSize - (y + 2), 0);
    glVertex3f(x + 6, windowYSize - (y + 23), 0);
    glEnd();
}
void drawHealthBar() {
    
    //draw health background
    if(healthbar1==5) drawHeart(250, 50, 1, 0, 0);
    else drawHeart(250, 50, .5, 0.5, 0.5);
    if (healthbar1 >= 4) drawHeart(200, 50, 1, 0, 0);
    else drawHeart(200, 50,0.5, 0.5, 0.5);
    if (healthbar1 >= 3) drawHeart(150, 50, 1, 0, 0);
    else drawHeart(150, 50,0.5, 0.5, 0.5);
    if (healthbar1 >= 2) drawHeart(100, 50, 1, 0, 0);
    else drawHeart(100, 50,0.5, 0.5, 0.5);
    if (healthbar1 >= 1) drawHeart(50, 50, 1, 0, 0);
    else drawHeart(50, 50,0.5, 0.5, 0.5);

    if (healthbar2 == 5) drawHeart(windowXSize - 250, 50, 1, 0, 0);
    else drawHeart(windowXSize - 250, 50, .5, 0.5, 0.5);
    if (healthbar2 >= 4) drawHeart(windowXSize - 200, 50, 1, 0, 0);
    else drawHeart(windowXSize - 200, 50, 0.5, 0.5, 0.5);
    if (healthbar2 >= 3) drawHeart(windowXSize - 150, 50, 1, 0, 0);
    else drawHeart(windowXSize - 150, 50, 0.5, 0.5, 0.5);
    if (healthbar2 >= 2) drawHeart(windowXSize - 100, 50, 1, 0, 0);
    else drawHeart(windowXSize - 100, 50, 0.5, 0.5, 0.5);
    if (healthbar2 >= 1) drawHeart(windowXSize - 50, 50, 1, 0, 0);
    else drawHeart(windowXSize - 50, 50, 0.5, 0.5, 0.5);

}
void drawPicakSafe(bool right) {
    int swapy = 900;
    glPushMatrix();
    glLineWidth(5);
    glScalef(scaling, scaling, 1);
    if (right) glTranslatef(1000 , transationPLayer2Y + xTransfer, 0);
    else glTranslatef(transationPLayer2X , transationPLayer2Y + xTransfer, 0);
    if(isHit ) glTranslatef(0 , 300, 0);
    if (right) glScalef(-1, 1, 1);
    // RIGHT EAR
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.11, 0.11);
    else glColor3f(0.992, 0.867, 0.286);
    glVertex3f(224, swapy - 280, 0);
    glVertex3f(184, swapy - 208, 0);
    glVertex3f(136, swapy - 152, 0);
    glVertex3f(109.14, swapy - 128.21, 0);
    glVertex3f(81.27, swapy - 108.83, 0);
    glVertex3f(48, swapy - 96, 0);
    glVertex3f(0, swapy - 96, 0);
    glVertex3f(24, swapy - 120, 0);
    glVertex3f(64, swapy - 216, 0);
    glVertex3f(96, swapy - 248, 0);
    glVertex3f(160, swapy - 328, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(224, swapy - 280, 0);
    glVertex3f(184, swapy - 208, 0);
    glVertex3f(136, swapy - 152, 0);
    glVertex3f(109.14, swapy - 128.21, 0);
    glVertex3f(81.27, swapy - 108.83, 0);
    glVertex3f(48, swapy - 96, 0);
    glVertex3f(0, swapy - 96, 0);
    glVertex3f(24, swapy - 120, 0);
    glVertex3f(64, swapy - 216, 0);
    glVertex3f(96, swapy - 248, 0);
    glVertex3f(160, swapy - 328, 0);
    glEnd();

    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else glColor3f(0, 0, 0);
    glVertex3f(136, swapy - 152, 0);
    glVertex3f(120, swapy - 136, 0);

    glVertex3f(109.14, swapy - 128.21, 0);
    glVertex3f(81.27, swapy - 108.83, 0);
    glVertex3f(48, swapy - 96, 0);
    glVertex3f(0, swapy - 96, 0);
    glVertex3f(24, swapy - 120, 0);
    glVertex3f(64, swapy - 216, 0);
    glVertex3f(80, swapy - 232, 0);
    glVertex3f(80, swapy - 120, 0);
    glEnd();
    // LEFT EAR
    glBegin(GL_POLYGON);
    if (right) glColor3f(.11, 0.11, 0.11);
    else glColor3f(0.992, 0.867, 0.286);

    glVertex3f(438.34, swapy - 284.14, 0);
    glVertex3f(507.94, swapy - 171.71, 0);
    glVertex3f(570.85, swapy - 111.47, 0);
    glVertex3f(639.12, swapy - 48.56, 0);
    glVertex3f(708.72, swapy - 17.77, 0);
    glVertex3f(695.34, swapy - 80.69, 0);
    glVertex3f(649.83, swapy - 171.71, 0);
    glVertex3f(560.14, swapy - 284.14, 0);
    glVertex3f(498.57, swapy - 348.39, 0);
    //glVertex3f(186.07, swapy - 286.93, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(438.34, swapy - 284.14, 0);
    glVertex3f(507.94, swapy - 171.71, 0);
    glVertex3f(570.85, swapy - 111.47, 0);
    glVertex3f(639.12, swapy - 48.56, 0);
    glVertex3f(708.72, swapy - 17.77, 0);
    glVertex3f(695.34, swapy - 80.69, 0);
    glVertex3f(649.83, swapy - 171.71, 0);
    glVertex3f(560.14, swapy - 284.14, 0);
    glVertex3f(498.57, swapy - 348.39, 0);
    glEnd();

    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else glColor3f(0, 0, 0);
    glVertex3f(570.85, swapy - 111.47, 0);
    glVertex3f(639.12, swapy - 48.56, 0);
    glVertex3f(708.72, swapy - 17.77, 0);
    glVertex3f(695.34, swapy - 80.69, 0);
    glVertex3f(649.83, swapy - 171.71, 0);

    glVertex3f(605.86, swapy - 228.03, 0);
    glVertex3f(616.36, swapy - 207.85, 0);
    glVertex3f(628.41, swapy - 185.09, 0);
    glVertex3f(628.41, swapy - 126.2, 0);
    glVertex3f(616.36, swapy - 80.69, 0);
    glVertex3f(601.64, swapy - 87.38, 0);
    glVertex3f(588.36, swapy - 97.26, 0);
    glEnd();


    // TAIL
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.1, 0.11);
    else  glColor3f(0.988, 0.8627, 0.286);
    glVertex3f(481.93, swapy - 568.37, 0);

    glVertex3f(499.34, swapy - 462.63, 0);
    glVertex3f(563.59, swapy - 393.02, 0);
    glVertex3f(670.67, swapy - 281.92, 0);
    glVertex3f(866.09, swapy - 411.76, 0);
    glVertex3f(704.13, swapy - 540.26, 0);
    glVertex3f(650.59, swapy - 621.91, 0);


    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(704.13, swapy - 540.26, 0);
    glVertex3f(837.98, swapy - 611.2, 0);
    glVertex3f(704.13, swapy - 698.21, 0);
    glVertex3f(681.38, swapy - 751.75, 0);
    glVertex3f(574.29, swapy - 704.9, 0);
    glVertex3f(650.59, swapy - 621.91, 0);
    glEnd();


    glBegin(GL_POLYGON);
    glVertex3f(704.13, swapy - 698.21, 0);
    glVertex3f(801.84, swapy - 751.75, 0);
    glVertex3f(589.02, swapy - 886.94, 0);
    glVertex3f(580.99, swapy - 838.75, 0);
    glVertex3f(681.38, swapy - 751.75, 0);
    glEnd();




    // END OF THE TAIL
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0.643, 0.341, 0.125);

    glVertex3f(644.91, swapy - 784, 0);
    glVertex3f(640, swapy - 806.36, 0);
    glVertex3f(663.96, swapy - 795.39, 0);
    glVertex3f(656.61, swapy - 814.62, 0);
    glVertex3f(679.28, swapy - 808.36, 0);
    glVertex3f(670.44, swapy - 824.63, 0);
    glVertex3f(696, swapy - 816, 0);
    glVertex3f(589.02, swapy - 886.94, 0);
    glVertex3f(580.99, swapy - 838.75, 0);
    glEnd();
    //glLineWidth(13);

    glBegin(GL_LINE_LOOP);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0.643, 0.341, 0.125);
    glVertex3f(644.91, swapy - 784, 0);
    glVertex3f(640, swapy - 806.36, 0);
    glVertex3f(663.96, swapy - 795.39, 0);
    glVertex3f(656.61, swapy - 814.62, 0);
    glVertex3f(679.28, swapy - 808.36, 0);
    glVertex3f(670.44, swapy - 824.63, 0);
    glVertex3f(696, swapy - 816, 0);
    glVertex3f(589.02, swapy - 886.94, 0);
    glVertex3f(580.99, swapy - 838.75, 0);
    glEnd();
    glLineWidth(5);

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(499.34, swapy - 462.63, 0);
    glVertex3f(563.59, swapy - 393.02, 0);
    glVertex3f(670.67, swapy - 281.92, 0);
    glVertex3f(866.09, swapy - 411.76, 0);
    glVertex3f(704.13, swapy - 540.26, 0);
    glVertex3f(837.98, swapy - 611.2, 0);
    glVertex3f(704.13, swapy - 698.21, 0);
    glVertex3f(801.84, swapy - 751.75, 0);
    glVertex3f(589.02, swapy - 886.94, 0);
    glVertex3f(580.99, swapy - 838.75, 0);
    glVertex3f(681.38, swapy - 751.75, 0);
    glVertex3f(574.29, swapy - 704.9, 0);
    glVertex3f(650.59, swapy - 621.91, 0);
    glVertex3f(481.93, swapy - 568.37, 0);
    glEnd();
    //


    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(162.07, swapy - 533.89, 0);
    glVertex3f(181.43, swapy - 512.79, 0);
    glVertex3f(193.29, swapy - 507.82, 0);
    glVertex3f(200.78, swapy - 507.82, 0);
    glVertex3f(228.26, swapy - 519.62, 0);
    glVertex3f(262.6, swapy - 540.72, 0);
    glEnd();

    // BODY
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.11, 0.11);
    else glColor3f(0.992, 0.867, 0.286);
    glVertex3f(179.09, swapy - 620.69, 0);
    glVertex3f(130.48, swapy - 578.62, 0);
    glVertex3f(101.03, swapy - 546.49, 0);
    glVertex3f(94.34, swapy - 499.65, 0);
    glVertex3f(107.72, swapy - 459.49, 0);
    glVertex3f(123.78, swapy - 423.35, 0);
    glVertex3f(130.48, swapy - 376.5, 0);
    glVertex3f(162.6, swapy - 330.99, 0);
    glVertex3f(176, swapy - 312, 0);
    glVertex3f(208, swapy - 288, 0);
    glVertex3f(206.15, swapy - 285.48, 0);
    glVertex3f(310.83, swapy - 257.37, 0);

    glVertex3f(394.03, swapy - 257.37, 0);
    glVertex3f(446.37, swapy - 273.43, 0);
    glVertex3f(472, swapy - 296, 0);
    glVertex3f(496, swapy - 320, 0);
    glVertex3f(512, swapy - 344, 0);
    glVertex3f(511.96, swapy - 341.7, 0);
    glVertex3f(529.67, swapy - 384.53, 0);
    glVertex3f(537.9, swapy - 413.36, 0);

    //glVertex3f(537.9, swapy - 420.57, 0);
    glVertex3f(537.39, swapy - 422.01, 0);
    glVertex3f(528.6, swapy - 546.2, 0);

    glVertex3f(521.43, swapy - 573.99, 0);
    glVertex3f(521.43, swapy - 595.61, 0);
    glVertex3f(526.68, swapy - 617.44, 0);

    glVertex3f(566.84, swapy - 697.75, 0);
    glVertex3f(598.96, swapy - 818.22, 0);
    glVertex3f(598.96, swapy - 866.4, 0);
    glVertex3f(592.27, swapy - 925.3, 0);
    glVertex3f(558.81, swapy - 962.78, 0);
    glVertex3f(477.16, swapy - 989.55, 0);
    glVertex3f(336.61, swapy - 974.82, 0);
    glVertex3f(289.76, swapy - 981.52, 0);
    glVertex3f(281.73, swapy - 981.52, 0);
    glVertex3f(246.93, swapy - 981.52, 0);
    glVertex3f(217.48, swapy - 981.52, 0);
    glVertex3f(184.02, swapy - 966.79, 0);
    glVertex3f(180, swapy - 962.78, 0);
    glVertex3f(142.52, swapy - 910.58, 0);
    glVertex3f(137.17, swapy - 866.4, 0);
    glVertex3f(142.52, swapy - 806.17, 0);
    glVertex3f(161.26, swapy - 729.87, 0);
    //glVertex3f(161.26, swapy - 996.7, 0);
    glVertex3f(177.32, swapy - 622.79, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(179.09, swapy - 620.69, 0);
    glVertex3f(130.48, swapy - 578.62, 0);
    glVertex3f(101.03, swapy - 546.49, 0);
    glVertex3f(94.34, swapy - 499.65, 0);
    glVertex3f(107.72, swapy - 459.49, 0);
    glVertex3f(123.78, swapy - 423.35, 0);
    glVertex3f(130.48, swapy - 376.5, 0);
    glVertex3f(162.6, swapy - 330.99, 0);
    glVertex3f(176, swapy - 312, 0);
    glVertex3f(208, swapy - 288, 0);
    glVertex3f(206.15, swapy - 285.48, 0);
    glVertex3f(310.83, swapy - 257.37, 0);

    glVertex3f(394.03, swapy - 257.37, 0);
    glVertex3f(446.37, swapy - 273.43, 0);
    glVertex3f(472, swapy - 296, 0);
    glVertex3f(496, swapy - 320, 0);
    glVertex3f(512, swapy - 344, 0);
    glVertex3f(511.96, swapy - 341.7, 0);
    glVertex3f(529.67, swapy - 384.53, 0);
    glVertex3f(537.9, swapy - 413.36, 0);

    //glVertex3f(537.9, swapy - 420.57, 0);
    glVertex3f(537.39, swapy - 422.01, 0);
    glVertex3f(528.6, swapy - 546.2, 0);

    glVertex3f(521.43, swapy - 573.99, 0);
    glVertex3f(521.43, swapy - 595.61, 0);
    glVertex3f(526.68, swapy - 617.44, 0);

    glVertex3f(566.84, swapy - 697.75, 0);
    glVertex3f(598.96, swapy - 818.22, 0);
    glVertex3f(598.96, swapy - 866.4, 0);
    glVertex3f(592.27, swapy - 925.3, 0);
    glVertex3f(558.81, swapy - 962.78, 0);
    glVertex3f(477.16, swapy - 989.55, 0);
    glVertex3f(336.61, swapy - 974.82, 0);
    glVertex3f(289.76, swapy - 981.52, 0);
    glVertex3f(281.73, swapy - 981.52, 0);
    glVertex3f(246.93, swapy - 981.52, 0);
    glVertex3f(217.48, swapy - 981.52, 0);
    glVertex3f(184.02, swapy - 966.79, 0);
    glVertex3f(180, swapy - 962.78, 0);
    glVertex3f(142.52, swapy - 910.58, 0);
    glVertex3f(137.17, swapy - 866.4, 0);
    glVertex3f(142.52, swapy - 806.17, 0);
    glVertex3f(161.26, swapy - 729.87, 0);
    glVertex3f(177.32, swapy - 622.79, 0);

    glEnd();

    glLineWidth(5);



    //side trianles
    glBegin(GL_TRIANGLES);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.643, 0.341, 0.125);

    glVertex3f(594.47, swapy - 785.06, 0);
    glVertex3f(550.26, swapy - 828.31, 0);
    glVertex3f(601.64, swapy - 828.31, 0);
    glEnd();

    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.643, 0.341, 0.125);
    glVertex3f(559.53, swapy - 682.1, 0);
    glVertex3f(532.26, swapy - 696, 0);
    glVertex3f(524.52, swapy - 708.72, 0);
    glVertex3f(532.26, swapy - 714.02, 0);
    glVertex3f(545.16, swapy - 714.02, 0);
    glVertex3f(568, swapy - 712, 0);
    glEnd();


    //NOSE
    if (right) {
        glColor3f(0.71, 0.423, 0);
        glPushMatrix();
        //glRotatef(90, 1, 0, 0);
        glBegin(GL_QUADS);

        glVertex3f(204.16, swapy - 441.75, 0);
        glVertex3f(204.16, swapy - 484.11, 0);
        glVertex3f(258.58, swapy - 441.75, 0);
        glVertex3f(258.58, swapy - 484.11, 0);


        glEnd();
        glPopMatrix();
    }
    else {
        glColor3f(0, 0, 0);
        glBegin(GL_TRIANGLES);


        glVertex3f(223.86, swapy - 461.75, 0);
        glVertex3f(234.16, swapy - 474.11, 0);
        glVertex3f(248.58, swapy - 461.75, 0);
        glEnd();
    }


    //NOSE
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);

    glVertex3f(200.08, swapy - 501.91, 0);
    glVertex3f(212.54, swapy - 512.21, 0);
    glVertex3f(227.98, swapy - 512.21, 0);
    glVertex3f(242.4, swapy - 507.06, 0);
    glVertex3f(264.02, swapy - 512.21, 0);
    glVertex3f(284.61, swapy - 512.21, 0);
    glVertex3f(293.88, swapy - 501.91, 0);
    glEnd();



    // LEGS RIGHT
    glBegin(GL_POLYGON);
    if (right) glColor3f(.11, 0.11, 0.11);
    else  glColor3f(0.98, 0.832, 0.25);
    glVertex3f(179.74, swapy - 975.73, 0);
    glVertex3f(148.45, swapy - 1008.41, 0);
    glVertex3f(128.56, swapy - 1042.75, 0);
    glVertex3f(128.56, swapy - 1050.67, 0);
    glVertex3f(138.31, swapy - 1056.54, 0);
    glVertex3f(155.98, swapy - 1059.2, 0);
    glVertex3f(197.41, swapy - 1046.79, 0);
    glVertex3f(232.75, swapy - 1028.74, 0);
    glVertex3f(265.65, swapy - 1007.41, 0);
    glVertex3f(276.61, swapy - 994.01, 0);
    glVertex3f(265.09, swapy - 990.01, 0);
    glVertex3f(243.68, swapy - 990.01, 0);
    glVertex3f(218.73, swapy - 990.01, 0);
    glVertex3f(190.14, swapy - 981.98, 0);

    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(179.74, swapy - 975.73, 0);
    glVertex3f(148.45, swapy - 1008.41, 0);
    glVertex3f(128.56, swapy - 1042.75, 0);
    glVertex3f(128.56, swapy - 1050.67, 0);
    glVertex3f(138.31, swapy - 1056.54, 0);
    glVertex3f(155.98, swapy - 1059.2, 0);
    glVertex3f(197.41, swapy - 1046.79, 0);
    glVertex3f(232.75, swapy - 1028.74, 0);
    glVertex3f(265.65, swapy - 1007.41, 0);
    glVertex3f(276.61, swapy - 994.01, 0);
    glVertex3f(265.09, swapy - 990.01, 0);
    glVertex3f(243.68, swapy - 990.01, 0);
    glVertex3f(218.73, swapy - 990.01, 0);
    glVertex3f(190.14, swapy - 981.98, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(180.96, swapy - 1025.08, 0);
    glVertex3f(145.01, swapy - 1053.71, 0);
    glVertex3f(159.64, swapy - 1022.03, 0);
    glVertex3f(132.22, swapy - 1050.67, 0);
    glEnd();
    // LEGS LEFT

    glBegin(GL_POLYGON);
    if (right) glColor3f(.11, 0.11, 0.11);
    else  glColor3f(0.98, 0.832, 0.25);
    glVertex3f(560.52, swapy - 965.98, 0);
    glVertex3f(575.15, swapy - 1023.86, 0);
    glVertex3f(578.8, swapy - 1062.85, 0);
    glVertex3f(575.15, swapy - 1084.79, 0);
    glVertex3f(564.79, swapy - 1089.66, 0);
    glVertex3f(544.07, swapy - 1084.79, 0);
    glVertex3f(514.83, swapy - 1055.54, 0);
    glVertex3f(473.4, swapy - 997.66, 0);
    glVertex3f(488.63, swapy - 996.7, 0);
    glVertex3f(531.46, swapy - 986.09, 0);
    glVertex3f(548.95, swapy - 976.34, 0);
    glVertex3f(559.6, swapy - 966.57, 0);


    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(560.52, swapy - 965.98, 0);
    glVertex3f(575.15, swapy - 1023.86, 0);
    glVertex3f(578.8, swapy - 1062.85, 0);
    glVertex3f(575.15, swapy - 1084.79, 0);
    glVertex3f(564.79, swapy - 1089.66, 0);
    glVertex3f(544.07, swapy - 1084.79, 0);
    glVertex3f(514.83, swapy - 1055.54, 0);
    glVertex3f(473.4, swapy - 997.66, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(532.5, swapy - 1039.09, 0);
    glVertex3f(559.31, swapy - 1086.61, 0);
    glVertex3f(553.82, swapy - 1032.39, 0);
    glVertex3f(572.1, swapy - 1084.18, 0);
    glEnd();


    // right red circle
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.976, 0.27, 0.196);
    glVertex3f(104, swapy - 472, 0);
    glVertex3f(123.99, swapy - 491.61, 0);
    glVertex3f(129.14, swapy - 510.15, 0);
    glVertex3f(128, swapy - 536, 0);
    glVertex3f(112, swapy - 560, 0);
    glVertex3f(113.33, swapy - 559.91, 0);
    glVertex3f(101.03, swapy - 546.49, 0);
    glVertex3f(94.34, swapy - 499.65, 0);
    glVertex3f(103.15, swapy - 473.2, 0);
    glEnd();
    glColor3f(0, 0, 0);

    glBegin(GL_LINE_LOOP);
    //glColor3f(0.976, 0.27, 0.196);
    glVertex3f(104, swapy - 472, 0);
    glVertex3f(123.99, swapy - 491.61, 0);
    glVertex3f(129.14, swapy - 510.15, 0);
    glVertex3f(128, swapy - 536, 0);
    glVertex3f(112, swapy - 560, 0);
    glVertex3f(113.33, swapy - 559.91, 0);
    glVertex3f(101.03, swapy - 546.49, 0);
    glVertex3f(94.34, swapy - 499.65, 0);
    glVertex3f(103.15, swapy - 473.2, 0);
    glEnd();
    // left red circle
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.976, 0.27, 0.196);
    glVertex3f(393.76, swapy - 497.79, 0);
    glVertex3f(405.84, swapy - 471.02, 0);
    glVertex3f(428.9, swapy - 461.75, 0);
    glVertex3f(458.55, swapy - 471.02, 0);
    glVertex3f(470.63, swapy - 488.535, 0);
    glVertex3f(476.13, swapy - 507.06, 0);
    glVertex3f(470.63, swapy - 526.62, 0);
    glVertex3f(447.57, swapy - 542.07, 0);
    glVertex3f(423.41, swapy - 542.07, 0);
    glVertex3f(405.84, swapy - 533.83, 0);
    glVertex3f(398.15, swapy - 526.62, 0);
    glVertex3f(393.76, swapy - 517.36, 0);
    glVertex3f(393.76, swapy - 507.06, 0);

    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(393.76, swapy - 497.79, 0);
    glVertex3f(405.84, swapy - 471.02, 0);
    glVertex3f(428.9, swapy - 461.75, 0);
    glVertex3f(458.55, swapy - 471.02, 0);
    glVertex3f(470.63, swapy - 488.535, 0);
    glVertex3f(476.13, swapy - 507.06, 0);
    glVertex3f(470.63, swapy - 526.62, 0);
    glVertex3f(447.57, swapy - 542.07, 0);
    glVertex3f(423.41, swapy - 542.07, 0);
    glVertex3f(405.84, swapy - 533.83, 0);
    glVertex3f(398.15, swapy - 526.62, 0);
    glVertex3f(393.76, swapy - 517.36, 0);
    glVertex3f(393.76, swapy - 507.06, 0);
    glEnd();


    // EYES OUTER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0, 0, 0);
    glVertex3f(380.37, swapy - 377.53, 0);
    glVertex3f(398.9, swapy - 382.47, 0);
    glVertex3f(414.35, swapy - 395.86, 0);
    glVertex3f(418.47, swapy - 418.51, 0);
    glVertex3f(409.2, swapy - 439.1, 0);
    glVertex3f(398.9, swapy - 451.46, 0);
    glVertex3f(375.22, swapy - 456.61, 0);
    glVertex3f(355.66, swapy - 451.46, 0);
    glVertex3f(344.33, swapy - 439.1, 0);
    glVertex3f(339.18, swapy - 418.51, 0);
    glVertex3f(344.33, swapy - 395.86, 0);
    glVertex3f(355.66, swapy - 382.47, 0);
    glVertex3f(369.04, swapy - 377.53, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(380.37, swapy - 377.53, 0);
    glVertex3f(398.9, swapy - 382.47, 0);
    glVertex3f(414.35, swapy - 395.86, 0);
    glVertex3f(418.47, swapy - 418.51, 0);
    glVertex3f(409.2, swapy - 439.1, 0);
    glVertex3f(398.9, swapy - 451.46, 0);
    glVertex3f(375.22, swapy - 456.61, 0);
    glVertex3f(355.66, swapy - 451.46, 0);
    glVertex3f(344.33, swapy - 439.1, 0);
    glVertex3f(339.18, swapy - 418.51, 0);
    glVertex3f(344.33, swapy - 395.86, 0);
    glVertex3f(355.66, swapy - 382.47, 0);
    glVertex3f(369.04, swapy - 377.53, 0);
    glEnd();

    // EYES INNER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(365.11, swapy - 384.53, 0);
    glVertex3f(378.67, swapy - 388.65, 0);
    glVertex3f(384.49, swapy - 400.39, 0);
    glVertex3f(384.49, swapy - 409.91, 0);
    glVertex3f(378.67, swapy - 418.51, 0);
    glVertex3f(365.11, swapy - 423.66, 0);
    glVertex3f(351.54, swapy - 414.14, 0);
    glVertex3f(351.54, swapy - 400.39, 0);
    glVertex3f(356.39, swapy - 394.05, 0);
    //glVertex3f(365.11, swapy - 384.53, 0);


    glEnd();

    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(365.11, swapy - 384.53, 0);
    glVertex3f(378.67, swapy - 388.65, 0);
    glVertex3f(384.49, swapy - 400.39, 0);
    glVertex3f(384.49, swapy - 409.91, 0);
    glVertex3f(378.67, swapy - 418.51, 0);
    glVertex3f(365.11, swapy - 423.66, 0);
    glVertex3f(351.54, swapy - 414.14, 0);
    glVertex3f(351.54, swapy - 400.39, 0);
    glVertex3f(356.39, swapy - 394.05, 0);

    glEnd();
    glLineWidth(5);

    // EYES OUTER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0, 0, 0);
    glVertex3f(191.95, swapy - 390.71, 0);
    glVertex3f(199.15, swapy - 399.98, 0);
    glVertex3f(203.27, swapy - 417.48, 0);
    glVertex3f(199.15, swapy - 438.07, 0);
    glVertex3f(191.95, swapy - 448.37, 0);
    glVertex3f(172.38, swapy - 454.55, 0);
    glVertex3f(157.97, swapy - 448.37, 0);
    glVertex3f(147.67, swapy - 428.81, 0);
    glVertex3f(147.67, swapy - 409.24, 0);
    glVertex3f(157.97, swapy - 390.71, 0);
    glVertex3f(172.38, swapy - 384.53, 0);
    glVertex3f(184.74, swapy - 384.53, 0);


    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(191.95, swapy - 390.71, 0);
    glVertex3f(199.15, swapy - 399.98, 0);
    glVertex3f(203.27, swapy - 417.48, 0);
    glVertex3f(199.15, swapy - 438.07, 0);
    glVertex3f(191.95, swapy - 448.37, 0);
    glVertex3f(172.38, swapy - 454.55, 0);
    glVertex3f(157.97, swapy - 448.37, 0);
    glVertex3f(147.67, swapy - 428.81, 0);
    glVertex3f(147.67, swapy - 409.24, 0);
    glVertex3f(157.97, swapy - 390.71, 0);
    glVertex3f(172.38, swapy - 384.53, 0);
    glVertex3f(184.74, swapy - 384.53, 0);

    glEnd();

    // EYES INNER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(184.02, swapy - 392.77, 0);
    glVertex3f(191.95, swapy - 397.92, 0);
    glVertex3f(196.06, swapy - 409.24, 0);
    glVertex3f(192, swapy - 416, 0);
    glVertex3f(184.02, swapy - 423.66, 0);
    glVertex3f(175.47, swapy - 423.66, 0);
    glVertex3f(170.32, swapy - 416.45, 0);
    glVertex3f(170.32, swapy - 409.24, 0);
    glVertex3f(175.47, swapy - 397.92, 0);



    glEnd();

    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(184.02, swapy - 392.77, 0);
    glVertex3f(191.95, swapy - 397.92, 0);
    glVertex3f(196.06, swapy - 409.24, 0);
    glVertex3f(192, swapy - 416, 0);
    glVertex3f(184.02, swapy - 423.66, 0);
    glVertex3f(175.47, swapy - 423.66, 0);
    glVertex3f(170.32, swapy - 416.45, 0);
    glVertex3f(170.32, swapy - 409.24, 0);
    glVertex3f(175.47, swapy - 397.92, 0);

    glEnd();
    glLineWidth(5);

    //right hand
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(161.26, swapy - 728.53, 0);
    glVertex3f(169.29, swapy - 806.17, 0);
    glVertex3f(185.36, swapy - 843.65, 0);
    glVertex3f(185.36, swapy - 869.08, 0);
    glVertex3f(201.42, swapy - 859.71, 0);
    glVertex3f(201.42, swapy - 875.77, 0);
    glVertex3f(220.16, swapy - 859.71, 0);
    glVertex3f(228.19, swapy - 875.77, 0);
    glVertex3f(237.56, swapy - 859.71, 0);
    glVertex3f(246.93, swapy - 869, 0);
    glVertex3f(253.62, swapy - 853.02, 0);
    glVertex3f(268.34, swapy - 853.02, 0);
    glVertex3f(253.62, swapy - 826.25, 0);
    glVertex3f(268.34, swapy - 778.06, 0);
    glVertex3f(260.31, swapy - 707.12, 0);
    glEnd();
    //LEFT hand
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(375, swapy - 699.09, 0);
    glVertex3f(360.7, swapy - 753.97, 0);
    glVertex3f(352.67, swapy - 807.51, 0);
    glVertex3f(360.7, swapy - 835.62, 0);
    glVertex3f(352.67, swapy - 854.36, 0);
    glVertex3f(352.67, swapy - 867.74, 0);
    glVertex3f(367.4, swapy - 861.05, 0);
    glVertex3f(375, swapy - 881.13, 0);
    glVertex3f(383.46, swapy - 867.74, 0);
    glVertex3f(391.49, swapy - 881.13, 0);
    glVertex3f(400.86, swapy - 867.74, 0);
    glVertex3f(414.24, swapy - 881.13, 0);
    glVertex3f(414.24, swapy - 867.74, 0);
    glVertex3f(427.63, swapy - 874.43, 0);
    glVertex3f(427.63, swapy - 861.05, 0);
    glVertex3f(459.76, swapy - 807.51, 0);
    glVertex3f(479.83, swapy - 764.68, 0);
    glVertex3f(487.86, swapy - 723.18, 0);
    glEnd();
    glPopMatrix();
    //glTranslatef(-1*transationPLayer2X, -1*transationPLayer2Y, 0);
    //glScalef(1/scaling, 1/scaling,1);
}
void drawPicakWin(bool right) {
    int swapy = 900;
    glPushMatrix();
    glScalef(scaling, scaling, 1);

    if (right) glTranslatef(1000 + xTransfer, transationPLayer2Y, 0);
    else glTranslatef(transationPLayer2X + xTransfer, transationPLayer2Y, 0);
    if (right) glScalef(-1, 1, 1);
    glLineWidth(5);


    // body with legs
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.11, 0.11);
    else glColor3f(0.992, 0.867, 0.286);
    glVertex3f(293.04, swapy - 991.21, 0);
    glVertex3f(305, swapy - 1000.99, 0);
    glVertex3f(279.88, swapy - 1021.32, 0);
    glVertex3f(206.93, swapy - 1042.84, 0);
    glVertex3f(176, swapy - 1048, 0);
    glVertex3f(163.88, swapy - 1042.84, 0);
    glVertex3f(151.92, swapy - 1042.84, 0);
    glVertex3f(147.13, swapy - 1036.86, 0);
    glVertex3f(136.37, swapy - 1030.88, 0);

    glVertex3f(163.88, swapy - 1008.16, 0);
    glVertex3f(206.93, swapy - 991.21, 0);
    glVertex3f(180.62, swapy - 971.09, 0);
    glVertex3f(157.9, swapy - 934.01, 0);
    glVertex3f(163.88, swapy - 898.14, 0);
    glVertex3f(199.76, swapy - 815.62, 0);
    glVertex3f(211.71, swapy - 773.76, 0);
    glVertex3f(180.62, swapy - 768.98, 0);
    glVertex3f(160, swapy - 752, 0);
    glVertex3f(152, swapy - 736, 0);
    glVertex3f(144, swapy - 720, 0);
    glVertex3f(147.13, swapy - 691.24, 0);
    glVertex3f(180.62, swapy - 642.21, 0);
    glVertex3f(211.71, swapy - 626.66, 0);
    glVertex3f(157.9, swapy - 574.04, 0);
    glVertex3f(136.37, swapy - 544.14, 0);
    glVertex3f(128, swapy - 507.07, 0);
    glVertex3f(147.13, swapy - 467.6, 0);
    glVertex3f(157.9, swapy - 444.88, 0);
    glVertex3f(168.66, swapy - 379.11, 0);
    glVertex3f(185.4, swapy - 337.25, 0);
    glVertex3f(227.26, swapy - 291.8, 0);
    glVertex3f(279.88, swapy - 281.04, 0);

    glVertex3f(284.67, swapy - 279.85, 0);
    glVertex3f(338.48, swapy - 273.87, 0);
    glVertex3f(371.44, swapy - 276.66, 0);
    glVertex3f(409.04, swapy - 279.85, 0);
    glVertex3f(454.49, swapy - 297.78, 0);
    glVertex3f(481.99, swapy - 318.11, 0);
    glVertex3f(503.89, swapy - 334.3, 0);
    glVertex3f(520, swapy - 353, 0);
    glVertex3f(537, swapy - 378, 0);
    glVertex3f(538.2, swapy - 382.69, 0);
    glVertex3f(538.2, swapy - 493.92, 0);
    glVertex3f(524.24, swapy - 624.27, 0);
    glVertex3f(531.03, swapy - 704.4, 0);
    glVertex3f(574.08, swapy - 888.57, 0);
    glVertex3f(562.12, swapy - 949.56, 0);
    glVertex3f(515.48, swapy - 998.59, 0);
    glVertex3f(544, swapy - 1000, 0);
    glVertex3f(589.63, swapy - 1023.71, 0);
    glVertex3f(589.63, swapy - 1035.67, 0);
    glVertex3f(577.25, swapy - 1046.67, 0);
    glVertex3f(538.2, swapy - 1048.67, 0);
    glVertex3f(459.27, swapy - 1035.67, 0);
    glVertex3f(429.37, swapy - 1016.53, 0);
    glVertex3f(436.55, swapy - 998.59, 0);
    glVertex3f(394.69, swapy - 989.03, 0);
    glVertex3f(330.11, swapy - 985.03, 0);
    glVertex3f(305.44, swapy - 989.081, 0);

    glEnd();
    glLineWidth(7);

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(293.04, swapy - 991.21, 0);
    glVertex3f(305, swapy - 1000.99, 0);
    glVertex3f(279.88, swapy - 1021.32, 0);
    glVertex3f(206.93, swapy - 1042.84, 0);
    glVertex3f(176, swapy - 1048, 0);
    glVertex3f(163.88, swapy - 1042.84, 0);
    glVertex3f(151.92, swapy - 1042.84, 0);
    glVertex3f(147.13, swapy - 1036.86, 0);
    glVertex3f(136.37, swapy - 1030.88, 0);

    glVertex3f(163.88, swapy - 1008.16, 0);
    glVertex3f(206.93, swapy - 991.21, 0);
    glVertex3f(180.62, swapy - 971.09, 0);
    glVertex3f(157.9, swapy - 934.01, 0);
    glVertex3f(163.88, swapy - 898.14, 0);
    glVertex3f(199.76, swapy - 815.62, 0);
    glVertex3f(211.71, swapy - 773.76, 0);
    glVertex3f(180.62, swapy - 768.98, 0);
    glVertex3f(160, swapy - 752, 0);
    glVertex3f(152, swapy - 736, 0);
    glVertex3f(144, swapy - 720, 0);
    glVertex3f(147.13, swapy - 691.24, 0);
    glVertex3f(180.62, swapy - 642.21, 0);
    glVertex3f(211.71, swapy - 626.66, 0);
    glVertex3f(157.9, swapy - 574.04, 0);
    glVertex3f(136.37, swapy - 544.14, 0);
    glVertex3f(128, swapy - 507.07, 0);
    glVertex3f(147.13, swapy - 467.6, 0);
    glVertex3f(157.9, swapy - 444.88, 0);
    glVertex3f(168.66, swapy - 379.11, 0);
    glVertex3f(185.4, swapy - 337.25, 0);
    glVertex3f(227.26, swapy - 291.8, 0);
    glVertex3f(279.88, swapy - 281.04, 0);

    glVertex3f(284.67, swapy - 279.85, 0);
    glVertex3f(338.48, swapy - 273.87, 0);
    glVertex3f(371.44, swapy - 276.66, 0);
    glVertex3f(409.04, swapy - 279.85, 0);
    glVertex3f(454.49, swapy - 297.78, 0);
    glVertex3f(481.99, swapy - 318.11, 0);
    glVertex3f(503.89, swapy - 334.3, 0);
    glVertex3f(520, swapy - 353, 0);
    glVertex3f(537, swapy - 378, 0);
    glVertex3f(538.2, swapy - 382.69, 0);
    glVertex3f(538.2, swapy - 493.92, 0);
    glVertex3f(524.24, swapy - 624.27, 0);
    glVertex3f(531.03, swapy - 704.4, 0);
    glVertex3f(574.08, swapy - 888.57, 0);
    glVertex3f(562.12, swapy - 949.56, 0);
    glVertex3f(515.48, swapy - 998.59, 0);
    glVertex3f(544, swapy - 1000, 0);
    glVertex3f(589.63, swapy - 1023.71, 0);
    glVertex3f(589.63, swapy - 1035.67, 0);
    glVertex3f(577.25, swapy - 1046.67, 0);
    glVertex3f(538.2, swapy - 1048.67, 0);
    glVertex3f(459.27, swapy - 1035.67, 0);
    glVertex3f(429.37, swapy - 1016.53, 0);
    glVertex3f(436.55, swapy - 998.59, 0);
    glVertex3f(394.69, swapy - 989.03, 0);
    glVertex3f(330.11, swapy - 985.03, 0);
    glVertex3f(305.44, swapy - 989.081, 0);

    glEnd();
    glLineWidth(5);

    // face small part
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(208, swapy - 624, 0);
    glVertex3f(242.81, swapy - 645.8, 0);
    glEnd();
    // right hand
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(216.5, swapy - 657.17, 0);
    glVertex3f(266.73, swapy - 672.11, 0);
    glVertex3f(294.23, swapy - 691.24, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(214.11, swapy - 772.56, 0);
    glVertex3f(263.14, swapy - 772.56, 0);
    glEnd();

    // left hand
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(435.35, swapy - 638.62, 0);
    glVertex3f(365.99, swapy - 641.41, 0);
    glVertex3f(302.6, swapy - 676.5, 0);
    glVertex3f(264.33, swapy - 731.9, 0);
    glVertex3f(255.96, swapy - 760.61, 0);
    glVertex3f(275.1, swapy - 784.52, 0);
    glVertex3f(295.43, swapy - 792.9, 0);
    glVertex3f(336.09, swapy - 796.68, 0);
    glVertex3f(421, swapy - 784.52, 0);
    glEnd();

    // body low part
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(254.77, swapy - 997.4, 0);
    glVertex3f(290.66, swapy - 991.5, 0);
    glVertex3f(305.44, swapy - 989.08, 0);
    glVertex3f(330.11, swapy - 985.03, 0);
    glVertex3f(394.69, swapy - 989.03, 0);
    glVertex3f(435.35, swapy - 997.4, 0);
    glVertex3f(459.27, swapy - 1003.59, 0);
    glEnd();
    // right leg finger
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(184, swapy - 1008, 0);
    glVertex3f(144, swapy - 1040, 0);

    glVertex3f(200, swapy - 1016, 0);
    glVertex3f(168, swapy - 1043.99, 0);
    glEnd();
    // left leg finger
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(528, swapy - 1024, 0);
    glVertex3f(578.22, swapy - 1040.1, 0);
    glVertex3f(539.4, swapy - 1008.98, 0);
    glVertex3f(586.78, swapy - 1029.1, 0);
    glEnd();

    // nose //here
    if (right) {
        glColor3f(0.71, 0.423, 0);
        glPushMatrix();
        //glRotatef(90, 1, 0, 0);
        glBegin(GL_QUADS);

        glVertex3f(240.58, swapy - 454.45, 0);
        glVertex3f(265.53, swapy - 473.58, 0);
        glVertex3f(278.69, swapy - 454.45, 0);


        glVertex3f(240.58, swapy - 473.58, 0);


        glEnd();
        glPopMatrix();
    }
    else {
        glColor3f(0, 0, 0);
        glBegin(GL_TRIANGLES);
        glVertex3f(250.58, swapy - 454.45, 0);
        glVertex3f(265.53, swapy - 473.58, 0);
        glVertex3f(278.69, swapy - 454.45, 0);
        glEnd();
    }

    // mouse
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(216.5, swapy - 502.29, 0);
    glVertex3f(224.87, swapy - 514.25, 0);
    glVertex3f(242.81, swapy - 514.25, 0);
    glVertex3f(265.53, swapy - 502.29, 0);
    glVertex3f(297.82, swapy - 526.2, 0);
    glVertex3f(324.13, swapy - 526.2, 0);
    glVertex3f(345.66, swapy - 514.25, 0);
    glEnd();
    //side triangles
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else glColor3f(0.643, 0.341, 0.125);

    glVertex3f(572.22, swapy - 700.81, 0);
    glVertex3f(503.52, swapy - 725.92, 0);
    glVertex3f(496.34, swapy - 735.49, 0);
    glVertex3f(503.52, swapy - 742.67, 0);
    glVertex3f(579.4, swapy - 742.67, 0);
    glEnd();



    glBegin(GL_TRIANGLES);
    if (right) glColor3f(0.58, 0, 0.062);
    else glColor3f(0.643, 0.341, 0.125);
    glVertex3f(589.73, swapy - 826.38, 0);
    glVertex3f(520, swapy - 872, 0);
    glVertex3f(596.9, swapy - 873.02, 0);
    glEnd();


    // RIGHT CIRCLE
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.9137, 0.302, 0.1412);

    glVertex3f(424.59, swapy - 489.13, 0);
    glVertex3f(453.29, swapy - 487.35, 0);
    glVertex3f(483.19, swapy - 502.29, 0);
    glVertex3f(491.17, swapy - 536.97, 0);
    glVertex3f(477.21, swapy - 570.45, 0);

    glVertex3f(453.29, swapy - 581.22, 0);
    glVertex3f(432.96, swapy - 581.22, 0);
    glVertex3f(412.63, swapy - 575.24, 0);
    glVertex3f(400.67, swapy - 558.49, 0);
    glVertex3f(393.49, swapy - 542.95, 0);

    glVertex3f(393.49, swapy - 526.2, 0);
    glVertex3f(400.67, swapy - 502.29, 0);

    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(424.59, swapy - 489.13, 0);
    glVertex3f(453.29, swapy - 487.35, 0);
    glVertex3f(483.19, swapy - 502.29, 0);
    glVertex3f(491.17, swapy - 536.97, 0);
    glVertex3f(477.21, swapy - 570.45, 0);

    glVertex3f(453.29, swapy - 581.22, 0);
    glVertex3f(432.96, swapy - 581.22, 0);
    glVertex3f(412.63, swapy - 575.24, 0);
    glVertex3f(400.67, swapy - 558.49, 0);
    glVertex3f(393.49, swapy - 542.95, 0);

    glVertex3f(393.49, swapy - 526.2, 0);
    glVertex3f(400.67, swapy - 502.29, 0);
    glEnd();

    // LEFT CIRCLE
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.9137, 0.302, 0.1412);
    glVertex3f(149.53, swapy - 462.82, 0);
    glVertex3f(168.66, swapy - 473.98, 0);
    glVertex3f(183.01, swapy - 497.5, 0);
    glVertex3f(183.01, swapy - 529.79, 0);
    glVertex3f(159.88, swapy - 558.71, 0);

    glVertex3f(149.53, swapy - 565.67, 0);
    glVertex3f(136.37, swapy - 544.14, 0);
    glVertex3f(128, swapy - 507.07, 0);
    glVertex3f(147.13, swapy - 467.6, 0);
    glVertex3f(149.02, swapy - 463.62, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(149.53, swapy - 462.82, 0);
    glVertex3f(168.66, swapy - 473.98, 0);
    glVertex3f(183.01, swapy - 497.5, 0);
    glVertex3f(183.01, swapy - 529.79, 0);
    glVertex3f(159.88, swapy - 558.71, 0);

    glVertex3f(149.53, swapy - 565.67, 0);
    glVertex3f(136.37, swapy - 544.14, 0);
    glVertex3f(128, swapy - 507.07, 0);
    glVertex3f(147.13, swapy - 467.6, 0);
    glVertex3f(149.02, swapy - 463.62, 0);
    glEnd();

    // RIGHT EAR
    glBegin(GL_POLYGON);
    if (right) glColor3f(.11, 0.11, 0.11);
    else glColor3f(0.992, 0.867, 0.286);
    glVertex3f(205.73, swapy - 312.14, 0);
    glVertex3f(236.83, swapy - 157.86, 0);
    glVertex3f(291.84, swapy - 64.58, 0);
    glVertex3f(367.18, swapy - 0, 0);
    glVertex3f(367.18, swapy - 53.82, 0);
    glVertex3f(355.22, swapy - 129.16, 0);
    glVertex3f(284.67, swapy - 279.85, 0);
    glVertex3f(227.26, swapy - 291.8, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(205.73, swapy - 312.14, 0);
    glVertex3f(236.83, swapy - 157.86, 0);
    glVertex3f(291.84, swapy - 64.58, 0);
    glVertex3f(367.18, swapy - 0, 0);
    glVertex3f(367.18, swapy - 53.82, 0);
    glVertex3f(355.22, swapy - 129.16, 0);
    glVertex3f(284.67, swapy - 279.85, 0);
    glVertex3f(227.26, swapy - 291.8, 0);
    glEnd();

    //INNER EAR
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else glColor3f(0, 0, 0);
    glVertex3f(280, swapy - 80, 0);
    glVertex3f(236.83, swapy - 157.86, 0);
    glVertex3f(291.84, swapy - 64.58, 0);
    glVertex3f(367.18, swapy - 0, 0);
    glVertex3f(367.18, swapy - 53.82, 0);
    glVertex3f(355.22, swapy - 129.16, 0);

    glVertex3f(336.09, swapy - 172.21, 0);
    glVertex3f(307.39, swapy - 107.63, 0);
    glEnd();
    // LEFT EAR
    glBegin(GL_POLYGON);
    if (right) glColor3f(.11, 0.11, 0.11);
    else glColor3f(0.992, 0.867, 0.286);
    glVertex3f(459.88, swapy - 328, 0);
    glVertex3f(488, swapy - 316.66, 0);
    glVertex3f(602.78, swapy - 289.41, 0);
    glVertex3f(717.59, swapy - 289.41, 0);
    glVertex3f(795.32, swapy - 312.14, 0);
    glVertex3f(765.43, swapy - 330.07, 0);
    glVertex3f(694.87, swapy - 357.58, 0);
    glVertex3f(611.15, swapy - 375.52, 0);
    glVertex3f(502.32, swapy - 382.69, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(459.88, swapy - 328, 0);
    glVertex3f(488, swapy - 316.66, 0);
    glVertex3f(602.78, swapy - 289.41, 0);
    glVertex3f(717.59, swapy - 289.41, 0);
    glVertex3f(795.32, swapy - 312.14, 0);
    glVertex3f(765.43, swapy - 330.07, 0);
    glVertex3f(694.87, swapy - 357.58, 0);
    glVertex3f(611.15, swapy - 375.52, 0);
    glVertex3f(502.32, swapy - 382.69, 0);
    glEnd();
    //INNER EAR
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else glColor3f(0, 0, 0);
    glVertex3f(704, swapy - 328, 0);
    glVertex3f(717.59, swapy - 289.41, 0);
    glVertex3f(795.32, swapy - 312.14, 0);
    glVertex3f(765.43, swapy - 330.07, 0);
    glVertex3f(694.87, swapy - 357.58, 0);

    glVertex3f(654.21, swapy - 365.95, 0);
    glVertex3f(676.93, swapy - 344.43, 0);
    glEnd();

    //LEFT EYE OUTER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0, 0, 0);
    glVertex3f(382.73, swapy - 426.94, 0);
    glVertex3f(376.56, swapy - 443.49, 0);
    glVertex3f(382.73, swapy - 460.43, 0);
    glVertex3f(389.91, swapy - 471.19, 0);
    glVertex3f(400.67, swapy - 480.76, 0);
    glVertex3f(411.43, swapy - 485.54, 0);
    glVertex3f(424.59, swapy - 480.76, 0);

    glVertex3f(437.74, swapy - 471.19, 0);
    glVertex3f(446.11, swapy - 450.86, 0);
    glVertex3f(446.11, swapy - 426.94, 0);
    glVertex3f(437.74, swapy - 416.18, 0);
    glVertex3f(424.59, swapy - 405.42, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(382.73, swapy - 426.94, 0);
    glVertex3f(376.56, swapy - 443.49, 0);
    glVertex3f(382.73, swapy - 460.43, 0);
    glVertex3f(389.91, swapy - 471.19, 0);
    glVertex3f(400.67, swapy - 480.76, 0);
    glVertex3f(411.43, swapy - 485.54, 0);
    glVertex3f(424.59, swapy - 480.76, 0);

    glVertex3f(437.74, swapy - 471.19, 0);
    glVertex3f(446.11, swapy - 450.86, 0);
    glVertex3f(446.11, swapy - 426.94, 0);
    glVertex3f(437.74, swapy - 416.18, 0);
    glVertex3f(424.59, swapy - 405.42, 0);
    glEnd();
    //INNER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(411.34, swapy - 416.18, 0);
    glVertex3f(423.39, swapy - 426.94, 0);
    glVertex3f(418.61, swapy - 437.71, 0);
    glVertex3f(407.34, swapy - 445.71, 0);
    glVertex3f(397.08, swapy - 437.71, 0);
    glVertex3f(397.08, swapy - 426.94, 0);
    glEnd();
    // WIDTH
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0, 0, 0);
    glVertex3f(411.34, swapy - 416.18, 0);
    glVertex3f(423.39, swapy - 426.94, 0);
    glVertex3f(418.61, swapy - 437.71, 0);
    glVertex3f(407.34, swapy - 445.71, 0);
    glVertex3f(397.08, swapy - 437.71, 0);
    glVertex3f(397.08, swapy - 426.94, 0);
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(435.35, swapy - 393.46, 0);
    glVertex3f(360, swapy - 440, 0);
    glEnd();
    //RIGHT EYE OUTER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0, 0, 0);
    glVertex3f(214.11, swapy - 393.46, 0);
    glVertex3f(200.95, swapy - 400.63, 0);
    glVertex3f(188.38, swapy - 421.36, 0);
    glVertex3f(183.01, swapy - 443.69, 0);
    glVertex3f(191.38, swapy - 465.21, 0);
    glVertex3f(200.95, swapy - 472.39, 0);
    glVertex3f(214.11, swapy - 465.21, 0);

    glVertex3f(227.26, swapy - 449.67, 0);
    glVertex3f(233.24, swapy - 429.34, 0);
    glVertex3f(233.24, swapy - 410.2, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(214.11, swapy - 393.46, 0);
    glVertex3f(200.95, swapy - 400.63, 0);
    glVertex3f(188.38, swapy - 421.36, 0);
    glVertex3f(183.01, swapy - 443.69, 0);
    glVertex3f(191.38, swapy - 465.21, 0);
    glVertex3f(200.95, swapy - 472.39, 0);
    glVertex3f(214.11, swapy - 465.21, 0);

    glVertex3f(227.26, swapy - 449.67, 0);
    glVertex3f(233.24, swapy - 429.34, 0);
    glVertex3f(233.24, swapy - 410.2, 0);
    glEnd();
    //INNER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(221.48, swapy - 406.2, 0);
    glVertex3f(210.52, swapy - 412.59, 0);
    glVertex3f(204.54, swapy - 423.36, 0);
    glVertex3f(210.52, swapy - 432.92, 0);
    glVertex3f(221.48, swapy - 432.92, 0);
    glVertex3f(227.26, swapy - 412.59, 0);
    glVertex3f(221.48, swapy - 406.2, 0);
    glEnd();
    // WIDTH
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0, 0, 0);
    glVertex3f(221.48, swapy - 406.2, 0);
    glVertex3f(210.52, swapy - 412.59, 0);
    glVertex3f(204.54, swapy - 423.36, 0);
    glVertex3f(210.52, swapy - 432.92, 0);
    glVertex3f(221.48, swapy - 432.92, 0);
    glVertex3f(227.26, swapy - 412.59, 0);
    glVertex3f(221.48, swapy - 406.2, 0);
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(202.15, swapy - 382.69, 0);
    glVertex3f(248, swapy - 424, 0);
    glEnd();

    // TAIL
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.1, 0.11);
    else  glColor3f(0.992, 0.867, 0.286);

    glVertex3f(531.04, swapy - 577.01, 0);
    glVertex3f(541.79, swapy - 487.94, 0);
    glVertex3f(541.79, swapy - 416.18, 0);
    glVertex3f(692.47, swapy - 366.15, 0);
    glVertex3f(857.51, swapy - 328.88, 0);
    glVertex3f(839.57, swapy - 551.32, 0);
    glVertex3f(588.43, swapy - 571.65, 0);
    glEnd();
    // WIDTH
    //glLineWidth(3);

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(531.04, swapy - 577.01, 0);
    glVertex3f(541.79, swapy - 487.94, 0);
    glVertex3f(541.79, swapy - 416.18, 0);
    glVertex3f(692.47, swapy - 366.15, 0);
    glVertex3f(857.51, swapy - 328.88, 0);
    glVertex3f(839.57, swapy - 551.32, 0);
    glVertex3f(588.43, swapy - 571.65, 0);
    glEnd();

    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.1, 0.11);
    else  glColor3f(0.992, 0.867, 0.286);
    glVertex3f(531.04, swapy - 577.01, 0);
    glVertex3f(588.43, swapy - 571.65, 0);
    glVertex3f(525.05, swapy - 626.66, 0);
    glEnd();
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.1, 0.11);
    else  glColor3f(0.992, 0.867, 0.286);
    glVertex3f(588.43, swapy - 571.65, 0);
    glVertex3f(525.05, swapy - 626.66, 0);
    glVertex3f(577.67, swapy - 678.09, 0);
    glVertex3f(672.14, swapy - 655.36, 0);
    glEnd();
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.1, 0.11);
    else  glColor3f(0.992, 0.867, 0.286);
    glVertex3f(577.67, swapy - 678.09, 0);
    glVertex3f(672.14, swapy - 655.36, 0);
    glVertex3f(588.43, swapy - 741.47, 0);
    glVertex3f(537, swapy - 730.71, 0);
    glEnd();

    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.1, 0.11);
    else  glColor3f(0.992, 0.867, 0.286);
    glVertex3f(537, swapy - 730.71, 0);
    glVertex3f(588.43, swapy - 741.47, 0);
    glVertex3f(546.57, swapy - 766.58, 0);
    glEnd();



    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0.643, 0.341, 0.125);
    glVertex3f(588.43, swapy - 741.47, 0);
    glVertex3f(546.57, swapy - 766.58, 0);
    glVertex3f(588.43, swapy - 792.9, 0);
    glVertex3f(642.25, swapy - 780.94, 0);

    glEnd();
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0.643, 0.341, 0.125);
    glVertex3f(588.43, swapy - 792.9, 0);
    glVertex3f(642.25, swapy - 780.94, 0);
    glVertex3f(572.88, swapy - 871.83, 0);
    glVertex3f(558.53, swapy - 827.58, 0);
    glEnd();
    // WIDTH
    glLineWidth(7);

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(588.43, swapy - 571.65, 0);
    glVertex3f(672.14, swapy - 655.36, 0);
    glVertex3f(588.43, swapy - 741.47, 0);
    glVertex3f(642.25, swapy - 780.94, 0);
    glVertex3f(572.88, swapy - 871.83, 0);
    glVertex3f(558.53, swapy - 827.58, 0);
    glVertex3f(588.43, swapy - 792.9, 0);
    glVertex3f(546.57, swapy - 766.58, 0);
    glVertex3f(537, swapy - 730.71, 0);
    glVertex3f(577.67, swapy - 678.09, 0);
    glVertex3f(525.05, swapy - 626.66, 0);
    glVertex3f(531.04, swapy - 577.01, 0);
    glEnd();
    //
    glPopMatrix();

}
void drawPicakFight(bool right) {
    int swapy = 900;
    //glTranslatef(<#GLfloat x#>, <#GLfloat y#>, <#GLfloat z#>)
    //glScalef(-1, 1,1);
    glPushMatrix();

    glLineWidth(5);
    //glScalef(-1, 1,1);
    glScalef(scaling, scaling, 1);
    if (right) glTranslatef(1000, transationPLayer2Y, 0);
    else glTranslatef(transationPLayer2X, transationPLayer2Y, 0);
    if (right) glScalef(-1, 1, 1);
    //glTranslatef(<#GLfloat x#>, <#GLfloat y#>, <#GLfloat z#>)
    // RIGHT EAR
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.11, 0.11);
    else glColor3f(0.992, 0.867, 0.286);
    glVertex3f(128, swapy - 323.31, 0);
    glVertex3f(222.26, swapy - 144.06, 0);
    glVertex3f(305.25, swapy - 57.05, 0);
    glVertex3f(322.65, swapy - 46.34, 0);
    glVertex3f(333.36, swapy - 46.34, 0);
    glVertex3f(333.36, swapy - 101.22, 0);
    glVertex3f(295.88, swapy - 192.24, 0);
    glVertex3f(247.69, swapy - 269.88, 0);
    glVertex3f(228.08, swapy - 274.25, 0);
    glVertex3f(186.07, swapy - 286.93, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    //glColor3f( 0.851,0.251,0.251);
    glColor3f(0, 0, 0);
    glVertex3f(128, swapy - 323.31, 0);
    glVertex3f(222.26, swapy - 144.06, 0);
    glVertex3f(305.25, swapy - 57.05, 0);
    glVertex3f(322.65, swapy - 46.34, 0);
    glVertex3f(333.36, swapy - 46.34, 0);
    glVertex3f(333.36, swapy - 101.22, 0);
    glVertex3f(295.88, swapy - 192.24, 0);
    glVertex3f(247.69, swapy - 269.88, 0);
    glVertex3f(228.08, swapy - 274.25, 0);
    glVertex3f(186.07, swapy - 286.93, 0);
    glEnd();

    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else glColor3f(0, 0, 0);
    glVertex3f(253.46, swapy - 112.42, 0);
    glVertex3f(305.25, swapy - 57.05, 0);
    glVertex3f(322.65, swapy - 46.34, 0);
    glVertex3f(333.36, swapy - 46.34, 0);
    glVertex3f(333.36, swapy - 101.22, 0);
    glVertex3f(295.88, swapy - 192.24, 0);
    glVertex3f(285.75, swapy - 209.64, 0);
    glVertex3f(273.82, swapy - 148.2, 0);
    glEnd();

    // BODY
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.11, 0.11);
    else glColor3f(0.992, 0.867, 0.286);
    //glColor3f(0.992, 0.867, 0.286);
    glVertex3f(155.33, swapy - 651.36, 0);
    glVertex3f(137.93, swapy - 636.63, 0);
    glVertex3f(116.52, swapy - 619.23, 0);
    glVertex3f(99.11, swapy - 600.49, 0);
    glVertex3f(75.02, swapy - 581.76, 0);
    glVertex3f(61.64, swapy - 561.68, 0);
    glVertex3f(50.93, swapy - 534.91, 0);
    glVertex3f(50.93, swapy - 504.12, 0);
    glVertex3f(61.64, swapy - 480.03, 0);
    glVertex3f(75.02, swapy - 442.55, 0);
    glVertex3f(84.39, swapy - 399.71, 0);
    glVertex3f(99.11, swapy - 359.56, 0);
    glVertex3f(131.24, swapy - 320.74, 0);
    glVertex3f(186.07, swapy - 286.93, 0);
    glVertex3f(228.08, swapy - 274.25, 0);
    glVertex3f(247.69, swapy - 267.2, 0);
    glVertex3f(286.51, swapy - 263.18, 0);
    glVertex3f(333.36, swapy - 269.88, 0);
    glVertex3f(408.32, swapy - 293.97, 0);
    glVertex3f(408, swapy - 296, 0);
    glVertex3f(448, swapy - 320, 0);
    glVertex3f(475.24, swapy - 360.9, 0);
    glVertex3f(481.993, swapy - 375.62, 0);
    glVertex3f(488.63, swapy - 402.39, 0);
    glVertex3f(495.32, swapy - 434.52, 0);
    glVertex3f(495.32, swapy - 474.67, 0);
    glVertex3f(488.63, swapy - 514.83, 0);
    glVertex3f(480, swapy - 560, 0);
    glVertex3f(481.93, swapy - 584.43, 0);
    glVertex3f(488.63, swapy - 617.9, 0);
    glVertex3f(506.03, swapy - 652.7, 0);
    glVertex3f(531.46, swapy - 694.19, 0);
    glVertex3f(542.17, swapy - 715.61, 0);
    glVertex3f(555.55, swapy - 746.39, 0);
    glVertex3f(567.6, swapy - 779.86, 0);
    glVertex3f(580.99, swapy - 825.37, 0);
    glVertex3f(589.02, swapy - 850.8, 0);
    glVertex3f(589.02, swapy - 893.63, 0);
    glVertex3f(580.99, swapy - 927.1, 0);
    glVertex3f(567.6, swapy - 959.22, 0);
    glVertex3f(548.95, swapy - 976.34, 0);
    glVertex3f(531.46, swapy - 986.09, 0);
    glVertex3f(488.63, swapy - 996.7, 0);
    glVertex3f(449.81, swapy - 996.7, 0);
    glVertex3f(417.69, swapy - 990.01, 0);
    glVertex3f(390.91, swapy - 981.98, 0);
    glVertex3f(369.5, swapy - 981.98, 0);
    glVertex3f(338.71, swapy - 981.98, 0);
    glVertex3f(314.62, swapy - 981.98, 0);
    glVertex3f(293.67, swapy - 986.09, 0);
    glVertex3f(265.09, swapy - 990.01, 0);
    glVertex3f(243.68, swapy - 990.01, 0);
    glVertex3f(218.73, swapy - 990.01, 0);
    glVertex3f(190.14, swapy - 981.98, 0);
    glVertex3f(172.73, swapy - 968.59, 0);
    glVertex3f(152.66, swapy - 947.17, 0);
    glVertex3f(132.58, swapy - 920.4, 0);
    glVertex3f(121.87, swapy - 878.91, 0);
    glVertex3f(121.87, swapy - 850.8, 0);
    glVertex3f(121.87, swapy - 825.37, 0);
    glVertex3f(132.58, swapy - 779.86, 0);
    glVertex3f(115.18, swapy - 790.57, 0);
    glVertex3f(91.08, swapy - 790.57, 0);
    glVertex3f(65.65, swapy - 779.86, 0);
    glVertex3f(48.25, swapy - 762.46, 0);
    glVertex3f(41.56, swapy - 734.35, 0);
    glVertex3f(41.56, swapy - 700.88, 0);
    glVertex3f(57.89, swapy - 671.71, 0);
    glVertex3f(91.08, swapy - 644.67, 0);
    glVertex3f(108.48, swapy - 639.42, 0);
    glVertex3f(118.21, swapy - 644.67, 0);
    glVertex3f(108.48, swapy - 652.7, 0);
    glVertex3f(125.52, swapy - 656.48, 0);
    glVertex3f(132.58, swapy - 660.73, 0);
    glVertex3f(143.29, swapy - 644.67, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(155.33, swapy - 651.36, 0);
    glVertex3f(137.93, swapy - 636.63, 0);
    glVertex3f(116.52, swapy - 619.23, 0);
    glVertex3f(99.11, swapy - 600.49, 0);
    glVertex3f(75.02, swapy - 581.76, 0);
    glVertex3f(61.64, swapy - 561.68, 0);
    glVertex3f(50.93, swapy - 534.91, 0);
    glVertex3f(50.93, swapy - 504.12, 0);
    glVertex3f(61.64, swapy - 480.03, 0);
    glVertex3f(75.02, swapy - 442.55, 0);
    glVertex3f(84.39, swapy - 399.71, 0);
    glVertex3f(99.11, swapy - 359.56, 0);
    glVertex3f(131.24, swapy - 320.74, 0);
    glVertex3f(186.07, swapy - 286.93, 0);
    glVertex3f(228.08, swapy - 274.25, 0);
    glVertex3f(247.69, swapy - 267.2, 0);
    glVertex3f(286.51, swapy - 263.18, 0);
    glVertex3f(333.36, swapy - 269.88, 0);
    glVertex3f(408.32, swapy - 293.97, 0);
    glVertex3f(408, swapy - 296, 0);
    glVertex3f(448, swapy - 320, 0);
    glVertex3f(475.24, swapy - 360.9, 0);
    glVertex3f(481.993, swapy - 375.62, 0);
    glVertex3f(488.63, swapy - 402.39, 0);
    glVertex3f(495.32, swapy - 434.52, 0);
    glVertex3f(495.32, swapy - 474.67, 0);
    glVertex3f(488.63, swapy - 514.83, 0);
    glVertex3f(480, swapy - 560, 0);
    glVertex3f(481.93, swapy - 584.43, 0);
    glVertex3f(488.63, swapy - 617.9, 0);
    glVertex3f(506.03, swapy - 652.7, 0);
    glVertex3f(531.46, swapy - 694.19, 0);
    glVertex3f(542.17, swapy - 715.61, 0);
    glVertex3f(555.55, swapy - 746.39, 0);
    glVertex3f(567.6, swapy - 779.86, 0);
    glVertex3f(580.99, swapy - 825.37, 0);
    glVertex3f(589.02, swapy - 850.8, 0);
    glVertex3f(589.02, swapy - 893.63, 0);
    glVertex3f(580.99, swapy - 927.1, 0);
    glVertex3f(567.6, swapy - 959.22, 0);
    glVertex3f(548.95, swapy - 976.34, 0);
    glVertex3f(531.46, swapy - 986.09, 0);
    glVertex3f(488.63, swapy - 996.7, 0);
    glVertex3f(449.81, swapy - 996.7, 0);
    glVertex3f(417.69, swapy - 990.01, 0);
    glVertex3f(390.91, swapy - 981.98, 0);
    glVertex3f(369.5, swapy - 981.98, 0);
    glVertex3f(338.71, swapy - 981.98, 0);
    glVertex3f(314.62, swapy - 981.98, 0);
    glVertex3f(293.67, swapy - 986.09, 0);
    glVertex3f(265.09, swapy - 990.01, 0);
    glVertex3f(243.68, swapy - 990.01, 0);
    glVertex3f(218.73, swapy - 990.01, 0);
    glVertex3f(190.14, swapy - 981.98, 0);
    glVertex3f(172.73, swapy - 968.59, 0);
    glVertex3f(152.66, swapy - 947.17, 0);
    glVertex3f(132.58, swapy - 920.4, 0);
    glVertex3f(121.87, swapy - 878.91, 0);
    glVertex3f(121.87, swapy - 850.8, 0);
    glVertex3f(121.87, swapy - 825.37, 0);
    glVertex3f(132.58, swapy - 779.86, 0);
    glVertex3f(115.18, swapy - 790.57, 0);
    glVertex3f(91.08, swapy - 790.57, 0);
    glVertex3f(65.65, swapy - 779.86, 0);
    glVertex3f(48.25, swapy - 762.46, 0);
    glVertex3f(41.56, swapy - 734.35, 0);
    glVertex3f(41.56, swapy - 700.88, 0);
    glVertex3f(57.89, swapy - 671.71, 0);
    glVertex3f(91.08, swapy - 644.67, 0);
    glVertex3f(108.48, swapy - 639.42, 0);
    glVertex3f(118.21, swapy - 644.67, 0);
    glVertex3f(108.48, swapy - 652.7, 0);
    glVertex3f(125.52, swapy - 656.48, 0);
    glVertex3f(132.58, swapy - 660.73, 0);
    glVertex3f(143.29, swapy - 644.67, 0);
    glEnd();

    glLineWidth(5);






    // LEFT EAR
    glBegin(GL_POLYGON);
    if (right) glColor3f(.11, 0.11, 0.11);
    else glColor3f(0.988, 0.8627, 0.286);
    glVertex3f(392, swapy - 312, 0);
    glVertex3f(429.25, swapy - 284.61, 0);
    glVertex3f(476.58, swapy - 249.8, 0);
    glVertex3f(558.23, swapy - 212.32, 0);
    glVertex3f(651.93, swapy - 185.55, 0);
    glVertex3f(710.82, swapy - 176.18, 0);
    glVertex3f(744.29, swapy - 176.18, 0);
    glVertex3f(777.75, swapy - 185.55, 0);
    glVertex3f(767.04, swapy - 205.63, 0);
    glVertex3f(733.58, swapy - 233.74, 0);
    glVertex3f(673.35, swapy - 280.59, 0);
    glVertex3f(621.14, swapy - 304.68, 0);
    glVertex3f(544.85, swapy - 334.13, 0);
    glVertex3f(435.09, swapy - 372.94, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(392, swapy - 312, 0);
    glVertex3f(429.25, swapy - 284.61, 0);
    glVertex3f(476.58, swapy - 249.8, 0);
    glVertex3f(558.23, swapy - 212.32, 0);
    glVertex3f(651.93, swapy - 185.55, 0);
    glVertex3f(710.82, swapy - 176.18, 0);
    glVertex3f(744.29, swapy - 176.18, 0);
    glVertex3f(777.75, swapy - 185.55, 0);
    glVertex3f(767.04, swapy - 205.63, 0);
    glVertex3f(733.58, swapy - 233.74, 0);
    glVertex3f(673.35, swapy - 280.59, 0);
    glVertex3f(621.14, swapy - 304.68, 0);
    glVertex3f(544.85, swapy - 334.13, 0);
    glVertex3f(435.09, swapy - 372.94, 0);
    glEnd();


    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else glColor3f(0, 0, 0);
    glVertex3f(600, swapy - 200, 0);
    glVertex3f(651.93, swapy - 185.55, 0);
    glVertex3f(710.82, swapy - 176.18, 0);
    glVertex3f(744.29, swapy - 176.18, 0);
    glVertex3f(777.75, swapy - 185.55, 0);
    glVertex3f(767.04, swapy - 205.63, 0);
    glVertex3f(733.58, swapy - 233.74, 0);
    glVertex3f(673.35, swapy - 280.59, 0);
    glVertex3f(621.14, swapy - 304.68, 0);
    glVertex3f(597.08, swapy - 314.08, 0);
    glVertex3f(626.93, swapy - 292.15, 0);
    glVertex3f(639.73, swapy - 275.09, 0);
    glVertex3f(654.96, swapy - 247.06, 0);
    glVertex3f(670.8, swapy - 218.43, 0);
    glVertex3f(676.89, swapy - 192.23, 0);
    glEnd();
    glColor3f(0, 0, 0);

    //RIGHT HAND

    glBegin(GL_LINE_STRIP);
    glVertex3f(117.85, swapy - 686.33, 0);
    glVertex3f(135.26, swapy - 686.33, 0);
    glVertex3f(147.45, swapy - 697.3, 0);
    glVertex3f(145.01, swapy - 704.61, 0);
    glVertex3f(135.26, swapy - 708.27, 0);
    glVertex3f(138.31, swapy - 719.23, 0);
    glVertex3f(135.26, swapy - 735.68, 0);
    glVertex3f(124.91, swapy - 757.62, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(113.33, swapy - 668.67, 0);
    glVertex3f(133.72, swapy - 668.67, 0);
    glVertex3f(143.31, swapy - 689.38, 0);
    glEnd();


    // LEFT HAND
    glBegin(GL_LINE_STRIP);
    glVertex3f(380.21, swapy - 656.71, 0);
    glVertex3f(372.26, swapy - 670.49, 0);
    glVertex3f(385.67, swapy - 684.51, 0);
    glVertex3f(335.71, swapy - 663.79, 0);
    glVertex3f(324.74, swapy - 656.71, 0);
    glVertex3f(316.21, swapy - 652.83, 0);
    glVertex3f(311.95, swapy - 656.71, 0);
    glVertex3f(311.95, swapy - 661.35, 0);
    glVertex3f(298.55, swapy - 656.71, 0);
    glVertex3f(288, swapy - 656, 0);
    glVertex3f(286.97, swapy - 684.51, 0);
    glVertex3f(294.28, swapy - 670.49, 0);
    glVertex3f(280.88, swapy - 667.45, 0);
    glVertex3f(276.61, swapy - 676.59, 0);
    glVertex3f(280.88, swapy - 684.51, 0);
    glVertex3f(272.35, swapy - 684.51, 0);
    glVertex3f(265.04, swapy - 700.35, 0);
    glVertex3f(272.35, swapy - 707.66, 0);
    glVertex3f(261.38, swapy - 705.22, 0);
    glVertex3f(255.9, swapy - 714.97, 0);
    glVertex3f(272.35, swapy - 735.07, 0);
    glVertex3f(280.88, swapy - 751.52, 0);
    glVertex3f(344.85, swapy - 803.31, 0);
    glVertex3f(378.28, swapy - 809.65, 0);
    glVertex3f(380.21, swapy - 810.01, 0);
    glVertex3f(405.77, swapy - 810.01, 0);
    glVertex3f(436.24, swapy - 792.95, 0);
    glVertex3f(475.23, swapy - 742.99, 0);
    glEnd();

    // TAIL
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.11, 0.1, 0.11);
    else  glColor3f(0.988, 0.8627, 0.286);
    //glColor3f(0.988, 0.8627, 0.286);
    glVertex3f(481.93, swapy - 568.37, 0);

    glVertex3f(499.34, swapy - 462.63, 0);
    glVertex3f(563.59, swapy - 393.02, 0);
    glVertex3f(670.67, swapy - 281.92, 0);
    glVertex3f(866.09, swapy - 411.76, 0);
    glVertex3f(704.13, swapy - 540.26, 0);
    glVertex3f(650.59, swapy - 621.91, 0);


    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(704.13, swapy - 540.26, 0);
    glVertex3f(837.98, swapy - 611.2, 0);
    glVertex3f(704.13, swapy - 698.21, 0);
    glVertex3f(681.38, swapy - 751.75, 0);
    glVertex3f(574.29, swapy - 704.9, 0);
    glVertex3f(650.59, swapy - 621.91, 0);
    glEnd();


    glBegin(GL_POLYGON);
    glVertex3f(704.13, swapy - 698.21, 0);
    glVertex3f(801.84, swapy - 751.75, 0);
    glVertex3f(589.02, swapy - 886.94, 0);
    glVertex3f(580.99, swapy - 838.75, 0);
    glVertex3f(681.38, swapy - 751.75, 0);
    glEnd();




    // END OF THE TAIL
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0.643, 0.341, 0.125);

    glVertex3f(644.91, swapy - 784, 0);
    glVertex3f(640, swapy - 806.36, 0);
    glVertex3f(663.96, swapy - 795.39, 0);
    glVertex3f(656.61, swapy - 814.62, 0);
    glVertex3f(679.28, swapy - 808.36, 0);
    glVertex3f(670.44, swapy - 824.63, 0);
    glVertex3f(696, swapy - 816, 0);
    glVertex3f(589.02, swapy - 886.94, 0);
    glVertex3f(580.99, swapy - 838.75, 0);
    glEnd();

    //  glLineWidth(13);

    glBegin(GL_LINE_LOOP);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0.643, 0.341, 0.125);
    glVertex3f(644.91, swapy - 784, 0);
    glVertex3f(640, swapy - 806.36, 0);
    glVertex3f(663.96, swapy - 795.39, 0);
    glVertex3f(656.61, swapy - 814.62, 0);
    glVertex3f(679.28, swapy - 808.36, 0);
    glVertex3f(670.44, swapy - 824.63, 0);
    glVertex3f(696, swapy - 816, 0);
    glVertex3f(589.02, swapy - 886.94, 0);
    glVertex3f(580.99, swapy - 838.75, 0);
    glEnd();
    glLineWidth(5);

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(499.34, swapy - 462.63, 0);
    glVertex3f(563.59, swapy - 393.02, 0);
    glVertex3f(670.67, swapy - 281.92, 0);
    glVertex3f(866.09, swapy - 411.76, 0);
    glVertex3f(704.13, swapy - 540.26, 0);
    glVertex3f(837.98, swapy - 611.2, 0);
    glVertex3f(704.13, swapy - 698.21, 0);
    glVertex3f(801.84, swapy - 751.75, 0);
    glVertex3f(589.02, swapy - 886.94, 0);
    glVertex3f(580.99, swapy - 838.75, 0);
    glVertex3f(681.38, swapy - 751.75, 0);
    glVertex3f(574.29, swapy - 704.9, 0);
    glVertex3f(650.59, swapy - 621.91, 0);
    glVertex3f(481.93, swapy - 568.37, 0);
    glEnd();
    //


    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 0);
    glVertex3f(162.07, swapy - 533.89, 0);
    glVertex3f(181.43, swapy - 512.79, 0);
    glVertex3f(193.29, swapy - 507.82, 0);
    glVertex3f(200.78, swapy - 507.82, 0);
    glVertex3f(228.26, swapy - 519.62, 0);
    glVertex3f(262.6, swapy - 540.72, 0);
    glEnd();

    // sides triangles
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.643, 0.341, 0.125);

    glVertex3f(522.14, swapy - 686.33, 0);
    glVertex3f(505.69, swapy - 699.13, 0);
    glVertex3f(495.94, swapy - 711.92, 0);
    glVertex3f(495.94, swapy - 717.41, 0);
    glVertex3f(501.43, swapy - 722.89, 0);
    glVertex3f(522.14, swapy - 722.89, 0);
    glVertex3f(542.25, swapy - 717.41, 0);
    glEnd();


    glBegin(GL_TRIANGLES);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0.643, 0.341, 0.125);
    glVertex3f(568, swapy - 792, 0);
    glVertex3f(534.11, swapy - 827.81, 0);
    glVertex3f(582.46, swapy - 833.77, 0);
    glEnd();

    // nose

    if (right) {
        glColor3f(0.71, 0.423, 0);
        glBegin(GL_QUADS);


        glVertex3f(179.74, swapy - 444.31, 0);
        glVertex3f(204.72, swapy - 444.31, 0);
        glVertex3f(179.74, swapy - 490.15, 0);
        glVertex3f(204.72, swapy - 490.15, 0);
        glEnd();
    }
    else {
        glColor3f(0, 0, 0);
        glBegin(GL_TRIANGLES);


        glVertex3f(179.74, swapy - 474.31, 0);
        glVertex3f(204.72, swapy - 474.31, 0);
        glVertex3f(190.1, swapy - 490.15, 0);
        glEnd();
    }
    // right red circle
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.976, 0.27, 0.196);

    glVertex3f(60.94, swapy - 486.05, 0);
    glVertex3f(71.29, swapy - 493.2, 0);
    glVertex3f(83.48, swapy - 514.52, 0);
    glVertex3f(83.48, swapy - 534.02, 0);
    glVertex3f(74.95, swapy - 554.74, 0);
    glVertex3f(65.2, swapy - 562.05, 0);
    glVertex3f(50.93, swapy - 534.91, 0);
    glVertex3f(50.93, swapy - 504.12, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(60.94, swapy - 486.05, 0);
    glVertex3f(71.29, swapy - 493.2, 0);
    glVertex3f(83.48, swapy - 514.52, 0);
    glVertex3f(83.48, swapy - 534.02, 0);
    glVertex3f(74.95, swapy - 554.74, 0);
    glVertex3f(65.2, swapy - 562.05, 0);
    glVertex3f(50.93, swapy - 534.91, 0);
    glVertex3f(50.93, swapy - 504.12, 0);
    glEnd();
    // left red circle
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else  glColor3f(0.976, 0.27, 0.196);
    glVertex3f(350.94, swapy - 496.25, 0);
    glVertex3f(368.61, swapy - 479.19, 0);
    glVertex3f(389.32, swapy - 474.31, 0);
    glVertex3f(407.6, swapy - 479.19, 0);
    glVertex3f(419.18, swapy - 489.55, 0);
    glVertex3f(427.1, swapy - 507.21, 0);
    glVertex3f(427.1, swapy - 525.49, 0);
    glVertex3f(419.18, swapy - 544.38, 0);
    glVertex3f(399.07, swapy - 554.13, 0);
    glVertex3f(378.97, swapy - 554.13, 0);
    glVertex3f(360.08, swapy - 544.38, 0);
    glVertex3f(350.94, swapy - 535.85, 0);
    glVertex3f(346.68, swapy - 521.23, 0);

    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(350.94, swapy - 496.25, 0);
    glVertex3f(368.61, swapy - 479.19, 0);
    glVertex3f(389.32, swapy - 474.31, 0);
    glVertex3f(407.6, swapy - 479.19, 0);
    glVertex3f(419.18, swapy - 489.55, 0);
    glVertex3f(427.1, swapy - 507.21, 0);
    glVertex3f(427.1, swapy - 525.49, 0);
    glVertex3f(419.18, swapy - 544.38, 0);
    glVertex3f(399.07, swapy - 554.13, 0);
    glVertex3f(378.97, swapy - 554.13, 0);
    glVertex3f(360.08, swapy - 544.38, 0);
    glVertex3f(350.94, swapy - 535.85, 0);
    glVertex3f(346.68, swapy - 521.23, 0);

    glEnd();
    // LEGS
    glBegin(GL_POLYGON);
    if (right) glColor3f(.11, 0.11, 0.11);
    else  glColor3f(0.98, 0.832, 0.25);
    //glColor3f(0.98, 0.832, 0.25);
    glVertex3f(179.74, swapy - 975.73, 0);
    glVertex3f(148.45, swapy - 1008.41, 0);
    glVertex3f(128.56, swapy - 1042.75, 0);
    glVertex3f(128.56, swapy - 1050.67, 0);
    glVertex3f(138.31, swapy - 1056.54, 0);
    glVertex3f(155.98, swapy - 1059.2, 0);
    glVertex3f(197.41, swapy - 1046.79, 0);
    glVertex3f(232.75, swapy - 1028.74, 0);
    glVertex3f(265.65, swapy - 1007.41, 0);
    glVertex3f(276.61, swapy - 994.01, 0);
    glVertex3f(265.09, swapy - 990.01, 0);
    glVertex3f(243.68, swapy - 990.01, 0);
    glVertex3f(218.73, swapy - 990.01, 0);
    glVertex3f(190.14, swapy - 981.98, 0);

    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(179.74, swapy - 975.73, 0);
    glVertex3f(148.45, swapy - 1008.41, 0);
    glVertex3f(128.56, swapy - 1042.75, 0);
    glVertex3f(128.56, swapy - 1050.67, 0);
    glVertex3f(138.31, swapy - 1056.54, 0);
    glVertex3f(155.98, swapy - 1059.2, 0);
    glVertex3f(197.41, swapy - 1046.79, 0);
    glVertex3f(232.75, swapy - 1028.74, 0);
    glVertex3f(265.65, swapy - 1007.41, 0);
    glVertex3f(276.61, swapy - 994.01, 0);
    glVertex3f(265.09, swapy - 990.01, 0);
    glVertex3f(243.68, swapy - 990.01, 0);
    glVertex3f(218.73, swapy - 990.01, 0);
    glVertex3f(190.14, swapy - 981.98, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(180.96, swapy - 1025.08, 0);
    glVertex3f(145.01, swapy - 1053.71, 0);
    glVertex3f(159.64, swapy - 1022.03, 0);
    glVertex3f(132.22, swapy - 1050.67, 0);
    glEnd();

    glBegin(GL_POLYGON);
    if (right) glColor3f(.11, 0.11, 0.11);
    else  glColor3f(0.98, 0.832, 0.25);
    glVertex3f(560.52, swapy - 965.98, 0);
    glVertex3f(575.15, swapy - 1023.86, 0);
    glVertex3f(578.8, swapy - 1062.85, 0);
    glVertex3f(575.15, swapy - 1084.79, 0);
    glVertex3f(564.79, swapy - 1089.66, 0);
    glVertex3f(544.07, swapy - 1084.79, 0);
    glVertex3f(514.83, swapy - 1055.54, 0);
    glVertex3f(473.4, swapy - 997.66, 0);
    glVertex3f(488.63, swapy - 996.7, 0);
    glVertex3f(531.46, swapy - 986.09, 0);
    glVertex3f(548.95, swapy - 976.34, 0);
    glVertex3f(559.6, swapy - 966.57, 0);


    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(560.52, swapy - 965.98, 0);
    glVertex3f(575.15, swapy - 1023.86, 0);
    glVertex3f(578.8, swapy - 1062.85, 0);
    glVertex3f(575.15, swapy - 1084.79, 0);
    glVertex3f(564.79, swapy - 1089.66, 0);
    glVertex3f(544.07, swapy - 1084.79, 0);
    glVertex3f(514.83, swapy - 1055.54, 0);
    glVertex3f(473.4, swapy - 997.66, 0);
    glVertex3f(488.63, swapy - 996.7, 0);
    glVertex3f(531.46, swapy - 986.09, 0);
    glVertex3f(548.95, swapy - 976.34, 0);
    glVertex3f(559.6, swapy - 966.57, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(532.5, swapy - 1039.09, 0);
    glVertex3f(559.31, swapy - 1086.61, 0);
    glVertex3f(553.82, swapy - 1032.39, 0);
    glVertex3f(572.1, swapy - 1084.18, 0);
    glEnd();





    // EYES OUTER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0, 0, 0);
    glVertex3f(128.56, swapy - 393.89, 0);
    glVertex3f(119.42, swapy - 397.55, 0);
    glVertex3f(108.46, swapy - 409.12, 0);
    glVertex3f(102.37, swapy - 421.92, 0);
    glVertex3f(101.37, swapy - 432.28, 0);
    glVertex3f(101.37, swapy - 444.46, 0);
    glVertex3f(108.46, swapy - 459.08, 0);
    glVertex3f(118.47, swapy - 467.39, 0);
    glVertex3f(128.56, swapy - 467.39, 0);
    glVertex3f(140.75, swapy - 462.74, 0);
    glVertex3f(150.5, swapy - 455.6, 0);
    glVertex3f(155.37, swapy - 442.02, 0);
    glVertex3f(159.64, swapy - 417.65, 0);


    glEnd();
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(128.56, swapy - 393.89, 0);
    glVertex3f(119.42, swapy - 397.55, 0);
    glVertex3f(108.46, swapy - 409.12, 0);
    glVertex3f(102.37, swapy - 421.92, 0);
    glVertex3f(101.37, swapy - 432.28, 0);
    glVertex3f(101.37, swapy - 444.46, 0);
    glVertex3f(108.46, swapy - 459.08, 0);
    glVertex3f(118.47, swapy - 467.39, 0);
    glVertex3f(128.56, swapy - 467.39, 0);
    glVertex3f(140.75, swapy - 462.74, 0);
    glVertex3f(150.5, swapy - 455.6, 0);
    glVertex3f(155.37, swapy - 442.02, 0);
    glVertex3f(159.64, swapy - 417.65, 0);
    glEnd();

    // EYES INNER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(136, swapy - 407, 0);
    glVertex3f(147.75, swapy - 416, 0);
    glVertex3f(147.75, swapy - 424, 0);
    glVertex3f(144, swapy - 432, 0);
    glVertex3f(139.19, swapy - 436.01, 0);
    glVertex3f(132.34, swapy - 436.01, 0);
    glVertex3f(128, swapy - 433.87, 0);
    glVertex3f(125.28, swapy - 428.52, 0);
    glVertex3f(125.28, swapy - 421.46, 0);
    glVertex3f(125.28, swapy - 416, 0);
    glVertex3f(130.2, swapy - 410.11, 0);
    glVertex3f(132.34, swapy - 408, 0);
    glVertex3f(136, swapy - 407, 0);


    glEnd();

    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(136, swapy - 407, 0);
    glVertex3f(147.75, swapy - 416, 0);
    glVertex3f(147.75, swapy - 424, 0);
    glVertex3f(144, swapy - 432, 0);
    glVertex3f(139.19, swapy - 436.01, 0);
    glVertex3f(132.34, swapy - 436.01, 0);
    glVertex3f(128, swapy - 433.87, 0);
    glVertex3f(125.28, swapy - 428.52, 0);
    glVertex3f(125.28, swapy - 421.46, 0);
    glVertex3f(125.28, swapy - 416, 0);
    glVertex3f(130.2, swapy - 410.11, 0);
    glVertex3f(132.34, swapy - 408, 0);
    glVertex3f(136, swapy - 407, 0);
    glEnd();
    glLineWidth(5);


    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(117.85, swapy - 383.93, 0);
    glVertex3f(168.77, swapy - 425.35, 0);
    glEnd();



    // EYES OUTER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(0, 0, 0);
    glVertex3f(293.06, swapy - 420.7, 0);
    glVertex3f(293.06, swapy - 435.32, 0);
    glVertex3f(298.55, swapy - 449.94, 0);
    glVertex3f(307.68, swapy - 461.74, 0);
    glVertex3f(320.48, swapy - 467.61, 0);
    glVertex3f(337.54, swapy - 467.61, 0);
    glVertex3f(350.94, swapy - 465.17, 0);
    glVertex3f(360.08, swapy - 460.08, 0);
    glVertex3f(368, swapy - 449.94, 0);
    glVertex3f(372.87, swapy - 432.28, 0);
    glVertex3f(372.87, swapy - 420.7, 0);
    glVertex3f(368, swapy - 405.47, 0);
    glVertex3f(358.08, swapy - 395.72, 0);
    glVertex3f(344.24, swapy - 391.46, 0);


    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex3f(293.06, swapy - 420.7, 0);
    glVertex3f(293.06, swapy - 435.32, 0);
    glVertex3f(298.55, swapy - 449.94, 0);
    glVertex3f(307.68, swapy - 461.74, 0);
    glVertex3f(320.48, swapy - 467.61, 0);
    glVertex3f(337.54, swapy - 467.61, 0);
    glVertex3f(350.94, swapy - 465.17, 0);
    glVertex3f(360.08, swapy - 460.08, 0);
    glVertex3f(368, swapy - 449.94, 0);
    glVertex3f(372.87, swapy - 432.28, 0);
    glVertex3f(372.87, swapy - 420.7, 0);
    glVertex3f(368, swapy - 405.47, 0);
    glVertex3f(358.08, swapy - 395.72, 0);
    glVertex3f(344.24, swapy - 391.46, 0);
    glEnd();

    // EYES INNER
    glBegin(GL_POLYGON);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(305.91, swapy - 416, 0);
    glVertex3f(328, swapy - 401.98, 0);
    glVertex3f(333.51, swapy - 405.19, 0);
    glVertex3f(338.44, swapy - 411.83, 0);
    glVertex3f(340.15, swapy - 421.46, 0);
    glVertex3f(338.44, swapy - 428.52, 0);
    glVertex3f(333.51, swapy - 433.87, 0);
    glVertex3f(328, swapy - 436.65, 0);
    glVertex3f(320, swapy - 437.94, 0);
    glVertex3f(312, swapy - 433.87, 0);
    glVertex3f(308.05, swapy - 428.52, 0);
    glVertex3f(305.91, swapy - 421.46, 0);
    glVertex3f(305.91, swapy - 416, 0);


    glEnd();

    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    if (right) glColor3f(0.58, 0, 0.062);
    else   glColor3f(1, 1, 1);
    glVertex3f(305.91, swapy - 416, 0);
    glVertex3f(328, swapy - 401.98, 0);
    glVertex3f(333.51, swapy - 405.19, 0);
    glVertex3f(338.44, swapy - 411.83, 0);
    glVertex3f(340.15, swapy - 421.46, 0);
    glVertex3f(338.44, swapy - 428.52, 0);
    glVertex3f(333.51, swapy - 433.87, 0);
    glVertex3f(328, swapy - 436.65, 0);
    glVertex3f(320, swapy - 437.94, 0);
    glVertex3f(312, swapy - 433.87, 0);
    glVertex3f(308.05, swapy - 428.52, 0);
    glVertex3f(305.91, swapy - 421.46, 0);
    glVertex3f(305.91, swapy - 416, 0);

    glEnd();
    glLineWidth(5);

    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(282.36, swapy - 424, 0);
    glVertex3f(357.03, swapy - 378.05, 0);
    glEnd();
    glPopMatrix();


}
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

void drawWall(){
    glPushMatrix();

        glBegin(GL_POLYGON);
        glColor3f(0.545, .31, 0.223);
        glVertex3f(535, 250, 0);
        glVertex3f(595, 0, 0);
        glVertex3f(595, 300, 0);
        glVertex3f(535, 450, 0);
        glEnd();
        glLineWidth(2);
        glBegin(GL_LINES);
        glColor3f(0.6, .6, 0.6);
        for(int i=1; i<6;i++){
            glVertex3f(535, 250+(33.34)*i, 0);
            glVertex3f(595, 50*i, 0);
        }
        for(int i=1; i<5;i++){
            glVertex3f(595-12*i, 50*i, 0);
            glVertex3f(595-12*i, 300+30*i, 0);
        }
        glEnd();
       
        glBegin(GL_POLYGON);
    glColor3f(0.698, .341, 0.203);
        glVertex3f(625, 0, 0);
          glVertex3f(595, 0, 0);
          glVertex3f(595, 300, 0);
          glVertex3f(625, 300, 0);
          glEnd();
        glBegin(GL_POLYGON);
           glColor3f(0.545, .31, 0.223);
           glVertex3f(595, 300, 0);
           glVertex3f(625, 300, 0);
           glVertex3f(565, 450, 0);
           glVertex3f(535, 450, 0);
           glEnd();
         glLineWidth(3);
        glBegin(GL_LINE_LOOP);
           glColor3f(0,0, 0);
           glVertex3f(535, 250, 0);
           glVertex3f(595, 0, 0);
           glVertex3f(595, 300, 0);
           glVertex3f(535, 450, 0);
           glEnd();
       glBegin(GL_LINE_LOOP);
        glVertex3f(625, 0, 0);
        glVertex3f(595, 0, 0);
        glVertex3f(595, 300, 0);
        glVertex3f(625, 300, 0);
        glEnd();
        glBegin(GL_LINE_LOOP);
        glVertex3f(595, 300, 0);
        glVertex3f(625, 300, 0);
        glVertex3f(565, 450, 0);
        glVertex3f(535, 450, 0);
        glEnd();
      glPopMatrix();
    if(isWallHitted){
        if(turn%2==0)  glColor3f(1, 0.933, 0.549);
        else glColor3f(0.537, 0.812, 0.941);
   
        drawCircle1(10, WallHitPos[0], WallHitPos[1]);
    }
}


void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    //the sence
    //SHAPE1
    glPushMatrix();
    glBegin(GL_POLYGON);
    glColor3f(0.992, 0.804, 0.404);
    glVertex3f(0, 0, 0);
    glVertex3f(0, windowYSize, 0);
    glVertex3f(windowXSize, windowYSize, 0);
    glVertex3f(windowXSize, 0, 0);
    glEnd();
    glPopMatrix();
    drawCloud(false,true);
       drawCloud(false,false);
    //MOOn
       //SHAPE2
    glPushMatrix();
    glTranslatef(0, -translateMoon, 0);
        glColor3f(0.9, 0.9, 0.9);
       drawCircle1(100, 250, 650);
                   glColor3f(0, 0, 0);
                   drawCircleHole(100, 250, 650);
                glPopMatrix();
   
     glPopMatrix();
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.781, 0.479, 0.204);
    glVertex3f(0, (windowYSize / 2) - 200, 0);
    glVertex3f(windowXSize, (windowYSize / 2) - 200, 0);
    glColor3f(0.867, 0.647, 0.357);
    glVertex3f(windowXSize, 0, 0);
    glVertex3f(0, 0, 0);
    glEnd();
    glPopMatrix();
   
    
    //shape 3 grace
    glPushMatrix();
   // glTranslatef(xDaynamic, 0 ,0);
    int isMarked=0;
    
     glVertex3f(windowXSize, (windowYSize / 2) - 200, 0);
    for(int i=-30;i<windowXSize;i+=30){
    
 int newSide=i+30;
        if(isMarked%3==d%3){
        glBegin(GL_TRIANGLES);
                glColor3f(0.501, 0.745, 0.173);
            glVertex3f(i, (windowYSize / 2) - 200, 0);
            glVertex3f(i+60, (windowYSize / 2) - 200, 0);
               
               glColor3f(0.388, 0.58, 0.208);
             glVertex3f(newSide, (windowYSize / 2) - 170, 0);
                glEnd();
           
        }else{
            
            if(isMarked%3==(d+1)%3){
                glBegin(GL_TRIANGLES);
                               glColor3f(0.501, 0.745, 0.173);
                               glVertex3f(i+60, (windowYSize / 2) - 200, 0);
                                          glColor3f(0.388, 0.58, 0.208);
                                                       
                                                    glVertex3f(i, (windowYSize / 2) - 200, 0);
                                          
                                                      
                                                     glVertex3f(newSide, (windowYSize / 2) - 170, 0);
                                             glEnd();
          
            }else{
               glBegin(GL_POLYGON);
                          glColor3f(0.388, 0.58, 0.208);
                                       
                                    glVertex3f(i, (windowYSize / 2) - 200, 0);
                          glColor3f(0.501, 0.745, 0.173);
                                    glVertex3f(i+60, (windowYSize / 2) - 200, 0);
                                      
                                     glVertex3f(newSide, (windowYSize / 2) - 170, 0);
                             glEnd();
            }
        }
         glBegin(GL_LINES);
          glColor3f(0.388, 0.58, 0.208);
                     
                  glVertex3f(i, (windowYSize / 2) - 200, 0);
        glColor3f(0.501, 0.745, 0.173);
                    
                   glVertex3f(newSide, (windowYSize / 2) - 170, 0);
          
         glEnd();
        glBegin(GL_LINES);
                 glColor3f(0.388, 0.58, 0.208);
                           glVertex3f(newSide, (windowYSize / 2) - 170, 0);
                         
               glColor3f(0.501, 0.745, 0.173);
                          glVertex3f(i+60, (windowYSize / 2) - 200, 0);
                         
                 
                glEnd();
        isMarked++;
    }
    //wall
    glPopMatrix();
    glBegin(GL_LINES);
       glColor3f(0.781, 0.479, 0.204);
             glVertex3f(0, (windowYSize / 2) - 200, 0);
             glVertex3f(windowXSize, (windowYSize / 2) - 200, 0);
          
         glEnd();
    drawWall();

    

    if (inProcess && turn % 2 == 1)
        if (isHit)    drawPicakWin(true);
        else drawPicakFight(true);
     else drawPicakSafe(true);
     if (inProcess && turn % 2 == 0)
         if (isHit)    drawPicakWin(false);
         else drawPicakFight(false);
     else drawPicakSafe(false);




    
    if(turn%2==0)glColor3f(1, 0.933, 0.549);
    else   glColor3f(0.537, 0.812, 0.941);
        glPushMatrix();
           drawCircle1(20, ballPos[0], ballPos[1]);
              glColor3f(0, 0, 0);
              drawCircleHole(10, ballPos[0], ballPos[1]);
           glPopMatrix();
   


    
    



    glPopMatrix();
    
    drawCloud(true,false);
    drawPowerBar(false);
    drawPowerBar(true);
    glPushMatrix();
    
    
    drawHealthBar();

    
    glFlush();
}


int main(int argc, char** argr) {
    glutInit(&argc, argr);

    glutInitWindowSize(1200, 900);
    p0[0] = 160;
    p0[1] = 110;

    p1[0] = 100;
    p1[1] = 500;

    p2[0] = 500;
    p2[1] = 500;

    p3[0] = 980;
    p3[1] = 110;

    p4[0] = 980;
    p4[1] = 110;
    ballPos[0]=-100;
    ballPos[1]=-100;

    player1Pos[0] = p0[0];
    player1Pos[1] = p0[1];
    player2Pos[0] = p3[0];
    player2Pos[1] = p3[1];

    glutCreateWindow("Assignment");
    glutDisplayFunc(Display);
    glutIdleFunc(Anim);
    glutMouseFunc(mouseListener);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gluOrtho2D(0.0, 1200, 0.0, 900);
 //SoundEngine->play2D("audio/pikachou.mp3", GL_TRUE);
    glutMainLoop();
    return 0;
}
