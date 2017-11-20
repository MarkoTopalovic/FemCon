#define CardH1 "C /1/ HEADING CARD (80A1)\n"
#define CardV1 "C NASLOV\n"
#define CardHF2 "C /2/ FORMAT FOR INPUT DATA (free format)\n"
#define CardVF2 "C INDFOR\n"
#define CardHF3 "C /3/ BASIC DATA FOR THE PROBLEM (8I5)\n"
#define CardVF3 "C NP,NGET,NMATM,NSTAC,NPER,NPRINT,INDF,IPENAL\n"
#define CardHF4 "C /4/ BASIC DATA FOR THE PROBLEM (4I5,2F10.0,2I5)\n"
#define CardVF4 "C INTEB,INDSC,IFORM,MAXIT,EPSTA,EPSTR,NJRAP,MBAND\n"
#define CardHF5 "C /5/ DATA FOR RESTART (I5)\n"
#define CardVF5 "C IREST\n"
#define CardHF6 "C /6/ DATA FOR TIME STEPS\n"
#define CardVF6 "C NDT(I),TIME(I)\n"
#define CardHF7 "C /7/ DATA FOR NODAL POINT DATA (I5,5I5,3F10.6)\n"
#define CardVF7 "C   N,    (ID(N,I),I=1,5)     CORD(1,N), CORD(2,N), CORD(3,N)\n"

//Card /8/
#define CardHF8 "C /8/ ELEMENT GROUP DATA (4I5)\n"
#define CardVF8 "C NETIP,NET,INDAX,IZIP\n"
#define CardHF8_1 "C /8-2/ DATA FOR 2/D ELEMENT\n"
#define CardVF8_1_a_1 "C a) Card with data about 2D element\n"
#define CardVF8_1_a_2 "C NMAT2D,MAT2D,NP2DMX,PENALT (4I5)\n"
#define CardVF8_1_b_1 "C b) Data for each element in group (11I5,F10.0)\n"
#define CardVF8_1_b_2 "C NN,   (NEL(NN,I),I=1,4), NMAT,NQQ,KORE,NBEG,IPRCO,IPGS,THI\n"
#define CardVF8_1_c_1 "C c) Midnodes for 2D 8-node element (5I5)\n"
#define CardVF8_1_c_2 "C (NEL(NN,I),I=5,9)\n"
#define CardHF8_2 "C /8-2/ DATA FOR 3/D ELEMENT\n"
#define CardVF8_2_a_1 "C a) Card with data about 3D element\n"
#define CardVF8_2_a_2 "C NMAT2D,MAT2D,NP3DMX,PENALT (4I5)\n"
#define CardVF8_2_b_1 "C b) Data for each element in group (11I5,F10.0)\n"
#define CardVF8_2_b_2 "C NN,   (NEL(NN,I),I=1,8) \n"
#define CardVF8_2_c_1 "C c) Midnodes for 3D 20-node element (5I5)\n"
#define CardVF8_2_c_2 "C (NEL(NN,I),I=9,20)\n"

//Card /9/
#define CardHF9 "C /9/ DATA FOR MATERIAL CONSTANTS\n"
#define CardHF9_1 "C /9-1/ DATA ABOUT FLUID DENSITY, INDICATOR FOR NON-NEWTONIAN FLUID (F10.2,I5)\n"
#define CardVF9_1 "C DENSIT, INDAMI\n"

#define CardHF9_2 "C /9-2/ DATA ABOUT CONDUCTION COEFFICIENT (F10.2)\n"
#define CardVF9_2 "C  CONDUC\n"
#define CardHF9_3 "C /9-3/ SPECIFIC HEAT AT CONSTANT PRESSURE (F10.2)\n"
#define CardVF9_3 "C   SPECH\n"
#define CardHF9_4_1 "C /9-4/ GRAVITY ACCELERATION IN X, Y AND Z DIRECTIONS, DYNAMIC VISCOSITY,\n"
#define CardHF9_4_2 "C THERMAL EXPANSION, REFERENCE TEMPERATURE (6D10.5)\n"
#define CardVF9_4 "C      GX,       GY,       GZ,      AMI,     BETA,     TETA0\n"

//Card /10/
#define CardHF10 "C /10/ DATA FOR PRESCRIBED VALUES (2I5)\n"
#define CardVF10 "C NUMZAD, NUMST\n"
#define CardHF10_1 "C /10-1/ PRESCRIBED VALUES AT NODES (3I5,F10.3)\n"
#define CardVF10_1 "C NNODE, INDPR, ITIMF, VALUE\n"

//Card /11/
#define CardHF11 "C /11/ DATA ABOUT INITIAL VALUES (5F10.3)\n"
#define CardVF11 "C   UINIT,    VINIT,    WINIT,    PINIT,     TINIT\n"

//Card /12/
#define CardHF12 "C /10/ DATA ABOUT TIME FUNCTIONS (2I5)\n"
#define CardVF12 "C NTABFT,MAXTFT\n"
#define CardHF12_1 "C /10-1/ GROUP OF CARDS WITH TABLES FOR TIME FUNCTIONS\n"
#define CardVF12_1_a_1 "C a) data about function in a table form (2I5)\n"
#define CardVF12_1_a_2 "C IBR,IMAX    (IMAX.LE.MAXTFT)\n"
#define CardVF12_1_b_1 "C b) values for argument - function (2F10.0)\n"
#define CardVF12_1_b_2 "C ((FN(I,IBR,J),I=1,2),J=1,IMAX)\n"

//Card /13/
#define CardHF13 "C /13/ DATA ABOUT BOUNDARY CONDITIONS - SURFACE TRACTIONS\n"
#define CardVF13a "C Boundary conditions for 2D analysis\n"
#define CardVF13b "C Boundary conditions for 3D analysis\n"
#define CardHF13_1 "C NELEM NODE1 NODE2 INDSTX INDSTY\n"
#define CardHF13_1a "C NELEM NODE1 NODE2 NODE3 NODE4 INDSTX INDSTY INDSTZ\n"

//Card /14/
#define CardHF16 "C /16/ FINAL CARD (A4)\n"
#define CardVF16 "STOP"


//Default values

//Card /2/
#define INDFOR		2
//Card /3/
#define NSTAC		1
#define NPER		1
#define NPRINT		0
#define INDF		1
#define IPENAL		0
//Card /4/
#define INTEB		1
#define INDSC		0
#define IFORM		0
#define MAXIT		0
#define EPSTA		0.
#define EPSTR		0.
#define NJRAP		1
#define MBAND		0
//Card /5/
#define IREST		1
//Card /6/
#define NDT			1
#define TIME		1.
//Card /7/
#define IP			0
#define IT			0
//Card /8/
#define INDAX		0
#define IZIP		0
//Card /8-2/
#define NMAT2D		1
#define MAT2D		1
#define NP2DMX		9
#define PENALTY		0.
//Card /9/
#define INDAMI      0
//Card /10/
#define NUMST		0
//Card /11/
#define UINIT		0.
#define VINIT		0.
#define WINIT		0.
#define PINIT		0.
#define TINIT		0.


//Errors

#define NO_NODES		"There are no nodes."
#define NO_GROUPS		"There are no groups of elements."
#define NO_LOADS		"There are no loads."

//Maximalni broj funkcija
#define MAX_TF		100

//Tolerancije
#define VELOCITY_TOL		1.e-8