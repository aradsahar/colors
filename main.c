#include<stdio.h>
#define RGB_SIZE 3
#define SCREEN_X 20
#define SCREEN_Y 20


struct Pixel
{
    int background[RGB_SIZE];
    int foreground[RGB_SIZE];
    float alpha;
    char texture;
};


struct Pixel get_pixel_addition(struct Pixel first, struct Pixel last)
{
    struct Pixel addition;
    int difference;
    for (int i = 0; i < RGB_SIZE; i++)
    {
        difference = last.background[i] - first.background[i];
        addition.background[i] = first.background[i] + difference * last.alpha;
        if (last.texture)
        {
            addition.foreground[i] = last.foreground[i];
        }
        else
        {
            difference = last.background[i] - first.foreground[i];
            addition.foreground[i] = first.foreground[i] + difference * last.alpha;
        }
    }
    addition.alpha = first.alpha + last.alpha;
    addition.texture = last.texture ? last.texture : first.texture;
    return addition;
}


void place_pixel(struct Pixel pixel, int location[2])
{
    printf("\e[%i;%iH\e[48;2;%i;%i;%im\e[38;2;%i;%i;%im%c",
    location[0], location[1],
    pixel.background[0], pixel.background[1], pixel.background[2],
    pixel.foreground[0], pixel.foreground[1], pixel.foreground[2],
    pixel.texture);
}


void print_pixel(struct Pixel pixel)
{
    printf("\e[48;2;%i;%i;%im\e[38;2;%i;%i;%im%c",
    pixel.background[0], pixel.background[1], pixel.background[2],
    pixel.foreground[0], pixel.foreground[1], pixel.foreground[2],
    pixel.texture);
}


void add_pixel(struct Pixel pixel, int location[2], struct Pixel screen_memory[SCREEN_X][SCREEN_Y])
{
    int x = location[0], y = location[1];
    screen_memory[x][y] = get_pixel_addition(screen_memory[x][y], pixel);
    place_pixel(screen_memory[x][y], location);
}


void set_pixel(struct Pixel pixel, int location[2], struct Pixel screen_memory[SCREEN_X][SCREEN_Y])
{
    int x = location[0], y = location[1];
    screen_memory[x][y] = pixel;
    place_pixel(screen_memory[x][y], location); // less readable optimization: screen[x][y] -> pixel (i think)
}


void initialize_screen(struct Pixel screen_memory[SCREEN_X][SCREEN_Y], struct Pixel background)
{
    for (int y = 0; y < SCREEN_Y; y++)
    {
        for (int x = 0; x < SCREEN_X; x++)
        {
            screen_memory[x][y] = background;
            print_pixel(background);          
        }
        printf("\e[0m\n");
    }
}


int main()
{   
    struct Pixel screen_memory[SCREEN_X][SCREEN_Y];
    struct Pixel background = {{255, 255, 255}, {0, 0, 0}, 1.0, ' '};
    struct Pixel pixel = {{32, 49, 102}, {0, 0, 0}, 1.0, '#'};
    initialize_screen(screen_memory, background);
    int point[2] = {3, 4};
    add_pixel(pixel, point, screen_memory);
    return 0;
}
