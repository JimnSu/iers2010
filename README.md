# libiers10++
C++ functions and definitions implementing the IERS 2010 standards.

## Introduction
This project contains a number of functions implementing models defined in
IERS Conventions (2010). The functions are available in FORTRAN from the IERS
[website](http://maia.usno.navy.mil/conv2010/software.html). Note that the
software found at this website is routinely updated.
The FORTRAN subroutines are translated to C++ (standard c++11) with (as much as
possible) minor modifications.

## Compilation / Installation
Source code is ISO C++11. Compilation should be trivial using any gcc version 
supporting the c++11 standard (option -std=c++11).

The installation process (will) be trivial, using the standard FSF GNU tools
(autoconf,automake,etc...). 

This software is meant to be implemented on Unix-type OSs. No effort will be
undertaken for compatibility with otherOS types.

## Prerequisites
None. This is a standalone library. Of course a C++ compiler is assumed!

## Bugs & Maintanance
Xanthos, xanthos@mail.ntua.gr
Mitsos, danast@mail.ntua.gr

{% include figure.html src="doc/figures/flag.png" caption="Support the revolution" %}
