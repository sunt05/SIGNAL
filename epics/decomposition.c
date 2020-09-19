// softIoc -d virtural_bpm.db
#include <stdio.h>
#include <math.h>
#include <cadef.h>

#include "signal.h"

int main(){

    // test data
    double pi = 2.0*acos(0.0) ;
    double f1 = 1.0*0.123456789, a1=1.0E+0, b1 = 1.0E-1 ;
    double f2 = 2.0*0.123456789, a2=1.0E-1, b2 = 5.0E-2 ;
    double f3 = 3.0*0.123456789, a3=1.0E-3, b3 = 0.0E+0 ;
    int length = 2048 ;
    double data[length] ;
    for(int i = 1; i <= length; i++){
        data[i-1]  = a1*cos(2.0*pi*f1*i) + b1*sin(2.0*pi*f1*i) +
            a2*cos(2.0*pi*f2*i) + b2*sin(2.0*pi*f2*i) +
            a3*cos(2.0*pi*f3*i) + b3*sin(2.0*pi*f3*i) ;
    }

    // set channel
    double pend = 1.0 ;
    char *label = "virtual_bpm:x_data" ;
    chid bpm ;

    // write data
    SEVCHK(ca_context_create(ca_disable_preemptive_callback), "error: ca_context_create") ;
    SEVCHK(ca_create_channel(label, NULL, NULL, 99, &bpm), "error: ca_create_channel") ;
    SEVCHK(ca_pend_io(pend), "error: ca_pend_io") ;
    SEVCHK(ca_array_put(DBR_DOUBLE, (unsigned long) length, bpm, (void *) & data), "error: ca_array_put") ;
    SEVCHK(ca_pend_io(pend), "error: ca_pend_io") ;

    // read data
    double read[length] ;
    SEVCHK(ca_array_get(DBR_DOUBLE, (unsigned long) length, bpm, (void *) &read), "ca_array_get") ;
    SEVCHK(ca_pend_io(pend), "error: ca_pend_io") ;

    // format
    double signal[2*length] ;
    convert_real_(&length, read, signal) ;

    // set window
    int order = 2 ;
    double window[length] ;
    window_(&length, &order, window) ;
    double total = 0.0 ;
    for(int i = 0; i < length; i++){
        total = total + window[i] ;
    }

    int flag ;
    int peak ;
    double frequency  ;

    // frequency (bin)
    flag = 0 ;
    peak = 0 ;
    frequency = frequency_(&flag, &peak, &length, &total, window, signal) ;
    printf("frequency_\n") ;
    printf("%.15f\n", frequency) ;
    printf("\n") ;

    // frequency (peak)
    flag = 0 ;
    peak = 1 ;
    frequency = frequency_(&flag, &peak, &length, &total, window, signal) ;
    printf("frequency_\n") ;
    printf("%.15f\n", frequency) ;
    printf("\n") ;

    // decomposition (subtract)
    int loop = 3 ; 
    double fre_amp[loop], cos_amp[loop], sin_amp[loop] ;
    peak = 0 ;
    decomposition_(&flag, &peak, &length, &total, window, signal, &loop, fre_amp, cos_amp, sin_amp) ;
    printf("decomposition_\n") ;
    for(int i = 0; i < loop; i++){
        printf("%.15f %.15f %.15f\n", fre_amp[i], cos_amp[i], sin_amp[i]) ;
        fre_amp[i] = 0.0 ;
        cos_amp[i] = 0.0 ;
        sin_amp[i] = 0.0 ;
    }
    printf("\n") ;

    // frequency list (subtract)
    peak = 0 ;
    frequency_list_(&flag, &peak, &length, &total, window, signal, &loop, fre_amp) ;
    printf("frequency_list_\n") ;
    for(int i = 0; i < loop; i++){
        printf("%.15f\n", fre_amp[i]) ;
    }
    printf("\n") ;

    // amplitude list (subtract)
    amplitude_list_(&flag, &length, &total, window, signal, &loop, fre_amp, cos_amp, sin_amp) ;
    printf("amplitude_list_\n") ;
    for(int i = 0; i < loop; i++){
        printf("%.15f %.15f %.15f\n", fre_amp[i], cos_amp[i], sin_amp[i]) ;
    }
    printf("\n") ;

    // fit_
    printf("fit_ \n") ;
    for (int i = 1; i <= loop ; i++)
    {
        peak = i ;
        frequency = frequency_(&flag, &peak, &length, &total, window, signal) ;
        fre_amp[i-1] = frequency ;
    }
    double mean ;
    double error ;
    fit_(&length, data, &loop, fre_amp, &mean, cos_amp, sin_amp, &error) ;
    printf("%.15f\n", error) ;
    printf("%.15f\n", mean) ;
    for (int i = 0; i < loop ; i++)
    {
        printf("%.15f %.15f %.15f\n", fre_amp[i], cos_amp[i], sin_amp[i]) ;
    }

    return 0 ;
}