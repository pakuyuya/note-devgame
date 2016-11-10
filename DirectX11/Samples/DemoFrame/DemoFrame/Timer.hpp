#pragma once
#include <Windows.h>

struct MomentTimer {
    double time;         //<! タイマー開始からの経過時間
    double elapsedTime;  //<! 前回snap時からの経過時間
};

class Timer {
private:
    LONGLONG m_ticksPerSec;  //<! 秒あたりの周波数
    double m_secPerTick;     //<! 周波数あたりの秒

    double m_startSystemTime;      // タイマーの基準となる開始時間
    double m_prevSnapedSystemTime; // 前回snapした時からの経過時間

public:
    Timer() {
        // CPUの周波数取得
        LARGE_INTEGER qwTicksPerSec = { 0 };
        QueryPerformanceFrequency(&qwTicksPerSec);

        m_ticksPerSec = qwTicksPerSec.QuadPart;
        m_secPerTick = 1.0 / static_cast<double>(m_ticksPerSec);

        reset();
    }

public:
    /**
     * タイマーをリセットします
     */
    void reset() {
        LARGE_INTEGER qwTime;
        QueryPerformanceCounter(&qwTime);

        double crtTime = static_cast<double>(qwTime.QuadPart) / m_secPerTick;
        m_prevSnapedSystemTime = m_prevSnapedSystemTime = crtTime;
    }

    /**
     * 現在のシステム時間を取得し、結果を返却します。
     */
    MomentTimer snap() {
        LARGE_INTEGER qwTime;
        QueryPerformanceCounter(&qwTime);
        
        double crtTime = static_cast<double>(qwTime.QuadPart) / m_secPerTick;
        MomentTimer ret = {
            crtTime - m_startSystemTime,
            crtTime - m_prevSnapedSystemTime
        };

        m_prevSnapedSystemTime = crtTime;

        return ret;
    }
};