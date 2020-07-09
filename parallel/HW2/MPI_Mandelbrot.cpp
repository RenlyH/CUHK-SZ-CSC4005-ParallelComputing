#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <ctime>
#include <fstream>

using namespace std;


typedef struct complextype {
    float real, imag;
} Compl;
int *output;
int *sub_output;


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

int main(int argc, char *argv[]) {
    int X_RESN = 200, Y_RESN = 200;

    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-x") {
            X_RESN = atoi(argv[i + 1]);
        }
        if (string(argv[i]) == "-y") {
            Y_RESN = atoi(argv[i + 1]);
        }
    }

    /* start MPI */
    MPI_Init(&argc, &argv);
    /* timing */
    clock_t start, end;
    start = clock();
    int rank,   /* rank ID  */
            size;/* number of ranks */
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int length = X_RESN / size;

    sub_output = new int[length * Y_RESN];
    output = new int[X_RESN * Y_RESN];

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < Y_RESN; j++) {
            sub_output[i * X_RESN + j] = quasi_stable(i + rank * length, j, X_RESN, Y_RESN);
        }

    }

    int receive_counts[size];
    int receive_displacements[size];
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            receive_counts[i] = length * Y_RESN;
            receive_displacements[i] = i * length * Y_RESN;
            cout << i * length * Y_RESN<<' ';
        }
    }
    int ac = MPI_Gatherv(sub_output, length * Y_RESN, MPI_INT, output, receive_counts, receive_displacements,
                         MPI_INT, 0, MPI_COMM_WORLD);
    if (ac) {
        printf("Gathering error for rank: %d\n", rank);
    }
    end = clock();
    delete[] sub_output;
    double endtime = (double) (end - start) / CLOCKS_PER_SEC;
    double all_time[size];
//    printf("Total time: %fs for node: %d\n", endtime, rank);
    ac = MPI_Gather(&endtime, 1, MPI_DOUBLE_PRECISION, &all_time, 1, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
    if (ac) {
        printf("Gathering error for rank: %d\n", rank);
    }

    double runtime = 0;
    for (int i = 0; i < size; ++i) {
        if (runtime < all_time[i]) {
            runtime = all_time[i];
        }
    }
    if (rank == 0) printf("Total time: %f\n", runtime);

    MPI_Finalize();


    if (rank == 0) {

/*        for (int i = 0; i < X_RESN; ++i) {
            for (int j = 0; j < Y_RESN; ++j) {
                cout << output[i * X_RESN + j] << "\t";
            }
            cout << "\n";
        }*/

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

//        ofstream fs;
//        fs.open("/Users/renly/Downloads/p_performance.csv", ios::app);
//        if (!fs) {
//            cout << "file create error";
//            return 0;
//        }
//        fs << "mpi," << X_RESN << ',' << Y_RESN << ',' << size << ',' << runtime << endl;
//        fs.close();
        sleep(100);
    } else {
        sleep(101);
    }

    sleep(10);

    delete[] output;

    return 0;
}

