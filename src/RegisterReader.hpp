#pragma once
#include "SensorMock.hpp"
#include <vector>

namespace SensorProject
{
    class RegisterReader
    {
    public:
        RegisterReader(std::shared_ptr<std::fstream> sharedFile, std::shared_ptr<std::mutex> sharedMutex, int startPos, std::string_view sensorID);
        ~RegisterReader() = default;

        void ReadRegisters();
    private:
        std::shared_ptr<std::fstream> m_SharedFile;
        std::shared_ptr<std::mutex> m_SharedMutex;
        int m_CurrentPos;
        std::vector<LogRecord> m_Records;
        std::string m_SensorID;
    };
}