#include "..\Headers\GfxStats.h"
#include "..\Headers\Timer.h"

//Default constructor.
Timer::Timer()
{
    QueryPerformanceFrequency( (LARGE_INTEGER *)&m_ticksPerSecond );
    m_currentTime = m_lastTime = m_lastFPSUpdate = 0;
    m_numFrames = 0;
    m_runningTime = m_timeElapsed = m_fps = 0.0f;
    m_FPSUpdateInterval = m_ticksPerSecond >> 1;
    m_timerStopped = TRUE;
}

//Starts the timer.
void Timer::Start()
{
    if ( !m_timerStopped )
    {
        // Already started
        return;
    }
    QueryPerformanceCounter( (LARGE_INTEGER *)&m_lastTime );
    m_timerStopped = FALSE;
}

//Stops the timer.
void Timer::Stop()
{
    if ( m_timerStopped )
    {
        // Already stopped
        return;
    }
    INT64 stopTime = 0;
    QueryPerformanceCounter( (LARGE_INTEGER *)&stopTime );
    m_runningTime += (float)(stopTime - m_lastTime) / (float)m_ticksPerSecond;
    m_timerStopped = TRUE;
}

//Updates the timer. Calculates the time elapsed since the last Update call.
//Updates the frames per second and updates the total running time.     
void Timer::Update()
{
    if ( m_timerStopped )
    {
        return;
    }

    // Get the current time
    QueryPerformanceCounter( (LARGE_INTEGER *)&m_currentTime );
    
    m_timeElapsed = (float)(m_currentTime - m_lastTime) / (float)m_ticksPerSecond;
    m_runningTime += m_timeElapsed;

    // Update FPS
    m_numFrames++;
    if ( m_currentTime - m_lastFPSUpdate >= m_FPSUpdateInterval )
    {
        float currentTime = (float)m_currentTime / (float)m_ticksPerSecond;
        float lastTime = (float)m_lastFPSUpdate / (float)m_ticksPerSecond;
        m_fps = (float)m_numFrames / (currentTime - lastTime);

        m_lastFPSUpdate = m_currentTime;
        m_numFrames = 0;
    }

    m_lastTime = m_currentTime;
}