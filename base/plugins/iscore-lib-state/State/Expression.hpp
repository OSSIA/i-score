#pragma once
#include <State/Relation.hpp>
#include <iscore/tools/TreeNode.hpp>
#include <iscore/tools/VariantBasedNode.hpp>
#include <boost/optional.hpp>
namespace iscore
{
enum class BinaryOperator {
    And, Or, Xor
};
enum class UnaryOperator {
    Not
};

struct ExprData : public VariantBasedNode<Relation, BinaryOperator, UnaryOperator>
{
        ISCORE_SERIALIZE_FRIENDS(ExprData, DataStream)
        ISCORE_SERIALIZE_FRIENDS(ExprData, JSONObject)

        ExprData() = default;
        template<typename T>
        ExprData(const T& data):
            VariantBasedNode{data}
        {

        }

        friend bool operator==(const ExprData& lhs, const ExprData& rhs)
        {
            return lhs.m_data == rhs.m_data;
        }

        QString toString() const;
};

}
using iscore::ExprData;

template<>
/**
 * @brief The TreeNode<ExprData> class
 *
 * This class is specialized from TreeNode<T>
 * because we want to have an additional check :
 * a node is a leaf iff a node is a iscore::Relation
 */
class TreeNode<ExprData> : public ExprData
{
        ISCORE_SERIALIZE_FRIENDS(TreeNode<ExprData>, DataStream)
        ISCORE_SERIALIZE_FRIENDS(TreeNode<ExprData>, JSONObject)

        friend bool operator!=(const TreeNode<ExprData>& lhs, const TreeNode<ExprData>& rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator==(const TreeNode<ExprData>& lhs, const TreeNode<ExprData>& rhs)
        {
            const auto& ltd = static_cast<const ExprData&>(lhs);
            const auto& rtd = static_cast<const ExprData&>(rhs);

            bool b = (ltd == rtd) && (lhs.m_children.count() == rhs.m_children.count());
            if(!b)
                return false;

            for(int i = 0; i < lhs.m_children.count(); i++)
            {
                if(*lhs.m_children[i] != *rhs.m_children[i])
                    return false;
            }

            return true;
        }

    public:
        TreeNode();
        TreeNode(const ExprData& data);
        TreeNode(ExprData&& data);

        template<typename T>
        TreeNode(const T& data, TreeNode<ExprData> * parent):
            ExprData(data),
            m_parent{parent}
        {
            if(m_parent) {
                m_parent->addChild(this);
            }
        }

        QString toString() const;

        // Clone
        TreeNode(const TreeNode<ExprData>& source,
                 TreeNode<ExprData>* parent = nullptr);

        TreeNode& operator=(const TreeNode<ExprData>& source);
        ~TreeNode();

        void setParent(TreeNode<ExprData>* parent);
        TreeNode<ExprData>* parent() const;

        // returns 0 if invalid index
        TreeNode<ExprData>* childAt(int index) const
        { return m_children.value(index); }

        // returns -1 if not found
        int indexOfChild(const TreeNode<ExprData>* child) const
        { return m_children.indexOf(const_cast<TreeNode<ExprData>*>(child)); }

        int childCount() const
        { return m_children.count(); }

        bool hasChildren() const
        { return ! m_children.empty(); }

        QList<TreeNode<ExprData>*> children() const
        { return m_children;  }

        void insertChild(int index, TreeNode<ExprData>* n);
        void addChild(TreeNode<ExprData>* n);
        void swapChildren(int oldIndex, int newIndex);
        TreeNode<ExprData>* takeChild(int index);

        // Won't delete the child!
        void removeChild(TreeNode<ExprData>* child);

    protected:
        TreeNode<ExprData>* m_parent {};
        QList<TreeNode<ExprData>*> m_children;
};

namespace iscore
{
using Expression = TreeNode<ExprData>;
using Condition = Expression;
using Trigger = Expression;

boost::optional<iscore::Expression> parse(const QString& str);
}
