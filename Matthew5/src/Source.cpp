// Davant Stewart
// Version 2.000 - Triangles

#include <iostream>
#include <memory>
#include <Windows.h>
#include "Matthew5StandardMicrosoftHeaders.h"

#include "Matthew5Windows.h"
#include "Matthew5DxDesktop.h"
#include "Matthew5DxOculus.h"
#include "IMatthew5DrawInstructions.h"

using namespace Matthew5::Windows;
using namespace Matthew5::Matthew5Graphics;
using namespace Matthew5::Matthew5GraphicsOculus;

std::unique_ptr<IMatthew5Graphics> ChooseOculusOrDesktop(HWND vHWND)
{
	// First try to create the Oculus version and if that fails create the desktop version.

	std::unique_ptr<IMatthew5Graphics> vpIMatthew5Graphics = std::make_unique<Matthew5DxOculus>(vHWND);

	// Get the pointer/address from the smart pointer and cast it to the derived Matthew5DxOculus class 
	// so that we can call functions that only exist in the derived class.
	Matthew5DxOculus * vpMatthew5DxOculus = (Matthew5DxOculus *)vpIMatthew5Graphics.get();

	if ( ! vpMatthew5DxOculus->IsOculusAvailable())
	{
		// Oculus is Not Available, so create the desktop version.
		vpIMatthew5Graphics = std::make_unique<Matthew5DxDesktop>(vHWND);
	}

	return vpIMatthew5Graphics;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	// Create windows wrapper which provides one window.
	int vWidth = 800, vHeight = 600;
	Matthew5Windows vMatthew5Windows(hInstance, vWidth, vHeight);

	// This will point to Oculus or Desktop; however, the interface does not care.
	std::unique_ptr<IMatthew5Graphics> vpIMatthew5Graphics = ChooseOculusOrDesktop(vMatthew5Windows.GetHWND());

	// Add a scene object.
	M5SceneObjectCpuPosition vM5SceneObjectCpuPosition;
	vM5SceneObjectCpuPosition.mPositionX = 2.1f;
	vM5SceneObjectCpuPosition.mPositionY = -1.1f;
	vM5SceneObjectCpuPosition.mPositionZ = -2.5f;
	vpIMatthew5Graphics->AddSceneObject(vM5SceneObjectCpuPosition);

	// Add 3 more.
	for (int i = 0; i < 3; i++)
	{
		vM5SceneObjectCpuPosition.mPositionX = -3.1f;
		vM5SceneObjectCpuPosition.mPositionY = -1.1f;
		vM5SceneObjectCpuPosition.mPositionZ = -3.8f + (i * 10);
		vpIMatthew5Graphics->AddSceneObject(vM5SceneObjectCpuPosition);
	}

	IMatthew5DrawInstructions vIMatthew5DrawInstructions;

	// We will keep the "Camera Look At" the same for now.. which is 1000 units in
	// the forward direction.
	vIMatthew5DrawInstructions.GetCameraLookAt().mPositionZ = -1000.0f;

	while (true)
	{
		Matthew5WindowsUserHoldingKeys & vUserInput = vMatthew5Windows.GetWindowsMessages();

		if (vUserInput.CharHit('W'))
		{
			// Move Forwards, reduce the z.
			// Right Handed Coordinates means larger z is backward; so to move camera forward reduce the z.
			vIMatthew5DrawInstructions.GetCameraPosition().mPositionZ -= 0.01f;
		}
		else if (vUserInput.CharHit('S'))
		{
			// Move Backwards, increase the z.
			// Right Handed Coordinates means larger z is backward; so to move camera backward increase the z.
			vIMatthew5DrawInstructions.GetCameraPosition().mPositionZ += 0.01f;
		}

		vpIMatthew5Graphics->Draw(vIMatthew5DrawInstructions);

		if (vUserInput.CloseProgram == true)
		{
			break;
		}
	}
	return 0;
}


