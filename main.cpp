#include <cmath>
#include <raylib/raylib.h>
#include <thread>


#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>

using namespace std;

#define WINDW_REAL 400
#define WINDH_REAL 400

//for calculations
#define WINDW 400
#define WINDH 400

#define LINE_NUM 5

const float W_LINE_SPAC = (float)WINDW_REAL/LINE_NUM;
const float H_LINE_SPAC = (float)WINDH_REAL/LINE_NUM;

float x = 0;
float y = 0;

void DrawVertGridLine(float offset){
    DrawLine(offset+fmod(x*WINDW_REAL,W_LINE_SPAC),0,offset+fmod(x*WINDW_REAL,W_LINE_SPAC),WINDH_REAL,GRAY);
}

void DrawHorizGridLine(float offset){
    DrawLine(0,offset+fmod(y*WINDH_REAL,H_LINE_SPAC),WINDW_REAL,offset+fmod(y*WINDH_REAL,H_LINE_SPAC),GRAY);
}

float zoom = 1;

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

void ThreadBuffer(Color *data,int startrow,int rows){
    for(int iy=startrow;iy<rows;iy++){
        for(int ix=0;ix<WINDW;ix++){
            int mandelbrot = getMandelbrot((float)ix/WINDW/zoom-x,(float)iy/WINDH/zoom-y);
            data[iy*WINDW+ix] = (Color){(unsigned char)(mandelbrot*50),(unsigned char)(mandelbrot*10),(unsigned char)(mandelbrot*255),(unsigned char)((mandelbrot/(10*resolution))*255)};
            if(mandelbrot == 0) {(data[iy*WINDW+ix]).a = 255;}
        }
    }
}

int main(int argc, char* argv[]){
    const int NUM_THREADS = atoi(argv[1]);

#define THREAD_LOWSTEP floor(WINDH/NUM_THREADS)
#define THREAD_HIGHSTEP ceil(WINDH/NUM_THREADS)


    InitWindow(WINDW_REAL, WINDH_REAL, "Raylib - Mandelbrot set");

    SetWindowIcon(LoadImage("icon.png"));

    SetTargetFPS(240);
    Image image = GenImageColor(WINDW, WINDH, RED);  // Initial image filled with red
    Texture2D pixels = LoadTextureFromImage(image);  // Create texture from the image
    SetTextureFilter(pixels, 0);
    SetTextureWrap(pixels, TEXTURE_WRAP_CLAMP);
    UnloadImage(image); // Once the texture is loaded, unload the image
    Color pixels_data[WINDW*WINDH];

    while(!WindowShouldClose()){

        if(IsKeyDown(KEY_W)){
            y+=velocity/40;
        }if(IsKeyDown(KEY_S)){
            y-=velocity/40;
        }if(IsKeyDown(KEY_A)){
            x+=velocity/40;
        }if(IsKeyDown(KEY_D)){
            x-=velocity/40;
        }

        if(IsKeyPressed(KEY_F11)){
            cinematicmode = !cinematicmode;
        }

        Vector2 mwheel = GetMouseWheelMoveV();        

            zoom-= mwheel.x*mwheel_mult;
            zoom+= mwheel.y*mwheel_mult;

        if(x != oldx || y != oldy || zoom != oldzoom || resolution != oldres){
            thread* threads[NUM_THREADS];
            for(int i=0;i<NUM_THREADS;i++){
                if(i==NUM_THREADS){
                    thread t (ThreadBuffer,pixels_data, THREAD_LOWSTEP*i, WINDH);
                    threads[i] = new thread (ThreadBuffer,pixels_data, THREAD_LOWSTEP*i, WINDH);
                    break;
                }
                threads[i] = new thread (ThreadBuffer,pixels_data, THREAD_LOWSTEP*i, THREAD_HIGHSTEP*(i+1));
            }
            for(thread *t : threads){
                t->join();
            }
            UpdateTexture(pixels, pixels_data);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexturePro(pixels,{0,0,WINDW,WINDH},{0,0,WINDW_REAL,WINDH_REAL},{0,0},0.0f,WHITE);

        if(!cinematicmode){
            GuiSliderBar({220,330,140,10}, "Mousewheel speed", TextFormat("%.2f",mwheel_mult*10), &mwheel_mult, 0.1, 50);
            oldres = resolution;
            GuiSliderBar({220,350,140,10}, "Resolution", TextFormat("%.2f",resolution), &resolution, 0, 15);
            GuiSliderBar({220,370,140,10}, "Speed", TextFormat("%.4f",velocity), &velocity, 0.0001, 1);
                        
            DrawFPS(5, 5);
            
            DrawText(TextFormat("x: %f",x),5,20,20,GRAY); 
            DrawText(TextFormat("y: %f",y),5,35,20,GRAY); 
            DrawText(TextFormat("THREADS: %i",NUM_THREADS),5,50,20,GRAY); 
            
            DrawText("Press F11 for cinematic mode",5,385,10,GRAY);

        }
        EndDrawing();

        oldx = x;
        oldy = y;
        oldzoom = zoom;
    }

    return 0;
}
