#pragma once
#include <public_interface/command/SerializableCommand.hpp>
#include <public_interface/tools/ObjectPath.hpp>
#include <QList>
#include <QPair>

namespace iscore
{
    /**
    * @brief AggregateCommand: allows for grouping of multiple commands.
    */
    class AggregateCommand : public iscore::SerializableCommand
    {
        public:
            AggregateCommand(QString parname, QString cmdname, QString text) :
                iscore::SerializableCommand {parname, cmdname, text}
            {
            }

            template<typename T, typename... Args>
            AggregateCommand(QString parname, QString cmdname, QString text,
                             const T& cmd, Args&& ... remaining) :
                AggregateCommand {parname, cmdname, text, std::forward<Args> (remaining)...}
            {
                // Because they will be called in reverse order due to constructor
                // ordering, we push_front instead of push_back.
                m_serializedCommands.push_front({{cmd->parentName(), cmd->name() },
                    cmd->serialize()
                });
            }

            virtual void undo() override;
            virtual void redo() override;
            virtual bool mergeWith(const Command* other) override;

            void addCommand(const iscore::SerializableCommand* cmd)
            {
                m_serializedCommands.push_back({{cmd->parentName(), cmd->name() },
                    cmd->serialize()
                });
            }

        protected:
            virtual void serializeImpl(QDataStream&) const override;
            virtual void deserializeImpl(QDataStream&) override;

        private:
            // Meta-data : {{parent name, command name}, command data}
            QList<QPair<
            QPair<QString,
                  QString>,
                  QByteArray>> m_serializedCommands;
    };
}
