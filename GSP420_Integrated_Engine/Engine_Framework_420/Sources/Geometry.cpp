#include "..\Headers\GfxStats.h"
#include "..\Headers\Geometry.h"

//Generates vertices with a position, normal, and texture coordinates to create an indexed triangle strip plane.

void Geometry::GeneratePositionNormalTextured( PositionNormalTextured** ppVertices, int verticesAlongWidth, int verticesAlongLength )
{
    SAFE_DELETE_ARRAY( *ppVertices );
    *ppVertices = new PositionNormalTextured[verticesAlongLength * verticesAlongWidth];
    for ( int z = 0; z < verticesAlongLength; z++ )
    {
        for ( int x = 0; x < verticesAlongWidth; x++ )
        {
            float halfWidth = ((float)verticesAlongWidth - 1.0f) / 2.0f;
            float halfLength = ((float)verticesAlongLength - 1.0f) / 2.0f;
            (*ppVertices)[z * verticesAlongLength + x] = PositionNormalTextured(
                (float)x - halfWidth, 0.0f, (float)z - halfLength,
                0.0f, 1.0f, 0.0f,
                (float)x / (verticesAlongWidth - 1), (float)z / (verticesAlongLength - 1)
            );
        }
    }
}

//Generates vertices with a position, normal, and texture coordinates to create an indexed triangle strip plane.
void Geometry::GeneratePositionTexturedWithHeight( PositionTextured** ppVertices, int verticesAlongWidth, int verticesAlongLength, UCHAR* pHeight )
{
    SAFE_DELETE_ARRAY( *ppVertices );
    *ppVertices = new PositionTextured[verticesAlongLength * verticesAlongWidth];
    for ( int z = 0; z < verticesAlongLength; z++ )
    {
        for ( int x = 0; x < verticesAlongWidth; x++ )
        {
            float halfWidth = ((float)verticesAlongWidth - 1.0f) / 2.0f;
            float halfLength = ((float)verticesAlongLength - 1.0f) / 2.0f;
            (*ppVertices)[z * verticesAlongLength + x] = PositionTextured(
                (float)x - halfWidth, 
                (float)pHeight[z * verticesAlongLength + x], 
                (float)z - halfLength,
                (float)x / (verticesAlongWidth - 1), (float)z / (verticesAlongLength - 1)
            );
        }
    }
}

//Generates 32-bit indices for an indexed triangle strip plane.
int Geometry::GenerateIndices( int** ppIndices, int verticesAlongWidth, int verticesAlongLength )
{
    int numIndices = (verticesAlongWidth * 2) * (verticesAlongLength - 1) + (verticesAlongLength - 2);

    SAFE_DELETE_ARRAY( *ppIndices );
    *ppIndices = new int[numIndices];

    int index = 0;
    for ( int z = 0; z < verticesAlongLength - 1; z++ )
    {
        // Even rows move left to right, odd rows move right to left.
        if ( z % 2 == 0 )
        {
            // Even row
            int x;
            for ( x = 0; x < verticesAlongWidth; x++ )
            {
                (*ppIndices)[index++] = x + (z * verticesAlongWidth);
                (*ppIndices)[index++] = x + (z * verticesAlongWidth) + verticesAlongWidth;
            }
            // Insert degenerate vertex if this isn't the last row
            if ( z != verticesAlongLength - 2)
            {
                (*ppIndices)[index++] = --x + (z * verticesAlongWidth);
            }
        } 
        else
        {
            // Odd row
            int x;
            for ( x = verticesAlongWidth - 1; x >= 0; x-- )
            {
                (*ppIndices)[index++] = x + (z * verticesAlongWidth);
                (*ppIndices)[index++] = x + (z * verticesAlongWidth) + verticesAlongWidth;
            }
            // Insert degenerate vertex if this isn't the last row
            if ( z != verticesAlongLength - 2)
            {
                (*ppIndices)[index++] = ++x + (z * verticesAlongWidth);
            }
        }
    } 
    return numIndices;
}

//Generates 16-bit indices for an indexed triangle strip plane.
int Geometry::GenerateIndices( USHORT** ppIndices, int verticesAlongWidth, int verticesAlongLength )
{
    int numIndices = (verticesAlongWidth * 2) * (verticesAlongLength - 1) + (verticesAlongLength - 2);

    SAFE_DELETE_ARRAY( *ppIndices );
    *ppIndices = new USHORT[numIndices];

    int index = 0;
    for ( int z = 0; z < verticesAlongLength - 1; z++ )
    {
        // Even rows move left to right, odd rows move right to left.
        if ( z % 2 == 0 )
        {
            // Even row
            int x;
            for ( x = 0; x < verticesAlongWidth; x++ )
            {
                (*ppIndices)[index++] = (USHORT)(x + (z * verticesAlongWidth));
                (*ppIndices)[index++] = (USHORT)(x + (z * verticesAlongWidth) + verticesAlongWidth);
            }
            // Insert degenerate vertex if this isn't the last row
            if ( z != verticesAlongLength - 2)
            {
                (*ppIndices)[index++] = (USHORT)(--x + (z * verticesAlongWidth));
            }
        } 
        else
        {
            // Odd row
            int x;
            for ( x = verticesAlongWidth - 1; x >= 0; x-- )
            {
                (*ppIndices)[index++] = (USHORT)(x + (z * verticesAlongWidth));
                (*ppIndices)[index++] = (USHORT)(x + (z * verticesAlongWidth) + verticesAlongWidth);
            }
            // Insert degenerate vertex if this isn't the last row
            if ( z != verticesAlongLength - 2)
            {
                (*ppIndices)[index++] = (USHORT)(++x + (z * verticesAlongWidth));
            }
        }
    } 
    return numIndices;
}