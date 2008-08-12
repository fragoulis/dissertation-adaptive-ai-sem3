#include "DeltaTimer.h"

DeltaTimer::DeltaTimer( void )
{
	this->active = false; // preset
	this->Reset(); // reset / populate the values
	return;
}; // ctor
 
/*************************************************/
 
void DeltaTimer::Start ( void )
{
	this->active = true;
	QueryPerformanceCounter(&this->startCount);
	return;
}; // Start
 
/*************************************************/
 
void DeltaTimer::Stop ( void )
{
	this->active = false;
	QueryPerformanceCounter(&this->endCount);
	return;
}; // Stop
 
/*************************************************/
 
void DeltaTimer::Reset ( void )
{
	if ( this->active ) this->Stop(); // we need to stop
	QueryPerformanceFrequency(&this->frequency); // poll freq
	{
		this->startCount.QuadPart =
		this->endCount.QuadPart	 = 0;
		this->startTime =
		this->endTime = 0;
	}
	this->active = false;
	return;
}; // Reset
 
/*************************************************/
 
bool DeltaTimer::IsActive( void )
{
	return this->active;
}; // IsActive
 
/*************************************************/
 
double DeltaTimer::GetMicroTime( void )
{
	if(this->active) QueryPerformanceCounter(&endCount);
 
	this->startTime = 
		double (
			this->startCount.QuadPart * 
			( 1000000.0 / this->frequency.QuadPart )
		);
 
	this->endTime =
		double (
			this->endCount.QuadPart * 
			( 1000000.0 / this->frequency.QuadPart )
		);
 
	return double( this->endTime - this->startTime );
}; // GetTime
 
/*************************************************/
 
double DeltaTimer::GetMilliTime( void )
{
	return double( this->GetMicroTime() * 0.001 );
}; // GetMilliTime
 
/*************************************************/
 
double DeltaTimer::GetTime( void )
{
	return double( this->GetMicroTime() * 0.000001);
}; // GetTime

/*************************************************/

float DeltaTimer::GetDeltaTime()
{
    static double before = GetTime();
    double now = GetTime();
    double dt = now - before;
    before = now;

    //if( dt < 0.016 ) return 0.016f;
    return float(dt);
}