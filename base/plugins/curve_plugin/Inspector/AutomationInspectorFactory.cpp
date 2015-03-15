#include "AutomationInspectorFactory.hpp"
#include "AutomationInspectorWidget.hpp"
#include "../Automation/AutomationModel.hpp"

//using namespace iscore;

InspectorWidgetBase* AutomationInspectorFactory::makeWidget(QObject* sourceElement, QWidget* parent)
{
    return new AutomationInspectorWidget(static_cast<AutomationModel*>(sourceElement), parent);

}
