#include <math.h>
#include <stdio.h>
#include <string.h>

#define TWO_PI 6.28
#define BUFFER_SIZE 1760
#define DELTA_X 0.005
#define DELTA_Z 0.002
#define NEWLINE 10
#define CLEAR_SCREEN "\x1b[2J"
#define RETURN_CURSOR "\x1b[H"
#define DELTA_CIRCLE 0.02
#define DELTA_TORUS 0.07


int main(void){
  int k;
  float rotationX = 0;
  float rotationZ = 0;
  float angleCircle, angleTorus;
  float zBuffer[BUFFER_SIZE];
  char frameBuffer[BUFFER_SIZE];

  printf(CLEAR_SCREEN);

  while(1){
    memset(frameBuffer, 32, BUFFER_SIZE);
    memset(zBuffer, 0, 7040);
    for(angleTorus = 0; angleTorus < TWO_PI; angleTorus += DELTA_TORUS){
      for(angleCircle = 0; angleCircle < TWO_PI; angleCircle += DELTA_CIRCLE){
        float c = sin(angleCircle);
        float d = cos(angleTorus);
        float e = sin(rotationX);
        float f = sin(angleTorus);
        float g = cos(rotationX);
        float h = d + 2;
        float D = 1 / (c * h * e + f * g + 5);
        float l = cos(angleCircle);
        float m = cos(rotationZ);
        float n = sin(rotationZ);
        float t = c * h * g - f * e;
        int x = 40 + 30 * D * (l * h * m - t * n);
        int y = 12 + 15 * D * (l * h * n + t * m);
        int o = x + 80 * y;
        int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
        if(22 > y && y > 0 && x > 0 && 80 > x && D > zBuffer[o]){
          zBuffer[o] = D;
          frameBuffer[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
        }
      }
    }
    printf(RETURN_CURSOR);
    for(k = 0; k < (BUFFER_SIZE + 1); k++) {
      putchar(k % 80 ? frameBuffer[k] : NEWLINE);
    }
    rotationX += DELTA_X;
    rotationZ += DELTA_Z;
  }
}
