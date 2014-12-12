#include "MoveConstraintCommand.hpp"

#include "Process/ScenarioProcessSharedModel.hpp"
#include "Document/Constraint/ConstraintModel.hpp"

#include <core/application/Application.hpp>
#include <core/view/View.hpp>

#include <QApplication>

using namespace iscore;

// @todo : maybe should we use deplacement value and not absolute ending point.

MoveConstraintCommand::MoveConstraintCommand(ObjectPath &&scenarioPath, int constraintId, int endEvent, double deltaHeight):
	SerializableCommand{"ScenarioControl",
						"MoveEventCommand",
						QObject::tr("Constraint move")},
	m_scenarioPath(std::move(scenarioPath)),
	m_constraintId{constraintId},
	m_endEventId{endEvent},
    m_deltaHeight{deltaHeight}
{

}

void MoveConstraintCommand::undo()
{
	auto scenar = static_cast<ScenarioProcessSharedModel*>(m_scenarioPath.find());
	if(scenar != nullptr)
	{
        scenar->moveConstraint(m_constraintId, m_oldHeightPosition);
	}
}

void MoveConstraintCommand::redo()
{
	auto scenar = static_cast<ScenarioProcessSharedModel*>(m_scenarioPath.find());
	if(scenar != nullptr)
	{
		m_oldHeightPosition = scenar->constraint(m_constraintId)->heightPercentage();
//        m_heightPosition = m_oldHeightPosition + m_deltaHeight;
        scenar->moveConstraint(m_constraintId, m_oldHeightPosition + m_deltaHeight);
	}
}

int MoveConstraintCommand::id() const
{
	return 1;
}

bool MoveConstraintCommand::mergeWith(const QUndoCommand* other)
{
	return false;
}

void MoveConstraintCommand::serializeImpl(QDataStream& s)
{
    s << m_scenarioPath << m_constraintId << m_endEventId << m_deltaHeight;
}

void MoveConstraintCommand::deserializeImpl(QDataStream& s)
{
    s >> m_scenarioPath >> m_constraintId >> m_endEventId >> m_deltaHeight;
}
