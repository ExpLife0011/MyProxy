
#pragma once
#include <string>

/// ��־���
class Logger {
public:

    /// �Ƿ����������̨������ʹ�� OutputDebugString WinAPI
    static bool CONSOLE;

    enum OutputLevel {
        OL_INFO, ///< �����ͨ��Ϣ�������Ϣ
        OL_ERROR, ///< ֻ���������Ϣ
    };

    static OutputLevel LEVEL;

    /// �����־
    /// 
    /// �����ɲ��� @a level �ṩ
    static void Log(const char *msg, OutputLevel level);
    static void Log(const std::string &msg, OutputLevel level) {
        Log(msg.c_str(), level);
    }

    /// �����ͨ��Ϣ
    static void LogInfo(const char *msg);
    static void LogInfo(const std::string &msg) {
        LogInfo(msg.c_str());
    }

    /// ���������Ϣ
    static void LogError(const char *msg);
    static void LogError(const std::string &msg) {
        LogError(msg.c_str());
    }
};
