
/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

/*
Vector2* convexHull(Vector2* points, int numberOfPoints) {

}
*/

void positionTriangle(Vector3 *outTri, Vector3 *origTri[], Quaternion transform, Vector3 offset) {
    for (int i = 0; i < 3; i++) {
        //printf("%f %f %f", *origTri[1], *orig);
        *(outTri + i) = Vector3Add(offset, Vector3RotateByQuaternion(*origTri[i], transform));
    }
}

float similarity(Vector3 prevDisplay, Vector3 prevUp, Vector3 newDisplay, Vector3 newUp) {
    return 1.0f;
}

typedef struct {
    Vector3 *firstTri[3];
    Vector3 *secondTri[3];
    Vector3 direction;    
} CubeFace;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float interpolationSpeed = 0.45f;
    const float cubeSize = 1.0f;
    const float bounciness = 1.0f;

    srand(time(NULL));

    Vector3 *CubeCorners = (Vector3*) malloc(sizeof(Vector3) * 8);
    
    for (int x = 0; x <= 1; x++) {
        for (int y = 0; y <= 1; y++) {
            for (int z = 0; z <= 1; z++) {
                Vector3 *corner = CubeCorners + 4*x + 2*y + z;
                (*corner).x = (x == 0) ? -cubeSize : cubeSize;
                (*corner).y = (y == 0) ? -cubeSize : cubeSize;
                (*corner).z = (z == 0) ? -cubeSize : cubeSize;
            }
        }
    }

    CubeFace *faces = (CubeFace*) malloc(sizeof(CubeFace) * 6);

    // In default position, the front face faces away from the camera, back face looks toward camera (Because camera is at -z looking in the +z direction)

    Vector3 top = Vector3Zero();
    top.y = 1;
    Vector3 bottom = Vector3Zero();
    bottom.y = -1;
    Vector3 back = Vector3Zero();
    back.z = -1;
    Vector3 front = Vector3Zero();
    front.z = 1;
    Vector3 left = Vector3Zero();
    left.x = 1;
    Vector3 right = Vector3Zero();
    right.x = -1;

    // Back face
     (*faces).firstTri[0] = CubeCorners + 0;
     (*faces).firstTri[1] = CubeCorners + 5;
     (*faces).firstTri[2] = CubeCorners + 4;
    (*faces).secondTri[0] = CubeCorners + 5;
    (*faces).secondTri[1] = CubeCorners + 0;
    (*faces).secondTri[2] = CubeCorners + 1;
    (*faces).direction = back;


    // Front face
     (*(faces + 1)).firstTri[0] = CubeCorners + 2;
     (*(faces + 1)).firstTri[1] = CubeCorners + 7;
     (*(faces + 1)).firstTri[2] = CubeCorners + 3;
    (*(faces + 1)).secondTri[0] = CubeCorners + 7;
    (*(faces + 1)).secondTri[1] = CubeCorners + 2;
    (*(faces + 1)).secondTri[2] = CubeCorners + 6;
    (*(faces + 1)).direction = front;

    // Left face
     (*(faces + 2)).firstTri[0] = CubeCorners + 6;
     (*(faces + 2)).firstTri[1] = CubeCorners + 5;
     (*(faces + 2)).firstTri[2] = CubeCorners + 7;
    (*(faces + 2)).secondTri[0] = CubeCorners + 5;
    (*(faces + 2)).secondTri[1] = CubeCorners + 6;
    (*(faces + 2)).secondTri[2] = CubeCorners + 4;
    (*(faces + 2)).direction = left;

    // Right face
     (*(faces + 3)).firstTri[0] = CubeCorners + 2;
     (*(faces + 3)).firstTri[1] = CubeCorners + 1;
     (*(faces + 3)).firstTri[2] = CubeCorners + 0;
    (*(faces + 3)).secondTri[0] = CubeCorners + 1;
    (*(faces + 3)).secondTri[1] = CubeCorners + 2;
    (*(faces + 3)).secondTri[2] = CubeCorners + 3;
    (*(faces + 3)).direction = right;

    // Top face
     (*(faces + 4)).firstTri[0] = CubeCorners + 3;
     (*(faces + 4)).firstTri[1] = CubeCorners + 5;
     (*(faces + 4)).firstTri[2] = CubeCorners + 1;
    (*(faces + 4)).secondTri[0] = CubeCorners + 5;
    (*(faces + 4)).secondTri[1] = CubeCorners + 3;
    (*(faces + 4)).secondTri[2] = CubeCorners + 7;
    (*(faces + 4)).direction = top;

    // Bottom face
     (*(faces + 5)).firstTri[0] = CubeCorners + 4;
     (*(faces + 5)).firstTri[1] = CubeCorners + 2;
     (*(faces + 5)).firstTri[2] = CubeCorners + 0;
    (*(faces + 5)).secondTri[0] = CubeCorners + 2;
    (*(faces + 5)).secondTri[1] = CubeCorners + 4;
    (*(faces + 5)).secondTri[2] = CubeCorners + 6;
    (*(faces + 5)).direction = bottom;

    InitWindow(screenWidth, screenHeight, "C3ube");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 0.0f, -5.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type


    int currFace = 0;
    int prevFace;

    Quaternion destination = QuaternionFromMatrix(MatrixLookAt(Vector3Zero(), back, top));
    Quaternion prev;

    float interpolation = 0.0f;
    bool isMoving = false;
    bool stopped = false;

    //DisableCursor();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------

        if (IsKeyPressed(' ')) stopped = !stopped;

        if (!(isMoving || stopped)) {
            prevFace = currFace;
            prev = destination;
            do {
                currFace = rand() % 6;
            } while (currFace == prevFace);
            int upFace = rand() % 6;
            do {
                upFace = rand() % 6;
            } while (Vector3Equals((*(faces + upFace)).direction, (*(faces + currFace)).direction) || \
                     Vector3Equals(Vector3Negate((*(faces + upFace)).direction), (*(faces + currFace)).direction));
            destination = QuaternionFromMatrix(MatrixLookAt(Vector3Zero(), (*(faces + currFace)).direction, (*(faces + upFace)).direction));
            interpolation = 0;
            isMoving = true;
            printf("Changing to face %i, with up face %i", currFace, upFace);
        }

        interpolation += GetFrameTime() * interpolationSpeed;
        if (interpolation > 1.0f) {
            interpolation = 1.0f;
            isMoving = false;
        }
        //printf("interpolation: %f", interpolation);

        Quaternion cubeTransform = QuaternionSlerp(prev, destination, sin(interpolation * PI/2));
        Vector3 bounceOffset = Vector3Zero();
        bounceOffset.z = bounciness * (float) sin(interpolation * PI);

        BeginDrawing();

            ClearBackground(BLACK);


            BeginMode3D(camera);
            for (int faceNum = 0; faceNum < 6; faceNum++) {
                CubeFace face = *(faces + faceNum);
                Vector3* firstTriTransformed = (Vector3*) malloc(sizeof(Vector3) * 3);
                Vector3* secondTriTransformed = (Vector3*) malloc(sizeof(Vector3) * 3);
                positionTriangle(firstTriTransformed, face.firstTri, cubeTransform, bounceOffset);
                positionTriangle(secondTriTransformed, face.secondTri, cubeTransform, bounceOffset);
                DrawTriangle3D(*(firstTriTransformed + 0), *(firstTriTransformed + 1), *(firstTriTransformed + 2), RAYWHITE);
                DrawTriangle3D(*(secondTriTransformed + 0), *(secondTriTransformed + 1), *(secondTriTransformed + 2), RAYWHITE);
                free(firstTriTransformed);
                free(secondTriTransformed);
            }

            EndMode3D();

        EndDrawing();

        fflush(stdout);
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    free(faces);
    free(CubeCorners);

    return 0;
}