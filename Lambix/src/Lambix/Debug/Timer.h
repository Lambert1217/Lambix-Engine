#pragma once 
#include <chrono>

namespace Lambix
{
    template<typename Fn>
    class Timer
    {
    public:
        Timer(const char* name, Fn&& func) : m_Name(name), m_Stopped(false), m_Func(func)
        {
            m_StartTimePoint = std::chrono::steady_clock::now();
        }
        ~Timer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimePoint = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch();

            float duration = elapsedTime.count() * 0.001f;
            m_Func({ m_Name, duration });

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        Fn m_Func;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
        bool m_Stopped;
    };
}
