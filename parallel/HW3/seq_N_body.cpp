#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <fstream>

using namespace std;

/* define the global constants, G is the Gravitational constant, e is the collision distance (radius) */
const double G = 1e-9;//6.67 * 10E-11;
const double e = 1e-1;
const double period = 1;
const double dt = 0.001;


double MAX_MASS = 1e9;
double **X;
double **Y;
double energy_loss = 0.999;
double boundary_e_loss = 0.7;

int MAX_radius = 40;
int MIN_radius = 3;
int width = 800, height = 800;


double right_boundary = 10;
double Ratio = right_boundary / width;
double MAX_real_radius = MAX_radius * Ratio;
double MIN_real_radius = MIN_radius * Ratio;


/* X11 setup */
Display *x11setup(Window *win, GC *gc, int Width, int Height) {
    Display *display;
    int win_x, win_y, /* window position */
            border_width, /* border width in pixels */
            display_width, display_height, /* size of screen */
            screen; /* which screen */

    char window_name[] = "N-Body Simulation", *display_name = nullptr;
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

    border_width = 4; /* create opaque window */
    *win = XCreateSimpleWindow(display, RootWindow (display, screen),
                               win_x, win_y, Width, Height, border_width,
                               WhitePixel (display, screen), BlackPixel (display, screen));

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

    XSetBackground(display, *gc, BlackPixel (display, screen));
    XSetForeground(display, *gc, WhitePixel (display, screen));
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

double vertical_bouncing(double vx, double x, double r, double boundary, double e_loss) {
    if (x <= 2 * r || x >= boundary - 2 * r) return -vx * e_loss;
    else return vx;
}

double horizontal_bouncing(double vy, double y, double r, double boundary, double e_loss) {
    if (y <= 2 * r || y >= boundary - 2 * r) return -vy * e_loss;
    else return vy;
}


int main(int argc, char *argv[]) {
    double a_fac = 100;

    /* initialize position, speed, mass, and acceleration */
    int N = 180;
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-N") {
            N = atoi(argv[i + 1]);
        }
    }

    double m[N], aX[N], aY[N], vX[N], vY[N], r[N];
    int total_time_slots = int(period / dt);
    X = new double *[total_time_slots];
    for (int i = 0; i < total_time_slots; ++i) {
        X[i] = new double[N];
    }
    Y = new double *[total_time_slots];
    for (int i = 0; i < total_time_slots; ++i) {
        Y[i] = new double[N];
    }

    /* Generate uniform distribution */
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> u_dis(0, right_boundary);
    normal_distribution<> dis(MAX_MASS / 10, MAX_MASS / 50.0);
//    cout <<"MAX_real_radius: " <<MAX_real_radius << endl<<"MIN_real_radius: "<<MIN_real_radius<<endl;
    for (int i = 0; i < N; i++) {
        X[0][i] = u_dis(gen);
        Y[0][i] = u_dis(gen);
        m[i] = min(abs(dis(gen)), MAX_MASS);
//        m[i] = MAX_MASS / 5.0;
//        cout << ((exp(m[i]) - 1) / (exp(MAX_MASS) - 1)) << ' ' << (MAX_real_radius - MIN_real_radius) << endl;
        r[i] = MIN_real_radius +
               (MAX_real_radius - MIN_real_radius) * ((pow(m[i], 3)) / (pow(MAX_MASS, 3)));
        r[i] = r[i] * 0.9;
        aX[i] = 0;
        aY[i] = 0;
        vX[i] = 0;
        vY[i] = 0;
    }
/*
//    central huge particle
    X[0][1] = right_boundary / 2;
    Y[0][1] = right_boundary / 2;
    m[1] = pow(MAX_MASS, 4);
    r[1] = MAX_real_radius;
*/

/*// simple setting
    MAX_MASS = 3.2;
    N = 3;
    for (int i = 0; i < N; ++i) {
        X[0][0] = 4;
        X[0][1] = 5;
        X[0][2] = 6;
        Y[0][i] = right_boundary / 2;
        Y[0][1] = 6;
        m[i] = 1;
        r[i] = MIN_real_radius + (MAX_real_radius - MIN_real_radius) * (exp(m[i] - 1) / (exp(MAX_MASS) - 1));
    }*/

    /*//standard 4 body movement
    MAX_MASS = 3.2;
    N = 4;
    X[0][0] = 1;
    Y[0][0] = 1;
    X[0][1] = 1;
    Y[0][1] = -1;
    X[0][2] = -1;
    Y[0][2] = -1;
    X[0][3] = -1;
    Y[0][3] = 1;
    for (int l = 0; l < N; ++l) {
        X[0][l] += right_boundary / 2;
        Y[0][l] += right_boundary / 2;
        m[l] = 1;
        r[l] = MIN_real_radius + (MAX_real_radius - MIN_real_radius) * (exp(m[l] - 1) / (exp(MAX_MASS) - 1));
    }
    vX[0] = vX[3] = -pow(sqrt(2) * G / 4, 1 / 2.0);
    vX[1] = vX[2] = pow(sqrt(2) * G / 4, 1 / 2.0);
    vY[0] = vY[1] = pow(sqrt(2) * G / 4, 1 / 2.0);
    vY[2] = vY[3] = -pow(sqrt(2) * G / 4, 1 / 2.0);*/

    // Show initialization
//    for (int i = 0; i < N; ++i) {
//        cout << X[0][i] << ' ' << Y[0][i] << endl;
//    }
//    for (int i = 0; i < N; i++) {
//        cout << m[i] << ' ' << r[i] << ' ' << int(r[i] / Ratio) << endl;
//    }



    struct timeval start;
    struct timeval end;
    gettimeofday(&start, nullptr);
    for (int k = 0; k < total_time_slots - 1; ++k) {
        for (int i = 0; i < N; ++i) {
            aX[i] = 0;
            aY[i] = 0;
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                double square_dist = pow(X[k][j] - X[k][i], 2) + pow(Y[k][j] - Y[k][i], 2);
                if (square_dist <= pow(r[i] + r[j], 2) && j > i) {
//                    cout << i << " and " << j << endl;
//                    cout << "original speed: " << vX[i] << ' ' << vX[j] << endl;
                    double temp_v = vX[i];
                    vX[i] = ((m[i] - m[j]) * vX[i] + 2 * m[j] * vX[j]) / (m[i] + m[j]) * energy_loss;
                    vX[j] = ((m[j] - m[i]) * vX[j] + 2 * m[i] * temp_v) / (m[i] + m[j]) * energy_loss;
                    temp_v = vY[i];
                    vY[i] = ((m[i] - m[j]) * vY[i] + 2 * m[j] * vY[j]) / (m[i] + m[j]) * energy_loss;
                    vY[j] = ((m[j] - m[i]) * vY[j] + 2 * m[i] * temp_v) / (m[i] + m[j]) * energy_loss;
//                    cout << "After collision" << vX[i] << ' ' << vX[j] << endl;
                }
//                cout << G * m[j] * (X[j] - X[i]) << '/' << pow(square_dist, 3.0 / 2) << '|' << square_dist << ' ';
                aX[i] += G * m[j] * (X[k][j] - X[k][i]) * a_fac / pow(square_dist + e * e, 3.0 / 2);
                aY[i] += G * m[j] * (Y[k][j] - Y[k][i]) * a_fac / pow(square_dist + e * e, 3.0 / 2);
            }
//            cout << 'a' << aX[i] << ' ' << aY[i] << endl;
        }
//        cout << "position" << endl;
//        cout << "delta X: " << vX[0] * dt + aX[0] * dt * dt / 2 << endl;
        for (int i = 0; i < N; ++i) {
            /* Bouncing at boundary */
            vX[i] = vertical_bouncing(vX[i], X[k][i], r[i], right_boundary, boundary_e_loss);
            vY[i] = horizontal_bouncing(vY[i], Y[k][i], r[i], right_boundary, boundary_e_loss);
            /* Collision for two particle */

            X[k + 1][i] = X[k][i] + vX[i] * dt + aX[i] * dt * dt / 2;
            Y[k + 1][i] = Y[k][i] + vY[i] * dt + aY[i] * dt * dt / 2;
            vX[i] += aX[i] * dt;
            vY[i] += aY[i] * dt;
//            cout << '(' << X[k][i] << ',' << Y[k][i] << ") ";
        }
//        for (int i = 0; i < N; ++i) cout << vX[i] << ',' << vY[i] << ' ';
//        cout << endl;
//        X[k + 1][1] = right_boundary / 2;
//        Y[k + 1][1] = right_boundary / 2;
    }
    gettimeofday(&end, nullptr);
    double time_use = double((end.tv_sec - start.tv_sec)) + (end.tv_usec - start.tv_usec) * 1e-6;
    printf("Name: Xinhai Hou\nStudent ID: 116010073\nAssignment 3, N-Body Simulation, sequential implementation.\nRunning time is %.6fs\n", time_use);
//    for (int i = 0; i < total_time_slots; ++i) {
//        for (int j = 0; j < N; ++j) {
//            cout << '(' << X[i][j] << ',' << Y[i][j] << ") ";
//        }
//        cout << endl;
//    }


/*
    ofstream fs;
    fs.open(*/
/*"/code/116010073/HW3/p.csv"*//*
"/Users/renly/Downloads/p_performance.csv", ios::app);
    if (!fs) {
        cout << "file create error";
        return 0;
    }
    fs << "seq," << N << ',' << period << ',' << dt << ',' << "1," << time_use << endl;
    fs.close();
*/



    /* initialization for a window */

    Window win;
    Display *display;
    GC gc;   //this is a graphic content, it could be a pixel color
    display = x11setup(&win, &gc, width, height);
    XClearWindow(display, win);


//    double radius_ratio = MAX_radius / exp(MAX_MASS);
//    cout << "ratio: " << Ratio << ' ' << r[0] << endl;
    for (int k = 0; k < total_time_slots; k++) {
        XClearWindow(display, win);
        for (int i = 0; i < N; i++) {
            int radius = int(r[i] / Ratio);
//            if (k == 0)cout << radius << ' ';
            XFillArc(display, win, gc, int(X[k][i] / Ratio) - radius, int(Y[k][i] / Ratio) - radius, radius * 2,
                     radius * 2, 0, 360 * 64);
            usleep(1);
        }
        usleep(10000);
        XFlush(display);
    }
    sleep(5);


    for (int i = 0; i < total_time_slots; ++i) {
        delete[] X[i];
        delete[] Y[i];
    }
    delete[] X;
    delete[] Y;
    return 0;
}
