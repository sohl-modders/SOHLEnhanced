//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef COMMON_MINIBSPFILE_H
#define COMMON_MINIBSPFILE_H

// MINI-version of BSPFILE.H to support entity lump extraction stuff.

#define BSPVERSION_QUAKE 29
#define BSPVERSION	30

struct lump_t
{
	int		fileofs, filelen;
};

#define	LUMP_ENTITIES	0
#define	LUMP_PLANES		1
#define	LUMP_TEXTURES	2
#define	LUMP_VERTEXES	3
#define	LUMP_VISIBILITY	4
#define	LUMP_NODES		5
#define	LUMP_TEXINFO	6
#define	LUMP_FACES		7
#define	LUMP_LIGHTING	8
#define	LUMP_CLIPNODES	9
#define	LUMP_LEAFS		10
#define	LUMP_MARKSURFACES 11
#define	LUMP_EDGES		12
#define	LUMP_SURFEDGES	13
#define	LUMP_MODELS		14

#define	HEADER_LUMPS	15

struct dheader_t
{
	int			version;	
	lump_t		lumps[HEADER_LUMPS];
};


#endif //COMMON_MINIBSPFILE_H