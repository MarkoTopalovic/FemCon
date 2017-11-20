#define CardH1 "C /1/ HEADING CARD (80A1)\n"
#define CardV1 "C NASLOV\n"
#define CardH2 "C /2/ FORMAT FOR INPUT DATA (free format)\n"
#define CardV2 "C INDFOR\n"
#define CardH3 "C /3/ BASIC DATA FOR THE PROBLEM (10I5)\n"
#define CardV3 "C NP,NGET,NMATT,NSTAC,NPER,NPRINT,IANIZ,NULAZ,ITOR,IGRAF\n"
#define CardH4 "C /4/ BASIC DATA FOR THE PROBLEM (4I5,2F10.0,I5)\n"
#define CardV4 "C INTEB,INDSC,IFORM,MAXIT,EPSTA,EPSTR,NJRAP\n"
#define CardH5 "C /5/ DATA FOR RESTART (3I5,D15.8)\n"
#define CardV5 "C IREST,NCZK,NCZR,VREMO\n"
#define CardH6 "C /6/ DATA FOR TIME STEPS\n"
#define CardV6_a_1 "C a) Number of time steps card (16I5)\n"
#define CardV6_a_2 "C (NKDT(I),I=1,NPER)\n"
#define CardV6_b_1 "C b) Card of time increment per step in solution period (8F10.0)\n"
#define CardV6_b_2 "C (DDT(I),I=1,NPER)\n"
#define CardH7 "C /7/ DATA FOR NODAL POINT DATA (I5,3X,I2,3F10.2,10X,I5)\n"
#define CardV7 "C N,IDT(N),CORD(N,1),CORD(N,2),CORD(N,3),KORC\n"

//Card /8/
#define CardH8 "C /8/ ELEMENT GROUP DATA (4I5)\n"
#define CardV8 "C NETIP,NET,INDAX,IZIP\n"
#define CardH8_2 "C /8-2/ DATA FOR 2D ELEMENT GROUP (8I5,F10.0)\n"
#define CardV8_2_a_1 "C a) Card with data about 2D element\n"
#define CardV8_2_a_2 "C NMAT2D,MAT2D,NP2DMX,IPR2DC,IPTG2,NGAU2X,NGAU2Y,NTHIC,THIC\n"
#define CardV8_2_b_1 "C b) Data for each element in group (11I5,F10.0)\n"
#define CardV8_2_b_2 "C NN,(NEL(NN,I),I=1,4),NMAT,NQQ,KORE,NBEG,IPRCO,IPGS,THI\n"
#define CardV8_2_c_1 "C Midnodes for 2D element (5I5)\n"
#define CardV8_2_c_2 "C (NEL(NN,I),I=5,9)\n"

#define CardH8_3 "C /8-3/ DATA FOR 3D ELEMENT GROUP (8I5)\n"
#define CardV8_3_a_1 "C a) Card with data about 3D element\n"
#define CardV8_3_a_2 "C NMAT3d,MAT3D,NP3DMX,IPR3DC,IPRTG3,NGAUSX,NGAUSY,NGAUSZ\n"
#define CardV8_3_b_1 "C b) Data for each element in group (15I5)\n"
#define CardV8_3_b_2 "C NN,(NEL(NN,I),I=1,8),NMAT,NQQ,KORE,NBEG,IPRCO,IPGS\n"
#define CardV8_3_c_1 "C Midnodes for 3D element (13I5)\n"
#define CardV8_3_c_2 "C (NEL(NN,I),I=9,21)\n"

//Card /9/
#define CardH9 "C /9/ DATA ABOUT DEPENDENCE OF MATERIAL CONSTANTS ON POTENTIAL (6I5)\n"
#define CardV9 "C NANLK,NTABK,MAXTK,NANLC,NTABC,MAXTC\n"
#define CardH9_1 "C /9-1/ DATA ABOUT MATERIALS (3I5,6F10.0)\n"
#define CardV9_1 "C IBFK(I,1),IBFK(I,2),IBFK(I,3),FAKP(I,1),FAKP(I,2),FAKP(I,3),GUSM(I),TOPM(I),TMNM(I)\n"

#define CardH9_2 "C /9-2/ TABLES OF DEPENDENCE: ELECTRIC PERMITIVITY-POTENTIAL\n"
#define CardV9_2_a_1 "C a) Number of points in table for material set\n"
#define CardV9_2_a_2 "C NN,NTAKV(NN)\n"
#define CardV9_2_b_1 "C b) Table of values potential-electric permitivity\n"
#define CardV9_2_b_2 "C (TABK(K,NN,J),K=1..)\n"
#define CardH9_3 "C /9-3/ TABLES OF DEPENDENCE: SPECIFIC HEAT-TEMPERATURE\n"
#define CardV9_3_a_1 "C a) Number of points in table for material set\n"
#define CardV9_3_a_2 "C NN,NTACV(NN)\n"
#define CardV9_3_b_1 "C b) Table of values potential-electric permitivity\n"
#define CardV9_3_b_2 "C (TABC(K,NN,J),K=1..)\n"


//Card /10/
#define CardH10 "C /10/ DATA ABOUT TIME FUNCTIONS (2I5)\n"
#define CardV10 "C NTABFT,MAXTFT\n"
#define CardH10_1 "C /10-1/ GROUP OF CARDS WITH TABLES FOR TIME FUNCTIONS\n"
#define CardV10_1_a_1 "C a) data about function in a table form (2I5)\n"
#define CardV10_1_a_2 "C IBR,IMAX    (IMAX.LE.MAXTFT)\n"
#define CardV10_1_b_1 "C b) values for argument - function (2F10.0)\n"
#define CardV10_1_b_2 "C ((FN(I,IBR,J),I=1,2),J=1,IMAX)\n"

//Card/ /11/
#define CardH11 "C /11/ DATA FOR PRESCRIBED POTENTIALS (2I5,F10.2,I5)\n"
#define CardV11 "C N,NC,FAK,KORC\n"

//Card/ /12/
#define CardH12 "C /12/ DATA ABOUT INITIAL VALUES AND BOUNDARY CONDITIONS (11I5)\n"
#define CardV12 "C NPOC,NQP,MAXTQP,NHP,MAXTHP,NTOK,MAXTOK,NQE,MAXTQE,NHR,MAXTHR\n"

#define CardH12_1 "C /12-1/ DATA ABOUT INTERNAL HEAT GENERATION (SOURCE)\n"
#define CardV12_1_a_1 "C a) Function definition\n"
#define CardV12_1_a_2 "C IBR,NTAQE(IBR)\n"
#define CardV12_1_b_1 "C b) Values for argument - function\n"
#define CardV12_1_b_2 "C ((QEFN(I,IBR,J),I=1,2),J=1,NTAQE(IBR))\n"

#define CardH12_2 "C /12-2/ DATA ABOUT SURFACE FLUXES\n"
#define CardV12_2_a_1 "C a) Function definition\n"
#define CardV12_2_a_2 "C IBR,NTAQP(IBR)\n"
#define CardV12_2_b_1 "C b) Values for argument - function\n"
#define CardV12_2_b_2 "C ((QPFN(I,IBR,J),I=1,2),J=1,NTAQP(IBR))\n"

#define CardH12_3 "C /12-3/ DATA ABOUT ENVIRONMENTAL TEMPERATURE\n"
#define CardV12_3_a_1 "C a) Function definition\n"
#define CardV12_3_a_2 "C IBR,NTAOK(IBR)\n"
#define CardV12_3_b_1 "C b) Values for argument - function\n"
#define CardV12_3_b_2 "C ((TOKFN(I,IBR,J),I=1,2),J=1,NTAOK(IBR))\n"

#define CardH12_4 "C /12-4/ DATA ABOUT CONVECTION COEFFICIENT\n"
#define CardV12_4_a_1 "C a) Function definition\n"
#define CardV12_4_a_2 "C IBR,NTAHP(IBR)\n"
#define CardV12_4_b_1 "C b) Values for argument - function\n"
#define CardV12_4_b_2 "C ((HPFN(I,IBR,J),I=1,2),J=1,NTAHP(IBR))\n"

#define CardH12_5 "C /12-5/ DATA ABOUT EMISSIVITY COEFFICIENT\n"
#define CardV12_5_a_1 "C a) Function definition\n"
#define CardV12_5_a_2 "C IBR,NTAHR(IBR)\n"
#define CardV12_5_b_1 "C b) Values for argument - function\n"
#define CardV12_5_b_2 "C ((HRFN(I,IBR,J),I=1,2),J=1,NTAHR(IBR))\n"
#define CardV12_5_c_1 "C c) Radiation property data (D15.8,I5)\n"
#define CardV12_5_c_2 "C SBC,NFO\n"
#define CardV12_5_d_1 "C d) Shape factors\n"
#define CardV12_5_d_2 "C (FOHR(I),I=1,NFO)\n"

#define CardH12_6 "C /12-6/ INITIAL TEMPERATURE\n"
#define CardV12_6 "C TPOC\n"

#define CardH13_2 "C /13-3/ BOUNDARY CONDITIONS FOR TWO-DIMENSIONAL ELEMENT\n"
#define CardV13_2 "C NN,NPV1,NPV2,IFL,IHP,ITO,IHR,FO\n"


#define CardH13_3 "C /13-3/ BOUNDARY CONDITIONS FOR THREE-DIMENSIONAL ELEMENT\n"
#define CardV13_3 "C NN,NPV1,NPV2,NPV3,NPV4,IFL,IHP,ITO,IHR,FO\n"
#define CardV13_3_e "C EMPTY CARD\n\n"

#define CardH16 "C /16/ FINAL CARD (A4)\n"
#define CardV16 "STOP"


//Default values

//Card /2/
#define INDFOR		2
//Card /3/
#define NSTAC		1
#define NPER		1
#define NPRINT		0
#define IANIZ		0
#define NULAZ		1
#define ITOR		0
#define IGRAF		1
//Card /4/
#define INTEB		1
#define INDSC		0
#define IFORM		0
#define MAXIT		0
#define EPSTA		0.
#define EPSTR		0.
#define NJRAP		0
//Card /5/
#define IREST		1
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
#define IZIP		0
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
#define NANLK		0
#define NANLC		0
#define IBFK		0
#define FAKP		0.
#define TOPM		2000.
#define TMNM		0.
//Card /11/
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
#define MAX_TF		100

//Tolerancije
#define LOAD_FORCE_TOL		1.e-8
#define LOAD_PRESS_TOL		1.e-8
#define LOAD_NDISP_TOL		1.e-12