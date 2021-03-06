#include "dehanttideinel.hpp"

/**
 * @details This function gives the out-of-phase corrections induced by
 *          mantle anelasticity in the diurnal band. 
 *          This function is a translation/wrapper for the fortran ST1IDIU
 *          subroutine, found here : 
 *          http://maia.usno.navy.mil/conv2010/software.html
 * 
 * @param[in]  xsta    Geocentric position of the IGS station (Note 1)
 * @param[in]  xsun    Geocentric position of the Sun (Note 2)
 * @param[in]  xmon    Geocentric position of the Moon (Note 2)
 * @param[in]  fac2sun Degree 2 TGP factor for the Sun (Note 3)      
 * @param[in]  fac2mon Degree 2 TGP factor for the Moon (Note 3) 
 * @param[out] xcorsta Out of phase station corrections for diurnal band
 * 
 * @note
 *   -# The IGS station is in ITRF co-rotating frame. All coordinates are
 *      expressed in meters, as arrays, i.e. [x,y,z].
 *   -# The position is in Earth Centered Earth Fixed (ECEF) frame.  All
 *      coordinates are expressed in meters, as arrays, i.e. [x,y,z].
 *   -# The expressions are computed in the main program. TGP is the tide
 *      generated potential. The units are inverse meters.
 *   -# Status: Class 1
 *   -# This fucnction is part of the package dehanttideinel, see
 *      ftp://maia.usno.navy.mil/conv2010/convupdt/chapter7/dehanttideinel/
 *
 * @warning The vector norms of the geocentric position of the Sun and Moon 
 *          (see rmon2, rsun2) are pretty close to overflow. Maybe that should 
 *          be dealt with (e.g. use a scaling factor?). 
 * 
 * @verbatim
 *  Test case:
 *     given input: XSTA(1) = 4075578.385D0 meters
 *                  XSTA(2) =  931852.890D0 meters
 *                  XSTA(3) = 4801570.154D0 meters   
 *                  XSUN(1) = 137859926952.015D0 meters
 *                  XSUN(2) = 54228127881.4350D0 meters
 *                  XSUN(3) = 23509422341.6960D0 meters
 *                  XMON(1) = -179996231.920342D0 meters
 *                  XMON(2) = -312468450.131567D0 meters
 *                  XMON(3) = -169288918.592160D0 meters
 *                  FAC2SUN =  0.163271964478954D0 1/meters     
 *                  FAC2MON =  0.321989090026845D0 1/meters    
 *                  
 *expected output:  XCORSTA(1) = -0.2836337012840008001D-03 meters
 *                  XCORSTA(2) =  0.1125342324347507444D-03 meters
 *                  XCORSTA(3) = -0.2471186224343683169D-03 meters
 * @endverbatim
 *
 * @version 2009 July     31
 * 
 * @cite iers2010,
 *       Mathews, P. M., Dehant, V., and Gipson, J. M., 1997, "Tidal station
 *       displacements," J. Geophys. Res., 102(B9), pp. 20,469-20,477
 * 
 */
void iers2010::dtel::st1idiu (const double* xsta,const double* xsun,
        const double* xmon,const double& fac2sun,const double& fac2mon,
        double* xcorsta)
{

  const double dhi ( -0.0025e0 ), dli ( -0.0007e0 );

  // Compute the normalized position vector of the IGS station.
  double rsta    ( ::sqrt ( std::inner_product (xsta,xsta+3,xsta,.0e0) ) );

  double sinphi  ( xsta[2] / rsta );
  double cosphi  ( ::sqrt (xsta[0]*xsta[0] + xsta[1]*xsta[1]) / rsta );
  double cos2phi ( cosphi * cosphi - sinphi * sinphi );
  double sinla   ( xsta[1] / cosphi / rsta );
  double cosla   ( xsta[0] / cosphi / rsta );

  // Compute the normalized position vector of the Moon.
  double rmon2   ( std::inner_product (xmon,xmon+3,xmon,.0e0) );

  // Compute the normalized position vector of the Sun.
  double rsun2   ( std::inner_product (xsun,xsun+3,xsun,.0e0) );

  double drsun   ( -3e0*dhi*sinphi*cosphi*fac2sun*xsun[2]*(xsun[0]*
              sinla-xsun[1]*cosla)/rsun2 );

  double drmon   ( -3e0*dhi*sinphi*cosphi*fac2mon*xmon[2]*(xmon[0]*
              sinla-xmon[1]*cosla)/rmon2 );

  double dnsun   ( -3e0*dli*cos2phi*fac2sun*xsun[2]*(xsun[0]*sinla-
              xsun[1]*cosla)/rsun2 );

  double dnmon   ( -3e0*dli*cos2phi*fac2mon*xmon[2]*(xmon[0]*sinla-
              xmon[1]*cosla)/rmon2 );

  double desun   ( -3e0*dli*sinphi*fac2sun*xsun[2]*(xsun[0]*cosla+
              xsun[1]*sinla)/rsun2 );

  double demon   ( -3e0*dli*sinphi*fac2mon*xmon[2]*(xmon[0]*cosla+
              xmon[1]*sinla)/rmon2 );

  double dr      ( drsun + drmon );
  double dn      ( dnsun + dnmon );
  double de      ( desun + demon );

  // Compute the corrections for the station.
  xcorsta[0] = dr*cosla*cosphi-de*sinla-dn*sinphi*cosla;
  xcorsta[1] = dr*sinla*cosphi+de*cosla-dn*sinphi*sinla;
  xcorsta[2] = dr*sinphi+dn*cosphi;

  // Finished
  return;
}

/*
#include <stdio.h>
int main ()
{
  double xsta[] = {4075578.385e0,931852.890e0,4801570.154e0};
  double xsun[] = {137859926952.015e0,54228127881.4350e0,23509422341.6960e0};
  double xmon[] = {-179996231.920342e0,-312468450.131567e0,-169288918.592160e0};
  double fac2sun= 0.163271964478954e0;
  double fac2mon= 0.321989090026845e0;
  double xcorsta[3] = {.0e0,.0e0,.0e0};

  st1idiu ( xsta,xsun,xmon,fac2sun,fac2mon,xcorsta );

  printf ("\nSubroutine ST1IDIU");
  printf ("\nDifferences in meters:");
  printf ("\n\t|dx| = %15.12f",-0.2836337012840008001e-03 -xcorsta[0]);
  printf ("\n\t|dy| = %15.12f",0.1125342324347507444e-03 -xcorsta[1]);
  printf ("\n\t|dz| = %15.12f",-0.2471186224343683169e-03 -xcorsta[2]);
  printf ("\n");
  
  return 0;
}
*/
