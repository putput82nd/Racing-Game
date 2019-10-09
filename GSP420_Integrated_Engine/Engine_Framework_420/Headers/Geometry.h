#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "..\Headers\GfxStats.h"



typedef struct Position
{
public:
    Position() : X(0), Y(0), Z(0) {}
    
	Position( float x, float y, float z ) 
        : X(x), Y(y), Z(z) {}
    
	float X, Y, Z;

} Position;


typedef struct PositionColor
{
public:
    PositionColor() : X(0), Y(0), Z(0), Color(0) {}
    
	PositionColor( float x, float y, float z, DWORD color ) 
        : X(x), Y(y), Z(z), Color(color) {}
    
	float X, Y, Z;
    DWORD Color;

} PositionColor;


typedef struct PositionTextured
{
public:
    PositionTextured() : X(0), Y(0), Z(0), Tu(0), Tv(0) {}
    
	PositionTextured( float x, float y, float z, float tu, float tv ) 
        : X(x), Y(y), Z(z), Tu(tu), Tv(tv) {}
    
	float X, Y, Z;
    float Tu, Tv;

} PositionTextured;


typedef struct PositionColorTextured
{
public:
    PositionColorTextured() : X(0), Y(0), Z(0), Color(0), Tu(0), Tv(0) {}
    
	PositionColorTextured( float x, float y, float z, DWORD color, float tu, float tv ) 
        : X(x), Y(y), Z(z), Color(color), Tu(tu), Tv(tv) {}
    
	float X, Y, Z;
    DWORD Color;
    float Tu, Tv;

} PositionColorTextured;


typedef struct PositionNormalTextured
{
public:
    PositionNormalTextured() : X(0), Y(0), Z(0), Nx(0), Ny(0), Nz(0), Tu(0), Tv(0){}

    PositionNormalTextured( float x, float y, float z, float nx, float ny, float nz, float tu, float tv ) 
        : X(x), Y(y), Z(z), Nx(nx), Ny(ny), Nz(nz), Tu(tu), Tv(tv) {}

    float X, Y, Z;
    float Nx, Ny, Nz;
    float Tu, Tv;

} PositionNormalTextured;

class Geometry
{
public:
    static void GeneratePositionNormalTextured( PositionNormalTextured** ppVertices, int verticesAlongWidth, int verticesAlongLength );
    static void GeneratePositionTexturedWithHeight( PositionTextured** ppVertices, int verticesAlongWidth, int verticesAlongLength, UCHAR* pHeight );
    
	static int GenerateIndices( int** ppIndices, int verticesAlongWidth, int verticesAlongLength );
    static int GenerateIndices( USHORT** ppIndices, int verticesAlongWidth, int verticesAlongLength );
};




#endif