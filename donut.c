#include <math.h>
#include <stdio.h>
#include <string.h>

#define DELTA_DEGREES_X 0.3
#define DELTA_DEGREES_Z 0.1
#define NEWLINE 10
#define CLEAR_SCREEN "\x1b[2J"
#define RETURN_CURSOR "\x1b[H"
#define DELTA_CIRCLE 1.15
#define DELTA_TORUS 4
#define NUM_ROWS 22
#define NUM_COLS 80
#define BUFFER_SIZE NUM_ROWS * NUM_COLS
#define Z_BUFFER_SIZE NUM_ROWS * NUM_COLS * 4



int main(void){
  unsigned int bufferIdx;
  float radsX = 0;
  float radsZ = 0;
  float degsCircle, degsTorus;
  float angleCircle, angleTorus;
  float sinCircle, cosCircle;
  float sinTorus, cosTorus;
  float sinX, cosX;
  float sinZ, cosZ;
  float zBuffer[BUFFER_SIZE];
  char frameBuffer[BUFFER_SIZE];
  char charIndex;

  printf(CLEAR_SCREEN);

  while(1){
    memset(frameBuffer, 32, BUFFER_SIZE);
    memset(zBuffer, 0, Z_BUFFER_SIZE);
    sinX = sin(radsX);
    cosX = cos(radsX);
    sinZ = sin(radsZ);
    cosZ = cos(radsZ);
    for(degsTorus = 0; degsTorus < 360; degsTorus += DELTA_TORUS){
      angleTorus = degsTorus * M_PI / 180;
      sinTorus = sin(angleTorus);
      cosTorus = cos(angleTorus);
      for(degsCircle = 0; degsCircle < 360; degsCircle += DELTA_CIRCLE){
        angleCircle = degsCircle * M_PI / 180;
        sinCircle = sin(angleCircle);
        cosCircle = cos(angleCircle);
        float h = cosTorus + 2;
        float D = 1 / (sinCircle * h * sinX + sinTorus * cosX + 5);
        float t = sinCircle * h * cosX - sinTorus * sinX;
        int x = 40 + 30 * D * (cosCircle * h * cosZ - t * sinZ);
        int y = 12 + 15 * D * (cosCircle * h * sinZ + t * cosZ);
        int o = x + 80 * y;  // 80 = NUM_COLS?
        int N = 8 * ((sinTorus * sinX - sinCircle * cosTorus * cosX) * cosZ - sinCircle * cosTorus * sinX - sinTorus * cosX - cosCircle * cosTorus * sinZ);
        if(y < NUM_ROWS && y > 0 && x > 0 && x < NUM_COLS && D > zBuffer[o]){
          zBuffer[o] = D;
          charIndex = N > 0 ? N : 0;
          frameBuffer[o] = ".,-~:;=!*#$@"[charIndex];
        }
      }
    }
    printf(RETURN_CURSOR);
    for(bufferIdx = 0; bufferIdx < (BUFFER_SIZE + 1); bufferIdx++) {
      putchar(bufferIdx % NUM_COLS ? frameBuffer[bufferIdx] : NEWLINE);
    }
    radsX += DELTA_DEGREES_X * M_PI / 180;
    radsZ += DELTA_DEGREES_Z * M_PI / 180;
  }
}
