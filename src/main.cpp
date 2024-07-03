#include <cstdlib>
#include <iostream>
#include <utility>
#include <functional>
#include <thread>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

using boost::asio::ip::tcp;
#include "SensorMock.hpp"
#include "RegisterReader.hpp"

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    read_message();
  }

private:
  void read_message()
  {
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, buffer_, "\r\n",
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            std::istream is(&buffer_);
            std::string message(std::istreambuf_iterator<char>(is), {});
            std::cout << "Received: " << message << std::endl;
            write_message(message);
          }
        });
  }

  void write_message(const std::string& message)
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(message),
        [this, self, message](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            read_message();
          }
        });
  }

  tcp::socket socket_;
  boost::asio::streambuf buffer_;
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    accept();
  }

private:
  void accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket))->start();
          }

          accept();
        });
  }

  tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
    // if (argc != 2)
    // {
    //   std::cerr << "Usage: chat_server <port>\n";
    //   return 1;
    // }

    std::shared_ptr<std::mutex> sharedMutex;
    sharedMutex.reset(new std::mutex());
    std::shared_ptr<std::fstream> sharedFile;
    sharedFile.reset(new std::fstream("phonebook.dat", std::fstream::out | std::fstream::in | std::fstream::binary | std::fstream::app));
    if (!sharedFile->is_open())
    {
        exit(5);
    }

    SensorProject::SensorMock mockedSensor("a5f87451a5f87451a5f87451a5f87451", 1, 2, 2, sharedFile, sharedMutex);
    SensorProject::RegisterReader registerReader(sharedFile, sharedMutex, sharedFile->tellg(), "a5f87451a5f87451a5f87451a5f87451");
    std::thread* sensorThread;
    std::thread* bufferThread;
    sensorThread = new std::thread(std::bind(&SensorProject::SensorMock::SimSensor, &mockedSensor));
    bufferThread = new std::thread(std::bind(&SensorProject::RegisterReader::ReadRegisters, &registerReader));
    for(size_t i = 0; i < 50; ++i)
        mockedSensor.SimSensor();
    bufferThread->join();
    sensorThread->join();
    delete bufferThread;
    delete sensorThread;
    if (argc != 2)
    {
      std::cerr << "Usage: chat_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    server s(io_context, std::atoi(argv[1]));

    io_context.run();

    return 0;

}