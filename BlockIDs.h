#define UBID_NODES							15
#define UBID_ELEMENTS						71
#define UBID_LOADS							407
#define UBID_CONSTRAINTS					757	
#define UBID_NODAL_RESULTS					55
#define UBID_ELEMENT_RESULTS				57




//FEMAP Types
#define FET_ROD			     1
#define FET_BAR			     2
#define FET_BEAM		     5
#define FET_GAP				 9
#define FET_SHEAR_LIN		11
#define FET_SHEAR_PAR		12
#define FET_MEMBRANE_LIN	13
#define FET_MEMBRANE_PAR	14
#define FET_BENDING_LIN		15
#define FET_BENDING_PAR		16
#define FET_PLATE_LIN		17
#define FET_PLATE_PAR		18
#define FET_PLANESTRAIN_LIN	19
#define FET_PLANESTRAIN_PAR	20
#define FET_LAMINATE_LIN	21
#define FET_LAMINATE_PAR	22
#define FET_AXISYM_LIN		23
#define FET_AXISYM_PAR		24
#define FET_3D_LIN			25
#define FET_3D_PAR			26
#define FET_CONTACT			34
#define FET_BEAM37		    37     // od verzije 8.2 stampa u neu fajl vrednost 37



//FEMAP Topology
#define FETO_LINE		0
#define FETO_LINE3		1
#define FETO_TRI3		2
#define FETO_TRI6		3
#define FETO_QUAD4		4
#define FETO_QUAD8		5
#define FETO_TETRA4		6
#define FETO_WEDGE6		7
#define FETO_BRICK8		8
#define FETO_POINT		9
#define FETO_TETRA10	10
#define FETO_WEDGE15	11
#define FETO_BRICK20	12
#define FETO_RIGIDLIST	13
#define FETO_MULTILIST	15

//FEMAP Material types
#define FEMT_ISO			0
#define FEMT_2D_ORTHO		1
#define FEMT_3D_ORTHO		2
#define FEMT_2D_ANISO		3
#define FEMT_3D_ANISO		4
#define FEMT_HYPERELASTIC	5

//FEMAP Load types
#define FELT_NODAL_FORCE	1
#define FELT_NODAL_DISPL	2
#define FELT_VELOCITY		7
#define FELT_ELEM_PRESS		11
#define FELT_ELEM_HEAT_GEN	13
#define FELT_ELEM_HEAT_FL	14
#define FELT_NODAL_TOTAL_PR	14
#define FELT_ELEM_CONV		15
#define FELT_ELEM_RADIATION	16
//(4.4+)Type of load (1=Nodal Force, 2=Nodal Displacement, 3=Nodal Accel, 5=Nodal Heat Generation, 6=Nodal Heat Flux, 7=Velocity, 8=Nonlinear Transient, 10=Distributed Line Load, 11=Element Face Pressure, 13=Element Heat Generation, 14=Element Heat Flux, 15=Element Convection, 16=Element Radiation)

//PAK Types
#define PAK_TRUSS			1
#define PAK_ELAST			11
#define PAK_GAP				12
#define PAK_ISO_2D			2
#define PAK_ISO_TRI			21
#define PAK_ISO_3D			3
#define PAK_ISO_PRISM		31
#define PAK_ISO_PYR			32
#define PAK_ISO_TETRA		33
#define PAK_THINWALED_B		6
#define PAK_AXISYM_SHELL	7
#define PAK_ISO_SHELL		8
#define PAK_ISO_TRI_SHELL	81
#define PAK_SHELL_BD		84
#define PAK_BEAM_SUPER		9
#define PAK_CONTACT			93

//PAK Material models
#define PAKM_ELASTIC_ISO				1	
#define PAKM_ELASTIC_ORTHO				2
#define PAKM_THERMO_ELASTIC_ISO			3
#define PAKM_MISES_PLASTIC_ROD			5
#define PAKM_MISES_PLASTIC				6
#define PAKM_CAM_CLAY					709
#define PAKM_MOHR_COULOMB				742


//PAK-T Types
#define PAKT_ISO_1D			1
#define PAKT_ISO_2D			2
#define PAKT_ISO_3D			3

//PAK-E Types
#define PAKE_ISO_1D			1
#define PAKE_ISO_2D			2
#define PAKE_ISO_3D			3