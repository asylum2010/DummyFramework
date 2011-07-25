//=============================================================================================================
#include <dummyframework/base/CGameLogicSynchronizer.h>
#include <cmath>

namespace DummyFramework
{
    CGameLogicSynchronizer::CGameLogicSynchronizer()
    {
        UpdateInterval = 0.1;

        newtime = 0;
        lasttime = 0;
        currenttime = 0;
        accumulator = 0;
        lastfps = 0;

        fps = 0;
        frames = 0;
    }
    //=============================================================================================================
    void CGameLogicSynchronizer::Reset()
    {
        currenttime = clock.Time();
        lastfps = lasttime = currenttime;
        accumulator = 0;
    }
    //=============================================================================================================
    void CGameLogicSynchronizer::Jump()
    {
        currenttime = 0;
        lastfps = lasttime = currenttime;
        accumulator = 0;
    }
    //=============================================================================================================
    void CGameLogicSynchronizer::Update()
    {
        if( clock.IsStopped() )
            return;

        newtime = clock.Time();
        double delta = newtime - currenttime;

        currenttime = newtime;
        accumulator += delta;

        if( newtime - lastfps >= 1.0 )
        {
            lastfps = newtime;
            fps = frames;
            frames = 0;
        }
        else
            ++frames;
        
        // max 3 updatet engedek egy ciklus alatt (pl. megakadt a merevlemez)
        size_t counter = 0;

        while( counter < 3 && accumulator >= UpdateInterval )
        {
            accumulator -= UpdateInterval;
            synchronize();
            ++counter;
        }

        if( accumulator >= UpdateInterval )
            accumulator = fmod(accumulator, UpdateInterval);
    }
    //=============================================================================================================
    bool CGameLogicSynchronizer::Elapsed(double interval, bool reset)
    {
        double delta = newtime - lasttime;

        if( delta >= interval )
        {
            if( reset )
                lasttime = newtime;

            return true;
        }

        return false;
    }
    //=============================================================================================================
}
