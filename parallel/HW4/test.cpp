#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

using namespace std;

Display *x11setup(Window *win, GC *gc, int Width, int Height) {
    Display *display;
    int win_x, win_y, /* window position */
            border_width, /* border width in pixels */
            display_width, display_height, /* size of screen */
            screen; /* which screen */

    char window_name[] = "Heat Distribution Simulation", *display_name = nullptr;
    unsigned long valuemask = 0;
    XGCValues values;
    XSizeHints size_hints;
    XSetWindowAttributes attr[1];

    if ((display = XOpenDisplay(display_name)) == nullptr) { /* connect to Xserver */
        fprintf(stderr, "Cannot connect to X server %s\n", XDisplayName(display_name));
        exit(-1);
    }

    screen = DefaultScreen (display); /* get screen size */
//    display_width = DisplayWidth (display, screen);
//    display_height = DisplayHeight (display, screen);

    win_x = 70;
    win_y = 0; /* set window position */
    border_width = 0; /* create opaque window */
//    XColor bg_color;
//    bg_color.blue = 65535;
//    bg_color.green = 65535;
//    bg_color.red = 65535;
//    XAllocColor(display, DefaultColormap(display, screen), &bg_color);
    *win = XCreateSimpleWindow(display, RootWindow (display, screen),
                               win_x, win_y, Width, Height, border_width,
                               WhitePixel (display, screen), WhitePixel (display, screen));

    size_hints.flags = USPosition | USSize;
    size_hints.x = win_x;
    size_hints.y = win_y;
    size_hints.width = Width;
    size_hints.height = Height;
    size_hints.min_width = 300;
    size_hints.min_height = 300;

    XSetNormalHints(display, *win, &size_hints);
    XStoreName(display, *win, window_name);

    *gc = XCreateGC(display, *win, valuemask, &values); /* create graphics context */

    XSetLineAttributes(display, *gc, 1, LineSolid, CapRound, JoinRound);

    attr[0].backing_store = Always;
    attr[0].backing_planes = 1;
    attr[0].backing_pixel = BlackPixel(display, screen);

    XChangeWindowAttributes(display, *win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);
    XSelectInput(display, *win, KeyPressMask);
    XMapWindow(display, *win);
    XSync(display, 0);
    return display;
}

int main(int argc, char *argv[]) {
    int length = 200;

    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-y") {
            length = atoi(argv[i + 1]);
        }
    }
    int X_RESN = length, Y_RESN = length;

    Window win;
    Display *display;
    GC gc;   //this is a graphic content, it could be a pixel color
    display = x11setup(&win, &gc, X_RESN, Y_RESN);
    int screen = DefaultScreen (display);
    XColor color[200];
    for (int iter = 0; iter < 200; iter++) {

//        color[iter].red = min(102000 / (iter - 96.0) + 40000, 65535.0); //range from 0~65535
//        color[iter].green = -abs(500 * (iter - 100)) + 50000;
//        color[iter].blue = max(min(65530, 1000 * iter - 34500), 0);

        if (iter < 100) {
            color[iter].green = iter * 655;
            color[iter].red = 65530;
            color[iter].blue = max(1000 * iter - 34500, 0);
        } else {
            color[iter].green = 65530 - 450 * (iter - 99);
            color[iter].red = 65530 - (iter - 99) * 500;
            color[iter].blue = 65530;
        }
        XAllocColor(display, DefaultColormap(display, screen), &color[iter]);
        cout << iter << ' ' << color[iter].red << ' ' << color[iter].green << ' ' << color[iter].blue << endl;
    }

    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 200; ++j) {
            //set the color and attribute of the graphics content
            XSetForeground(display, gc, color[i].pixel);
            XDrawPoint(display, win, gc, i, j);

        }
    }
    XFlush(display);
    sleep(100);
}