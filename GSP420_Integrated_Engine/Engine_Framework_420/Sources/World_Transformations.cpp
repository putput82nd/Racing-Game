#include "..\Headers\GfxStats.h"
#include "..\Headers\World_Transformations.h"

//constructor
WorldTransform::WorldTransform()
{
    Reset();
}

// Reset the matrices to default position
void WorldTransform::Reset()
{
    D3DXMatrixIdentity( &m_translate );
    D3DXMatrixIdentity( &m_rotate );
    D3DXMatrixIdentity( &m_scale );
    D3DXMatrixIdentity( &m_transform );

    m_rotationX = m_rotationY = m_rotationZ = 0.0f;
}


//Absolute translation
void WorldTransform::TranslateAbs( float x, float y, float z )
{
    m_translate._41 = x;
    m_translate._42 = y;
    m_translate._43 = z;
}

//Relative translation
void WorldTransform::TranslateRel( float x, float y, float z )
{
    m_translate._41 += x;
    m_translate._42 += y;
    m_translate._43 += z;
}

//Absolute rotation
void WorldTransform::RotateAbs( float x, float y, float z )
{
    m_rotationX = x;
    m_rotationY = y;
    m_rotationZ = z;
    D3DXMatrixRotationYawPitchRoll( &m_rotate, y, x, z );
}

//Relative rotation
void WorldTransform::RotateRel( float x, float y, float z )
{
    m_rotationX += x;
    m_rotationY += y;
    m_rotationZ += z;
    D3DXMatrixRotationYawPitchRoll( &m_rotate,  m_rotationY,  m_rotationX,  m_rotationZ );
}

//Absolute scale.
void WorldTransform::ScaleAbs( float x, float y, float z )
{
    m_scale._11 = x;
    m_scale._22 = y;
    m_scale._33 = z;
}

//Relative scale.
void WorldTransform::ScaleRel( float x, float y, float z )
{
    m_scale._11 += x;
    m_scale._22 += y;
    m_scale._33 += z;
}

//Get the transformation matrix
D3DXMATRIX* WorldTransform::GetTransform()
{
    m_transform = m_scale * m_rotate * m_translate; 

    return &m_transform;
}