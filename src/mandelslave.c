
#include </software/include/pvm3.h>
#include <stdio.h>
#include <time.h>
#include </usr/include/sys/unistd.h>
#include </usr/include/sys/times.h>
#include </usr/include/sys/timeb.h>
#include </usr/include/X11R4/X11/Xlib.h>
#include </usr/include/X11R4/X11/Xatom.h>
#include </usr/include/X11R4/X11/Xutil.h>

#define maxmaxx 2000

char hello[]=("Appelmann-Window.     (Press 'Q' to make me disappear ! :-)");
char test[10]'
char host[]=("hpboot2:0");

extern GC XCreateGC();
char **ap;
char *display=NULL;
char *geom=NULL;
Window win,root;
XSetWindowAttributes xswa;
XGCValues gcv;
GC gc;
Display *dpy;
Screen *scr;
int WinW,WinH,WinX,WinY;
XColor mycolor;
Colormap cmap;
XEvent myevent;
KeySym mykey;
Status mystat;
int pixx,pixy,pixheight,pixwidth,borderwidth,pixdepth;
int winx,winy,winheight,winwidth,windepth;
long valuemask;

long colorcount=37;
long colors1[256],colors2[256],colors3[256];

XSizeHints myhint;
int myscreen,done,copy;
long myforegr,mybackgr;

long pixelcount;
long pixels[256];

Display *mydisp;
GC mygc;

int test,q,h,i,j,x,color,k,bufid,oldbufid;
int my_tid,line,masterid,maxtid;
float border,dx,dy,dn,z,z1,z2,c1,c2,u1,u2,max,y1,y2,x1,x2,may;
int maxx, maxy;
int colors[maxmaxx+1];
unsigned long mywin,mypix;

int pvm_init(){
        my_tid=pvm_mytid();
        bufid=pvm_recv(-1,1);             /*erste message vom master...*/

        pvm_upkulong(&mywin, 1, 1);
        pvm_upkulong(&mypix, 1, 1);
        pvm_upkfloat(&max, 1, 1);
        pvm_upkfloat(&x1, 1, 1);
        pvm_upkfloat(&dx, 1, 1);
        pvm_upkfloat(&x2, 1, 1);
        pvm_upkfloat(&may, 1, 1);
        pvm_upkfloat(&y1, 1, 1);
        pvm_upkfloat(&dy, 1, 1);
        pvm_upkfloat(&y2, 1, 1);
        pvm_upkint(&k, 1, 1);
        pvm_upkfloat(&border, 1, 1);
        pvm_upkint(&masterid, 1, 1);

        maxx=max;
        maxy=may;
        return(1);
        }

int fensterln(){
        
        mydisp=XOpenDisplay("");
        mybackgr=BlackPixel(mydisp,myscreen);
        myforegr=WhitePixel(mydisp,myscreen);
        cmap=DefaultColormap(mydisp,DefaultScreen(mydisp));

        mycg=XCreateCG(mydisp,mywin,0,0);
        mycg=XCreateCG(mydisp,mypix,0,0);

        XFlush(mydisp);

        }


int colorinit(){

        colors1[0]=0;
        colors2[0]=0;
        colors3[0]=0;

        h=1;
        for(i=1;i<colorcount;i++){
                colors1[h]=i*7;
                colors2[h]=255-i*7;
                colors3[h]=255;
                h++;
                };
        for(i=1;i<colorcount;i++){
                colors1[h]=255;
                colors2[h]=i*7;
                colors3[h]=255-i*7;
                h++;
                };
        for(i=1;i<colorcount;i++){
                colors1[h]=255-i*7;
                colors2[h]=255;
                colors3[h]=i*7;
                h++;
                };
        for(i=1;i<colorcount;i++){
                colors1[h]=i*7;
                colors2[h]=255;
                colors3[h]=255-i*7;
                h++;
                };
        for(i=1;i<colorcount;i++){
                colors1[h]=255;
                colors2[h]=255-i*7;
                colors3[h]=i*7;
                h++;
                };
        for(i=1;i<colorcount;i++){
                colors1[h]=255-i*7;
                colors2[h]=i*7;
                colors3[h]=255;
                h++;
                };

                mycolor.red=colors1[0];
                mycolor.green=colors2[0];
                mycolor.blue=colors3[0];
                XAllocColor(mydisp,cmap,&mycolor);
                pixels[0]=mycolor.pixel;

        for(i=0;i<h+1;i++){
                mycolor.red=colors1[i]*255;
                mycolor.green=colors2[i]*255;
                mycolor.blue=colors3[i]*255;
                XAllocColor(mydisp,cmap,&mycolor);
                pixels[i]=mycolor.pixel;
                };

        pixelcount=h;
        }

int pvm_which_line(){

        bufid=pvm_recv(masterid,2)  /*zweite message vom master...*/
        pvm_upkint(&line,1,1);
        return(1);
        }

int pvm_back(){

        bufid=pvm_initsend(PvmDataRaw);
        pvm_pkint(&mytid,1,1);
        pvm_send(masterid,4);
        return(1);
        }

int compute_line(){

        c1=(float)line*dy+(float)y2;
        for(x=1;x<maxx+1;x++){          /*zn=(zn-1)**2+c*/
                c2=(float)x*dx+(float)x2;
                z1=c1;
                z2=c2;
                for(j=1;j<k;j++){       /*z=z1+i*z2, c=c1_+i*c2 etc. ...*/
                        u1=(z1*z1-z2*z2+c1);
                        u2=(2*z1*z2+c2);
                        z=((u1*u1)+(u2*u2));
                        if(z<border){
                                z1=u1;
                                z2=u2;
                                }
                        else{break;};
                        };
                colors[x-1]=j%pixelcount;
                XSetForeground(mydisp,mygc,pixels[colors[x-1]]);
                XDrawPoint(mydisp,mywin,mygc,line,x-1);
                XDrawPoint(mydisp,mypix,mygc,line,x-1);
        };

        return(1);
        }

main(){

        while(1){
                pvm_init();
                fensterln;
                colorinit();
                pvm_which_line();
                while(line<9999){
                        compute_line();
                        pvm_back();
                        pvm_which_line();
                        };
                XFlush(mydisp);
                };
      /*return(1);*/
        }

