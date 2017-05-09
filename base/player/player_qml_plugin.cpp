#include "player_qml_plugin.hpp"
#include <ossia-qt/qml_plugin.hpp>
#include <ossia/context.hpp>
#include <QQmlEngine>
#include <spdlog/sinks/android_sink.h>
#include <ossia/detail/logger.hpp>

namespace iscore
{
//! A sink to use with spdlog, that will send its log messages through Qt logs.
struct qt_log_sink final : public spdlog::sinks::sink
{
  void log(const spdlog::details::log_msg& msg) override
  {
    QMessageLogger m;
    switch(msg.level)
    {
      case spdlog::level::info:
      case spdlog::level::trace:
      {
        m.info(msg.raw.c_str());
        break;
      }
      case spdlog::level::debug:
      {
        m.debug(msg.raw.c_str());
        break;
      }
      case spdlog::level::warn:
      {
        m.warning(msg.raw.c_str());
        break;
      }
      case spdlog::level::err:
      case spdlog::level::critical:
      {
        m.critical(msg.raw.c_str());
        break;
      }
    }
  }

  void flush() override
  {
  }
};


QMLPlayer::QMLPlayer()
{
  registerDevice(&ossia::qt::qml_singleton_device::instance());
}

QMLPlayer::~QMLPlayer()
{

}

int QMLPlayer::port() const
{
  return m_port;
}

void QMLPlayer::load(QString str)
{
  m_player.sig_loadFile(str);
}

void QMLPlayer::play()
{
  m_player.sig_play();
}

void QMLPlayer::stop()
{
  m_player.sig_stop();
}

void QMLPlayer::registerDevice(ossia::qt::qml_device* dev)
{
  if(dev)
    m_player.registerDevice(&dev->device());
}

void QMLPlayer::setPort(int p)
{
  if(m_port != p)
  {
    m_port = p;
    m_player.sig_setPort(p);
    emit portChanged(p);
  }
}


PlayerPlugin::PlayerPlugin()
{
}

void PlayerPlugin::registerTypes(const char* uri)
{
#if defined(__ANDROID__)
  ossia::context c{{std::make_shared<spdlog::sinks::android_sink>()}};
#else
  ossia::context c{{std::make_shared<qt_log_sink>()}};
#endif

  qmlRegisterType<iscore::QMLPlayer>(uri, 1, 0, "Player");
  ossia::qt::qml_plugin::reg(uri);
}

PlayerPlugin::~PlayerPlugin()
{

}

}
