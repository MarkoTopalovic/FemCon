#define CardH1 "C /1/ HEADING CARD (80A1)\n"
#define CardV1 "C NASLOV\n"
#define CardH2 "C /2/ FORMAT FOR INPUT DATA (free format)\n"
#define CardV2 "C INDFOR\n"
#define CardH3 "C /3/ BASIC DATA FOR THE PROBLEM (10I5)\n"
#define CardV3 "C NP,               NPER,NPRINT,NSTAC,IVDP,ISNUMBER\n"
#define CardH4 "C /4/ BASIC DATA FOR THE PROBLEM (4I5,2F10.0,I5)\n"
#define CardV4 "C              MAXIT,EPSTA,EPSTR,NJRAP\n"
#define CardH5 "C /5/ DATA FOR RESTART (3I5,D15.8)\n"
#define CardV5 "C IREST\n"
#define CardH6 "C /6/ DATA FOR TIME STEPS (2I5,F10.0)\n"
#define CardV6 "C   NKDT(I),DTDT(I)    (I=1,NPER)\n"
#define CardH7 "C /7/ DATA FOR NODAL POINT DATA (I5,3X,I2,3F10.2,10X,I5)\n"
#define CardV7 "C N,IDT(N),CORD(N,1),CORD(N,2),CORD(N,3),KORC\n"

//Card /8/
#define CardH8 "C /8/ ELEMENT GROUP DATA (4I5)\n"
#define CardV8 "C NETIP,NET,INDAX,HEIGHT,RADIUS\n"
#define CardH8_2 "C /8-2/ DATA FOR 2D ELEMENT GROUP (I5,5X,I5)\n"
#define CardV8_2_a_1 "C a) Card with data about 2D element\n"
#define CardV8_2_a_2 "C NMAT2D,  NP2DMX\n"
#define CardV8_2_b_1 "C b) Data for each element in group (11I5,F10.0)\n"
#define CardV8_2_b_2 "C NN,(NEL(NN,I),I=1,4),NMAT\n"
#define CardV8_2_c_1 "C Midnodes for 2D element (5I5)\n"
#define CardV8_2_c_2 "C (NEL(NN,I),I=5,9)\n"

#define CardH8_3 "C /8-3/ DATA FOR 3D ELEMENT GROUP (I5,5X,I5)\n"
#define CardV8_3_a_1 "C a) Card with data about 3D element\n"
#define CardV8_3_a_2 "C NMAT3D,  NP3DMX\n"
#define CardV8_3_b_1 "C b) Data for each element in group (15I5)\n"
#define CardV8_3_b_2 "C NN,(NEL(NN,I),I=1,8),NMAT\n"
#define CardV8_3_c_1 "C Midnodes for 3D element (13I5)\n"
#define CardV8_3_c_2 "C (NEL(NN,I),I=9,21)\n"

//Card /9/
#define CardH9 "C /9/ DATA FOR PRESCRIBED POTENTIAL\n"
#define CardH9_a "C a) Number of prescribed values (I5)\n"
#define CardH9t "C /9/ DATA FOR PRESCRIBED TEMPERATIRE\n"
#define CardH9_at "C a) Number of temperature values (I5)\n"
#define CardV9_a "C NUMZAD\n"
#define CardH9_b "C b) Prescribed values (3I5,F10.0)\n"
#define CardV9_b "C NNODE,INDPR,NUMFUN,ZADVR\n"

//Card /10/
#define CardH10p "C /10/ DATA ABOUT INITIAL VALUES (30X,F10.0)\n"
#define CardV10p "C                                   GAMA,IOSA,ISIL,IFIL,ZASIC\n"
#define CardH10tp "C /10/ DATA ABOUT INITIAL TEMPERATURE (F10.3)\n"
#define CardV10tp "C TPOC\n"


//Card/ /11/
#define CardH11 "C /11/ DATA ABOUT BOUNDARY CONDITIONS (3I5)\n"
#define CardV11 "C MAXSIL,INDFS,INFILT\n"
#define CardV11t "C MAXSIL, MAXTQE, MAXER\n"
#define CardH11_a1 "C GIVEN FLUX ON ELEMENT\n"
#define CardH11_a "C a) BOUNDARY CONDITIONS FOR TWO-DIMENSIONAL ELEMENT\n"
#define CardV11_a "C NN,NPV1,NPV2,NUMFUN,P1,P2\n"
#define CardH11_b "C b) BOUNDARY CONDITIONS FOR THREE-DIMENSIONAL ELEMENT\n"
#define CardV11_b "C NN,NPV1,NPV2,NPV3,NPV4,NPV5,NPV6,NPV7,NPV8,NUMFUN,P1,P2,P3,P4\n"
#define CardH11_ac1 "C GIVEN CONVECTION ON ELEMENT\n"
#define CardV11_bc "C NN,NPV1,NPV2,NPV3,NPV4,NPV5,NPV6,NPV7,NPV8,NUMFUNH,NUMFUNTOK\n"
#define CardH11_ar1 "C GIVEN CONVECTION ON ELEMENT\n"
#define CardV11_br "C NN,NPV1,NPV2,NPV3,NPV4,NPV5,NPV6,NPV7,NPV8,NUMFUNH,NUMFUNSF\n"

//Card /12/
#define CardH12 "C /12/ DATA FOR FLUX CALCULATION ALONG CONTOUR\n"
#define CardV12 "C NKONT,MAXSUR\n"
#define CardH12_1 "C Group of cards with tables for flux contours\n"
#define CardH12_1_a "C Data about flux contour in a table form\n"
#define CardV12_1_a "C KKONT,NSUR\n"
#define CardH12_1_b "C b) BOUNDARY CONDITIONS FOR TWO-DIMENSIONAL ELEMENT\n"
#define CardV12_1_b "C NN,NPV1,NPV2\n"
#define CardH12_1_c "C c) BOUNDARY CONDITIONS FOR THREE-DIMENSIONAL ELEMENT\n"
#define CardV12_1_c "C NN,NPV1,NPV2,NPV3,NPV4\n"


//Card /13/
#define CardH13 "C /13/ DATA ABOUT MATERIALS (I5)\n"
#define CardV13 "C NUMMAT\n"
#define CardH13_1 "C /13-1/ DATA ABOUT MATERIAL CONSTANTS\n"
#define CardV13_1_1 "C b) Group of cards with material constants(4F10.0)\n"
#define CardV13_1_2 "C       KX,       KY,       KZ,        S\n"



//Card /14/
#define CardH14 "C /14/ DATA ABOUT TIME FUNCTIONS (2I5)\n"
#define CardV14 "C NTABFT,MAXTFT\n"
#define CardH14_1 "C /14-1/ GROUP OF CARDS WITH TABLES FOR TIME FUNCTIONS\n"
#define CardV14_1_a_1 "C a) data about function in a table form (2I5)\n"
#define CardV14_1_a_2 "C IBR,IMAX    (IMAX.LE.MAXTFT)\n"
#define CardV14_1_b_1 "C b) values for argument - function (2F10.0)\n"
#define CardV14_1_b_2 "C ((FN(I,IBR,J),I=1,2),J=1,IMAX)\n"


#define CardH15 "C /15/ FINAL CARD (A4)\n"
#define CardV15 "STOP"


//Default values

//Card /2/
#define INDFOR		2
//Card /3/
#define NSTAC		1
#define NPER		1
#define NPRINT		0
#define IVDP		0
#define ISNUMBER    1
//Card /4/
#define INTEB		1
#define INDSC		0
#define IFORM		0
#define MAXIT		0
#define EPSTA		0.
#define EPSTR		0.
#define NJRAP		0
//Card /5/
#define IREST		0
#define NCZK		0
#define NCZR		0
#define VREMO		0.
//Card /6/
#define NKDT		1
#define DDT			1.
//Card /7/
#define KORC		0
//Card /8/
#define INDAX		0
#define HEIGHT		0.
#define RADIUS		0.
//Card /8-2/
#define NMAT2D		1
#define MAT2D		0
#define NP2DMX		4
#define IPR2DC		0
#define IPTG2		0
#define NGAU2X		2
#define NGAU2Y		2
#define NTHIC		0
#define NQQ			0
#define KORE		0
#define NBEG		0
#define IPRCO		0
#define IPGS		0
//Card /8-3/
#define NMAT3D		0
#define MAT3D		0
#define NP3DMX		8
#define IPR3DC		0
#define IPRTG3		0
#define NGAUSX		2
#define NGAUSY		2
#define NGAUSZ		2
//Card /9/
#define INDPR		1
//Card /10/
#define GAMA		9.81
#define IOSA		3
#define ISIL		1
#define IFIL		0
#define ZASIC		0.999999
//Card /11/
#define INDFS		0
#define INFILT		0
//Card /12/
#define NPOC		1
#define NTOK		0
#define NHR			0
//Card /13-3/
#define ITO			0
#define IHR			0
#define FO			0




//Errors

#define NO_NODES		"There are no nodes."
#define NO_GROUPS		"There are no groups of elements."
#define NO_LOADS		"There are no loads."

//Maximalni broj funkcija
#define MAX_TF		1000

//Tolerancije
#define LOAD_FORCE_TOL		1.e-8
#define LOAD_PRESS_TOL		1.e-8
#define LOAD_NDISP_TOL		1.e-12