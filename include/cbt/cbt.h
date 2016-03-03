#ifndef CBT_H_
#define CBT_H_

#include <boost/bind.hpp>
#include <boost/coroutine/coroutine.hpp>
#include <initializer_list>
#include <vector>
#include <memory>

namespace cbt
{
    enum ActStatus
    {
        ACT_SUCCESS,
        ACT_FAILURE,
        ACT_RUNNING
    };

    typedef boost::coroutines::coroutine<ActStatus()> coroutine;
    typedef coroutine::caller_type yield_to;

    class Act
    {
        public:
            typedef std::shared_ptr<Act> shared_ptr;
            Act();
            Act(const std::vector<Act::shared_ptr>& childList);
            Act(std::initializer_list<Act::shared_ptr> explicitChildList);
            virtual ~Act();

            virtual void Initialize() = 0;
            virtual ActStatus Next() = 0;

            virtual void AddChild(shared_ptr child);

        protected:
            std::vector<shared_ptr> children;
    };

    class Leaf : public Act
    {
        public:
            typedef std::shared_ptr<Leaf> shared_ptr;
            Leaf();
            Leaf(const std::shared_ptr<coroutine>& fn);
            virtual ~Leaf();

            static Leaf::shared_ptr Create(std::shared_ptr<coroutine> fn);

            virtual void Initialize();
            virtual ActStatus Next();

            void SetFunc(const std::shared_ptr<coroutine>& fn);

        protected:
            std::shared_ptr<coroutine> func;
    };

    class Sequence : public Act
    {
        public:
            typedef std::shared_ptr<Sequence> shared_ptr;
            Sequence();
            Sequence(const std::vector<Act::shared_ptr>& childList);
            Sequence(std::initializer_list<Act::shared_ptr> explicitChildList);
            virtual void Initialize();
            virtual ActStatus Next();

        protected:
            size_t currentIdx;
            bool firstIter;
    };
}

#endif
