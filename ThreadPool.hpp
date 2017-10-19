
#pragma once
#include <Windows.h>

/// �̳߳�
/// 
/// Windows Ϊÿ���߳�Ĭ�ϴ������̳߳��ƺ���֧��������С�߳���
class ThreadPool {
public:

    /// ���캯��
    ThreadPool();

    /// ��������
    ~ThreadPool();

public:

    /// �̳߳��Ƿ����
    bool IsOk() const {
        return m_handle != nullptr;
    }

    /// ����һ������
    bool CreateWork(PTP_SIMPLE_CALLBACK pfns, void *pv);

    /// ������С�߳���
    bool SetThreadMinimum(int minimun);

    /// ��������߳���
    void SetThreadMaximum(int maximun);

private:

    PTP_POOL m_handle;
    TP_CALLBACK_ENVIRON m_env;
};
