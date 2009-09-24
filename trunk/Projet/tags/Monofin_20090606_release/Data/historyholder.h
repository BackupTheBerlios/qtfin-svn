#ifndef HISTORYHOLDER_H
#define HISTORYHOLDER_H

#include <QStack>
#include <QList>
#include <QMap>
#include <iostream>

namespace Data{

    template <class Type>
    class HistoryHolder
    {
    private:
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
    };

    template <class Type>
    class HistoryMaker{
    protected:
        QList<HistoryHolder<Type> *> _makedHistory;
    public:

        virtual void startHistory(Type t) = 0;

        virtual ~HistoryMaker();

        virtual QList<HistoryHolder<Type> *> retrieveHistory(Type t) = 0;

        virtual void undo(QList<HistoryHolder<Type> *> history) = 0;

        virtual HistoryHolder<Type> * getCurrentHistoryHolder(Type t);
    };

    template <class Type>
    class HistoryCareTaker{
    protected:
        QList<HistoryMaker<Type> *> _historyMakers;
        QMap<Type,bool> _isStarted;
        QMap< Type, QStack< QList<HistoryHolder<Type> *> > > _passedHistory;
        QMap< Type, QStack< QList<HistoryHolder<Type> *> > > _futureHistory;
    public:

        virtual ~HistoryCareTaker();

        virtual void undo(Type t);

        virtual void redo(Type t);

        virtual void addHistoryMaker(HistoryMaker<Type> * hM);

        virtual int numberOfUndo(Type t);

        virtual int numberOfRedo(Type t);

        virtual void startHistory(Type t);

        virtual void stopHistory(Type t);

        virtual void dropLastHistory(Type t);

    protected:

        virtual void dropAllHistory();
    };

#include "historyholder.imp"

}

#endif // HISTORYHOLDER_H
