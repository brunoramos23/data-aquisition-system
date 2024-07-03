#include "RegisterReader.hpp"
#include <iostream>

SensorProject::RegisterReader::RegisterReader(std::shared_ptr<std::fstream> sharedFile, std::shared_ptr<std::mutex> sharedMutex, int startPos, std::string_view sensorID) :
    m_SharedFile(sharedFile), m_SharedMutex(sharedMutex), m_CurrentPos(startPos), m_SensorID(sensorID)
{

}

void SensorProject::RegisterReader::ReadRegisters()
{
    SensorProject::LogRecord logRecord(m_SensorID);
    std::string SensorID;
    char sensorIDRead[33];
    sensorIDRead[32] = '\0';
    
    static int filePos = m_CurrentPos;
    while(true)
    {
        auto recordStart = std::chrono::steady_clock::now();
        std::cout << "Entered loop\n";
        m_SharedMutex->lock();
        int finalPos = m_SharedFile->tellp();
        m_SharedFile->seekg(m_CurrentPos);
        while(finalPos != m_CurrentPos)
        {
          m_SharedFile->read((char*)sensorIDRead, 32);
          SensorID = sensorIDRead;
          if(SensorID.compare(logRecord.sensor_id) != 0)
          {
              break;
          }
          m_SharedFile->read((char *)&logRecord.timestamp, sizeof(std::chrono::steady_clock::time_point));
          m_SharedFile->write((char *)&logRecord.value, sizeof(double));
          m_CurrentPos += 32 + sizeof(std::chrono::steady_clock::time_point) + sizeof(double);
          
          std::cout << "ID: " << logRecord.sensor_id << " value: " << logRecord.value << "\n";
        }
        m_SharedMutex->unlock();
        auto recordEnd = std::chrono::steady_clock::now();
        while(std::chrono::duration_cast<std::chrono::milliseconds>(recordEnd - recordStart).count() < (double)(5000))
            recordEnd = std::chrono::steady_clock::now();
        std::cout << "In/Out\n";
        std::cout << m_SharedFile->tellg() << "\n";
        std::cout << m_SharedFile->tellp() << "\n";
    }
}