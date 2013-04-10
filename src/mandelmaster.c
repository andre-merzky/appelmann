
#include <pvm3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include </usr/include/X11/Xlib.h>
#include </usr/include/X11/Xatom.h>
#include </usr/include/X11/Xutil.h>


/* (x1,y1) rechte obere Ecke
   (x2,y2) linke untere Ecke */

#define maxx 300        /* 1024  */
#define maxy 300        /* 1280  *//* Bitte durch 10 teilbar!!!!!      */
#define k 217           /* 217*i *//* Anzahl der maximalen Iterationen */
#define border 7
#define x1 0.475        /*0.44          1.7             0.64            0.475*/

#define x2 0.425        /*0.415         -1.7            0.69            0.425*/

#define y1 0.35         /*0.315         1.2             -0.35           0.35*/
#define y2 0.3          /*0.309         -2.2            -0.40           0.3*/

#define host1 "localhost"
#define host2 "localhost"
#define host3 "localhost"
#define host4 "localhost"
#define host5 "localhost"
#define host6 "localhost"
#define host7 "localhost"
#define host8 "localhost"
#define host9 "localhost"
#define masterhist "hpboot2"
#define slave "/home/merzky/projects/appelmann/src/mandelslave"
#define maxtids 16



char hello[]=("Appelmann-Window.     (Press 'Q' to make me disappear ! :-)");
char text[10];

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

unsigned long mywin,mypix;
Display *mydisp;
GC mygc;

int info,h,i,j,m,l,maxtid,maxtid2,maxtid3,atid,bufid,line,aline,msgtype;
float z1,z2,c1,c2,x,y,ii,jj,dx,dy,bborder,xx1,xx2,xx,yy1,yy2,yy;
int tids[maxtids+10];
int lines[maxy+1111];
int maintid,maxtidd,kk,host;
FILE *stream;

int tid;

int fensterln(){
        
        mydisp=XOpenDisplay("");
        mybackgr=BlackPixel(mydisp,myscreen);
        myforegr=WhitePixel(mydisp,myscreen);
        myhint.x=0;
        myhint.y=0;
        myhint.width=maxy;
        myhint.height=maxx;
        myhint.flags=PPosition|PSize;

        mywin=XCreateSimpleWindow(mydisp,DefaultRootWindow(mydisp),myhint.x,myhint.y,myhint.width,myhint.height,5,myforegr,mybackgr);
        mypix=XCreatePixmap(mydisp,mywin,myhint.width,myhint.height,8);
        XSetStandardProperties(mydisp,mywin,hello,hello,None,0,0,&myhint);
        
        mygc=XCreateGC(mydisp,mywin,0,0);
        mygc=XCreateGC(mydisp,mypix,0,0);
        XMapRaised(mydisp,mywin);
        XMoveWindow(mydisp,mywin,0,0);

        XSetBackground(mydisp,mygc,mybackgr);
        XSetForeground(mydisp,mygc,myforegr);

        cmap=DefaultColormap(mydisp,DefaultScreen(mydisp));

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

int produce(){

    pvm_setopt(PvmDebugMask,0xFFFFF);

    fprintf (stderr, "PvmBadParam : %d\n", PvmBadParam);
    fprintf (stderr, "PvmNoHost   : %d\n", PvmNoHost  );
    fprintf (stderr, "PvmNoFile   : %d\n", PvmNoFile  );
    fprintf (stderr, "PvmNoMem    : %d\n", PvmNoMem   );
    fprintf (stderr, "PvmSysErr   : %d\n", PvmSysErr  );
    fprintf (stderr, "PvmOutOfRes : %d\n", PvmOutOfRes);

    maintid=pvm_mytid();
    fprintf (stderr, "maintid: %d\n", maintid);
    maxtid2=maxtids/8;
    maxtid2=maxtid2*8;
    /*pvm_spawn("pvmgs", (char**)0, PvmMppFront,(char *)0, 1, &gstid);
    pvm_joingroup("producer_grp");
    */


    for(i=0;i<maxtid2;i+=8){

        pvm_spawn(slave,(char**)0,0,host2,1,&tids[i]);
        pvm_spawn(slave,(char**)0,0,host3,1,&tids[i+1]);
        pvm_spawn(slave,(char**)0,0,host4,1,&tids[i+2]);
        pvm_spawn(slave,(char**)0,0,host5,1,&tids[i+3]);
        pvm_spawn(slave,(char**)0,0,host6,1,&tids[i+4]);
        pvm_spawn(slave,(char**)0,0,host7,1,&tids[i+5]);
        pvm_spawn(slave,(char**)0,0,host8,1,&tids[i+6]);
        pvm_spawn(slave,(char**)0,PvmTaskDebug,host9,1,&tids[i+7]);
        fprintf (stderr, "last spawn (%s): %d\n", host9, tids[i+7]);
        };

    return(1);
    }

int initall(){

        maxtidd=maxtid2;
        xx=maxx;
        xx1=x1;
        xx2=x2;
        yy=maxy;
        yy1=y1;
        yy2=y2;
        kk=k;
        bborder=border;
        dx=(float)(xx1-xx2)/xx;
        dy=(float)(yy1-yy2)/yy;

        for(i=0;i<maxy;i++){
                lines[i+1]=i;
                };
        for(i=maxy;i<maxy+maxtid2;i++){
                lines[i+1]=9999;
                };

        bufid=pvm_initsend(PvmDataRaw);

        pvm_pkulong(&mywin, 1, 1);
        pvm_pkulong(&mypix, 1, 1);
        pvm_pkfloat(&xx, 1, 1);
        pvm_pkfloat(&xx1, 1, 1);
        pvm_pkfloat(&dx, 1, 1);
        pvm_pkfloat(&xx2, 1, 1);
        pvm_pkfloat(&yy, 1, 1);
        pvm_pkfloat(&yy1, 1, 1);
        pvm_pkfloat(&dy, 1, 1);
        pvm_pkfloat(&yy2, 1, 1);
        pvm_pkint(&kk, 1, 1);
        pvm_pkfloat(&bborder, 1, 1);
        pvm_pkint(&maintid, 1, 1);

        for(i=0;i<maxtid2;i++){
                pvm_send(tids[i], 1);
                };


        return (1);
        };

main(argc,argv)
    int argc;
    char **argv;
{
    maxtid=atoi(argv[1]);
    if(!maxtid){maxtid=3;};
    produce();

    fensterln();
    initall();
    colorinit();

    for(i=0;i<maxtid2;i++){
                bufid=pvm_initsend(PvmDataRaw);
                pvm_pkint(&lines[i],1,1);
                pvm_send(tids[i],2);
        };

    while(i<maxy+maxtid2){

                bufid=pvm_recv(-1,4);

                pvm_upkint(&atid,1,1);
                bufid=pvm_initsend(PvmDataRaw);
                pvm_pkint(&lines[i],1,1);
                pvm_send(atid,2);

                i++;
                };
        XCopyArea(mydisp,mypix,mywin,mygc,0,0,myhint.width,myhint.height,0,0);

        XSelectInput(mydisp,mywin,KeyPressMask|ExposureMask);
        while(done==0){
                XNextEvent(mydisp,&myevent);
                switch(myevent.type)
                        {
                        case Expose:
                                if(myevent.xexpose.count==0)
                                        {
                                        XCopyArea(mydisp,mypix,mywin,mygc,0,0,myhint.width,myhint.height,0,0);
                                        };
                                        break;
                        case KeyPress:
                                i=XLookupString(&myevent,text,10,&mykey,0);
                                if(i==1 &&text[0]=='Q')done=1;
                                break;
                        };
                };
        for(i=0;i<maxtid2;i++){
                pvm_kill(tids[i]);
                };

        pvm_exit();

        XFreeGC(mydisp,mygc);
        XDestroyWindow(mydisp,mywin);
        XCloseDisplay(mydisp);

    return(1);
    }

