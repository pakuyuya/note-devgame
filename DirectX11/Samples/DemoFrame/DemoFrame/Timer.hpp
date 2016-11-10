#pragma once
#include <Windows.h>

struct MomentTimer {
    double time;         //<! �^�C�}�[�J�n����̌o�ߎ���
    double elapsedTime;  //<! �O��snap������̌o�ߎ���
};

class Timer {
private:
    LONGLONG m_ticksPerSec;  //<! �b������̎��g��
    double m_secPerTick;     //<! ���g��������̕b

    double m_startSystemTime;      // �^�C�}�[�̊�ƂȂ�J�n����
    double m_prevSnapedSystemTime; // �O��snap����������̌o�ߎ���

public:
    Timer() {
        // CPU�̎��g���擾
        LARGE_INTEGER qwTicksPerSec = { 0 };
        QueryPerformanceFrequency(&qwTicksPerSec);

        m_ticksPerSec = qwTicksPerSec.QuadPart;
        m_secPerTick = 1.0 / static_cast<double>(m_ticksPerSec);

        reset();
    }

public:
    /**
     * �^�C�}�[�����Z�b�g���܂�
     */
    void reset() {
        LARGE_INTEGER qwTime;
        QueryPerformanceCounter(&qwTime);

        double crtTime = static_cast<double>(qwTime.QuadPart) / m_secPerTick;
        m_prevSnapedSystemTime = m_prevSnapedSystemTime = crtTime;
    }

    /**
     * ���݂̃V�X�e�����Ԃ��擾���A���ʂ�ԋp���܂��B
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