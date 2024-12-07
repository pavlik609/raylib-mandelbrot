#version 330 core

out vec4 fragColor; // Output color for the fragment
vec2 fragCoord = gl_FragCoord.xy; // This gives pixel coordinates

uniform float x;
uniform float y;
uniform int WINDW;
uniform int WINDH;
uniform float resolution;
uniform float zoom;

int getMandelbrot(float ix,float iy){
    float z_real = 0;
    float z_imag = 0;

    float c_real = ix;
    float c_imag = iy;

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

void main()
{
    int mandelbrot = getMandelbrot((fragCoord.x-WINDW/2)/zoom-x/2,(fragCoord.y-WINDH/2)/zoom+y/2);
    float alpha = float(mandelbrot)/(10*resolution);
    if(mandelbrot == 0){
        alpha = 1.0;
    }
                                                                    //mod((mandelbrot*255),255.0)/255 is also cool
    fragColor = vec4(mod((mandelbrot*50),256.0)/255, mod((mandelbrot*10),256.0)/255, mod((mandelbrot*255),256.0)/255, alpha);
}
