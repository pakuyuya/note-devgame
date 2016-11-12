#pragma once

#include <Windows.h>
#include <memory>
#include <vector>
#include "OnFrameExutable.hpp"

/**
 * OnFrameExcutableコンポーネントのリストに対するObserverです。
 */
class FrameExcutableObserver : OnFrameExcutable {
private:
    typedef std::shared_ptr<OnFrameExcutable> UNIT_T;
    std::vector<UNIT_T> m_units;

public:
    void add(OnFrameExcutable* ptr) {
        m_units.push_back(UNIT_T(ptr));
    }

    virtual HRESULT onUpdate(double time, double elapsedTime) override {
        for (UNIT_T unit : m_units) {
            HRESULT hr = unit->onUpdate(time, elapsedTime);
            if (FAILED(hr)) {
                return hr;
            }
        }
        return S_OK;
    }

    virtual HRESULT onFrameUpdate(double time, double elapsedTime) override {
        for (UNIT_T unit : m_units) {
            HRESULT hr = unit->onFrameUpdate(time, elapsedTime);
            if (FAILED(hr)) {
                return hr;
            }
        }
        return S_OK;
    }

    virtual HRESULT onRender(double time, double elapsedTime) override {
        for (UNIT_T unit : m_units) {
            HRESULT hr = unit->onRender(time, elapsedTime);
            if (FAILED(hr)) {
                return hr;
            }
        }
        return S_OK;
    }
};