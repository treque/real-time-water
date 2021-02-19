#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "Vecteur3.h"

using namespace Math3D;

class CGlobal
{
    public :
	
        // Window variables
        static int g_WindowX;
        static int g_WindowY;
        static int g_WindowW;
        static int g_WindowH;

		// Camera variables
		static float     g_CameraYaw;
		static float     g_CameraPitch;
		static float     g_CameraZoom;
        static CVecteur3 g_CameraPosition;
	
		// Mouse variables
		static int g_MouseX;
		static int g_MouseY;
		static int g_MouseButton;
}; 

#endif