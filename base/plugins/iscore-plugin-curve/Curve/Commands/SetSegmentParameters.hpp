#pragma once
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

class CurveModel;
class CurveSegmentModel;
using SegmentParameterMap = QMap<id_type<CurveSegmentModel>, QPair<double, double>>;
class SetSegmentParameters : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL("SetSegmentParameters", "SetSegmentParameters")
    public:
        ISCORE_SERIALIZABLE_COMMAND_DEFAULT_CTOR(SetSegmentParameters, "AutomationControl")
        SetSegmentParameters(ModelPath<CurveModel>&& model, SegmentParameterMap&& parameters);

        void undo() override;
        void redo() override;

        void update(ModelPath<CurveModel>&& model, SegmentParameterMap&&  segments);

    protected:
        void serializeImpl(QDataStream & s) const override;
        void deserializeImpl(QDataStream & s) override;

    private:
        ModelPath<CurveModel> m_model;
        SegmentParameterMap m_new;
        QMap<
            id_type<CurveSegmentModel>,
            QPair<
                boost::optional<double>,
                boost::optional<double>
            >
        > m_old;
};
