#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>
#include <cmath>
#include<bits/stdc++.h>
#include <iostream>

using namespace std;

struct types
{
    string ShapeName;   //shape name
    string ShapeAlgo;   //shape algorithm
    string ShapeColor;  // shape color
    int xc=-1,xr=-1;    //center and radius for x axis
    int yc=-1,yr=-1;    //center and radius for y axis
    int first=-1,second=-1,third=-1,forth=-1;       //Quarters
};

//Global vector of struct type
vector<types>Shapes;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Bioinformatics_Project");
//definition for menu clicks
#define LINE_PARAMETRIC     1
#define LINE_DDA            2
#define LINE_MID            3
#define CIRCLE_DIRECT       4
#define CIRCLE_POLAR        5
#define CIRCLE_ITERATIVE    6
#define CIRCLE_MIDPOINT     7
#define CIRCLE_MODIFIED     8
#define ELLIPSE_POLAR       9
#define ELLIPSE_DIRECT      10
#define CLIPPING_POINT      11
#define CLIPPING_lINE       12
#define SAVE                13
#define LOAD                14
#define CLEAR               15
#define CIRCLE_FILLING      16
#define Color_id_red        17
#define Color_id_green      18
#define Color_id_blue       19
#define Color_id_black      20
#define Color_id_Yellow     21
#define Color_id_Cyan_Aqua  22
#define Color_id_Magenta_Fuchsia 23
#define Color_id_Silver     24
#define Color_id_Gray       25
#define Color_id_Maroon     26
#define Color_id_Olive      27
#define Color_id_Purple     28
#define Color_id_Teal       29
#define Color_id_Navy       30
#define Fill_first          31
#define Fill_second         32
#define Fill_third          33
#define Fill_fourth         34
#define Exit                35

//declaration of menu
void CreateMenu(HWND hwnd);

string Color_String="Black";

string quad_type;

int Round(double x)
{
    return (int)(x+0.5);
}

COLORREF ChoosingColor(string color){
    if(color=="RED")
        return RGB(255,0,0);
    if(color=="BLUE")
        return RGB(0,0,255);
    if(color=="GREEN")
        return RGB(0,255,0);
    if(color=="BLACK")
        return RGB(0,0,0);
    if(color=="Yellow")
        return RGB(255,255,0);
    if(color=="Cyan / Aqua")
        return RGB(0,255,255);
    if(color=="Magenta / Fuchsia")
        return RGB(255,0,255);
    if(color=="Silver")
        return RGB(192,192,192);
    if(color=="Gray")
        return RGB(128,128,128);
    if(color=="Maroon")
        return RGB(128,0,0);
    if(color=="Olive")
        return RGB(128,128,0);
    if(color=="Purple")
        return RGB(128,0,128);
    if(color=="Purple")
        return RGB(0,128,128);
    if(color=="Purple")
        return RGB(0,0,128);

}

struct point{
    int x,y;
    point(){} //default constructor
    point(int x,int y) //parametrized constructor
    {
        this->x=x;
        this->y=y;
    }
};
//function to get radius length for circle or ellipse
int RadiusLength(point a,point b)
{
    int xLen = abs(a.x)-abs(b.x);
    int yLen = abs(a.y)-abs(b.y);
    return sqrt(pow((xLen),2)+pow((yLen),2));
}

struct ellipse{
    ellipse(){}         //default constructor
    void draw(HDC hdc, int xc, int yc, int xr, int yr,string type, string color_String){
        int length1=0,length2=0;
        point a(xc,yc);     //object of point passing to it center point
        point b(xr,yr);     //object of point passing to it radius point
        COLORREF color = ChoosingColor(color_String);       //passing chosen color to draw with it
        length1=RadiusLength(a,b);                          //large radius
        length2=((double)(1.0/3.0)*(double)RadiusLength(a,b));  //small radius
        if(type=="direct"){                 //type if it is direct
            if(abs(xc-xr)>abs(yc-yr)){         //check to see radius in x axis is greater or not
                direct_ellipse(hdc,xc,yc,length1,length2,color);
            }
            else
                direct_ellipse(hdc,xc,yc,length2,length1,color);
        }
        else if(type =="polar"){            //type if it is polar
            if(abs(xc-xr)>abs(yc-yr)){      //check to see radius in x axis is greater or not
                polar_ellipse(hdc,xc,yc,length1,length2,color);
            }
            else
                polar_ellipse(hdc,xc,yc,length2,length1,color);
        }
    }

    void draw_ellipse(HDC hdc, int xc, int yc,double x,double y,COLORREF color){
        SetPixel(hdc, Round(xc+x), Round(yc+y),color);
        SetPixel(hdc, Round(xc+x), Round(yc-y),color);
        SetPixel(hdc, Round(xc-x), Round(yc-y),color);
        SetPixel(hdc, Round(xc-x), Round(yc+y),color);
    }

    void direct_ellipse(HDC hdc, int xc, int yc, int xr, int yr,COLORREF color){
        double y = xr;
        double x=0;
        draw_ellipse(hdc, xc, yc, y, x,color);
        for (x; x<=y; x+=0.01){
            y =Round(sqrt((1.0-((double)x*(double)x)/((double)xr*(double)xr))*((double)yr*(double)yr)));
            draw_ellipse(hdc, xc, yc, x, y,color);
        }
        x=yr;
        for (y=0; y<=x; y+=0.01){
            x =Round(sqrt((1.0-((double)y*(double)y)/((double)yr*(double)yr))*((double)xr*(double)xr)));
            draw_ellipse(hdc, xc, yc, x, y,color);
        }
    }

    void polar_ellipse(HDC hdc, int xc, int yc, int xr, int yr,COLORREF color){
        double x=0;
        double y=yr;
        double range=xr;
        for(double angle=0; angle<=range; angle+=0.05){
            draw_ellipse(hdc,xc,yc,x,y,color);
            x=(xr*cos(angle));
            y=(yr*sin(angle));
        }
    }
};

struct line{
    line(){}
    void draw(HDC hdc,int x1,int y1,int x2,int y2,string type,string color_String){
        COLORREF color = ChoosingColor(color_String);
        if (type=="parametric")
            Parametric_line(hdc,x1,y1,x2,y2, color);
        if (type=="DDA")
            DDA(hdc,x1,y1,x2,y2, color);
        if(type=="midPoint")
            midPointLine(hdc,x1,y1,x2,y2, color);
    }

    void Parametric_line(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
    {
        double dx = x2-x1;
        double dy = y2-y1;
        double t=0;
        while(t<1){
            int point_x = x1+(t*dx);
            int point_y = y1+(t*dy);
            SetPixel(hdc,point_x,point_y,color);
            t+=0.001;
        }
    }

    void DDA(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
    {
        int dx=x2-x1;
        int dy=y2-y1;
        SetPixel(hdc,x1,y2,color);
        if(abs(dx)>=abs(dy))
        {
            int x=x1,xinc=0;
            //for test swapping if x2>x1
            if (dx>0){
                xinc=1;
            }
            else
                xinc=-1;
            double y=y1, slope=(double)dy/(double)dx*(double)xinc;
            while(x!=x2)
            {
                x+=xinc;
                y+=slope;
                SetPixel(hdc,x,Round(y),color);
            }
        }
        else
        {
            int y=y1,yinc=0;
            //for test swapping if y2>y1
            if (dy>0){
                yinc=1;
            }
            else
                yinc=-1;
            double x=x1,slope=(double)dx/(double)dy*(double)yinc;
            while(y!=y2)
            {
                x+=slope;
                y+=yinc;
                SetPixel(hdc,Round(x),y,color);
            }
        }
    }

    void midPointLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
    {
        int dx = (x2 - x1);
        int dy = (y2 - y1);
        double slope = (dy * 1.0) / dx;
        if((slope >= 0) &&(slope <= 1))
        {
            if(x1 > x2)
                swap(x1, x2), swap(y1, y2), dx = (x2 - x1), dy = (y2 - y1);
            int x = x1, y = y1;
            int d = dx - 2 * dy;
            int change1 = -2 * dy;
            int change2 = 2 * dx - 2 * dy;
            SetPixel(hdc, x, y, c);
            while(x < x2)
            {
                if(d < 0)
                    y++, x++, d += change2;
                else
                    x++, d += change1;
                SetPixel(hdc, x, y, c);
            }
        }
        else if (slope <= -1)
        {
            if(y1 > y2)
                swap(x1, x2), swap(y1, y2);
            else
                dx = -1 * dx, dy = -1 * dy;

            int x = x1, y = y1;
            int d = 2 * dx - dy;
            int change1 = 2 * dx;
            int change2 = 2 * dx + 2 * dy;
            SetPixel(hdc, x, y, c);
            while(y < y2)
            {
                if(d < 0)
                    y++, d += change1;
                else
                    y++, x--, d += change2;
                SetPixel(hdc, x, y, c);
            }
        }
        else if (slope > 1)
        {
            if(y1 > y2)
                swap(x1, x2), swap(y1, y2), dx = (x2 - x1), dy = (y2 - y1);
            int x = x1, y = y1;
            int d = dx - 2 * dy;
            int change1 = 2 * dx;
            int change2 = 2 * dx - 2 * dy;
            SetPixel(hdc, x, y, c);
            while(y < y2)
            {
                if(d < 0)
                    y++, d += change1;
                else
                    x++, y++, d += change2;
                SetPixel(hdc, x, y, c);
            }
        }
            ///   -1 to 0
        else
        {
            if(x1 > x2)
                swap(x1, x2), swap(y1, y2), dx = -1 * dx, dy = -1 * dy;
            int x = x1, y = y1;
            int d = dx + 2 * dy;
            int change1 = 2 * dy;
            int change2 = 2 * dx + 2 * dy;
            SetPixel(hdc, x, y, c);
            while(x < x2)
            {
                if(d > 0)
                    x++, d += change1;
                else
                    x++, y--, d += change2;
                SetPixel(hdc, x, y, c);
            }
        }
    }
};

void Filling(HDC hdc,int x1,int y1,int x2,int y2,string Color,COLORREF fillColor,string quad){
    COLORREF borderColor = ChoosingColor(Color);
    struct line l;
    point a(x1,y1);
    point b(x2,y2);
    int radius = RadiusLength(a,b);
    int y = radius;
    double d = 1 - radius;
    for(int x =0; x<y; x++)
    {
        if(d<=0){
            d = d+(2*x+3);
        }
        else{
            d = d+(2*(x-y)+5);
            y--;
        }
        if(quad=="first"){
            l.Parametric_line(hdc,x1,y1,x1+x,y1-y,fillColor);//First quad
            l.Parametric_line(hdc,x1,y1,x1+y,y1-x,fillColor);//Second quad
        }
        else if(quad =="second"){
            l.Parametric_line(hdc,x1,y1,x1+y,y1+x,fillColor);//third quad
            l.Parametric_line(hdc,x1,y1,x1+x,y1+y,fillColor);//FOURTH quad
        }
        else if(quad == "third")
        {
            l.Parametric_line(hdc,x1,y1,x1-x,y1+y,fillColor);//FIFTH quad
            l.Parametric_line(hdc,x1,y1,x1-y,y1+x,fillColor);//six quad
        }
        else if(quad =="fourth"){
            l.Parametric_line(hdc,x1,y1,x1-y,y1-x,fillColor);//seventh quad
            l.Parametric_line(hdc,x1,y1,x1-x,y1-y,fillColor);//eight quad
        }
    }
}

struct circle{
    circle(){}
    void draw(HDC hdc,int xc,int yc, int xr,int yr,string type,string color_String){
        point a(xc,yc);
        point b(xr,yr);
        int radius = RadiusLength(a,b);
        COLORREF color = ChoosingColor(color_String);
        if(type=="direct")
            DirectCircle(hdc, xc, yc, radius, color);
        if(type=="polar")
            PolarCircle( hdc, xc, yc, radius, color);
        if(type=="iterative")
            IterativePolarCircle(hdc, xc, yc, radius, color);
        if (type=="midPoint")
            MidpointCircle(hdc, xc, yc, radius, color);
        if(type=="modified")
            modifiedMidpointCircle(hdc, xc, yc, radius, color);
    }

    void draw8Points(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
    {
        SetPixel(hdc, xc+x , yc+y , color);
        SetPixel(hdc, xc-x , yc+y , color);
        SetPixel(hdc, xc-x , yc-y , color);
        SetPixel(hdc, xc+x , yc-y , color);
        SetPixel(hdc, xc+y , yc+x , color);
        SetPixel(hdc, xc-y , yc+x , color);
        SetPixel(hdc, xc-y , yc-x , color);
        SetPixel(hdc, xc+y , yc-x , color);
    }

    void DirectCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
    {
        int x=0;
        int y = R;
        int R2 = R*R;
        draw8Points(hdc,xc,yc,x,y,color);
        for(x; x<y; x++)
        {
            y = Round(sqrt((double)(R2-x*x)));
            draw8Points(hdc,xc,yc,x,y,color);
        }
    }

    void PolarCircle(HDC hdc,int xc,int yc,int R,COLORREF color)
    {
        double x = R;
        double y = 0;
        double theta = 0;
        double dtheta = 1.0/R;
        draw8Points(hdc,xc,yc,R,0,color);
        while(x>y)
        {
            theta+=dtheta;
            x = R*cos(theta);
            y = R*sin(theta);
            draw8Points(hdc,xc,yc,Round(x),Round(y),color);
        }
    }

    void IterativePolarCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
    {
        double x = R;
        double y = 0;
        double dtheta = 1.0/R;
        double cdtheta = cos(dtheta);
        double sdtheta = sin(dtheta);
        draw8Points(hdc,xc,yc,R,0,color);
        while(x>y)
        {
            double x1 = x*cdtheta - y*sdtheta;
            y = x*sdtheta + y*cdtheta;
            x = x1;
            draw8Points(hdc,xc,yc,round(x),round(y),color);
        }
    }

    void MidpointCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
    {
        int x=0;
        int y=R;
        int d=1-R;
        draw8Points(hdc,xc,yc,x,y,color);
        for(x; x<y; x++)
        {
            if(d<0){
                d+=2*x+2;
            }
            else{
                d+=2*(x-y)+5;
                y--;
            }
            draw8Points(hdc,xc,yc,x,y,color);
        }
    }

    void modifiedMidpointCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
    {
        int x=0;
        int y=R;
        int d=1-R;
        int c1=3;
        int c2=5-2*R;
        draw8Points(hdc,xc,yc,x,y,color);
        for(x; x<y; x++){
            if(d<0){
                d+=c1;
                c2+=2;
            }
            else{
                d+=c2;
                c2+=4;
                y--;
            }
            c1+=2;
            draw8Points(hdc,xc,yc,x,y,color);
        }
    }
};

//declaration of 4 bits
union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;     //L1 T1 R1 B1
    };
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{//get side code of the window  in respect to point (x,y)
    OutCode out;    //contain 4 bits
    out.All=0;      //decleared to 0
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    if(y<ytop)
        out.top=1;
    else if(y>ybottom)
        out.bottom=1;
    return out;
}

//point clipping
void PointClipping(HDC hdc, int x,int y,int xleft,int ytop,int xright,int ybottom,string color_string)
{//to test if the point x and y in the screen or not
    COLORREF color = ChoosingColor(color_string);
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)//y>+top as screen coordinates in c++
    {
        SetPixel(hdc,x,y,color);
    }
}

//Line clipping
void Vertical_Intersect(double x1,double y1,double x2,double y2,int x,double *xi,double *yi)
{
    double dx = x2-x1;
    double dy = y2-y1;
    double slope = dy/dx;
    *xi=x;
    *yi=y1+(x-x1)*slope;
}

void Horizontal_Intersect(double x1,double y1,double x2,double y2,int y,double *xi,double *yi)
{
    double dx = x2-x1;
    double dy = y2-y1;
    double slope = dx/dy;
    *yi=y;
    *xi=x1+(y-y1)*slope;
}

void CohenAlgo(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{   //to test line in the window or not
    double x1 = xs*1.0;
    double y1 = ys*1.0;
    double x2 = xe*1.0;
    double y2 = ye*1.0;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)
                Vertical_Intersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)
                Horizontal_Intersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)
                Vertical_Intersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                Horizontal_Intersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                Vertical_Intersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)
                Horizontal_Intersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)
                Vertical_Intersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                Horizontal_Intersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {

        MoveToEx(hdc,Round(x1),Round(y1),NULL);
        LineTo(hdc,Round(x2),Round(y2));
    }
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);    //mouse pointer change to cross
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */

    /* Use Windows's default colour as the background of the window */
    //wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND + 5;              //from gray to white
    //wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;                 //default
    //wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);      //from gray to white
     wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));     //from gray to white
    //wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);             //from gray to white

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
            0,                   /* Extended possibilites for variation */
            szClassName,         /* Classname */
            _T("Graphics Project"),       /* Title Text */
            WS_OVERLAPPEDWINDOW, /* default window */
            CW_USEDEFAULT,       /* Windows decides the position */
            CW_USEDEFAULT,       /* where the window ends up on the screen */
            1200,                 /* The programs width */
            600,                 /* and height in pixels */
            HWND_DESKTOP,        /* The window is a child-window to desktop */
            NULL,                /* No menu */
            hThisInstance,       /* Program Instance handler */
            NULL                 /* No Window Creation data */
    );
    /* Make the window visible on the screen */
    CreateMenu(hwnd);
    ShowWindow (hwnd, nCmdShow);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void Save(const vector<types>&Shape){
    ofstream file("shapes.txt");        //file name to save file using ofstream
    for (auto const &i : Shape){        //for loop for exporting file contents // access by const reference
        file<< i.ShapeName<<" "<<i.ShapeAlgo<<" "<<i.ShapeColor<<" "<<i.xc<<" "<<i.yc<<" "<<i.xr<<" "<<i.yr<<" "<<i.first<<" "<<i.second<<" "<<i.third<<" "<<i.forth<<" "<<endl;
    }
}

void load(HWND hwnd,vector<types>&Shape){
    HDC hdc=GetDC(hwnd);
    fstream file("shapes.txt");
    PAINTSTRUCT p ;     //struct contain all the windows draws
    bool check =false;
    string type, shape, color;
    types shape_obj;
    int center_x,center_y,radius_x,radius_y,first,second,forth,third,ch1,ch2,ch3,ch4;
    circle cir;
    line lin;
    ellipse ell;

    while (file>>shape>>type>>color>>center_x>>center_y>>radius_x>>radius_y>>first>>second>>third>>forth) {
        shape_obj.ShapeName=shape ,shape_obj.ShapeAlgo=type,shape_obj.ShapeColor=color;
        shape_obj.xc=center_x,shape_obj.yc=center_y,shape_obj.xr=radius_x,shape_obj.yr=radius_y,shape_obj.first=first,shape_obj.second=second,shape_obj.third=third,shape_obj.forth=forth;
        if(shape=="Ellipse"){
            ell.draw(hdc,center_x,center_y,radius_x,radius_y,type,color);
        }
        if(shape=="Circle"){
            cir.draw(hdc,center_x,center_y,radius_x,radius_y,type,color);
        }
        if(shape=="Fill"){
            Filling(hdc,center_x,center_y,radius_x,radius_y,color,RGB(255,0,0),quad_type);
        }
        if(shape=="Line"){
            lin.draw(hdc,center_x,center_y,radius_x,radius_y,type,color);
        }
        if(shape=="Clipping"){
            if(!check||(ch1!=center_x&&ch2!=center_y&&ch3!=radius_x&&ch4!=radius_y)) {
                Rectangle(hdc, center_x, center_y, radius_x, radius_y);
                check=true;
                ch1=center_x,ch2=center_y,ch3=radius_x,ch4=radius_y;
            }
            if(type=="point"){
                PointClipping(hdc,first,second,center_x,center_y,radius_x,radius_y,color);
            }
            if(type=="line"){
                CohenAlgo(hdc,first,second,third,forth,center_x,center_y,radius_x,radius_y);
            }
        }
        Shape.push_back(shape_obj);
    }
}

void clearScreen(HWND hwnd){
    CloseWindow(hwnd);
    ShowWindow(hwnd,1);
    Shapes.clear();
}

//Global variables
int x,y;
bool flag = false;
point c, radius;
vector<point> points;
string type="DDA";
ellipse ell;
string shape ="Line";
circle cir;
line lin;
//Structure named shap of data type types
types shape_obj;
//int X1,Y1,X2,Y2;

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT p;       //struct contain all the windows draws
    switch (message)                  /* handle the messages */
    {
        case WM_PAINT:
            BeginPaint(hwnd, &p);
            if (flag)
            {
                shape_obj.ShapeName=shape;
                shape_obj.ShapeAlgo=type;
                shape_obj.ShapeColor=Color_String;
                shape_obj.xc=c.x;
                shape_obj.yc=c.y;
                shape_obj.xr=radius.x;
                shape_obj.yr=radius.y;

                if(shape=="Ellipse"){
                    ell.draw(p.hdc,c.x,c.y,radius.x,radius.y,type,Color_String);
                }
                if(shape=="Circle"){
                    cir.draw(p.hdc,c.x,c.y,radius.x,radius.y,type,Color_String);
                }
                if(shape=="Fill"){
                    Filling(p.hdc,c.x,c.y,radius.x,radius.y,Color_String,RGB(255,0,0),quad_type);
                }
                if(shape=="Line"){
                    lin.draw(p.hdc,c.x,c.y,radius.x,radius.y,type,Color_String);
                }
                if(shape=="Clipping"){
                    if(type=="point"){
                        shape_obj.first=points[2].x;
                        shape_obj.second=points[2].y;
                        PointClipping(p.hdc,points[2].x,points[2].y,points[0].x,points[0].y,points[1].x,points[1].y,Color_String);
                        points.pop_back();
                    }
                    if(type=="line"){
                        shape_obj.first=points[2].x;
                        shape_obj.second=points[2].y;
                        shape_obj.third=points[3].x;
                        shape_obj.forth=points[3].y;
                        CohenAlgo(p.hdc,points[2].x,points[2].y,points[3].x,points[3].y,points[0].x,points[0].y,points[1].x,points[1].y);
                        points.pop_back();
                        points.pop_back();
                    }
                }
                Shapes.push_back(shape_obj);
                cout<<"SHAPE: "<<shape<<endl;
                if(shape=="Fill")
                    cout<<"Quad: "<<quad_type<<endl;
                else
                    cout<<"TYPE: "<<type<<endl;
                cout<<"Color: "<<Color_String<<endl;
                flag=false;
            }
            EndPaint(hwnd, &p);
            break;

        case WM_LBUTTONDOWN:
            if(!flag&&shape!="Fill"&&shape!="Clipping")
            {
                if (points.size()==2)
                {
                    points.clear();
                }
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                points.push_back(point(x,y));
                if (points.size()==2)
                {
                    flag=true;

                    c=points[0];
                    radius=points[1];
                }
            }
            else if(!flag&&shape=="Clipping"){
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                points.push_back(point(x,y));
                if(points.size()==1){
                    c.x=x;
                    c.y=y;
                }
                if(points.size()==2){
                    radius.x=x;
                    radius.y=y;
                }
                if(points.size()==2){
                    HDC hdc=GetDC(hwnd);

                    Rectangle(hdc, c.x,c.y,radius.x,radius.y);
                    cout<<"RECTANGLE"<<endl;
                }

                if (type=="point")
                {

                    if(points.size()>2){
                        flag=true;
                    }
                }
                if (points.size()==4&&type=="line")
                {
                    if(points.size()==4){
                        flag=true;
                    }
                }
            }
            InvalidateRect(hwnd, NULL, false);
            break;
        case WM_CREATE:
            CreateMenu(hwnd);
            break;
        case WM_COMMAND:

            if (LOWORD(wParam) == Color_id_red) {
                Color_String="RED";
            }
            else if (LOWORD(wParam) == Color_id_green) {
                Color_String="GREEN";
            }
            else if (LOWORD(wParam) == Color_id_blue) {
                Color_String="BLUE";
            }
            else if (LOWORD(wParam) == Color_id_black) {
                Color_String="BLACK";
            }
            else if (LOWORD(wParam) == Color_id_Yellow) {
                Color_String="Yellow";
            }
            else if (LOWORD(wParam) == Color_id_Cyan_Aqua) {
                Color_String="Cyan / Aqua";
            }
            else if (LOWORD(wParam) == Color_id_Magenta_Fuchsia) {
                Color_String="Magenta / Fuchsia";
            }
            else if (LOWORD(wParam) == Color_id_Silver) {
                Color_String="Silver";
            }
            else if (LOWORD(wParam) == Color_id_Gray) {
                Color_String="Gray";
            }
            else if (LOWORD(wParam) == Color_id_Maroon) {
                Color_String="Maroon";
            }
            else if (LOWORD(wParam) == Color_id_Olive) {
                Color_String="Olive";
            }
            else if (LOWORD(wParam) == Color_id_Purple) {
                Color_String="Purple";
            }
            else if (LOWORD(wParam) == Color_id_Teal) {
                Color_String="Teal";
            }
            else if (LOWORD(wParam) == Color_id_Navy) {
                Color_String="Navy";
            }

            else if(LOWORD(wParam) == ELLIPSE_DIRECT){
                shape ="Ellipse";
                type="direct";
            }
            else if(LOWORD(wParam) == ELLIPSE_POLAR){
                shape ="Ellipse";
                type="polar";
            }
            else if(LOWORD(wParam) == CIRCLE_DIRECT){
                shape ="Circle";
                type="direct";
            }
            else if(LOWORD(wParam) == CIRCLE_POLAR){
                shape ="Circle";
                type="polar";
            }
            else if(LOWORD(wParam) == CIRCLE_ITERATIVE){
                shape ="Circle";
                type="iterative";
            }
            else if(LOWORD(wParam) == CIRCLE_MIDPOINT){
                shape ="Circle";
                type="midPoint";
            }
            else if(LOWORD(wParam) == CIRCLE_MODIFIED){
                shape ="Circle";
                type="modified";
            }
            else if(LOWORD(wParam) == Fill_first){
                shape ="Fill";
                quad_type="first";
                flag=true;
                InvalidateRect(hwnd, NULL, false);
            }
            else if(LOWORD(wParam) == Fill_second){
                shape ="Fill";
                quad_type="second";
                flag=true;
                InvalidateRect(hwnd, NULL, false);
            }
            else if(LOWORD(wParam) == Fill_third){
                shape ="Fill";
                quad_type="third";
                flag=true;
                InvalidateRect(hwnd, NULL, false);
            }
            else if(LOWORD(wParam) == Fill_fourth){
                shape ="Fill";
                quad_type="fourth";
                flag=true;
                InvalidateRect(hwnd, NULL, false);
            }
            else if(LOWORD(wParam) == LINE_PARAMETRIC){
                shape ="Line";
                type="parametric";
            }
            else if(LOWORD(wParam) == LINE_MID){
                shape ="Line";
                type="midPoint";
            }
            else if(LOWORD(wParam) == LINE_DDA){
                shape ="Line";
                type="DDA";
            }
            else if(LOWORD(wParam) == CLIPPING_POINT){
                shape ="Clipping";
                type="point";
                points.clear();
            }
            else if(LOWORD(wParam) == CLIPPING_lINE){
                shape ="Clipping";
                type="line";
                points.clear();
            }
            else if(LOWORD(wParam) == SAVE){
                Save(Shapes);
            }
            else if(LOWORD(wParam) == LOAD){
                load(hwnd,Shapes);
            }
            else if(LOWORD(wParam) == CLEAR){
                clearScreen(hwnd);
            }
            else if(LOWORD(wParam) == Exit){
                PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            }
            cout<<Color_String<<endl;
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}

void CreateMenu(HWND hwnd)
{
    HMENU hMenubar = CreateMenu();
    HMENU hMenu = CreateMenu();
    HMENU data = CreateMenu();
    HMENU hMenuColor=CreateMenu();
    HMENU lineSubMenu = CreatePopupMenu();
    HMENU circleSubMenu = CreatePopupMenu();
    HMENU ellipseSubMenu = CreatePopupMenu();
    HMENU clippingSubMenu = CreatePopupMenu();
    HMENU hMenuFill=CreateMenu();
    HMENU hExit = CreateMenu();
    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) lineSubMenu, L"&Line");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_PARAMETRIC, L"&Parametric");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_DDA, L"&DDA");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_MID, L"&Midpoint");

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) circleSubMenu, L"&Circle");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_DIRECT, L"&Direct");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_POLAR, L"&Polar");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_ITERATIVE, L"&Iterative Polar");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_MIDPOINT, L"&Midpoint");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_MODIFIED, L"&Modified Midpoint");
    AppendMenuW(hMenuFill, MF_STRING, Fill_first, L"&First");
    AppendMenuW(hMenuFill, MF_STRING, Fill_second,L"&Second");
    AppendMenuW(hMenuFill, MF_STRING, Fill_third, L"&Third");
    AppendMenuW(hMenuFill, MF_STRING, Fill_fourth, L"&Fourth");
    AppendMenuW(circleSubMenu, MF_STRING|MF_POPUP, (UINT_PTR) hMenuFill, L"&Quarter Filling");

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) ellipseSubMenu, L"&Ellipse");
    AppendMenuW(ellipseSubMenu, MF_STRING, ELLIPSE_DIRECT, L"&Direct");
    AppendMenuW(ellipseSubMenu, MF_STRING, ELLIPSE_POLAR, L"&Polar");

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) clippingSubMenu, L"&Clipping");
    AppendMenuW(clippingSubMenu, MF_STRING, CLIPPING_POINT, L"&Clipping Point");
    AppendMenuW(clippingSubMenu, MF_STRING, CLIPPING_lINE, L"&Clipping Line");


    AppendMenuW(hMenuColor, MF_STRING, Color_id_red, L"&RED");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_green,L"&GREEN");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_blue, L"&BLUE");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_black, L"&Black");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Yellow, L"&Yellow");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Cyan_Aqua, L"&Cyan / Aqua");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Magenta_Fuchsia, L"&Magenta / Fuchsia");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Silver, L"&Silver");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Gray, L"&Gray");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Maroon, L"&Maroon");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Olive, L"&Olive");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Purple, L"&Purple");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Teal, L"&Teal");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_Navy, L"&Navy");

    AppendMenuW(data, MF_STRING, LOAD, L"&Load");
    AppendMenuW(data, MF_STRING, SAVE, L"&Save");
    AppendMenuW(data, MF_STRING, CLEAR, L"&Clear");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hMenu, L"&Algorithms");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hMenuColor, L"&Colors");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) data, L"&Tools");

    AppendMenuW(hExit, MF_STRING, Exit, L"&Exit");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hExit, L"&Quit");
    SetMenu(hwnd, hMenubar);
}

