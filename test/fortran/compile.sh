gfortran -c FUNDARG.F -o FUNDARG.o
gfortran -c PMSDNUT2.F -o PMSDNUT2.o
gfortran -c UTLIBR.F -o UTLIBR.o
gfortran -c FCNNUT.F -o FCNNUT.o
gfortran -c ARG2.F -o ARG2.o
gfortran -c ORTHO_EOP.F -o ORTHO_EOP.o
gfortran -c CNMTX.F -o CNMTX.o
gfortran -c RG_ZONT2.F -o RG_ZONT2.o
gfortran -c APG.F -o APG.o
gfortran -c GPT.F -o GPT.o
gfortran -c FCUL_ZD_HPA.F -o FCUL_ZD_HPA.o

gfortran test.f -o test.e FCUL_ZD_HPA.o GPT.o APG.o PMSDNUT2.o UTLIBR.o FCNNUT.o ARG2.o ORTHO_EOP.o RG_ZONT2.o CNMTX.o FUNDARG.o

rm *.o
