#include <iostream>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <pthread.h>
#include <fstream>

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
    XColor bg_color;
    bg_color.blue = 65530;
    bg_color.green = 25530;
    bg_color.red = 15530;
    XAllocColor(display, DefaultColormap(display, screen), &bg_color);
    *win = XCreateSimpleWindow(display, RootWindow (display, screen),
                               win_x, win_y, Width, Height, border_width,
                               WhitePixel (display, screen), bg_color.pixel);

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

float ***output;

void initialize_output(int x, int y, int z, float temp) {
    output = new float **[z];
    for (int i = 0; i < z; i++) {
        output[i] = new float *[y];
        for (int j = 0; j < y; j++) {
            output[i][j] = new float[x];
        }
    }

    for (int j = 0; j < y; j++) {
        for (int i = 0; i < x; ++i) {
            output[0][j][i] = temp;
        }
    }

    for (int k = 1; k < z; k++) {
        for (int j = 0; j < y; j++) {
            output[k][0][j] = temp;
            output[k][y - 1][j] = temp;
            output[k][j][0] = temp;
            output[k][j][x - 1] = temp;
        }
    }
}

void print_output(int x, int y, int z, int start = 0) {
    for (int k = start; k < z; k++) {
        for (int j = 0; j < y; j++) {
            for (int i = 0; i < x; i++) {
                printf("%3.0f ", output[k][j][i]);
            }
            cout << endl;
        }
        cout << endl;
    }
}

bool flag = true;
float error = 1e-3;

struct thread_data {
    int current_time;
    int thread_id;
    int start;
    int end;
    int length;
};

void *thread_run(void *args) {
    auto *input = (thread_data *) args;
    int k = input->current_time;
    int id = input->thread_id;
    int start = input->start;
    int end = input->end;
    int length = input->length;
//    printf("id: %d, start %d, end %d k: %d\n", id, start, end, k);
    for (int j = start; j < end; j++) {
        for (int i = 1; i < length - 1; i++) {
            output[k + 1][j][i] =
                    float(0.25) *
                    (output[k][j - 1][i] + output[k][j + 1][i] + output[k][j][i - 1] + output[k][j][i + 1]);
            if (abs(output[k + 1][j][i] - output[k][j][i]) > error) flag = false;
        }
    }
}

int main(int argc, char *argv[]) {
    int length = 200;
    int thread_size = 4;
    int draw = 1;
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-l") {
            length = atoi(argv[i + 1]);
        }
        if (string(argv[i]) == "-n") {
            thread_size = atoi(argv[i + 1]);
        }
        if (string(argv[i]) == "-w") {
            draw = 0;
        }
    }
    int X_RESN = length, Y_RESN = length;

    thread_data input[thread_size];
    int receive_count[thread_size];
    int receive_placement[thread_size];
    receive_placement[0] = 1;
    for (int i = 0; i < thread_size; ++i) {
        receive_count[i] = int(i < (length - 2) % thread_size) + (length - 2) / thread_size;
        if (i < thread_size - 1)receive_placement[i + 1] = receive_placement[i] + receive_count[i];
        input[i].thread_id = i;
        input[i].start = receive_placement[i];
        input[i].end = receive_placement[i] + receive_count[i];
        input[i].length = length;
    }
    int MAX_ITERATION = length * 55;
    int converged_iter = MAX_ITERATION;

    initialize_output(X_RESN, Y_RESN, MAX_ITERATION, 0);

    int fire_width = int(0.4 * X_RESN);
    int fire_start = X_RESN / 2 - fire_width / 2;
    int fire_end = X_RESN / 2 + fire_width / 2;
    for (int k = 0; k < MAX_ITERATION; ++k) {
        for (int i = fire_start; i < fire_end; ++i) {
            output[k][0][i] = 100;
//            output[k][length - 1][i] = 100;
//            output[k][i][length - 1] = 100;
//            output[k][i][0] = 100;
        }
    }
    pthread_t thread[thread_size];
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, nullptr);
    for (int k = 0; k < converged_iter - 1; k++) {
        flag = true;
        for (int l = 0; l < thread_size; ++l) {
            input[l].current_time = k;
            pthread_create(&thread[l], nullptr, thread_run, &input[l]);
        }

        for (int i = 0; i < thread_size; i++)pthread_join(thread[i], nullptr);
        if (k > converged_iter)cout << k << "error";
        if (flag) {
            converged_iter = k;
            break;
        }
    }

    gettimeofday(&end, nullptr);
    double time_use = double((end.tv_sec - start.tv_sec)) + (end.tv_usec - start.tv_usec) * 1e-6;
    printf("Name: Xinhai Hou\nStudent ID: 116010073\nAssignment 4, Heat Distribution Simulation, Pthread implementation.\n"
           "Running time is %.6fs\n", time_use);
    cout << "converged at iteration: " << converged_iter << endl;

//    print_output(X_RESN, Y_RESN,converged_iter , converged_iter-2);

    if (draw) {
        Window win;
        Display *display;
        GC gc;   //this is a graphic content, it could be a pixel color
        display = x11setup(&win, &gc, X_RESN, Y_RESN);
        int screen = DefaultScreen (display);
        XColor color[200];
        for (int iter = 0; iter < 200; iter++) {
            if (iter < 100) {
                color[iter].green = iter * 655;
                color[iter].red = 65530;
                color[iter].blue = max(1000 * iter - 34500, 0);
            } else {
                color[iter].green = 65530 - (iter - 99) * 400;
                color[iter].red = 65530 - (iter - 99) * 500;
                color[iter].blue = 65530;
            }
            XAllocColor(display, DefaultColormap(display, screen), &color[iter]);
        }

        for (int iter = 0; iter < converged_iter; iter += 10) {
            if (iter >= converged_iter)break;
            for (int i = 0; i < X_RESN; i++) {
                for (int j = 0; j < Y_RESN; ++j) {
                    //set the color and attribute of the graphics content
                    int temp = max(int((output[iter][j][i] - 0) * 2) - 1, 0);
                    XSetForeground(display, gc, color[199 - temp].pixel);
                    XDrawPoint(display, win, gc, i, j);
                }
            }
            XFlush(display);
            usleep(1e4);


        }
        cout << endl << "Drawing complete" << endl;
        sleep(6);
    } else {
        ofstream fs;
        fs.open(/*"/code/116010073/HW4/p.csv"*/"/Users/renly/Downloads/p_performance.csv", ios::app);
        if (!fs) {
            cout << "file create error";
            return 0;
        }
        fs << "pth," << length << ',' << converged_iter << ',' << thread_size << ',' << time_use << endl;
        fs.close();
    }

    for (int j = 0; j < MAX_ITERATION; j++) {
        for (int i = 0; i < Y_RESN; i++) {
            delete[]output[j][i];
        }
        delete[]output[j];
    }
    delete[]output;

}