#ifndef _SIGNAL__H__
#define _SIGNAL__H__
void    convert_real_(int*, double*, double*) ;
void    convert_complex_(int*, double*, double*, double*) ;
void    fft_radix_two_(int*, int*, double*) ;
void    fft_radix_eight_(int*, int*, double*) ;
void    fft_external_(int*, int*, double*) ;
void    ffrft_(int*, double*, double*) ;
void    window_(int*, int*, double*) ;
int     peak_(int*, double*, int*) ;
double  frequency_(int*, int*, int*, double*, double*, double*) ;
void    decomposition_(int*, int*, int*, double*, double*, double*, int*, double*, double*, double*) ;
void    frequency_list_(int*, int*, int*, double*, double*, double*, int*, double*) ;
void    amplitude_list_(int*, int*, double*, double*, double*, int*, double*, double*, double*) ;
void    fit_(int*, double*, int*, double*, double*, double*, double*, double*) ;
void    filter_(int*, double*, int*) ;
#endif