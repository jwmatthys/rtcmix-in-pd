#include "pv.h"
#include <ugens.h>

/*
 * make balanced pair of analysis (A) and synthesis (S) windows;
 * window lengths are Nw, FFT length is N, synthesis interpolation
 * factor is I, and osc is true (1) if oscillator bank resynthesis 
 * is specified
 */
void
makewindows( float H[], float A[], float S[], int Nw, int N, int I, int osc )
{
 int i ;
 float sum ;
/*
 * basic Hamming windows
 */
    for ( i = 0 ; i < Nw ; i++ )
	H[i] = A[i] = S[i] = 0.54 - 0.46*cos( TWOPI*i/(Nw - 1) ) ;
/*
 * when Nw > N, also apply interpolating (sinc) windows to
 * ensure that window are 0 at increments of N (the FFT length)
 * away from the center of the analysis window and of I away
 * from the center of the synthesis window
 */
    if ( Nw > N ) {
     float x ;
/*
 * take care to create symmetrical windows
 */
	x = -(Nw - 1)/2. ;
	for ( i = 0 ; i < Nw ; i++, x += 1. )
	    if ( x != 0. ) {
		A[i] *= N*sin( PI*x/N )/(PI*x) ;
		if ( I )
		    S[i] *= I*sin( PI*x/I )/(PI*x) ;
	    }
    }
/*
 * normalize windows for unity gain across unmodified
 * analysis-synthesis procedure
 */
    for ( sum = i = 0 ; i < Nw ; i++ )
	sum += A[i] ;

    for ( i = 0 ; i < Nw ; i++ ) {
     float afac = 2./sum ;
     float sfac = Nw > N ? 1./afac : afac ;
	A[i] *= afac ;
	S[i] *= sfac ;
    }

    if ( Nw <= N && I ) {
	for ( sum = i = 0 ; i < Nw ; i += I )
	    sum += S[i]*S[i] ;
	for ( sum = 1./sum, i = 0 ; i < Nw ; i++ )
	    S[i] *= sum ;
    }
}
