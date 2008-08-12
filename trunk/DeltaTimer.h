/* *****************************************************************
 * 
 * Following code taken from:
 *
 * http://scriptionary.com/A_High_Resolution_DeltaTimer_for_Win32
 *
 * ****************************************************************/

#pragma once
#ifndef _DeltaTimer_H_
#define _DeltaTimer_H_

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
 
#include <windows.h>
 
// Defines and Creates a simple High Resolution DeltaTimer
class DeltaTimer
{
protected:
	double startTime;
	double endTime;
	bool active;
	LARGE_INTEGER frequency;
	LARGE_INTEGER startCount;
	LARGE_INTEGER endCount;
 
public:
	DeltaTimer ( void ); // ctor
	~DeltaTimer ( void ){}; // dtor
 
	void Start ( void ); // Starts DeltaTimer
	void Stop ( void ); // Stops the DeltaTimer
	void Reset ( void ); // Resets the DeltaTimer, everything = 0
	double GetTime ( void ); // Time in Seconds
	double GetMilliTime ( void ); // Time in Milliseconds
	double GetMicroTime ( void ); // Time in Microseconds
	bool IsActive ( void ); // Is DeltaTimer active?
    float GetDeltaTime();
}; // class DeltaTimer
 
#endif