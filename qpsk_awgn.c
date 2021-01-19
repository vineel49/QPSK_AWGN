/* QPSK over AWGN */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>
#define PI 3.141592653589793238
void main()
{
clock_t start, end;
double cpu_time_used;
start = clock();
/*------------------------------------------------------*/    
    /* simulation parameters */
float SNR_dB=9.5; /*SNR per bit in dB */    
double noise_var_1D;/* 1D noise variance */ 
long int sim_runs=pow(10,3); /*simulation runs*/
int frame_size=1024; /*frame size */
noise_var_1D = (double)0.5*2/(2*pow(10,0.1*SNR_dB));
/*------------------------------------------------------*/
long int C_Ber=0; /*total number of channel errors*/
long int i; /* i is for for loop (simulation runs) */
for(i=1;i<=sim_runs;i++)
{
srand(time(0));
/*source*/
int a[2*frame_size],i1;
for(i1=0;i1<2*frame_size;i1++)
a[i1] = rand()%2;

/*QPSK mapping*/
int re_sym[frame_size],im_sym[frame_size],i2;
for(i2=0;i2<frame_size;i2++)
{
    re_sym[i2] = 1-2*a[2*i2];
    im_sym[i2] = 1-2*a[2*i2+1];
}

/* awgn (Box-Muller transform)*/
double re_awgn[frame_size],im_awgn[frame_size];
double U1,U2;
int i3;
for(i3=0;i3<frame_size;i3++)
{
do {
U1 =  rand() / (double)RAND_MAX ;
U2 =  rand() / (double)RAND_MAX ;
}while( U1 == 0 || U2 == 0 );
re_awgn[i3] = sqrt(noise_var_1D)*sqrt(-2*log(U1))*cos(2*PI*U2);
im_awgn[i3] = sqrt(noise_var_1D)*sqrt(-2*log(U1))*sin(2*PI*U2);
}

/* channel output */
double re_chan_op[frame_size],im_chan_op[frame_size];
int i4;
for(i4=0;i4<frame_size;i4++)
{
re_chan_op[i4] = re_sym[i4]+re_awgn[i4];
im_chan_op[i4] = im_sym[i4]+im_awgn[i4];
}

/*        RECEIVER            */
int a_hat[2*frame_size],i5;
int err1,err2,errors=0;
for(i5=0;i5<frame_size;i5++)
{
a_hat[2*i5] = re_chan_op[i5]>0?0:1;
a_hat[2*i5+1] = im_chan_op[i5]>0?0:1;
err1 = a_hat[2*i5]==a[2*i5]?0:1;
err2 = a_hat[2*i5+1]==a[2*i5+1]?0:1;
errors = errors+err1+err2;
}
C_Ber = C_Ber + errors;
}/*for forward recursion */
/*bit error rate */
double BER;
BER = (double)C_Ber/(2*frame_size*sim_runs);
system("cls"); /*clears the screen */
printf("\n bit error rate is %lf",BER);
end = clock();
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("\n time is %lf sec",cpu_time_used);
}
