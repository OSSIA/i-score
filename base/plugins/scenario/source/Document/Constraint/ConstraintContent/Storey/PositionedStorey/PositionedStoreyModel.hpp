#pragma once
#include "Document/Constraint/ConstraintContent/Storey/StoreyModel.hpp"

class ConstraintContentModel;
class PositionedStoreyModel : public StoreyModel
{
		Q_OBJECT
		Q_PROPERTY(int position
				   READ position
				   WRITE setPosition
				   NOTIFY positionChanged)

		friend QDataStream& operator << (QDataStream& s, const PositionedStoreyModel& c);
		friend QDataStream& operator >> (QDataStream& s, PositionedStoreyModel& c);

	public:
		PositionedStoreyModel(QDataStream& s, ConstraintContentModel* parent);
		PositionedStoreyModel(int position, int id, ConstraintContentModel* parent);

		int position() const
		{
			return m_position;
		}

	signals:
		void positionChanged(int arg);

	public slots:
		void setPosition(int arg)
		{
			if (m_position == arg)
				return;

			m_position = arg;
			emit positionChanged(arg);
		}

	private:
		int m_position{};
};
