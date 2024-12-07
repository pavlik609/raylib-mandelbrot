#include <cmath>
#include <raylib/raylib.h>


#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

using namespace std;

#define WINDW 600
#define WINDH 600

float x = 0;
float y = 0;

float zoom = 100;

float resolution = 1;

float velocity = 1;

float mwheel_mult = 0.1;

int getMandelbrot(float x,float y){
    float z_real = 0;
    float z_imag = 0;

    float c_real = x;
    float c_imag = y;

    for(int i=1;i<11*resolution;i++)
    {
        float z_real_new = z_real * z_real - z_imag * z_imag + c_real; // Real part
        float z_imag_new = 2.0f * z_real * z_imag + c_imag;           // Imaginary part

        // Update z
        z_real = z_real_new;
        z_imag = z_imag_new;
        if (sqrt(z_real*z_real+z_imag*z_imag) > 2) {
            return i;
        }
    }

    return 0;
}

float oldx = -1;
float oldy = -1;
float oldzoom = -1;
float oldres = -1;

bool cinematicmode = false;


int main(){

    InitWindow(WINDW, WINDH, "Raylib - Mandelbrot set");

    SetWindowIcon(LoadImage("icon.png"));
const Shader mandelbrot_fs = LoadShader(NULL,"./mandelbrot.fs");
    int dwindw=WINDW;
    int dwindh=WINDH;
    SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "x"), &x, SHADER_UNIFORM_FLOAT);
    SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "y"), &y, SHADER_UNIFORM_FLOAT);
    SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "zoom"), &zoom, SHADER_UNIFORM_FLOAT);
    SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "resolution"), &resolution, SHADER_UNIFORM_FLOAT);
    SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "WINDW"), &dwindw, SHADER_UNIFORM_INT);
    SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "WINDH"), &dwindh,SHADER_UNIFORM_INT);


    SetTargetFPS(240);

    while(!WindowShouldClose()){

        if(IsKeyDown(KEY_W)){
            y+=velocity/25/(zoom/100);
        }if(IsKeyDown(KEY_S)){
            y-=velocity/25/(zoom/100);
        }if(IsKeyDown(KEY_A)){
            x+=velocity/25/(zoom/100);
        }if(IsKeyDown(KEY_D)){
            x-=velocity/25/(zoom/100);
        }if(IsKeyDown(KEY_G)){
            resolution+=velocity/20;
        }if(IsKeyDown(KEY_H)){
            resolution-=velocity/20;
        }if(IsKeyDown(KEY_C)){
            zoom+=(mwheel_mult*zoom)/30;
        }if(IsKeyDown(KEY_V)){
            zoom-=(mwheel_mult*zoom)/30;
        }

        if(IsKeyPressed(KEY_F11)){
            cinematicmode = !cinematicmode;
        }

        Vector2 mwheel = GetMouseWheelMoveV();        

            zoom-= mwheel.x*(mwheel_mult*zoom);
            zoom+= mwheel.y*(mwheel_mult*zoom);

        if(x != oldx || y != oldy || zoom != oldzoom || resolution != oldres){
            SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "x"), &x, SHADER_UNIFORM_FLOAT);
            SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "y"), &y, SHADER_UNIFORM_FLOAT);
            SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "zoom"), &zoom, SHADER_UNIFORM_FLOAT);
            SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "resolution"), &resolution, SHADER_UNIFORM_FLOAT);
            SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "WINDW"), &dwindw, SHADER_UNIFORM_INT);
            SetShaderValue(mandelbrot_fs, GetShaderLocation(mandelbrot_fs, "WINDH"), &dwindh,SHADER_UNIFORM_INT);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginShaderMode(mandelbrot_fs);
                DrawRectangle(0, 0, WINDW, WINDH, WHITE);
            EndShaderMode();
        if(!cinematicmode){
            DrawFPS(5, 5);
            
            DrawText(TextFormat("x: %f",x),5,20,20,GRAY); 
            DrawText(TextFormat("y: %f",y),5,35,20,GRAY); 
            
            DrawText(TextFormat("Resolution: %f", resolution),5,WINDH-45,20,GRAY);
            DrawText("Press F11 for cinematic mode",5,WINDH-15,10,GRAY);

        }
        EndDrawing();

        oldx = x;
        oldy = y;
        oldzoom = zoom;
        oldres = resolution;
    }

    return 0;
}
