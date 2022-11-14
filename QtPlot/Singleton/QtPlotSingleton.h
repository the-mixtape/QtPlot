#pragma once

#include <QtGlobal>


template <class T>
class QtPlotSingleton {
public:
    QtPlotSingleton(T& rObject) {

        Q_ASSERT_X(!s_pInstance, "constructor", "Only one instance of this class is permitted.");
        s_pInstance = &rObject;
    }
    ~QtPlotSingleton() {
        Q_ASSERT_X(s_pInstance, "destructor", "The singleton instance is invalid.");
        s_pInstance = 0;
    }
    static T& instance() {
        if (!s_pInstance) {
            s_pInstance = new T();
        }
        //Q_ASSERT_X(s_pInstance, "instancing", "The singleton has not yet been created.");
        return (*s_pInstance);
    }
private:
    static T* s_pInstance;
    QtPlotSingleton(const QtPlotSingleton& Src);
    QtPlotSingleton& operator=(const QtPlotSingleton& Src);
};
template <class T> T* QtPlotSingleton<T>::s_pInstance = 0;