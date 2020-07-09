#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <pthread.h>
#include <fstream>

using namespace std;

/* define the global constants, G is the Gravitational constant, e is the collision distance (radius) */
const double G = 1e-9;//6.67 * 10E-11;
const double e = 1e-1;
const double period = 10;
const double dt = 0.001;
int total_time_slots = int(period / dt);


struct Particle {
//    double *X, *Y;//position components
    double vX, vY;//velocity components
    double aX, aY;//force components
    double mass;//mass of the particle
    double r; // radius

    int thread_id;
};

struct thread_data {
    int current_time;
    int N;
    int thread_id;
};

Particle *particles;
double MAX_MASS = 1e9;
double **X;
double **Y;
double energy_loss = 0.999;
double boundary_e_loss = 0.7;

int MAX_radius = 20;
int MIN_radius = 3;
int width = 800, height = 800;
double a_fac = 60;

double boundary = 10;
double Ratio = boundary / width;
double MAX_real_radius = MAX_radius * Ratio;
double MIN_real_radius = MIN_radius * Ratio;


double horizontal_bouncing(double vy, double y, double radius, double Boundary, double e_loss) {
    if (y <= 2 * radius || y >= Boundary - 2 * radius) return -vy * e_loss;
    else return vy;
}

double vertical_bouncing(double vx, double x, double radius, double Boundary, double e_loss) {
    if (x <= 2 * radius || x >= Boundary - 2 * radius) return -vx * e_loss;
    else return vx;
}

void *thread_run(void *args) {
    auto *input = (thread_data *) args;
//    printf("ID: %d, N: %d, k: %d\n", input->thread_id, input->N, input->current_time);
    int id = input->thread_id;
    int N = input->N;
    int k = input->current_time;
    for (int i = 0; i < N; ++i) {
        if (particles[i].thread_id != id)
            continue; //else printf("thread: %d will do particle %d at time %d\n", id, i, k);
        particles[i].aX = 0;
        particles[i].aY = 0;
        for (int j = 0; j < N; ++j) {
            if (i == j) continue;
            double square_dist = pow(X[k][j] - X[k][i], 2) + pow(Y[k][j] - Y[k][i], 2);
           /* if (square_dist <= pow(particles[i].r + particles[j].r, 2) && j > i) {
//                    cout << i << " and " << j << endl;
//                    cout << "original speed: " << particles[i].vX << ' ' << particles[j].vX << endl;
                double temp_v = particles[i].vX;
                particles[i].vX = ((particles[i].mass - particles[j].mass) * particles[i].vX +
                                   2 * particles[j].mass * particles[j].vX) /
                                  (particles[i].mass + particles[j].mass) * energy_loss;
                particles[j].vX = ((particles[j].mass - particles[i].mass) * particles[j].vX +
                                   2 * particles[i].mass * temp_v) /
                                  (particles[i].mass + particles[j].mass) * energy_loss;
                temp_v = particles[i].vY;
                particles[i].vY = ((particles[i].mass - particles[j].mass) * particles[i].vY +
                                   2 * particles[j].mass * particles[j].vY) /
                                  (particles[i].mass + particles[j].mass) * energy_loss;
                particles[j].vY = ((particles[j].mass - particles[i].mass) * particles[j].vY +
                                   2 * particles[i].mass * temp_v) /
                                  (particles[i].mass + particles[j].mass) * energy_loss;
//                    cout << "After collision" << particles[i].vX << ' ' << particles[j].vX << endl;

            }*/
//                cout << G * particles[j].mass * (X[j] - X[i]) << '/' << pow(square_dist, 3.0 / 2) << '|' << square_dist << ' ';
            particles[i].aX += G * particles[j].mass * (X[k][j] - X[k][i]) * a_fac / pow(square_dist + e * e, 3.0 / 2);
            particles[i].aY += G * particles[j].mass * (Y[k][j] - Y[k][i]) * a_fac / pow(square_dist + e * e, 3.0 / 2);
        }
//            cout << 'a' << particles[i].aX << ' ' << particles[i].aY << endl;
    }
//    cout << "position" << endl;
//        cout << "delta X: " << particles[i].vX * dt + aX[0] * dt * dt / 2 << endl;
    for (int i = 0; i < N; ++i) {
        if (particles[i].thread_id != id)continue;
        /* Bouncing at boundary */
        particles[i].vX = vertical_bouncing(particles[i].vX, X[k][i], particles[i].r, boundary, boundary_e_loss);
        particles[i].vY = horizontal_bouncing(particles[i].vY, Y[k][i], particles[i].r, boundary, boundary_e_loss);
        /* Collision for two particle */
        X[k + 1][i] = X[k][i] + particles[i].vX * dt + particles[i].aX * dt * dt / 2;
        Y[k + 1][i] = Y[k][i] + particles[i].vY * dt + particles[i].aY * dt * dt / 2;
        particles[i].vX += particles[i].aX * dt;
        particles[i].vY += particles[i].aY * dt;
//        if (id == 0) printf("(%.5f,%.5f) at k:%d\n", X[k][i], Y[k][i], k);
    }
}

/* X11 setup */
Display *x11setup(Window *win, GC *gc, int Width, int Height) {
    Display *display;
    int win_x, win_y, /* window position */
            border_width, /* border width in pixels */
//            display_width, display_height, /* size of screen */
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

int main(int argc, char *argv[]) {
    int thread_size = 4;
    int N = 200;


    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-n") {
            thread_size = atoi(argv[i + 1]);
        }
        if (string(argv[i]) == "-N") {
            N = atoi(argv[i + 1]);
        }
    }

    /* initialize position, speed, mass, and acceleration */

    particles = new Particle[N];
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
    uniform_real_distribution<> u_dis(0, boundary);
    normal_distribution<> dis(MAX_MASS / 10, MAX_MASS / 6.0);
//    cout <<"MAX_real_radius: " <<MAX_real_radius << endl<<"MIN_real_radius: "<<MIN_real_radius<<endl;
    for (int i = 0; i < N; i++) {
        X[0][i] = u_dis(gen);
        Y[0][i] = u_dis(gen);
        particles[i].mass = min(abs(dis(gen)), MAX_MASS) * 0.5;
        particles[i].r = MIN_real_radius +
                         (MAX_real_radius - MIN_real_radius) *
                         ((pow(particles[i].mass, 3) - 1) / (pow(MAX_MASS, 3) - 1));
        particles[i].r = particles[i].r * 0.95;
        particles[i].aX = 0;
        particles[i].aY = 0;
        particles[i].vX = 0;
        particles[i].vY = 0;
    }

//    central huge particle
    X[0][0] = boundary / 2;
    Y[0][0] = boundary / 2;
    particles[0].mass = pow(MAX_MASS, 1.);
    particles[0].r = MAX_real_radius * 0.8;
//    // Orbital speed
//    int fac = 10;
//    for (int i = 1; i < N; ++i) {
//        double dist = sqrt(pow(X[0][i] - boundary / 2.0, 2) + pow(Y[0][i] - boundary / 2.0, 2));
//        if (X[0][i] >= boundary / 2.0) {
//            particles[i].vY = fac * sqrt(G * particles[0].mass / dist) / dist * X[0][i] * boundary_e_loss;
//        } else {
//            particles[i].vY = fac * -sqrt(G * particles[0].mass / dist) / dist * X[0][i] * boundary_e_loss;
//        }
//        if (Y[0][i] >= boundary / 2.0) {
//            particles[i].vX = fac * -sqrt(G * particles[0].mass / dist) / dist * Y[0][i] * boundary_e_loss;
//        } else {
//            particles[i].vX = fac * sqrt(G * particles[0].mass / dist) / dist * Y[0][i] * boundary_e_loss;
//        }
//        cout << '(' << X[0][i] << "," << Y[0][i] << ") " << particles[i].vX << ' ' << particles[i].vY << endl;
//    }

/*
// simple setting
    MAX_MASS = 3.2;
    N = 5;
    for (int i = 0; i < N; ++i) {
        X[0][0] = 4;
        X[0][1] = 5;
        X[0][2] = 6;
        Y[0][i] = boundary / 2;
        Y[0][1] = 6;
        particles[i].mass = 1;
        particles[i].r = MIN_real_radius +
                         (MAX_real_radius - MIN_real_radius) * ((exp(particles[i].mass) - 1) / (exp(MAX_MASS) - 1));
    }
*/

/*
//standard 4 body movement, set N as 4
    MAX_MASS = 3.2;
    X[0][0] = 1;
    Y[0][0] = 1;
    X[0][1] = 1;
    Y[0][1] = -1;
    X[0][2] = -1;
    Y[0][2] = -1;
    X[0][3] = -1;
    Y[0][3] = 1;
    for (int l = 0; l < N; ++l) {
        X[0][l] += boundary / 2;
        Y[0][l] += boundary / 2;
        particles[l].mass = 1;
        particles[l].r = MIN_real_radius +
                         (MAX_real_radius - MIN_real_radius) * (exp(particles[l].mass - 1) / (exp(MAX_MASS) - 1));
    }
    particles[0].vX = particles[3].vX = -pow(sqrt(2) * G / 4, 1 / 2.0);
    particles[1].vX = particles[2].vX = pow(sqrt(2) * G / 4, 1 / 2.0);
    particles[0].vY = particles[1].vY = pow(sqrt(2) * G / 4, 1 / 2.0);
    particles[2].vY = particles[3].vY = -pow(sqrt(2) * G / 4, 1 / 2.0);*/

    // Show initialization
//    for (int i = 0; i < N; ++i) {
//        cout << X[0][i] << ' ' << Y[0][i] << endl;
//    }
//    for (int i = 0; i < N; i++) {
//        cout << particles[i].mass << ' ' << particles[i].r << ' ' << int(particles[i].r / Ratio) << endl;
//    }


    for (int m = 0; m < N; ++m) {
        particles[m].thread_id = m % thread_size;
    }

    thread_data threadData[thread_size];
    for (int i = 0; i < thread_size; ++i) {
        threadData[i].thread_id = i;
        threadData[i].N = N;
    }

    pthread_t thread[thread_size];
//    cout << "Begin timing" << endl;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, nullptr);
    for (int k = 0; k < total_time_slots - 1; ++k) {
        for (int l = 0; l < thread_size; l++) {
            threadData[l].current_time = k;
            int rc = pthread_create(&thread[l], nullptr, thread_run, &threadData[l]);
            if (rc) {
                printf("Error; return code is %ds\n", rc);
                return EXIT_FAILURE;
            }
        }
        for (int i = 0; i < thread_size; i++)pthread_join(thread[i], nullptr);
    }
    gettimeofday(&end, nullptr);
    double time_use = double((end.tv_sec - start.tv_sec)) + (end.tv_usec - start.tv_usec) * 1e-6;
    printf("Name: Xinhai Hou\nStudent ID: 116010073\nAssignment 3, N-Body Simulation, pthread implementation.\nRunning time is %.6fs\n",
           time_use);
    sleep(4);
//    ofstream fs;
//    fs.open("/code/116010073/HW3/p.csv"/*"/Users/renly/Downloads/p_performance.csv"*/, ios::app);
//    if (!fs) {
//        cout << "file create error";
//        return 0;
//    }
//    fs << "pth," << N << ',' << period << ',' << dt << ',' << thread_size << ',' << time_use << endl;
//    fs.close();

//    for (int i = 0; i < total_time_slots; ++i) {
//        for (int j = 0; j < N; ++j) {
//            cout << '(' << X[i][j] << ',' << Y[i][j] << ") ";
//        }
//        cout << endl;
//    }

//     initialization for a window
    Window win;
    Display *display;
    GC gc;   //this is a graphic content, it could be a pixel color
    display = x11setup(&win, &gc, width, height);
    XClearWindow(display, win);

    for (int k = 0; k < total_time_slots; k++) {
        XClearWindow(display, win);
        for (int i = 0; i < N; i++) {
            int radius = int(particles[i].r / Ratio);
            XFillArc(display, win, gc, int(X[k][i] / Ratio) - radius, int(Y[k][i] / Ratio) - radius, radius * 2,
                     radius * 2, 0, 360 * 64);
            usleep(1);
        }
        usleep(10000);
        XFlush(display);
    }

    for (int k = 0; k < total_time_slots; k++) {
        XClearWindow(display, win);
        for (int i = 0; i < N; i++) {
            int radius = int(particles[i].r / Ratio);
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