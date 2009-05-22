#ifndef HISTORYHOLDER_H
#define HISTORYHOLDER_H

#include <QStack>
#include <QList>
#include <QMap>

namespace Data{

    template <class Type>
    class HistoryHolder
    {
    private:
        HistoryHolder * _next;
        HistoryHolder * _first;
        Type _t;

        struct AutomatedStorage{
            bool canBeDeleted;
            void * ptr;
            int size;
        };

        QStack<AutomatedStorage> _toStore;

    public:
        HistoryHolder(Type t);

        ~HistoryHolder();

        void pushInt(int data);

        template<typename PtrType>
        void pushPtr(PtrType * ptr);

        void * pop();

        int getNumberOfStoredElements();

        Type getType();

        HistoryHolder<Type> * createNewAction();

        void setNextAction(HistoryHolder<Type> *next);

        HistoryHolder<Type> * getNext();

        HistoryHolder<Type> * getFirst();

        static void freeStructure(HistoryHolder<Type> * toFree);

    private:
        void setFirst(HistoryHolder<Type>* first);

    };

    template <class Type>
    class HistoryMaker{
    protected:
        HistoryHolder<Type> * _makedHistory;
    public:

        HistoryMaker() : _makedHistory(NULL) {}

        virtual void startHistory(Type t) = 0;

        virtual ~HistoryMaker();

        virtual HistoryHolder<Type> * retrieveHistory(Type t) = 0;

        virtual void undo(HistoryHolder<Type> * history) = 0;

        virtual HistoryHolder<Type> * getCurrentHistoryHolder(Type t);
    };

    template <class Type>
    class HistoryCareTaker{
    protected:
        QList<HistoryMaker<Type> *> _historyMakers;
        QMap<Type,bool> _isStarted;
        QMap< Type, QStack<HistoryHolder<Type> *> *> _passedHistory;
        QMap< Type, QStack<HistoryHolder<Type> *> *> _futureHistory;

    public:

        virtual ~HistoryCareTaker();

        virtual void undo(Type t);

        virtual void redo(Type t);

        virtual void addHistoryMaker(HistoryMaker<Type> * hM);

        virtual int numberOfUndo(Type t);

        virtual int numberOfRedo(Type t);

        virtual void startHistory(Type t);

        virtual void stopHistory(Type t);
    };

#include "historyholder.imp"

}

#endif // HISTORYHOLDER_H
