#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>
#include<iostream>
using namespace std;

#define PI 3.14159265


int Size_x , Size_y ;
int x_real[1000]  , y_real[1000] ;
int x_Device[1000], y_Device[1000] ;
int n , D_max_X,D_min_X,D_max_Y,D_min_Y;
int min_x , max_x , min_y , max_y ;
int R_max_X,R_max_Y,R_min_Y,R_min_X ;
double x_scale , y_scale ,Scale;

struct point
{
    int x;
    int y;
};

void initial_window(int initPos_X ,int initPos_Y ,int Size_x ,int Size_y ,char name[] , int min_x , int max_x , int min_y , int max_y); // function to initialization windows
void Draw_Point(int x , int y); // function to draw point
void input_data(int *x_real , int *y_real); // to read data from user
void line(int x1 , int y1 , int x2 , int y2 , point arr_point[] , int &arrSize); // function to draw line
void polyline(int *x , int *y , int Size , point arr_point[] , int &arrSize); // function to draw polyline
void polyline_mapping_fit();
void mapping(int *x_Device , int *y_Device); // function to convert from real to device

int main()
{

    input_data(x_real , y_real );
    mapping(x_Device , y_Device);
    initial_window(100 , 100 , Size_x , Size_y ,"Mapping" , min_x , max_x ,min_y , max_y);
    glutDisplayFunc(polyline_mapping_fit);
    glutMainLoop();
    return 0 ;
}

void initial_window(int initPos_X ,int initPos_Y ,int Size_x ,int Size_y ,char name[] , int min_x , int max_x , int min_y , int max_y)
{
    glutInitWindowPosition(initPos_X , initPos_Y);
    glutInitWindowSize(Size_x , Size_y);
    glutCreateWindow(name);
    glClearColor(1.0,1.0,1.0,0.0);
    glLoadIdentity();
    gluOrtho2D(min_x , max_x , min_y , max_y);
}

void Draw_Point(int x , int y)
{
    glPointSize(2);
    glBegin(GL_POINTS);
    glColor3f(0,0,0);
    glVertex2i(x , y) ;
    glEnd();
}

void input_data(int *x_real , int *y_real )
{                                                     ////////////////////////////////////
    cout << "enter number of points please ";         //    read input from user        //
    cin >> n;                                         //    number of point in polyline //
    cout << "please enter value of device max for x"; //                                //
    cin >> D_max_X;                                   //    device max for x            //
    cout << "please enter value of device max for y"; //    device max for y            //
    cin >> D_max_Y;                                   //                                //
    cout << "please enter value of device min for x"; //    device min for x            //
    cin >> D_min_X;                                   //    device min for y            //
    cout << "please enter value of device min for y"; ////////////////////////////////////
    cin >> D_min_Y;

    for(int i = 0 ; i < n ; i++) // to read point(x,y) of polyline
    {
        cout << "Enter Value of point " << i+1 << endl;
        cin >> x_real[i]>>y_real[i];
        // to Determines maximum x and y in real
        // and minimum x and y in real
        if(i == 0)
        {
            R_max_X = R_min_X = x_real[i] ;
            R_max_Y = R_min_Y = y_real[i] ;
        }
        if(x_real[i] > R_max_X)
        {
            R_max_X = x_real[i] ;
        }
        else if(x_real[i] < R_min_X)
        {
            R_min_X = x_real[i] ;
        }
        if(y_real[i] > R_max_Y)
        {
            R_max_Y = y_real[i] ;
        }
        else if(y_real[i] < R_min_Y)
        {
            R_min_Y = y_real[i] ;
        }
    }
    // to Determines high and width window  [size of window]
    Size_x = D_max_X - D_min_X;
    Size_y = D_max_Y - D_min_Y;
}

void mapping(int *x_Device , int *y_Device)
{

    //In order to make sure not to divide by zero
    // when calculate x_scale and y_scale
    if( (R_max_X-R_min_X) == 0)
    {
        x_scale = 10000 ;
    }
    else
    {
        x_scale = ((D_max_X-D_min_X)/(R_max_X-R_min_X));
    }
    cout << "the value of x_scale " << x_scale << endl;

    if( (R_max_Y-R_min_Y) == 0)
    {
        y_scale = 10000 ;
    }
    else
    {
        y_scale = ((D_max_Y-D_min_Y)/(R_max_Y-R_min_Y));
    }
    cout << "the value of y_scale " << y_scale << endl;

    //𝑥_scale =  y_scale sℎ𝑎𝑝𝑒𝑠 𝑝𝑙𝑜𝑡𝑠 𝑤𝑖𝑙𝑙 𝑏𝑒 𝑤𝑖𝑡ℎ𝑜𝑢𝑡 𝑑𝑖𝑠𝑡𝑜𝑟𝑡𝑖𝑜𝑛
    //Choose the smallest to draw in window
    if(x_scale < y_scale)
    {
        Scale = x_scale ;
    }
    else if(x_scale > y_scale)
    {
        Scale = y_scale ;
    }
    else
    {
        Scale = y_scale = x_scale ;
    }


    // Determines all point in device
    for(int i = 0 ; i < n ; i++)
    {
        x_Device[i] = (D_min_X+(Scale*(x_real[i]-R_min_X)));

    }
    for(int j = 0 ; j < n ; j++ )
    {
        y_Device[j] = (D_min_Y+(Scale*(y_real[j]-R_min_Y)));
    }

    // Determines max_x and max_y and minimum
    max_x = min_x = x_Device[0];
    max_y = min_y = y_Device[0];
    for(int i = 1 ; i < n ; i++)
    {
        if(x_Device[i] > max_x)
        {
            max_x = x_Device[i]+5 ;
        }
        else if(x_Device[i] < min_x)
        {
            min_x = x_Device[i]-5 ;
        }
        if(y_Device[i] > max_y)
        {
            max_y = y_Device[i]+5 ;
        }
        else if(y_Device[i] < min_y)
        {
            min_y = y_Device[i]-5 ;
        }
    }

    cout << "the values of device point are "<<endl;
    for(int i = 0 ; i < n ; i++)
    {
        cout << x_Device[i] << "  "<<y_Device[i]<<endl;
    }

}

void line(int x1 , int y1 , int x2 , int y2 , point arr_point[] , int &arrSize)
{
    int minX , minY , maxX , maxY ;

    if(x1 > x2)
    {
        maxX = x1 ;
        minX = x2 ;
    }
    else
    {
        maxX = x2 ;
        minX = x1 ;
    }

    if(y1 > y2)
    {
        maxY = y1 ;
        minY = y2 ;
    }
    else
    {
        maxY = y2 ;
        minY = y1 ;
    }

    if(x1 == x2)
    {
       for(int y = minY ; y < maxY ; y++)
       {
           Draw_Point(x1 , y);
           arr_point[arrSize].x = x1  ;
           arr_point[arrSize].y = y ;
           arrSize++;
       }
       return ;
    }

    if(y1 == y2)
    {
       for(int x = minX ; x < maxX ; x++)
       {
           Draw_Point(x , y1);
           arr_point[arrSize].x = x  ;
           arr_point[arrSize].y = y1 ;
           arrSize++;
       }
       return ;
    }

    float m = (float)(y2 - y1)/(x2 - x1);
    float c = y2 - (float)m * x2 ;

    if (abs(int(m)) <= 1)
    {
        int y ;
        for(int x = minX ; x <= maxX ; x++)
        {
            y = (int)((m * x) + c + 0.5);
            Draw_Point(x , y);
            arr_point[arrSize].x = x ;
            arr_point[arrSize].y = y ;
            arrSize++;
        }
    }
    else
    {
        int x ;
        for(int y = minY ; y <= maxY ; y++)
        {
            x = (int)((( y - c ) / m ) + 0.5);
            Draw_Point(x , y);
            arr_point[arrSize].x = x ;
            arr_point[arrSize].y = y ;
            arrSize++;
        }
    }
}

void polyline(int *x , int *y , int Size , point arr_point[] , int &arrSize)
{
    for (int i = 0 ; i < (Size - 1 ) ; i++)
    {
        line(x[i] , y[i] , x[i + 1 ] , y[i + 1] ,arr_point ,arrSize);
    }
}

void polyline_mapping_fit()
{
    glClear(GL_COLOR_BUFFER_BIT);
    int arrSize = 0 ;
    point arr_point[1500];
    polyline(x_Device , y_Device, n , arr_point , arrSize );
    glFlush();
}
