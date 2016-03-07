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
        // TODO: Reset the state of the coroutine!? Copy a coroutine!? 
        // apparently not possible in boost!
        
        // This is necessary because leaves have to be reset every time they
        // start.
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

    Select::Select(const std::vector<Act::shared_ptr>& childList) :
            Act(childList)
    {
        currentIdx = 0;
        firstIter = true;
    }

    Select::Select(std::initializer_list<Act::shared_ptr> explicitChildList) :
            Act(explicitChildList)
    {
        currentIdx = 0;
        firstIter = true;
    }

    Select::Select() :
            currentIdx(0), firstIter(true)
    {

    }

    void Select::Initialize()
    {

    }

    ActStatus Select::Next()
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
            if (currentIdx >= children.size() - 1)
                return ACT_FAILURE;
            else
            {
                currentIdx++;
                if (currentIdx < children.size())
                {
                    children.at(currentIdx)->Initialize();
                }
                return ACT_RUNNING;
            }
        }
        else if (childStatus == ACT_SUCCESS)
        {
            currentIdx++;
            return ACT_SUCCESS;
        }
        else
        {
            return ACT_RUNNING;
        }
    }


    Parallel::Parallel(const std::vector<Act::shared_ptr>& childList) :
            Act(childList)
    {
        currentIdx = 0;
    }

    Parallel::Parallel(std::initializer_list<Act::shared_ptr> explicitChildList) :
            Act(explicitChildList)
    {
        currentIdx = 0;
    }

    Parallel::Parallel() :
            currentIdx(0)
    {

    }

    void Parallel::Initialize()
    {
        for (size_t i = 0; i < children.size(); i++)
        {
            children.at(i)->Initialize();
        }
    }

    ActStatus Parallel::Next()
    {
        if (currentIdx >= children.size())
        {
            currentIdx = 0;
        }

        ActStatus childStatus = children.at(currentIdx)->Next();

        if (childStatus == ACT_FAILURE)
        {
            return ACT_FAILURE;
        }
        else if (childStatus == ACT_SUCCESS)
        {
            return ACT_SUCCESS;
        }
        else
        {
            currentIdx++;
            return ACT_RUNNING;
        }
    }


    Leaf::shared_ptr Leaf::Create(std::shared_ptr<coroutine> fn)
    {
        return std::make_shared<Leaf>(fn);
    }
}
