#include "..\Headers\GfxStats.h"
#include "..\Headers\Main_Game.h"

//Default constructor
GameApp::GameApp()
{
    m_pFramework = NULL;
    m_showInstructions = FALSE;
	m_meshCar1 = NULL;
	m_meshCar2 = NULL;
	m_meshCar3 = NULL;
	m_meshTrack = NULL;
	
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	radian = .5235987756;
	reference = D3DXVECTOR3(0, 15, -50);
}

//Clean up resources
void GameApp::Release()
{
    SAFE_RELEASE( m_pFramework );
}

//Sets the Framework instance of the application.
void GameApp::SetFramework( Framework* pFramework )
{
    m_pFramework = pFramework;

	//m_interface.Initialize();
}

//Initialize application-specific resources and states here.
BOOL GameApp::Initialize()
{
    m_camera.SetMaxVelocity( 100.0f );
    m_camera.SetPosition( new D3DXVECTOR3( 0.0f, 25.0f, 0.0f ) );
    m_camera.SetLookAt( &car3.velocity /*new D3DXVECTOR3( -186.0f, 10.0f, -133.0f )*/ );
    m_camera.Update();

	    // Create the light
    ZeroMemory( &m_light, sizeof( D3DLIGHT9 ) );
    m_light.Type = D3DLIGHT_DIRECTIONAL;
    m_light.Diffuse.r = 1.0f;
    m_light.Diffuse.g = 1.0f;
    m_light.Diffuse.b = 1.0f;
    m_light.Direction.x = -1.0f;
    m_light.Direction.y = -1.0f;
    m_light.Direction.z = 1.0f;

    return TRUE;
}

 
//This callback function will be called immediately after the Direct3D device has been created. This is 
//the best location to create D3DPOOL_MANAGED resources. Resources created here should be released in 
//the OnDestroyDevice callback. 
void GameApp::OnCreateDevice( LPDIRECT3DDEVICE9 pDevice )
{
    D3DXCreateSprite( pDevice, &m_pTextSprite );
    m_font.Initialize( pDevice, "Arial", 12 );

    //m_terrain.Initialize( pDevice, "heightmap1_257.raw", "terrain.jpg" );
	//m_terrain.Initialize( pDevice, "heightmap1_257.raw", "grass.dds" );
	m_terrain.Initialize( pDevice, "TerragenExp1.raw", "grass.dds" );
    m_terrain.ScaleAbs( 2.5f, .5f, 2.5f );


	//Create the mesh
	m_meshObject1.Load(pDevice, "..\\Assets\\Meshes\\Cars\\Lambo\\Lamborghini.x", "..\\Assets\\Meshes\\Cars\\Lambo\\Lamburghini.jpg" );	

	//make sure that the mesh object is null
	
	m_meshCar1 = new MeshInstance[1];
	m_meshCar1[0].SetMesh(&m_meshObject1);

	m_meshObject2.Load(pDevice, "..\\Assets\\Meshes\\Cars\\McLarenCar\\mack.x", "..\\Assets\\Meshes\\Cars\\McLarenCar\\fw.bmp");
	
	m_meshCar2 = new MeshInstance[2];
	m_meshCar2[0].SetMesh(&m_meshObject2);

	m_meshObject3.Load(pDevice, "..\\Assets\\Meshes\\track2.x", "..\\Assets\\Meshes\\asphalt.bmp");
	
	m_meshTrack = new MeshInstance[3];
	m_meshTrack[0].SetMesh(&m_meshObject3);

	m_meshObject4.Load(pDevice, "..\\Assets\\Meshes\\Cars\\MGP Car\\w03.x", "..\\Assets\\Meshes\\Cars\\MGP Car\\pontons.bmp");

	m_meshCar3 = new MeshInstance[4];
	m_meshCar3[0].SetMesh(&m_meshObject4);
	
	m_meshCar3[0].SetXPosition(-41);
	m_meshCar3[0].SetYPosition(0);
	m_meshCar3[0].SetZPosition(169);
}

 
//This callback function will be called immediately after the Direct3D device has been created. This is 
//the best location to create D3DPOOL_DEFAULT resources. Resources created here should be released in 
//the OnLostDevice callback. 
void GameApp::OnResetDevice( LPDIRECT3DDEVICE9 pDevice )
{
    m_pTextSprite->OnResetDevice();
    m_font.OnResetDevice();

    // Set projection
    m_camera.SetAspectRatio( (float)m_pFramework->GetWidth() / (float)m_pFramework->GetHeight() );
    pDevice->SetTransform( D3DTS_PROJECTION, m_camera.GetProjectionMatrix() );

    // Set render states
    pDevice->SetRenderState( D3DRS_FILLMODE, m_pFramework->GetFillMode() );      
    pDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );  
    pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	//pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
 //   pDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB( 80, 80, 80 ) );
 //   pDevice->LightEnable( 0, TRUE );
 //   pDevice->SetLight( 0, &m_light );
}


//This callback function will be called immediately after the Direct3D device has entered a lost state 
//and before IDirect3DDevice9::Reset is called. Resources created in the OnResetDevice callback should 
//be released here, which generally includes all D3DPOOL_DEFAULT resources.
void GameApp::OnLostDevice()
{
    m_pTextSprite->OnLostDevice();
    m_font.OnLostDevice();
}

//This callback function will be called immediately after the Direct3D device has been destroyed. 
//Resources created in the OnCreateDevice callback should be released here, which generally includes 
//all D3DPOOL_MANAGED resources.
void GameApp::OnDestroyDevice()
{
    SAFE_RELEASE( m_pTextSprite );
    m_font.Release();
    m_terrain.Release();

	m_meshCar1[0].Release();
	m_meshCar2[0].Release();
	m_meshCar3[0].Release();
	m_meshTrack[0].Release();
	
	m_meshObject1.Release();
	m_meshObject2.Release();	
	m_meshObject3.Release();
	m_meshObject4.Release();
}

//Updates the current frame.
void GameApp::OnUpdateFrame( LPDIRECT3DDEVICE9 pDevice, float elapsedTime)
{
	//m_camera.SetLookAt(&car3.velocity);

    m_camera.Update();
	double MaxTurnRate = D3DXToRadian(45);

	car2.maxSpeed = 75;
	
	//
	 // Cheap collision detection
    if( car1.distance(car2.vehiclePos) < 2.0f )
    {
     car1.ChangeState(OvertakePath::Instance());
    }
    else
    {
     car1.ChangeState(OptimalPath::Instance());
    }
	m_meshCar1[0].SetXPosition( car1.vehiclePos.x );
	m_meshCar1[0].SetYPosition(car1.vehiclePos.y);
	m_meshCar1[0].SetZPosition(car1.vehiclePos.z);
    m_meshCar1[0].ScaleAbs( 1.0f, 1.0f, 1.0f );
	//if (car1.angle > MaxTurnRate) car1.angle = MaxTurnRate;
	if (car1.angle > .00001)
	{
		m_meshCar1[0].RotateRel( 0.0f, -car1.angle * elapsedTime, 0.0f );
		//car1.oldHeading = car1.heading;
	}
	
	m_meshCar2[0].SetXPosition( car2.vehiclePos.x );
	m_meshCar2[0].SetYPosition(car2.vehiclePos.y);
	m_meshCar2[0].SetZPosition(car2.vehiclePos.z);
    m_meshCar2[0].ScaleAbs( 3.5f, 3.5f, 3.5f );
	//if (car2.angle > MaxTurnRate) car2.angle = MaxTurnRate;
    if (car2.angle > .00001)
	{
		m_meshCar2[0].RotateRel( 0.0f, -car2.angle * elapsedTime, 0.0f );
		//car2.oldHeading = car2.heading;
	}	
	m_meshTrack[0].SetXPosition( 10.0f );
	m_meshTrack[0].SetYPosition( 0.0f);
	m_meshTrack[0].SetZPosition( -20.0f);
    m_meshTrack[0].ScaleAbs( 0.6f, 0.01f, 1.0f );

	m_meshCar3[0].SetXPosition( car3.vehiclePos.x );
	m_meshCar3[0].SetYPosition(car3.vehiclePos.y);
	m_meshCar3[0].SetZPosition(car3.vehiclePos.z);
    m_meshCar3[0].ScaleAbs( 2.6f, 2.6f, 2.6f );
	//if (car2.angle > MaxTurnRate) car2.angle = MaxTurnRate;
	
	AI1.update(car1);
	AI2.update(car2);

	car1.Update();
	car2.Update();
	car3.Update();

	updateCamera();
	
	// boundary values for the outer mountain ring
	float farOuterMountain   =  275;  // [maxOuterZValue]
	float nearOuterMountain  = -250;  // [minOuterZValue]
	float rightOuterMountain  =  230;  // [maxOuterXValue]
	float leftOuterMountain  = -280;  // [minOuterXValue]


	// check to see if the player car has reached the far outer mountain
	if (car3.vehiclePos.z > farOuterMountain)
	{
		car3.velocity.z = car3.velocity.z  * -1;
	}

	// check to see if the player car has reached the near outer mountain
	if (car3.vehiclePos.z < nearOuterMountain)
	{
		car3.velocity.z = car3.velocity.z * -1;
	}

	// check to see if the player car has reached the right outer mountain
	if (car3.vehiclePos.x > rightOuterMountain)
	{
		car3.velocity.x = car3.velocity.x * -1;
	}

	// check to see if the player car has reached the left outer mountain
	if (car3.vehiclePos.x < leftOuterMountain)
	{
		car3.velocity.x = car3.velocity.x * -1;
	}

	// inner mountain collision check

	// boundary values for the inner mountain ring
	float farInnerMountain   =   81;  // [maxInnerZValue]
	float nearInnerMountain  = -144;  // [minInnerZValue]
	float rightInnerMountain  =  115;  // [maxInnerXValue]
	float leftInnerMountain  = -136;  // [minInnerXValue]

	// check to see if the player car has breached far side inner mountain or near side inner mountain
	if ((car3.vehiclePos.z < farInnerMountain) && (car3.vehiclePos.z > nearInnerMountain))
	{
		if ((car3.vehiclePos.x < rightInnerMountain) && (car3.vehiclePos.x > leftInnerMountain))
		{
			car3.velocity.z = car3.velocity.z * -1; 
		}
	}

	// check to see if the player car has breached the right side inner mountain or left side inner mountain
	if ((car3.vehiclePos.x < rightInnerMountain) && (car3.vehiclePos.x > leftInnerMountain))
	{
		if ((car3.vehiclePos.z < farInnerMountain) && (car3.vehiclePos.z > nearInnerMountain))
		{
			car3.velocity.x = car3.velocity.x * -1;
		}
	}
}

//Renders the current frame.
void GameApp::OnRenderFrame( LPDIRECT3DDEVICE9 pDevice, float elapsedTime )
{
	char* g_instructions = 	
	 "WASD: To Move \r\n Click to Drag Mouse to Look Around \r\n Esc: Quit \r\n F2: Toggle Fullscreen \r\n F3: Toggle WireFrame \r\n F4: Blur Effect \r\n F5: Particle Effect \r\n F6: Play Music";

    pDevice->SetTransform( D3DTS_VIEW, m_camera.GetViewMatrix() );
    sprintf_s( m_fps, "%.2f FPS", m_pFramework->GetFPS() );

    pDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 100, 100, 255 ), 1.0f, 0 ); 
    pDevice->BeginScene();

    m_terrain.Render( pDevice );

	m_meshTrack[0].Render(pDevice);

	//Render the meshes
	m_meshCar1[0].Render(pDevice);
	m_meshCar2[0].Render(pDevice);
	m_meshCar3[0].Render(pDevice);
	
    // Display framerate and instructions
    m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
    m_font.Print( m_fps, 5, 5, D3DCOLOR_XRGB( 255, 255, 255 ), m_pTextSprite );
		

    if ( m_showInstructions )
    {
        m_font.Print( g_instructions, 5, 20, D3DCOLOR_XRGB( 255, 255, 255 ), m_pTextSprite );
    }
    else
    {
        m_font.Print( "F1 - Demo Instructions", 5, 20, D3DCOLOR_XRGB( 255, 255, 255 ), m_pTextSprite );

    }
    m_pTextSprite->End();

    pDevice->EndScene();
    pDevice->Present( 0, 0, 0, 0 );
}

//Responds to key presses
void GameApp::ProcessInput( long xDelta, long yDelta, long zDelta, BOOL* pMouseButtons, BOOL* pPressedKeys, float elapsedTime )
{
    float cameraSpeed = 20.0f;
	

	// Temp player positions
	float tempX;
	float tempZ;

    if ( pMouseButtons[0] )
    {
        m_camera.Yaw( xDelta * elapsedTime * 1.8f);
        m_camera.Pitch( yDelta * elapsedTime * 1.8f );
    }


    if ( pPressedKeys[DIK_W] )
    {
        m_camera.MoveForward( cameraSpeed * elapsedTime );
		
    }
    if ( pPressedKeys[DIK_A] )
    {
		m_camera.Strafe( -cameraSpeed * elapsedTime );
    }
    if ( pPressedKeys[DIK_S] )
    {
        m_camera.MoveForward( -cameraSpeed * elapsedTime );
    }
    if ( pPressedKeys[DIK_D] )
    {
        m_camera.Strafe( cameraSpeed * elapsedTime );	
    }
	// Player car keyboard controls
    if ( pPressedKeys[DIK_UP] )
    {
		car3.velocity = car3.velocity * 150;			//accelerate the player car		
	}
    if ( pPressedKeys[DIK_LEFT] )
    {	
		m_meshCar3[0].RotateRel( 0.0f, -radian * elapsedTime, 0.0f );
		    
		D3DXMatrixRotationAxis( &rotation, &up, radian * elapsedTime );
				
		D3DXVec3TransformCoord( &car3.heading, &car3.heading, &rotation );
		
		D3DXVec3TransformCoord( &car3.velocity, &car3.velocity, &rotation );
		
		updateCamera();
    }
    if ( pPressedKeys[DIK_DOWN] )
    {
		if (car3.velocity > D3DXVECTOR3(0.0, 0.0, 0.0))
			car3.velocity = car3.heading * -1.2;		//decelerate the player car
		else car3.velocity = D3DXVECTOR3(0.0, 0.0, 0.0);
	}
    if ( pPressedKeys[DIK_RIGHT] )
    {
		m_meshCar3[0].RotateRel( 0.0f, radian * elapsedTime, 0.0f );
    
		D3DXMatrixRotationAxis( &rotation, &up, radian * elapsedTime );
				
		D3DXVec3TransformCoord( &car3.heading, &car3.heading, &rotation );	
		
		D3DXVec3TransformCoord( &car3.velocity, &car3.velocity, &rotation );
		
		updateCamera();
    }
    if ( pPressedKeys[DIK_ESCAPE] )
    {
        m_pFramework->LockKey( DIK_ESCAPE );
        PostQuitMessage( 0 );
    }

    if ( pPressedKeys[DIK_F1] )
    {
        m_pFramework->LockKey( DIK_F1 );
        m_showInstructions = !m_showInstructions;
    }

    if ( pPressedKeys[DIK_F2] )
    {
        m_pFramework->LockKey( DIK_F2 );

        if ( m_pFramework != NULL )
        {
            m_pFramework->ToggleFullscreen();
        }
    }

    if ( pPressedKeys[DIK_F3] )
    {
        m_pFramework->LockKey( DIK_F3 );

        if ( m_pFramework != NULL )
        {
            m_pFramework->ToggleWireframe();
        }
    }
	
	if ( pPressedKeys[DIK_F4] )
    {
        m_pFramework->LockKey( DIK_F4 );

        if ( m_pFramework != NULL )
        {
            //Toggle Blur Effect
			//Creates Car Sound as well
        }
    }

	if ( pPressedKeys[DIK_F5] )
    {
        m_pFramework->LockKey( DIK_F5 );

        if ( m_pFramework != NULL )
        {
            //Toggle Particle Effect
        }
    }

	if ( pPressedKeys[DIK_F6] )
    {
        m_pFramework->LockKey( DIK_F6 );

        if ( m_pFramework != NULL )
        {
            Sound::Instance()->TogglePause(Sound::Song1);
        }
    }
}

//Application entry point
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{ 
    GameApp* pApplication = new GameApp();
    Framework* pFramework = new Framework( (BaseApp*)pApplication );

    pApplication->SetFramework( pFramework );

    // Initialize any application resources
    if ( !pApplication->Initialize() )
    {
        return 0;
    }

    // Initialize the Framework
    if ( !pFramework->Initialize( " Racing Game Engine GSP420 - Team B ", hInstance, 640, 480, TRUE ) )
    {
        return 0;
    }

    // Rock and roll
    pFramework->Run();
    
    // Clean up resources
    SAFE_RELEASE( pApplication );

    return 0;
}
void GameApp::updateCamera()
{
	D3DXVECTOR3 heading = D3DXVECTOR3((car3.velocity.x), 0.0, (car3.velocity.z));
	car3.heading = D3DXVECTOR3(car3.heading.x + car3.vehiclePos.x, 0.0f, car3.heading.z + car3.vehiclePos.z);
	car3.velocity = car3.heading;
	D3DXVec3TransformCoord(&movedReference, &reference, &rotation);
	camPos = D3DXVECTOR3((car3.vehiclePos.x), (car3.vehiclePos.y+5), (car3.vehiclePos.z+20));
	//m_camera.SetPosition(&camPos);
	//m_camera.SetLookAt(&car3.heading);
	m_camera.Update();
}