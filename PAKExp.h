#define CardH1 "C /1/ HEADING CARD (80A1)\n"
#define CardV1 "C NASLOV\n"
#define CardH2 "C /2/ FORMAT FOR INPUT DATA (free format)\n"
#define CardV2 "C INDFOR\n"
#define CardH3 "C /3/ BASIC DATA FOR THE PROBLEM (4I5,5X.I5)\n"
#define CardV3 "C NP,NGELEM,NMATM,NPER,NKRT,NCRACK,NCXFEM,IGBM\n"
#define CardH4 "C /4/ BASIC DATA FOR THE PROBLEM (6I2,3X,3I5)\n"
#define CardV4 "C (IOPGL(I),I=1,6),NDIN,ISOPS,ICVEL\n"
#define CardH5 "C /5/ DATA FOR POST-PROCESSING (10I5)\n"
#define CardV5 "C NULAZ,NBLPR,NBLGR,ISTKO,NCVPR,ISTEM,ISTVN,ISTSI,ISTDE,ISTNA\n"
#define CardH6 "C /6/ GROUP OF CARDS WITH DATA FOR BLOCKS (5I5)\n"
#define CardV6 "C NBL,(NPRGR(NBL,I),I=1,4)\n"
#define CardH7 "C /7/ DATA FOR RESTART (I5,10X,2I5,2F10.0)\n"
#define CardV7 "C IREST,       NMODS,ICCGG,    TOLG,    ALFAG\n"
#define CardH7_1 "C /7-1/ DATA ABOUT INITIAL IMPERFECTION (3I5,F10.0)\n"
#define CardV7_1 "C MODE,NODE,DOF,SCIM\n"
#define CardH8 "C /8/ GROUP OF CARDS WITH DATA FOR TIME STEPS (2I5,F10.0)\n"
#define CardV8 "C   I,NKDT(I),DTDT(I)    (I=1,NPER)\n"
#define CardH8_1 "C /8-1/ DATA FOR DYNAMIC ANALYSIS (2I5)\n"
#define CardV8_1 "C   IMASS,IDAMP\n"
#define CardH8_2 "C /8-2/ DATA FOR TIME INTEGRATION METHOD (I5,2F10.0)\n"
#define CardV8_2 "C   MDVI,      PIP,      DIP\n"
#define CardH8_3 "C /8-3/ EIGEN PROBLEM DATA (2I5)\n"
#define CardV8_3 "C   ISTYP,NSOPV\n"
#define CardH9 "C /9/ DATA FOR EQULIBRIUM ITERATIONS (5I5,4F10.0,I5)\n"
#define CardV9 "C METOD,MAXIT,KONVE,KONVS,KONVM,TOLE,    TOLS,     TOLM,     TOLA,NBRCR\n"
#define CardH9_1 "C /9-1/ DATA FOR AUTOMATIC LOAD STEPPING (3I5,3F10.0)\n"
#define CardV9_1 "C ITEOPT,KPNNOD,KPDIR,DTUK,ALFG,DELS\n"
#define CardH10 "C /10/ GROUP OF CARDS FOR NODAL POINT DATA (I5,A1,6I2,2X,3F10.0,2I5) (K=1,NP)\n"
#define CardV10 "C  N,CH,(ID(N,I),I=1,6),   (CORD(N,J),J=1,3),      KORC,LJ(N)\n"
#define CardH10_02 "C /10-4/ DATA ABOUT DEPENDENT DISPLACEMENT\n"
#define CardH10_02_a "C  a) Basic data about linear dependence (2I5)\n"
#define CardV10_02_a "C  MMP,NEZAV\n"
#define CardH10_02_b "C  b) Data for coefficients in linear relations (I5,6F10.0)\n"
#define CardV10_02_b "C  IPC,FMPC1,FMPC2,FMPC3,FMPC4,FMPC5,FMPC6\n"
#define CardH10_02_c "C  c) Degree of freedom in linear relation (14I5)\n"
#define CardV10_02_c "C  IMP, IZC, IZS, NC1, NS1, NC2, NS2, NC3, NS3, NC4, NS4, NC5, NS5, NC6, NS6\n"
#define CardH11 "C /11/ GENERAL DATA FOR MATERIAL MODELS (3I5)\n"
#define CardV11 "C (MODEL(I,K),I=1,3)    (K=1,NMATM)\n"

//Card /12/
#define CardH12 "C /12/ DATA FOR EACH MATERIAL (2I5,F10.0)\n"
#define CardV12 "C MOD  MAT     GUST\n"

#define CardH12_1 "C /12-1/ MATERIAL MODEL 1 (ELASTIC-ISOTROPIC)\n"
#define CardV12_1_1 "C YOUNGS MODULI (F10.0)\n"
#define CardV12_1_2 "C        E\n"
#define CardV12_1_2_1 "C        E         V\n"
#define CardV12_1_3 "C POISSONS RATIO (F10.0)\n"
#define CardV12_1_4 "C        V\n"

#define CardH12_2 "C /12-2/ MATERIAL MODEL 2 (ELASTIC-ORTHOTROPIC)\n"
#define CardV12_2_1 "C YOUNGS MODULI  (3F10.0)\n"
#define CardV12_2_2 "C       Ex        Ey        Ez\n"
#define CardV12_2_3 "C POISSONS RATIO (3F10.0)\n"
#define CardV12_2_4 "C      Vxy       Vyz       Vzx\n"
#define CardV12_2_5 "C SHEAR MODULI (3F10.0)\n"
#define CardV12_2_6 "C      Gxy       Gyz       Gzx\n"

#define CardH12_5 "C /12-5/ MATERIAL MODEL 5 (MISES ELASTIC-PLASTIC WITH ISOTROPIC HARDENING)\n"
#define CardH12_5_a1 "C Number of points for definition of strain-stress function (I5)\n"
#define CardV12_5_a1 "C NTFUN\n"
#define CardH12_5_a2 "C Youngs modulus and Poissons ration (2F10.0)\n"
#define CardV12_5_a2 "C   E,   V\n"
#define CardH12_5_a3 "C Value od yield stress and strain (2F10.0)\n"
#define CardV12_5_a3 "C   e,   Sy\n"

#define CardH12_6 "C /12-6/ MATERIAL MODEL 6 (MISES ELASTIC-PLASTIC WITH MIXED HARDENING)\n"
#define CardH12_6_a1 "C Number of points for definition of function strain-stress (I5)\n"
#define CardV12_6_a1 "C NTFUN\n"
#define CardH12_6_a2 "C Modulus of elasticity and Poissons coefficient (2F10.0)\n"
#define CardV12_6_a2 "C     E,    V\n"
#define CardH12_6_a3 "C Hardening function - Ramberg-Osgood (2F10.0)\n"
#define CardV12_6_a3 "C  TAUY,   Cy\n"
#define CardH12_6_a4 "C Hardening function Ramberg-Osgood  (2F10.0)\n"
#define CardV12_6_a4 "C    AN,   EM\n"

#define CardH12_9 "C YOUNG'S MODULUS, POISSON'S RATIO, TENSION CUTOFF,IEL (3F10.0,I5)\n"
#define CardV12_9_1 "C       E,        V,       AT, IEL,\n"
#define CardV12_9_2 "C  MATERIAL CONSTANTS FOR CAM-CLAY MATERIAL MODEL\n"
#define CardV12_9_3 "C     AEM,     ALAM,      AKA,      AE1,      AE0,\n"

#define CardH12_42 "C a) Modulus of elasticity E, Poisson's ratio (2F10.0)\n"
#define CardV12_42_1 "C        E         V   \n"
#define CardV12_42_2 "C b) Data for definition of failure curve (6F10.0)\n"
#define CardV12_42_3 "C       c,      phi,      psi,\n"

#define CardH12_14 "C /12-14/ MATERIAL MODEL 14 (MISES THERMO-ELASTIC-ISOTROPIC)\n"
#define CardH12_14_a1 "C Number of points for definition of modulus of elasticity E (I5)\n"
#define CardV12_14_a1 "C NTFUN\n"
#define CardH12_14_a2 "C Argument-function values for modulus of elasticity E (2F10.0)\n"
#define CardV12_14_a2 "C TEMP,   E\n"
#define CardH12_14_b1 "C Number of points for definition of Poissons ratio v (I5)\n"
#define CardV12_14_b1 "C NTFUN\n"
#define CardH12_14_b2 "C Argument-function values for Poissons ratio v (2F10.0)\n"
#define CardV12_14_b2 "C TEMP,   V\n"
#define CardH12_14_c1 "C Number of points for definition of modulus of thermal expansion coff. (I5)\n"
#define CardV12_14_c1 "C NTFUN\n"
#define CardH12_14_c2 "C Argument-function values for thermal expansion coefficient (2F10.0)\n"
#define CardV12_14_c2 "C TEMP,   ALFA\n"
#define CardH12_14_e "C Reference temperature for materijal (F10.0)\n"
#define CardV12_14_e "C TREF\n"

//Card /13/
#define CardH13       "C /13/ ELEMENT GROUP DATA (7I5,3F10.0)    (I=1,NGELEM)\n"
#define CardV13       "C NETIP,NE,IATYP,NMODM,INDBTH,INDDTH,INDKOV, COEF1, COEF2, COEF3\n"
#define CardH13_1     "C /13-1/ DATA FOR TRUSS ELEMENTS\n"
#define CardH13_1_a_1 "C a) Card with data for the curent element (3I5,F10.0,I5)\n"
#define CardV13_1_a_2 "C NN,NMAT,ISNA,APR,KORC\n"
#define CardH13_1_b_1 "C b) Card wiht nodal point data of the current element (2I5)\n"
#define CardV13_1_b_2 "C NEL(NN,1),NEL(NN,2)\n"
#define CardH13_2     "C /13-2/ DATA FOR 2D ISOPARAMETRIC ELEMENTS\n"
#define CardV13_2_a_1 "C a) First card with basic data for the element group (4I5,F10.0,I5,3F10.0,I5)\n"
#define CardV13_2_a_2 "C IETYP,NGAUSX,NGAUSY,MSET,BETA,MSLOJ,CPP1,CPP2,CPP3,IALFA\n"
#define CardV13_2_b_1 "C b) Card wiht data for the current element (5I5,F10.0,I5,2F10.0)\n"
#define CardV13_2_b_2 "C NN,NMAT,IPRCO,ISNA,IPGS,THI,KORC,BTH,DTH\n"
#define CardV13_2_c_1 "C c) Card with nodal point data for the current element (9I5)\n"
#define CardV13_2_c_2 "C (NEL(NN,I),I=1,9)\n"
#define CardH13_6     "C /13-6/ DATA ABOUT THIN-WALLED BEAM ELEMENTS\n"
#define CardH13_6_a_1 "C a) Card with general data about beam cross-section (4I5,4F10.0,2I5)\n"
#define CardV13_6_a_2 "C NT,NELM,NTIP,NKAR,OY,OZ,YM,ZM,ALFAU,INDOF\n"
#define CardH13_6_b_1 "C b) Card wiht cross-section characteristics. (7F10.0,2I1)\n"
#define CardV13_6_b_2 "C POVR,YI,ZI,TK,ALFA,CAPAY,CAPAZ,ILU,ICP\n"
#define CardH13_6_h_1 "C h) Data about thin-walled beam element. (6I5,5X,3F10.0)\n"
#define CardV13_6_h_2 "C NN,NEL(NN,1),NEL(NN,2),NTIP,MAT,NAP,CTR(NN,1),CTR(NN,2),CTR(NN,3)\n"
#define CardH13_12     "C /13-12/ DATA FOR GAP ELEMENTS\n"
#define CardH13_12_a "C a) Data for gap parameters (7F10.0)\n"
#define CardV13_12_a_3 "C      GAP,      AKN,     AKNZ,      AKS,      AMY,      AMZ,     PFOR\n"
#define CardH13_12_b "C b) Data for the current element (4I5,3F10.0,I5)\n"
#define CardV13_12_b_2 "C  NN,     ISNA,IDIR,     XCOS,     YCOS,     ZCOS,KORC\n"
#define CardH13_12_c "C c) Card with nodal point data of the current element (2I5)\n"
#define CardV13_12_c_2 "C NEL(NN,1),NEL(NN,2)\n"

#define CardH13_3 "C /13-3/ DATA FOR 3D ISOPARAMETRIC ELEMENTS\n"
#define CardV13_3_a_1 "C Card with basic data about 3D elements (3I5,5X,F10.0,35X,I5\n"
#define CardV13_3_a_2 "C NGAUSX,NGAUSY,NGARSZ,BETA,IALFA\n"
#define CardV13_3_b_1 "C Card with data about the current element (6I5,10X,2F10.0)\n"
#define CardV13_3_b_2 "C NN,NMAT,IPRCO,ISNA,IPGS,KORC,BTH,DTH\n"
#define CardV13_3_c_1 "C Card with nodal data (1 to K) of the element (8I5)\n"
#define CardV13_3_c_2 "C (NEL(NN,I),I=1,8)\n"

#define CardH13_9      "C /13-9/ DATA ABOUT BEAM SUPERELEMENT  (NN=1,NE)\n"
#define CardH13_9_a_1  "C a) First card wiht basic data for the element group (4I5,F10.0,I5,30X,I5)\n"
#define CardH13_9_a_2  "C NAGAUSX,NGAUSY,NGAUSZ,MSET,BETA,MSLOJ,                         IALFA\n"
#define CardH13_9_b_1  "C b) basic data about cross-section (8I5,3F10.0)\n"
#define CardH13_9_b_2  "C IPODT,NS,NCVP,NPRES,IND3D,NCFL,NPRL,IPOMT,(CPP1(I),I=1,3)\n"
#define CardH13_9_d1_1 "C d1) Data about current segment (4I5,5X,F10.0,I5,2F10.0)\n"
#define CardH13_9_d1_2 "C NN,NMAT,IPRCO,ISNA,THI,KORC,BTH,DTH   (NN=1,NS)\n"
#define CardH13_9_d2_1 "C d2) Card wiht data about nodes of the current segment (9I5)\n"
#define CardH13_9_d2_2 "C (NEL(NN,I),I=1,N)\n"
#define CardH13_9_e1_1 "C e1) Card with basic cross-section data (6I2,3X,4F10.0,I5)\n"
#define CardH13_9_e1_2 "C (IOPGL(I),I=1,6),(XYZ(I),I=1,3),ALFAU,INDOF\n"
#define CardH13_9_e2_1 "C e2) Group of card with data about nodes in cross-section (I5,A1,6I2,2X,3F10.0,I5) (N=1,NCVP)\n"
#define CardH13_9_e2_2 "C  N,CH,(ID(N,I),I=1,6),   (CORD(N,J),J=1,3),      KORC\n"
#define CardH13_9_f1_1 "C f1) First card with data about current beam superelement(I5,15X,I5,F10.0,I5)\n"
#define CardH13_9_f1_2 "C NN,IPGS,THIBA,KORC\n"
#define CardH13_9_f2_1 "C f2) Second card with data about current beam superelement (9I5)\n"
#define CardH13_9_f2_2 "C (NEL(NN,I),I=1,3),(NPRE(NN,J),J=1,3),(NOGR(NN,J),J=1,3)\n"
#define CardH13_9_f3_1 "C f3) Third card with data about current beam superelement (6F10.0,I5)\n"
#define CardH13_9_f3_2 "C (COPT(NN,J),J=1,3),(FPRE(NN,J),J=1,3),NPOMT(NN)\n"

#define CardV13_8_a_2 "C NGAUSX,NGAUSY,NGAUSZ,MSET,BETA,MSLOJ,CPP1,CPP2,CPP3,IALFA\n"

#define CardH13_93 "C /13-93/ CONTACT ELEMENT\n"
#define CardV13_93_a_1 "C a) Data about contact element group\n"
#define CardV13_93_a_2 "C NTSF,FMSTAT,FMDIN,EPSIL,IPENALTY,PENALTYN,PENLATYT\n"
#define CardV13_93_b1_1 "C b1) Target surface data\n"
#define CardV13_93_b1_2 "C NN,NSVSF,ITSRF,MASXE,IORT,KORC,XP(1),XP(2),XP(3)\n"
#define CardV13_93_b2_1 "C b2) Nodes of target surface polygon\n"
#define CardV13_93_b2_2 "C NELSF,NELSF,NELSF,NELSF,NELSF,NELSF\n"
#define CardV13_93_c_1 "C c) Data about contact pairs - contact elements\n"
#define CardV13_93_c_2 "C NEL(1),NEL(2),FSDF(1),FSFD(2)\n"

#define CardH14 "C /14/ DATA ABOUT TIME FUNCTIONS (2I5)\n"
#define CardV14 "C NTABFT,MAXTFT\n"
#define CardH14_1 "C /14-1/ GROUP OF CARDS WITH TABLES FOR TIME FUNCTIONS\n"
#define CardV14_1_a_1 "C a) Data about function in a table form (2I5)\n"
#define CardV14_1_a_2 "C IBR,IMAX    (IMAX.LE.MAXTFT)\n"
#define CardV14_1_b_1 "C b) Values for argument - function (2F10.0)\n"
#define CardV14_1_b_2 "C ((FN(I,IBR,J),I=1,2),J=1,IMAX)\n"

#define CardH15 "C /15/ GENERAL DATA ABOUT LOADS (4I5,5X,5I5)\n"
#define CardV15 "C NCF,NPP2,NPP3, NPGR,    NPLJ,NTEMP,NZADP,INDZS,ICERNE\n"
#define CardH15_1 "C /15-1/ CONCENTRATED LOADS DATA (3I5,F10.0,I5,F10.0)\n"
#define CardV15_1 "C    N   IP   NC   FAK     KORC    FPOM\n"
#define CardH15_2 "C /15-2/ LINE LOADING\n"
#define CardV15_2_a_1 "C a) Data about line loading (pressure) along line (A-B) (3I5,3F10.0,I5)\n"
#define CardV15_2_a_2 "C ITIPE,NFUN,IPRAV,(FAKP(J),J=1,2),THICV,KORC\n"
#define CardV15_2_b_1 "C Data for nodes of the line A-B (3I5)\n"
#define CardV15_2_b_2 "C (NODPR(J),J=1,3)\n"
#define CardH15_3 "C /15-3/ SURFACE LOADING\n"
#define CardV15_3_a_1 "C Data for surface loading (pressure) on surface ABCD (2I5,4F10.0,I5)\n"
#define CardV15_3_a_2 "C NFUN,IPRAV,(FAKP(J),J=1,4),KORC\n"
#define CardV15_3_b_1 "C Data for nodes of the surface ABCD (8I5)\n"
#define CardV15_3_b_2 "C (NODPR(J),J=1,8)\n"
#define CardH15_7 "C /15-7/ DATA FOR TEMPERATURES (2I5,F10.0,I5)\n"
#define CardV15_7 "C    N   NC   FAK     KORC\n"
#define CardH15_8 "C /15-8/ DATA FOR PRESCRIBED DISPLACEMENTS (3I5,F10.0,I5)\n"
#define CardV15_8 "C   N   IP   NC       FAK KORC\n"
#define CardH15_9 "C /15-9/ DATA ABOUT BODY FORCES (I5,3F10.0)\n"
#define CardV15_9 "C   NF        GX        GY        GZ\n"
#define CardH15_10 "C /15-10/ BOUNDARY CONDITIONS FOR THREE-DIMENSIONAL ELEMENT(I5,3F10.0)\n"
#define CardV15_10 "C   NN     NPV(1)    NPV(2)     NPV(3)     NPV(4)\n"


#define CardH16 "C /16/ FINAL CARD (A4)\n"
#define CardV16 "STOP"


//Default values

//Card /2/
#define INDFOR		2
//Card /3/
#define NPER		1
#define NKRT		0
//Card /4/
#define NDIN		0
#define ISOPS		0
#define ICVEL		1
//Card /5/
#define NULAZ		0
#define NBLPR		-1
#define NBLGR		1
#define ISTKO		0
/*#define NCVPR		0
#define ISTEM		-1
#define	ISTVN		-1
#define ISTSI		-1
#define ISTDE		-1
#define ISTNA		2
#define IDEAS		-1 */
#define NCVPR		0
#define ISTEM		0
#define	ISTVN		0
#define ISTSI		0
#define ISTDE		0
#define ISTNA		0
#define IDEAS		0
//Card /6/
#define NPRGR1		1
#define NPRGR2		1
#define NPRGR3	 9000
#define NPRGR4	    0
//Card /7/
#define IREST		0
#define NMODS		0
//#define ICCGG		-11
#define ICCGG		0
#define TOLG		0.
#define ALFAG		0.
#define OKORAK      0
//Card /8/
#define NKDT		1
#define DTDT		1.
//Card /9/
#define METOD		1
#define MAXIT		50
#define	KONVE		1
#define	KONVS		0
#define	KONVM		0
#define	TOLE		0.02    
#define	TOLS		0.
#define	TOLM		0.
#define	TOLA		0.0000001
#define	NBRCR		2
//Card /10/
#define CH			' '
#define KORC		0
#define LJ			0
//Card /11/
#define MODEL2		1
#define MODEL3		20
//Card /12/
#define MOD			1
#define MAT			1
#define GUST		0.
//Card /12-1/
#define FUNMAT1_1	1.e+5
#define FUNMAT2_1	0
//Card /13/
#define IATYP		0
//#define NMODM		1
#define INDBTH		0
#define INDDTH		0
#define	INDKOV		0
#define COEF1		0.
#define COEF2		0.
#define COEF3		0.
//Card /13-2/a
#define NGAUSX2		2
#define NGAUSY2		2
#define NGAUSZ2		2
#define MSET		0
#define BETA		0.
#define MSLOJ		0
#define CPP1		0.
#define CPP2		0.
#define CPP3		0.
//#define IALFA		-1
#define IALFA		0
//Card /13-2/b, /13-3/b
#define IPRCO		0
#define ISNA		0
#define IPGS		0
#define THI			1.
#define KORC		0
#define BTH			0.
#define DTH			0.
//Card /13-3/a
#define NGAUSX3		2
#define	NGAUSY3		2
#define NGAUSZ3		2
#define BETA		0.
//#define IALFA		-1
#define IALFA		0
//Card /13-6
#define NT			0
#define NELM        0
#define NTIP		0
#define NKAR		0
#define OY			0.
#define OZ			0.
#define YM			0.
#define ZM			0.
#define ALFAU		0.
#define INDOF		0
#define ALFA		0.
#define CAPAY		0.
#define CAPAZ		0.
#define ILU 		0
#define ICP 		0
#define NTIP_13_6   0
#define NAP_13_6    0
//Card /13-9
#define NS			1
#define NCVP		1
#define NPRES		1
#define IND3D		0
#define NCFL		0
#define NPRL		0
#define IPOMT		0
//Card /13-12
#define IPG			0
#define GAP_IETYP	2

//Card /13-93
#define FMSTAT		1.
#define FMDIN		1.
#define EPSIL		1.00e-3
#define ITSRF		0
#define IORT		0
#define KORC		0
#define XP1			0.
#define XP2			0.
#define XP3			0.
#define FSFD		0.


//Card /14/
#define MAXTFT		100
//Card /15/
#define	NPP2		0
#define NPP3		0
#define NPGR		0
#define NPLJ		0
#define NTEMP		0
#define NZADP		0
#define INDZS		0
#define ICERNE		0
//#define ICERNE		-1
//Card /15-1/
#define NC			0
#define KORC		0
#define FPOM		0
//Card /15-3/
#define NFUN		1
#define IPRAV		0







//Errors

#define NO_NODES		"There are no nodes."
#define NO_GROUPS		"There are no groups of elements."
#define NO_LOADS		"There are no loads."


//Tolerancije
#define LOAD_FORCE_TOL		1.e-8
#define LOAD_PRESS_TOL		1.e-8
#define LOAD_NDISP_TOL		1.e-12