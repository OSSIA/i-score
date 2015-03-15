#pragma once
#include <QObject>
#include <Inspector/InspectorWidgetFactoryInterface.hpp>


class EventInspectorFactory : public InspectorWidgetFactoryInterface
{
    public:
        EventInspectorFactory() :
            InspectorWidgetFactoryInterface {}
        {

        }

        virtual InspectorWidgetBase* makeWidget(QObject* sourceElement, QWidget* parent) override;

        virtual QList<QString> correspondingObjectsNames() const override
        {
            return {"EventModel"};
        }
};
