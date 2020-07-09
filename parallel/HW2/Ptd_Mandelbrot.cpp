#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <pthread.h>
#include <chrono>
#include <fstream>

int *output;
using namespace std;
using namespace chrono;

typedef struct complextype {
    float real, imag;
} Compl;
typedef struct _thread_data {
    int id;
    int X;
    int Y;
    int length;
} thread_data;

bool quasi_stable(int x, int y, int X_RESN, int Y_RESN) {
    Compl z, c;
    double lengthsq, temp;
    z.real = z.imag = 0.0;
    c.real = ((float) y - Y_RESN / 2) / (Y_RESN / 4);                //scale factors for 800 x 800 window
    c.imag = ((float) x - X_RESN / 2) / (X_RESN / 4);
    int k = 0;

    do {                                             // iterate for pixel color
        temp = z.real * z.real - z.imag * z.imag + c.real;
        z.imag = 2.0 * z.real * z.imag + c.imag;
        z.real = temp;
        lengthsq = z.real * z.real + z.imag * z.imag;
        k++;
    } while (lengthsq < 12 && k < 100); //lengthsq and k are the threshold
    return k >= 100;
}


void *threa_run(void *args) {
    thread_data *input = (thread_data *) args;
    int thread_id = input->id;
    int X_RESN = input->X;
    int Y_RESN = input->Y;
    int length = input->length;
//    printf("%d", thread_id);
//    printf("%d %d %d %d\n\n\n\n", thread_id, X_RESN, Y_RESN, length);
//    auto start = system_clock::now();
    for (int i = length * thread_id; i < length * (thread_id + 1); i++) {
        for (int j = 0; j < Y_RESN; j++) {
//                printf("%d \t", j);
            output[i * X_RESN + j] = quasi_stable(i, j, X_RESN, Y_RESN);
        }
    }
//    auto end = system_clock::now();
//    auto duration = duration_cast<microseconds>(end - start);
//    cout << "Time for id"<<thread_id<<"is: "
//         << double(duration.count()) * microseconds::period::num / microseconds::period::den
//         << "s" << endl;
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    int X_RESN = 200, Y_RESN = 200, m = 4;

    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-x") {
            X_RESN = atoi(argv[i + 1]);
        }
        if (string(argv[i]) == "-y") {
            Y_RESN = atoi(argv[i + 1]);
        }
        if (string(argv[i]) == "-m") {
            m = atoi(argv[i + 1]);
        }
    }

    int thread_size = m;
    output = new int[X_RESN * Y_RESN];
    pthread_t thread[thread_size];
    thread_data input[thread_size];
    int length = X_RESN / m;
    auto start = system_clock::now();
    for (int i = 0; i < thread_size; i++) {
        input[i].id = i;
        input[i].length = length;
        input[i].X = X_RESN;
        input[i].Y = Y_RESN;
        int rc = pthread_create(&thread[i], NULL, threa_run, &input[i]);
        if (rc) {
            printf("Error; return code is %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < thread_size; i++) {
        pthread_join(thread[i], nullptr);
    }
    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Total time: " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s"
         << endl;


    Window win;
    char *window_name = "test", *display_name = NULL;                     // initialization for a window
    Display *display;
    GC gc;   //this is a graphic content, it could be a pixel color
    unsigned long valuemask = 0;
    XGCValues values; //value of the graphics content
    XSizeHints size_hints;
    Pixmap bitmap;
    XSetWindowAttributes attr[1];
    int width, height,                  // window size
            x, y,                           // window position
            border_width,                   // border width in pixels
            display_width, display_height,  // size of screen
            screen;                         // which screen

    if ((display = XOpenDisplay(display_name)) == NULL) {
        fprintf(stderr, "drawon: cannot connect to X server %s\n",
                XDisplayName(display_name));
        exit(-1);
    }

    // get screen size
    screen = DefaultScreen(display);
    display_width = DisplayWidth(display, screen);
    display_height = DisplayHeight(display, screen);

    // set window size

    width = X_RESN;
    height = Y_RESN;

    // set window position

    x = 200;
    y = 200;

    // create opaque window

    border_width = 4;
    win = XCreateSimpleWindow(display, RootWindow(display, screen),
                              x, y, width, height, border_width,
                              WhitePixel(display, screen), WhitePixel(display,
                                                                      screen)); //Change to WhitePixel (display, screen) if you want a white background

    size_hints.flags = USPosition | USSize;
    size_hints.x = x;
    size_hints.y = y;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.min_width = 300;
    size_hints.min_height = 300;

    XSetNormalHints(display, win, &size_hints);
    XStoreName(display, win, window_name);

    // create graphics context
    gc = XCreateGC(display, win, valuemask, &values);
    XSetBackground(display, gc, BlackPixel(display, screen));
    XSetForeground(display, gc, WhitePixel(display, screen));
    XSetLineAttributes(display, gc, 1, LineSolid, CapRound, JoinRound);

    attr[0].backing_store = Always;
    attr[0].backing_planes = 1;
    attr[0].backing_pixel = BlackPixel(display, screen);

    XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);

    XMapWindow(display, win);
    XSync(display, 0);

    XFlush(display);

    XColor color;
    color.red = 10000; //range from 0~65535
    color.green = 0;
    color.blue = 10000;

    Status rc1 = XAllocColor(display, DefaultColormap(display, screen), &color);
    //set the color and attribute of the graphics content
    XSetForeground(display, gc, color.pixel);
    XSetBackground(display, gc, BlackPixel(display, screen));
    XSetLineAttributes(display, gc, 1, LineSolid, CapRound, JoinRound);
    for (int i = 0; i < X_RESN; i++) {
        for (int j = 0; j < Y_RESN; j++) {
            if (output[i * X_RESN + j] == 1) {
                XDrawPoint(display, win, gc, j, i);
                usleep(1);
                //XDrawPoint cannot draw too fast, otherwise the image cannot be drawn
                //normally you could try to not use the usleep(1), it only black images are shown, try to use this
            }
        }
    }
    //usleep(2500000);
    XFlush(display);

    ofstream fs;
    fs.open("/Users/renly/Downloads/p_performance.csv", ios::app);
    if (!fs) {
        cout << "file create error";
        return 0;
    }
    fs << "pth," << X_RESN << ',' << Y_RESN << ',' << thread_size << ','
       << double(duration.count()) * microseconds::period::num / microseconds::period::den << endl;
    fs.close();
    sleep(5);


    delete[] output;
    cout << "finish running" << endl;
    return 0;
}
