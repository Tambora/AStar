#ifndef GRAPHICS_H
#define GRAPHICS_H

// set the drawing screen dimensions and position
#define WINDOW_HEIGHT   800
#define WINDOW_WIDTH    1280
#define WINDOW_X        100
#define WINDOW_Y        150

// set the pre-defined colors
#define WHITE           1.0,1.0,1.0
#define BLACK           0.0,0.0,0.0
#define RED             1.0,0.0,0.0
#define BLUE            0.0,0.0,1.0
#define GREEN           0.0,1.0,0.0
#define GREY            1.0,0.5,0.0
#define PURPLE          0.5,0.25,0.0
#define FOREST_GREEN    0.0,0.25,0.0
#define MIDNIGHT_BLUE   0.0,0.0,0.25
#define CYAN            0.0,1.0,1.0
#define MAGENTA         1.0,0.0,1.0
#define YELLOW          1.0,0.5,0.0
#define BROWN           0.5,0.25,0.0

// Theme
#define DARKZERO     0.027,  0.0,  0.11
#define DARKONE     0.047,  0.066,  0.152
#define DARKTWO     0.25,   0.004,  0.199
#define DARKTHREE   0.334,  0,      0.1875
#define DARKFOUR    0.547,  0.008,  0.199
#define DARKFIVE    0.648,  0.012,  0.133

// initialization routine
void graphicsSetup(int argc, char **argv);
void drawScene();
void clearWindow();

// set line or fill color
void setColor(double red, double green, double blue);

// graphic object primatives
void drawTriangle(int x1, int y1,int x2,int y2,int x3,int y3);
void drawLine(int x1, int y1, int x2, int y2);
void drawBox(int x1, int y1, int x2, int y2);
void drawCircle(int x1, int y1, int radius);

// filled graphics primatives
void drawFilledTriangle(int x1, int y1,int x2,int y2,int x3,int y3);
void drawFilledBox(int x1, int y1, int x2, int y2);
void drawFilledCircle(int x1, int y1, int radius);


#endif
