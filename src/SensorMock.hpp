#pragma once
#include <chrono>
#include <string>
#include <fstream>
#include <mutex>
#include <memory>

namespace SensorProject
{
    struct LogRecord {
        const std::string sensor_id; // supondo um ID de sensor de até 32 caracteres
        std::chrono::steady_clock::time_point timestamp; // timestamp UNIX
        double value; // valor da leitura
        LogRecord(std::string_view id):
            sensor_id(id)
        {

        }
    };

    class SensorMock
    {
    public:
        SensorMock(std::string_view sensorID, size_t sensorPause, double sensorRange, double sensorOffset, std::shared_ptr<std::fstream> logFile, std::shared_ptr<std::mutex> sharedMutex);
        ~SensorMock() = default;

        void SimSensor();
    private:

        LogRecord m_CurrentLog;
        size_t m_SensorPause;
        std::shared_ptr<std::fstream> m_Logger;
        std::shared_ptr<std::mutex> m_SharedMutex;
        double m_SensorAmplitude;
        double m_SensorOffset;
        void GenRandomLog();
        void RecordLog();
    };
}