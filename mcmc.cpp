/*

    Simple MCMC:
    Utilizes a Metropolis-Hastings Algorithm with a 
    standard Gibbs sampler.

*/

#include <cmath>        /* exp */
#include <cstdio>       /* printf */
#include <cstdlib>      /* rand */
#include <random>       /* uniform_distribution, normal_distribution */
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

double get_random_uniform(void) {

    time_t seed = clock();
    default_random_engine generator (seed);
    //mt19937 generator (seed);
    uniform_real_distribution<double> distribution(0.0,1.0);
    double rnum = distribution(generator);
    return rnum;
}

double get_random_normal(double m, double s) {

    time_t seed = clock();
    default_random_engine generator (seed);
    //mt19937 generator (seed);
    normal_distribution<double> distribution(m,s);
    double rnum = distribution(generator);
    return rnum;
}

void lnfnc(int x[], double *y0, double a, double b, int n) {
/*
    Basic linear equation function
*/
    for (int i = 0; i < n; i++){
        y0[i] = (a * (double)x[i]) + b;
    } 
    return;
}

double chisqr(double yo[], int ye[], int syo[], int n) {
/*
    Basic chi squared function
    Note: data set error is variance not stddev
*/
    double csq = 0.0;

    for (int i = 0; i < n; i++) {
        csq += pow((double)(yo[i] - ye[i]), 2.0) / (double)syo[i]; 
    }
    return csq;
}

void mcmc(double a0, double sa, double b0, double sb, int n, 
          int x[], int y[], int dy[]) {
/*
    Basic Metropolis-Hastings Algorithm with a 
    standard Gibbs sampler.
*/
    void lnfnc(int x[], double *y0, double a, double b, int n);
    double chisqr(double yo[], int ye[], int syo[], int n);
    double get_random_normal(double m, double s);
    double get_random_uniform(void);

    double y0[16], yt[16] = {0.0};
    double chi0, chit;
    double randn;
    double at, bt, acal, aexp;
    double atrace[500000], btrace[500000] = {0.0};
    float arate, aave, bave = 0.0;
    int i, j, k = 0;
    int mcn = 500000;
    int brn = 1000;
    
    lnfnc(x, y0, a0, b0, n);
    chi0 = chisqr(y0, y, dy, n);

    do {
        at = get_random_normal(a0, sa);
        bt = get_random_normal(b0, sb);
        
        lnfnc(x, yt, at, bt, n);
        chit = chisqr(yt, y, dy, n);

        acal = exp((chi0 - chit) / 2.0);
        aexp = fmin(1.0, acal);
        randn = get_random_uniform();

        if (randn <= aexp) {
            a0 = at;
            b0 = bt;
            for (k=0; k < n; k++) y0[k] = yt[k];
            chi0 = chit;
            atrace[i] = at;
            btrace[i] = bt;
            i += 1;
        }
        j += 1;
    } while (i < mcn);

    arate = (float)i / (float)j;
    printf ("\ni = %d, j = %d", i, j);
    printf("\nAcceptance ratio: %6.4f", arate);
    
    for (k = brn; k < mcn; k++) {
        aave += atrace[k];
        bave += btrace[k];
    }
    aave = aave / (float)(mcn-brn);
    bave = bave / (float)(mcn-brn);

    printf("\na: %f, b: %f", aave, bave);

    return;
}

int main(int argc, char *argv[]) {

    int x[] = {203,  58, 210, 202,
               198, 158, 165, 201,
               157, 131, 166, 160,
               186, 125, 218, 146};

    int y[] = {495, 173, 479, 504,
               510, 416, 393, 442,
               317, 311, 400, 337,
               423, 334, 533, 344};

    int dy[] = {21, 15, 27, 14,
                30, 16, 14, 25,
                52, 16, 34, 31,
                42, 26, 16, 22};

    double  cputot;
    time_t  cpubeg, cpuend;

    double a0 = 2.5;
    double sa = 0.025;
    double b0 = 28.82;
    double sb = 2.5;
    int n = 16;

    cpubeg = clock();
    mcmc(a0, sa, b0, sb, n, x, y, dy);
    cpuend = clock();
    cputot = cpuend - cpubeg;
    printf("\nCPU time: %f", cputot/CLOCKS_PER_SEC);

    return 0;
}
