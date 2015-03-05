#include "hardisp.hpp"

double inline q (const double& u1,const double& x1,const double& u2,
        const double& x2)
{
  return ( u1/pow(x1,2) - u2/pow(x2,2) ) / ( 1.0e0/x1 - 1.0e0/x2 );
}

/**
 * @details The purpose of this function is to find an array s for the spline
 *          interpolator function EVAL.
 * 
 * @param[in]  nn   Number of data points supplied, which may be
 *                  negative (Note 1)
 * @param[in]  x    Array containing x-coordinates where function
 *                  is sampled (Note 2)
 * @param[in]  u    Array containing sample values that are to be
 *                  interpolated
 * @param[in]  a    Working space array of dimension at least nn
 * @param[out] s    Output array of 2nd derivative at sample points
 * @return          Always zero
 * 
 * @note
 *   -# If the user wishes to force the derivatives at the ends of the series
 *      to assume specified values, he or she should put du(1)/dx and du(n)/dx
 *      in the variables s1 and s2 and call the subroutine with nn = -(number
 *      of terms in the series).  Normally a parabola is fitted through the 
 *      1st and last 3 points to find the slopes.  If less than 4 points are
 *      supplied, straight lines are fitted.
 *   -# The sequence xx(1), xx(2), ... xx(nn) must be strictly increasing.
 *   -# Status: Canonical model
 * 
 * @version 2009 August 26
 * 
 */
int iers2010::hisp::spline (const int& nn,const double* x,const double* u,
        double* s,double* a)
{
    //constexpr int nmax = 20;
    //printf ("\n\tinto spline ... ");
    const int n ( abs (nn) );
    
    if (n<=3) {
        // series too short for cubic spline - use straight lines.
        for (int i=0;i<n;i++)
            s[i] = 0.0e0;
        //printf ("... short exit; ok\n");
        return 0;
    }

    //for (int i=0;i<n;i++)
    //    printf ("\nx=%14.6f u=%14.6f",x[i],u[i]);
    
    double q1 ( q(u[1]-u[0],x[1]-x[0],u[2]-u[0],x[2]-x[0]) );
    double qn ( q(u[n-2]-u[n-1],x[n-2]-x[n-1],u[n-3]-u[n-1],x[n-3]-x[n-1]) );
    //printf ("\nq1=%14.6f, qn=%14.6f",q1,qn);
    
    if (nn<=0) {
        q1 = s[0];
        qn = s[1];
    }
    
    s[0] = 6.0e0*((u[1]-u[0])/(x[1]-x[0]) - q1);
    //printf ("\ns[1]=%14.6f",s[0]);
    int n1 ( n - 1 );
    //printf ("\nBounds: n1 = %02i, n = %02i, nn = %02i",n1,n,nn);
    
    for (int i=1;i<n1;i++) {
        s[i] = (u[i-1]/(x[i]-x[i-1]) - u[i]*(1.0e0/(x[i]-x[i-1]) 
                  + 1.0e0/(x[i+1]-x[i])) + u[i+1]/(x[i+1]-x[i]))*6.0e0;
        //printf ("\ns(%03i)=%14.6f",i+1,s[i]);
    }

    s[n-1] = 6.0e0*(qn + (u[n1-1]-u[n-1])/(x[n-1]-x[n1-1]));
    a[0]   = 2.0e0*(x[1]-x[0]);
    a[1]   = 1.5e0*(x[1]-x[0]) + 2.0e0*(x[2]-x[1]);
    s[1]   = s[1] - 0.5e0*s[0];
    //printf ("\ns[n]=%14.6f s[2]=%14.6f",s[n-1],s[1]);
    
    double c;
    for (int i=2;i<n1;i++) {
        c     = (x[i]-x[i-1])/a[i-1];
        a[i]  = 2.0e0*(x[i+1]-x[i-1]) - c*(x[i]-x[i-1]);
        s[i] -= (c*s[i-1]);
    }
      
    c      = (x[n-1]-x[n1-1]) / a[n1-1];
    a[n-1] = (2.0e0-c)*(x[n-1]-x[n1-1]);
    s[n-1] -= (c*s[n1-1]);
    
    
    // Back substitute
    s[n-1] /= a[n-1];
    
    for (int j=1;j<=n1;j++) {
        int i (n - j - 1);
        s[i] =(s[i] - (x[i+1]-x[i])*s[i+1]) / a[i];
        //printf ("\ns(%1i)=%14.6f",i+1,s[i]);
    }
    //printf ("\naccessing s(%2i), x(%2i), x(%2i), s(%2i), a(%2i)",i,i+1,i,i+1,i);
    //printf ("... exiting\n");
    //for (int i=0;i<n;i++) printf ("\ns[%1i]=%14.6f",i+1,s[i]);
    
    // Finished
    return 0;
}
