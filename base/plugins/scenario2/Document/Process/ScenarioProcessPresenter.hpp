#pragma once
#include <interface/process/ProcessPresenterInterface.hpp>
namespace iscore
{
	class ProcessViewModelInterface;
	class ProcessViewInterface;
}
class IntervalPresenter;
class EventPresenter;
class ScenarioProcessViewModel;
class ScenarioProcessView;
class EventModel;
class IntervalModel;

class ScenarioProcessPresenter : public iscore::ProcessPresenterInterface
{
	Q_OBJECT

	public:
		ScenarioProcessPresenter(iscore::ProcessViewModelInterface* model,
								 iscore::ProcessViewInterface* view,
								 QObject* parent);
		virtual ~ScenarioProcessPresenter() = default;


	public slots:
		void on_eventCreated(int eventId);
		void on_intervalCreated(int intervalId);
		void on_eventDeleted(int eventId);
		void on_intervalDeleted(int intervalId);

	private:

		void on_eventCreated_sub(EventModel* event_model);
		void on_intervalCreated_sub(IntervalModel* interval_model);
		ScenarioProcessViewModel* m_model;
		ScenarioProcessView* m_view;

		std::vector<IntervalPresenter*> m_intervals;
		std::vector<EventPresenter*> m_events;
};

