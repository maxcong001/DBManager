#include "singleton.hpp"

template <class T>
T *Singleton<T>::m_pInstance = nullptr;
template <class T>
std::mutex Singleton<T>::mtx;