#include "mpi.h"
#include <iostream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <ctime>
#include <fstream>
#include "omp.h"

using namespace std;
/* define the global constants, G is the Gravitational constant, e is the collision distance (radius) */
const double G = 1e-9;//6.67 * 10E-11;
const double e = 1e-1;
const double period = 0.5;
const double dt = 0.001;
int total_time_slots = int(period / dt);


double *m, *aX, *aY, *vX, *vY, *r;
double MAX_MASS = 1e9;
double **X;
double **Y;
double energy_loss = 1;
double boundary_e_loss = 0.7;

int MAX_radius = 40;
int MIN_radius = 3;
int width = 800, height = 800;

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
    int N = 200;
    double a_fac = 150;
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-N") {
            N = atoi(argv[i + 1]);
        }
    }

    m = new double[N];
    aX = new double[N];
    aY = new double[N];
    vX = new double[N];
    vY = new double[N];
    r = new double[N];
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
    normal_distribution<> dis(MAX_MASS / 10, MAX_MASS / 50.0);
//    cout <<"MAX_real_radius: " <<MAX_real_radius << endl<<"MIN_real_radius: "<<MIN_real_radius<<endl;
    for (int i = 0; i < N; i++) {
        X[0][i] = u_dis(gen);
        Y[0][i] = u_dis(gen);
        m[i] = min(abs(dis(gen)), MAX_MASS);
        r[i] = MIN_real_radius + (MAX_real_radius - MIN_real_radius) * ((pow(m[i], 3)) / (pow(MAX_MASS, 3)));
        r[i] = r[i] * 0.9;
        aX[i] = 0;
        aY[i] = 0;
        vX[i] = 0;
        vY[i] = 0;
    }

    /*
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
        m[l] = 1;
        r[l] = MIN_real_radius + (MAX_real_radius - MIN_real_radius) * (exp(m[l] - 1) / (exp(MAX_MASS) - 1));
    }
    vX[0] = vX[3] = -pow(sqrt(2) * G / 4, 1 / 2.0);
    vX[1] = vX[2] = pow(sqrt(2) * G / 4, 1 / 2.0);
    vY[0] = vY[1] = pow(sqrt(2) * G / 4, 1 / 2.0);
    vY[2] = vY[3] = -pow(sqrt(2) * G / 4, 1 / 2.0);
    */

    /* start MPI */
    MPI_Init(&argc, &argv);

    int rank, size; /*  rank ID, number of ranks */
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


//    int num_particles = N / size;
    int receive_count[size];
    int receive_placement[size];
    receive_placement[0] = 0;
    for (int i = 0; i < size; ++i) {
        receive_count[i] = int(i < N % size) + N / size;
        if (i < size - 1)receive_placement[i + 1] = receive_placement[i] + receive_count[i];
    }

    int num_particles = receive_count[rank];

//    if (rank == 0) {
//        cout << endl;
//        for (int i = 0; i < 1; ++i) {
//            for (int j = 0; j < N; ++j) {
//                cout << '(' << X[i][j] << ',' << Y[i][j] << ") ";
//                cout << &Y[1][j] << ' ';
//            }
//            cout << endl;
//        }
//
//    }

    MPI_Bcast(X[0], N, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
    MPI_Bcast(Y[0], N, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
    MPI_Bcast(m, N, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
    MPI_Bcast(r, N, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);

    struct timeval start;
    struct timeval end;
    gettimeofday(&start, nullptr);
    double sub_vX[num_particles], sub_vY[num_particles], sub_X[num_particles], sub_Y[num_particles];
    for (int k = 0; k < total_time_slots - 1; ++k) {
        int j;
        double square_dist;
        double temp_v;
#pragma omp parallel for default(shared)   private(j, square_dist, temp_v)
        for (int i = receive_placement[rank]; i < num_particles + receive_placement[rank]; ++i) {
            aX[i] = 0;
            aY[i] = 0;
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                double square_dist = pow(X[k][j] - X[k][i], 2) + pow(Y[k][j] - Y[k][i], 2);
                if (square_dist <= pow(r[i] + r[j], 2)) {
//                    cout << i << " and " << j << endl;
//                    if (rank == 1)cout << rank << " original speed: " << vX[i] << ' ' << vX[j] << endl;
                    double temp_v = vX[i];
                    vX[i] = ((m[i] - m[j]) * vX[i] + 2 * m[j] * vX[j]) / (m[i] + m[j]) * energy_loss;
                    vX[j] = ((m[j] - m[i]) * vX[j] + 2 * m[i] * temp_v) / (m[i] + m[j]) * energy_loss;
                    temp_v = vY[i];
                    vY[i] = ((m[i] - m[j]) * vY[i] + 2 * m[j] * vY[j]) / (m[i] + m[j]) * energy_loss;
                    vY[j] = ((m[j] - m[i]) * vY[j] + 2 * m[i] * temp_v) / (m[i] + m[j]) * energy_loss;
//                    if (rank == 1)cout << "After collision" << vX[i] << ' ' << vX[j] << endl;
                }
//                cout << G * m[j] * (X[j] - X[i]) << '/' << pow(square_dist, 3.0 / 2) << '|' << square_dist << ' ';
                aX[i] += G * m[j] * (X[k][j] - X[k][i]) / pow(square_dist + e * e, 3.0 / 2) * a_fac;
                aY[i] += G * m[j] * (Y[k][j] - Y[k][i]) / pow(square_dist + e * e, 3.0 / 2) * a_fac;
            }
//            cout << 'a' << aX[i] << ' ' << aY[i] << endl;
        }
//        cout << "position" << endl;
//        cout << "delta X: " << vX[0] * dt + aX[0] * dt * dt / 2 << endl;
#pragma omp parallel for default(shared)
        for (int i = receive_placement[rank]; i < receive_placement[rank] + num_particles; ++i) {
            /* Bouncing at boundary */
            vX[i] = vertical_bouncing(vX[i], X[k][i], r[i], boundary, boundary_e_loss);
            vY[i] = horizontal_bouncing(vY[i], Y[k][i], r[i], boundary, boundary_e_loss);
            /* Collision for two particle */

            X[k + 1][i] = X[k][i] + vX[i] * dt + aX[i] * dt * dt / 2;
            Y[k + 1][i] = Y[k][i] + vY[i] * dt + aY[i] * dt * dt / 2;
            vX[i] += aX[i] * dt;
            vY[i] += aY[i] * dt;
//            printf("(%.3f, %.3f), (%.3f, %.3f) for i = %d, in rank %d\n", X[k + 1][i], Y[k + 1][i], X[k][i], Y[k][i], i,
//                   rank);
            sub_vX[i - receive_placement[rank]] = vX[i];
            sub_vY[i - receive_placement[rank]] = vY[i];
            sub_X[i - receive_placement[rank]] = X[k + 1][i];
            sub_Y[i - receive_placement[rank]] = Y[k + 1][i];

        }
//        cout << endl;
//        for (int i = 0; i < N; ++i) cout << vX[i] << ',' << vY[i] << ' ';
//        cout << endl;
//        X[k + 1][1] = right_boundary / 2;
//        Y[k + 1][1] = right_boundary / 2;


        int ac = MPI_Gatherv(&sub_vX, receive_count[rank], MPI_DOUBLE_PRECISION, vX, receive_count, receive_placement,
                             MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
        if (ac) printf("Gathering error for rank: %d\n", rank);
        ac = MPI_Gatherv(&sub_vY, receive_count[rank], MPI_DOUBLE_PRECISION, vY, receive_count, receive_placement,
                         MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
        if (ac) printf("Gathering error for rank: %d\n", rank);
        ac = MPI_Gatherv(&sub_X, receive_count[rank], MPI_DOUBLE_PRECISION, X[k + 1], receive_count, receive_placement,
                         MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
        if (ac) printf("Gathering error for rank: %d\n", rank);
        ac = MPI_Gatherv(&sub_Y, receive_count[rank], MPI_DOUBLE_PRECISION, Y[k + 1], receive_count, receive_placement,
                         MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
        if (ac) printf("Gathering error for rank: %d\n", rank);


        ac = MPI_Bcast(vX, N, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
        if (ac) printf("Broadcasting error for rank %d\n", rank);

        ac = MPI_Bcast(vY, N, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
        if (ac) printf("Broadcasting error for rank %d\n", rank);

        ac = MPI_Bcast(X[k + 1], N, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
        if (ac) printf("Broadcasting error for rank %d\n", rank);

        ac = MPI_Bcast(Y[k + 1], N, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
        if (ac) printf("Broadcasting error for rank %d\n", rank);


//        if (rank == 0) {
//            for (int i = 0; i < N; ++i) {
//                printf("(%.3f, %.3f) ", X[k][i], Y[k][i]);
//            }
//            cout << endl;
//            for (int i = 0; i < N; ++i) {
//                printf("%.3f %.3f ", vX[i], vY[i]);
//            }
//            cout << endl;
//        }
    }


    gettimeofday(&end, nullptr);
    double time_use = double((end.tv_sec - start.tv_sec)) + (end.tv_usec - start.tv_usec) * 1e-6;
//    printf("time_use is %.6f for rank %d\n", time_use, rank);
    double all_time[size];
    int ac = MPI_Gather(&time_use, 1, MPI_DOUBLE_PRECISION, &all_time, 1, MPI_DOUBLE_PRECISION, 0, MPI_COMM_WORLD);
    if (ac)printf("Gathering error for rank: %d\n", rank);


    double runtime = 0;
    for (int i = 0; i < size; ++i) {
        if (runtime < all_time[i]) {
            runtime = all_time[i];
        }
    }
    if (rank == 0)
        printf("Name: Xinhai Hou\nStudent ID: 116010073\nAssignment 3, N-Body Simulation, OpenMP+MPI implementation.\nRunning time is %.6fs\n",
               runtime);


//    if (rank == 0) {
//        ofstream fs;
//        fs.open(/*"/code/116010073/HW3/p.csv"*/"/Users/renly/Downloads/p_performance.csv", ios::app);
//        if (!fs) {
//            cout << "file create error";
//            return 0;
//        }
//        fs << "mpi," << N << ',' << period << ',' << dt << ',' << size << ',' << runtime << endl;
//        fs.close();
//    }
    if (rank == 0) {
//         initialization for a window
        Window win;
        Display *display;
        GC gc;   //this is a graphic content, it could be a pixel color
        display = x11setup(&win, &gc, width, height);
        XClearWindow(display, win);

        for (int k = 0; k < total_time_slots; k++) {
            XClearWindow(display, win);
            for (int i = 0; i < N; i++) {
                int radius = int(r[i] / Ratio);
//                if (k == 0)cout << radius << ' ';
                XFillArc(display, win, gc, int(X[k][i] / Ratio) - radius, int(Y[k][i] / Ratio) - radius, radius * 2,
                         radius * 2, 0, 360 * 64);
                usleep(1);
            }
            usleep(10000);
            XFlush(display);
        }
        sleep(3);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    for (int i = 0; i < total_time_slots; ++i) {
        delete[] X[i];
        delete[] Y[i];
    }
    delete[] X;
    delete[] Y;
    delete[] m;
    delete[] aX;
    delete[] aY;
    delete[] vX;
    delete[]vY;
    delete[]r;
    return 0;
}

