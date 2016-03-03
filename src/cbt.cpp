#include <cbt/cbt.h>

namespace cbt
{
    Act::Act()
    {

    }

    Act::~Act()
    {

    }

    void Act::AddChild(shared_ptr child)
    {
        children.push_back(child);
    }

    Leaf::Leaf()
    {

    }

    Leaf::Leaf(const std::shared_ptr<coroutine>& fn) :
            func(fn)
    {

    }

    Leaf::~Leaf()
    {

    }

    void Leaf::Initialize()
    {

    }

    void Leaf::SetFunc(const std::shared_ptr<coroutine>& fn)
    {
        func = fn;
    }

    ActStatus Leaf::Next()
    {
        if (!func.get())
        {
            return ACT_FAILURE;
        }

        (*func)();
        if (*func)
        {
            return func->get();
        }
        else
        {
            return ACT_SUCCESS;
        }
    }

    Sequence::Sequence() :
            currentIdx(0), firstIter(true)
    {

    }

    void Sequence::Initialize()
    {

    }

    ActStatus Sequence::Next()
    {
        if (currentIdx >= children.size())
        {
            return ACT_SUCCESS;
        }

        if (firstIter)
        {
            children.at(currentIdx)->Initialize();
            firstIter = false;
        }

        ActStatus childStatus = children.at(currentIdx)->Next();

        if (childStatus == ACT_FAILURE)
        {
            return ACT_FAILURE;
        }
        else if (childStatus == ACT_SUCCESS)
        {
            currentIdx++;
            if (currentIdx < children.size())
            {
                children.at(currentIdx)->Initialize();
            }
            return ACT_RUNNING;
        }
        else
        {
            return ACT_RUNNING;
        }
    }

    Act::Act(const std::vector<Act::shared_ptr>& childList) :
            children(childList)
    {

    }

    Act::Act(std::initializer_list<Act::shared_ptr> explicitChildList)
    {
        children.insert(children.end(), explicitChildList.begin(), explicitChildList.end());
    }

    Sequence::Sequence(const std::vector<Act::shared_ptr>& childList) :
            Act(childList)
    {
        currentIdx = 0;
        firstIter = true;
    }

    Sequence::Sequence(std::initializer_list<Act::shared_ptr> explicitChildList) :
            Act(explicitChildList)
    {
        currentIdx = 0;
        firstIter = true;
    }

    Leaf::shared_ptr Leaf::Create(std::shared_ptr<coroutine> fn)
    {
        return std::make_shared<Leaf>(fn);
    }
}
