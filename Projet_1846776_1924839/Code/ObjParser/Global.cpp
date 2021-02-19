/*
 *  Global.cpp
 *  JelloCube
 *
 *  Created by Olivier Dionne on 02/05/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Global.h"

SINGLETON_CPP_DECLARATION( CGlobal );

int CGlobal::g_WindowX = 100;
int CGlobal::g_WindowY = 100;
int CGlobal::g_WindowW = 800;
int CGlobal::g_WindowH = 600;

float    CGlobal::g_CameraYaw      = 0.2f;
float    CGlobal::g_CameraPitch    = 0.2f;
float    CGlobal::g_CameraZoom     = 7.0f;
CVector3 CGlobal::g_CameraPosition = CVector3::ZERO;

int CGlobal::g_MouseX      = 0;
int CGlobal::g_MouseY      = 0;
int CGlobal::g_MouseButton = 0;

CPhysicsManager* CGlobal::g_PhysicsManager = NULL;

CJelloCube* CGlobal::g_JelloCube = NULL;
CLight*     CGlobal::g_Light     = NULL;
CFloor*     CGlobal::g_Floor     = NULL;
