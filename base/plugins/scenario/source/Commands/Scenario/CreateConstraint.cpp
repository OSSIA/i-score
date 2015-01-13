#include "CreateConstraint.hpp"

#include "Process/ScenarioProcessSharedModel.hpp"
#include "Document/Event/EventModel.hpp"
#include "Document/Constraint/ConstraintModel.hpp"
#include "Document/Event/EventData.hpp"
#include "Document/Constraint/Temporal/TemporalConstraintViewModel.hpp"
#include "Process/Temporal/TemporalScenarioProcessViewModel.hpp"

using namespace iscore;
using namespace Scenario::Command;


CreateConstraint::CreateConstraint():
	SerializableCommand{"ScenarioControl",
						"CreateEventAfterEvent",
						QObject::tr("Event creation")}
{
}

CreateConstraint::CreateConstraint(ObjectPath &&scenarioPath, int startEvent, int endEvent):
	SerializableCommand{"ScenarioControl",
						"CreateEventAfterEvent",
						QObject::tr("Event creation")},
	m_path{std::move(scenarioPath)},
    m_startEventId{startEvent},
    m_endEventId{endEvent}
{
	auto scenar = m_path.find<ScenarioProcessSharedModel>();
	m_createdConstraintId = getNextId(scenar->constraints());

	// For each ScenarioViewModel of the scenario we are applying this command in,
	// we have to generate ConstraintViewModels, too
	for(auto& viewModel : viewModels(scenar))
	{
		m_createdConstraintViewModelIDs[identifierOfViewModelFromSharedModel(viewModel)] = getNextId(viewModel->constraints());
	}
}

void CreateConstraint::undo()
{
	auto scenar = m_path.find<ScenarioProcessSharedModel>();

    scenar->undo_createConstraintBetweenEvent(m_createdConstraintId);
}

void CreateConstraint::redo()
{
	auto scenar = m_path.find<ScenarioProcessSharedModel>();

    scenar->createConstraintBetweenEvents(m_startEventId,
                                         m_endEventId,
                                         m_createdConstraintId);

	// Creation of all the constraint view models
	for(auto& viewModel : viewModels(scenar))
	{
		auto cvm_id = identifierOfViewModelFromSharedModel(viewModel);
		if(m_createdConstraintViewModelIDs.contains(cvm_id))
		{
			viewModel->makeConstraintViewModel(m_createdConstraintId,
											   m_createdConstraintViewModelIDs[cvm_id]);
		}
		else
		{
			throw std::runtime_error("CreateEvent : missing identifier.");
		}
	}

	// @todo Creation of all the event view models
}

int CreateConstraint::id() const
{
	return 1;
}

bool CreateConstraint::mergeWith(const QUndoCommand* other)
{
	return false;
}

void CreateConstraint::serializeImpl(QDataStream& s)
{
	s << m_path
      << m_startEventId
      << m_endEventId
	  << m_createdConstraintId
	  << m_createdConstraintViewModelIDs;
}

void CreateConstraint::deserializeImpl(QDataStream& s)
{
	s >> m_path
      >> m_startEventId
      >> m_endEventId
	  >> m_createdConstraintId
	  >> m_createdConstraintViewModelIDs;
}
