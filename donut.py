import time
import numpy as np

DELTA_DEGREES_X = 0.3
DELTA_DEGREES_Z = 0.1
DELTA_CIRCLE = 1.15
DELTA_TORUS = 4
NUM_ROWS = 22
NUM_COLS = 80
BUFFER_SIZE = NUM_ROWS * NUM_COLS
Z_OFFSET = 5
X_OFFSET = NUM_COLS / 2
Y_OFFSET = NUM_ROWS / 2
chars = ".,-~:;=!*#$@"

radsX = 0
radsZ = 0

frameBuffer = np.full(BUFFER_SIZE, ' ')
zBuffer = np.zeros(BUFFER_SIZE)

while True:
    frameBuffer.fill(' ')
    zBuffer.fill(0)
    sinX = np.sin(radsX)
    cosX = np.cos(radsX)
    sinZ = np.sin(radsZ)
    cosZ = np.cos(radsZ)

    for degsTorus in np.arange(0, 360, DELTA_TORUS):
        angleTorus = degsTorus * np.pi / 180
        sinTorus = np.sin(angleTorus)
        cosTorus = np.cos(angleTorus)

        for degsCircle in np.arange(0, 360, DELTA_CIRCLE):
            angleCircle = degsCircle * np.pi / 180
            sinCircle = np.sin(angleCircle)
            cosCircle = np.cos(angleCircle)
            h = cosTorus + 2
            D = 1 / (sinCircle * h * sinX + sinTorus * cosX + Z_OFFSET)
            t = sinCircle * h * cosX - sinTorus * sinX
            x = int(X_OFFSET + 30 * D * (cosCircle * h * cosZ - t * sinZ))
            y = int(Y_OFFSET + 15 * D * (cosCircle * h * sinZ + t * cosZ))
            bufferIdx = x + NUM_COLS * y

            if 0 < y < NUM_ROWS and 0 < x < NUM_COLS and D > zBuffer[bufferIdx]:
                zBuffer[bufferIdx] = D
                N = 8 * ((sinTorus * sinX - sinCircle * cosTorus * cosX) * cosZ - sinCircle * cosTorus * sinX - sinTorus * cosX - cosCircle * cosTorus * sinZ)
                charIndex = min(len(chars) - 1, max(0, int(N)))
                frameBuffer[bufferIdx] = chars[charIndex]

    print('\n' * NUM_ROWS)  # "clear" the console
    print(''.join(frameBuffer[i] if (i % NUM_COLS or i == 0) else '\n' for i in range(BUFFER_SIZE)))  # print the frame

    radsX += DELTA_DEGREES_X * np.pi / 180
    radsZ += DELTA_DEGREES_Z * np.pi / 180
