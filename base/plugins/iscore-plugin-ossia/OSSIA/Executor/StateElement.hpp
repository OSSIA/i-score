#pragma once
#include <QObject>
#include <memory>
#include <iscore_plugin_ossia_export.h>

class DeviceList;
class StateModel;

namespace OSSIA
{
    class State;
}

namespace RecreateOnPlay
{
class ISCORE_PLUGIN_OSSIA_EXPORT StateElement final : public QObject
{
    public:
        StateElement(
                const StateModel& element,
                std::shared_ptr<OSSIA::State> root,
                const DeviceList& deviceList,
                QObject* parent);

        const StateModel& iscoreState() const;
        std::shared_ptr<OSSIA::State> OSSIAState() const
        { return m_ossia_state; }

    private:
        void on_stateUpdated();

        const StateModel& m_iscore_state;
        std::shared_ptr<OSSIA::State> m_ossia_state;

        const DeviceList& m_deviceList;
};
}
