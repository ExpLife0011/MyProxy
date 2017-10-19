
#pragma once
#include "Logger.hpp"
#include "ws-util.h"

#include <vector>
#include <map>
#include <atomic>
using namespace std;

/// �������
class MyProxy {
public:

    /// ���캯��
    MyProxy(SOCKET bsocket);

    /// ��������
    ~MyProxy();

    /// �������������������
    bool HandleBrowser();

    /// ��ӡ HTTP ͷ�ĵ�һ�У����� GET��POST ����Ϣ
    void PrintRequest(Logger::OutputLevel level) const;

    /// ͳ����Ϣ
    struct Statistics {
        /// �ܵ�������
        atomic_int requests;

        /// ���롢����ֽ���
        atomic_llong inBytes, outBytes;

        /// DNS ��ѯ��
        atomic_int dnsQueries;

        /// DNS ����������
        atomic_int dnsCacheHit;
    };

    /// ��ȡͳ����Ϣ
    static Statistics GetStatistics();

private:

    // ����������
    typedef vector<char> Buffer;

    // ��������
    void SplitHost(const string &host_decl, int default_port);

    // ��ת�������
    enum RelayResult {
        RR_ERROR, // �����˴���
        RR_CLOSE, // Զ�������ѹر�
        RR_ALIVE, // û�з����κδ���Զ��������Ȼ����
    };

    // ת����������
    RelayResult HandleServer();
    RelayResult DoHandleServer();

    // �Ͽ��������������
    bool ShutdownServerSocket();

    // ���ӵ�������
    bool SetUpServerSocket();

    // ����ʹ�� DNS ����� IP ��ַ���ӵ�������
    bool TryDNSCache();

    // ������������������� HTTP ͷ��
    bool SendBrowserHeaders();

    // ��ת SSL ����
    bool RelaySSLConnection();

    // �򵥡���е����ת
    // 
    // �� @a r ������ @a w д
    RelayResult SimpleRelay(SOCKET r, SOCKET w, Buffer &buf);

    // ת�������ʣ������ݸ�������
    bool RelayToServer();

    // ȡ�ط������Ļ�Ӧ�������
    bool RelayToBrowser();

    // ����ָ�� HTTP �ֶε�ʣ���ֽ���
    // 
    // @param buf ���ݻ�����
    // @param offset ��ʾ�ֶδ�С��ʮ�������ַ����ڻ������е�ƫ��
    // @return ����ʣ��δ�����ֽ�������Ϊ -1�����������˶�����
    int CountChunkRest(Buffer &buf, int offset);

    // ����� @a len �ֽڵ�����
    bool ReadMore(Buffer &buf, int len);

    // ��ȡ��ѵĻ�������С
    // 
    // @return ������ -1�����������˴���
    u_long GetBestBufferSize(SOCKET sd) const;

    // �� SOCKET д������
    RelayResult Write(SOCKET sd, const char *buf, size_t len);

private:

    void LogInfo(const string &msg) const;
    void LogError(const string &msg) const;

    void Log(const string &msg, Logger::OutputLevel level) const;

private:

    // ����������������İ������� HTTP ͷ����һ������
    // ���ܲ�����ֻ�� HTTP ͷ����Ϣ��
    Buffer m_vbuf;

    struct Host {
        void Clear() {
            this->name.clear();
            this->port = 0;
        }

        bool operator!=(const Host &other) {
            return this->name != other.name || this->port != other.port;
        }

        // ��ȡȫ�������϶˿ڣ�
        string GetFullName() const;

        string name;
        unsigned short port = 0;
    };

    Host m_host;

    // HTTP ͷ��
    struct Headers {
    public:

        // ����ͷ��
        // 
        // @param buf ���뱣֤�� 0 ��β
        // @param browser �Ƿ����������
        bool Parse(const char *buf, bool browser);

        // �������
        void Clear();

        // �Ƿ񱣳�����
        bool KeepAlive() const;

        // ����״̬��ȷ�������Ƿ���Ȼ����
        bool DetermineFinishedByStatusCode() const;

        // �Ƿ�ֶ�
        bool IsChunked() const;

    public:

        int status_code = 0;

        map<string, string> m;
        int bodyOffset = -1;
    };

    Headers m_headers;

    // ͳ����Ϣ
    static Statistics ms_stat;

    SOCKET m_bsocket;
    SOCKET m_ssocket;
};
