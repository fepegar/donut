#include <math.h>
#include <stdio.h>
#include <string.h>

// Constant Definitions
#define DELTA_DEGREES_X 0.3
#define DELTA_DEGREES_Z 0.1
#define DELTA_CIRCLE 1.15
#define DELTA_TORUS 4
#define NUM_ROWS 22
#define NUM_COLS 80
#define BUFFER_SIZE NUM_ROWS * NUM_COLS
#define Z_OFFSET 5
#define X_OFFSET NUM_COLS / 2
#define Y_OFFSET NUM_ROWS / 2
#define SHADE_CHARS ".,:;irsXA253hMHGS#9B&@"

// Screen Manipulation Commands
#define CLEAR_SCREEN "\x1b[2J"
#define RETURN_CURSOR "\x1b[H"
#define NEWLINE 10
#define SPACE 32

void print_frame(char frameBuffer[]) {
    printf(RETURN_CURSOR);
    for(int i = 0; i < BUFFER_SIZE; i++) {
        putchar(i % NUM_COLS ? frameBuffer[i] : NEWLINE);
    }
}

void update_inner_loop(
    float angleTorus,
    float sinTorus,
    float cosTorus,
    float sinX,
    float cosX,
    float sinZ,
    float cosZ,
    float zBuffer[],
    char frameBuffer[]) {
    for(float degsCircle = 0; degsCircle < 360; degsCircle += DELTA_CIRCLE){
        float angleCircle = degsCircle * M_PI / 180;
        float sinCircle = sin(angleCircle);
        float cosCircle = cos(angleCircle);

        float radiusOffset = cosTorus + 2;
        float inverseDepth = 1 / (sinCircle * radiusOffset * sinX + sinTorus * cosX + Z_OFFSET);
        float torusXRotation = sinCircle * radiusOffset * cosX - sinTorus * sinX;

        float ccro = cosCircle * radiusOffset;
        int x = X_OFFSET + 30 * inverseDepth * (ccro * cosZ - torusXRotation * sinZ);
        int y = Y_OFFSET + 15 * inverseDepth * (ccro * sinZ + torusXRotation * cosZ);
        int bufferIdx = x + NUM_COLS * y;

        float a = sinTorus * sinX;
        float b = sinCircle * cosTorus * cosX;
        float c = sinCircle * cosTorus * sinX;
        float d = sinTorus * cosX;
        float e = cosCircle * cosTorus * sinZ;
        float luminance = 8 * ((a - b) * cosZ - c - d - e);

        int inY = 0 < y && y < NUM_ROWS;
        int inX = 0 < x && x < NUM_COLS;
        int inZ = inverseDepth > zBuffer[bufferIdx];
        int inFrame = inY && inX && inZ;
        if(inFrame){
            zBuffer[bufferIdx] = inverseDepth;
            int charIndex = luminance > 0 ? luminance : 0;
            int lightIntensityIndex = luminance > 0 ? (int)luminance : 0;
            lightIntensityIndex = lightIntensityIndex >= 22 ? 22 - 1 : lightIntensityIndex;

            frameBuffer[bufferIdx] = SHADE_CHARS[charIndex];
        }
    }
}

void update_buffers(
    float radsX,
    float radsZ,
    float zBuffer[],
    char frameBuffer[]) {
    float sinX = sin(radsX);
    float cosX = cos(radsX);
    float sinZ = sin(radsZ);
    float cosZ = cos(radsZ);

    for(float degsTorus = 0; degsTorus < 360; degsTorus += DELTA_TORUS){
        float angleTorus = degsTorus * M_PI / 180;
        float sinTorus = sin(angleTorus);
        float cosTorus = cos(angleTorus);

        update_inner_loop(angleTorus, sinTorus, cosTorus, sinX, cosX, sinZ, cosZ, zBuffer, frameBuffer);
    }
}

int main(void){
    float radsX = 0;
    float radsZ = 0;
    float zBuffer[BUFFER_SIZE];
    char frameBuffer[BUFFER_SIZE];

    printf(CLEAR_SCREEN);

    while(1){
        memset(frameBuffer, SPACE, BUFFER_SIZE);
        memset(zBuffer, 0, sizeof zBuffer);

        update_buffers(radsX, radsZ, zBuffer, frameBuffer);
        print_frame(frameBuffer);

        radsX += DELTA_DEGREES_X * M_PI / 180;
        radsZ += DELTA_DEGREES_Z * M_PI / 180;
    }

    return 0;
}
