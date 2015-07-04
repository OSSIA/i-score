#include "EventPresenter.hpp"

#include "Document/Event/EventModel.hpp"
#include "Document/Event/EventView.hpp"

#include <iscore/document/DocumentInterface.hpp>
#include <core/document/Document.hpp>

#include <QGraphicsScene>
EventPresenter::EventPresenter(const EventModel& model,
                               QGraphicsObject* parentview,
                               QObject* parent) :
    NamedObject {"EventPresenter", parent},
    m_model {model},
    m_view {new EventView{*this, parentview}},
    m_dispatcher{iscore::IDocument::documentFromObject(m_model)->commandStack()}
{
    // The scenario catches this :
    connect(&m_model.selection, &Selectable::changed,
            m_view, &EventView::setSelected);

    connect(&(m_model.metadata),  &ModelMetadata::colorChanged,
            m_view,                 &EventView::changeColor);

    qDebug() << "TODO: " << Q_FUNC_INFO;
    /*connect(&m_model, &EventModel::heightPercentageChanged,
            this,    &EventPresenter::heightPercentageChanged);
    */
    connect(&m_model, &EventModel::conditionChanged,
            m_view,  &EventView::setCondition);
    connect(&m_model, &EventModel::triggerChanged,
            this,   &EventPresenter::triggerSetted) ;

    connect(m_view, &EventView::eventHoverEnter,
            this,   &EventPresenter::eventHoverEnter);

    connect(m_view, &EventView::eventHoverLeave,
            this,   &EventPresenter::eventHoverLeave);

    connect(m_view, &EventView::dropReceived,
            this, &EventPresenter::handleDrop);

    m_view->setCondition(m_model.condition());
    m_view->setTrigger(m_model.trigger());
}

EventPresenter::~EventPresenter()
{
    if(m_view)
    {
        auto sc = m_view->scene();

        if(sc)
        {
            sc->removeItem(m_view);
        }

        m_view->deleteLater();
    }
}

const id_type<EventModel>& EventPresenter::id() const
{
    return m_model.id();
}

EventView* EventPresenter::view() const
{
    return m_view;
}

const EventModel& EventPresenter::model() const
{
    return m_model;
}

bool EventPresenter::isSelected() const
{
    return m_model.selection.get();
}

void EventPresenter::triggerSetted(QString trig)
{
    m_view->setTrigger(trig);
}

#include "Commands/Event/AddStateToEvent.hpp"
#include <QMimeData>
#include <QJsonDocument>
#include <iscore/document/DocumentInterface.hpp>
void EventPresenter::handleDrop(const QMimeData *mime)
{
    // If the mime data has states in it we can handle it.
    if(mime->formats().contains("application/x-iscore-state"))
    {
        Deserializer<JSONObject> deser{
            QJsonDocument::fromJson(mime->data("application/x-iscore-state")).object()};
        iscore::State s;
        deser.writeTo(s);

        auto cmd = new Scenario::Command::AddStateToEvent{
                iscore::IDocument::path(m_model),
                std::move(s)};
        m_dispatcher.submitCommand(cmd);
    }
}

