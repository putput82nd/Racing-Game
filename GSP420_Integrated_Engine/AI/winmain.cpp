// winmain.cpp

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#include <windows.h>
#include "carState.cpp"
#include "AICoreInterface.h"

HINSTANCE hInst;				// holds the instance for this app
HWND wndHandle;					// global window handle

int WINAPI WinAIMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)  // Changed to sub main to prevent multiple mains from trying to compile 14 AUG 
{
    // Main message loop:
	// Enter the message loop
    MSG msg; 
    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
    {
		// check for messages
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		// this is called when no messages are pending
		else
		{
			// Create the opponents
			Opponent opponent(ent_Dale_Bearnhardt_Jr);

			AI_Interface AI(opponent);

			opponent.laps = 0; // Needs to be incremented each time the player vehicle crosses the finish line
									
			opponent = AI.SetState(OvertakePath::Instance());

			while (opponent.laps != 5)
			{				
				if (opponent.laps == 3)
					opponent = AI.SetState(OptimalPath::Instance());
				opponent.Update();
				AI.update(opponent);
			}			
		}
    }
	return (int) msg.wParam;
}