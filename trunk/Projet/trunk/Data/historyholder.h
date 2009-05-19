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
        HistoryHolder(Type t): _next(NULL), _t(t) {
            _first=this;
        }

        ~HistoryHolder(){
            while(!_toStore.isEmpty()){
                AutomatedStorage toDelete = _toStore.pop();
                if(toDelete.canBeDeleted){
                    operator delete(toDelete.ptr);
                }
            }
        }

        void pushInt(int data){
            AutomatedStorage toPush;
            toPush.canBeDeleted = false;
            toPush.ptr = (void*) data;
            toPush.size = 0;
            _toStore.push(toPush);
        }

        template<typename PtrType>
        void pushPtr(PtrType * ptr){
            AutomatedStorage toPush;
            toPush.canBeDeleted = true;
            toPush.ptr = (void *) ptr;
            toPush.size = sizeof(PtrType);
            _toStore.push(toPush);
        }

        void * pop(){
            if(!_toStore.isEmpty())
                return _toStore.pop().ptr;
            else
                return NULL;
        }

        int getNumberOfStoredElements(){
            return _toStore.size();
        }

        Type getType(){
            return _t;
        }

        HistoryHolder<Type> * createNewAction(){
            HistoryHolder<Type> * hh = new HistoryHolder<Type>(_t);
            hh->setNextAction(this);
            hh->setFirst(_first);
            return hh;
        }

        void setNextAction(HistoryHolder<Type> *next){
            _next = next;
        }

        HistoryHolder<Type> * getNext(){
            return _next;
        }

        HistoryHolder<Type> * getFirst(){
            return _first;
        }

        static void freeStructure(HistoryHolder<Type> * toFree){
            if(toFree != NULL){
                HistoryHolder<Type> * current = toFree;
                while(current!=NULL){
                    HistoryHolder<Type> * next = current->getNext();
                    delete current;
                    current = next;
                }
            }
        }

    private:
        void setFirst(HistoryHolder<Type>* first){
            _first = first;
        }

    };

    template <class Type>
    class HistoryMaker{
    protected:
        HistoryHolder<Type> * _makedHistory;
    public:

        virtual void startHistory(Type t) = 0;

        virtual ~HistoryMaker() {
            if(_makedHistory != NULL)
                HistoryHolder<Type>::freeStructure(_makedHistory);
        }

        virtual HistoryHolder<Type> * retrieveHistory(Type t) = 0;

        virtual void undo(HistoryHolder<Type> * history) = 0;

        virtual HistoryHolder<Type> * getCurrentHistoryHolder(Type t){
            if (_makedHistory == NULL)
                _makedHistory = new HistoryHolder<Type>(t);
            else
                _makedHistory = _makedHistory->createNewAction();

            return _makedHistory;
        }
    };

    template <class Type>
    class HistoryTakeCarer{
    protected:
        QList<HistoryMaker<Type> *> _historyMakers;
        QMap<Type,bool> _isStarted;
        QMap< Type, QStack<HistoryHolder<Type> *> *> _passedHistory;
        QMap< Type, QStack<HistoryHolder<Type> *> *> _futureHistory;

    public:

        virtual ~HistoryTakeCarer() {
            // if historic are started but not ended we must stop them
            // in order to de-allocate them correctly
            foreach(Type key,_isStarted.keys())
                if(_isStarted[key])
                    stopHistory(key);

            //de-allocation of all HistoryHolders.
            foreach(Type key,_passedHistory.keys())
                while(!_passedHistory.value(key)->isEmpty())
                    HistoryHolder<Type>::freeStructure(_passedHistory.value(key)->pop());

            foreach(Type key,_futureHistory.keys())
                while(!_passedHistory.value(key)->isEmpty())
                    HistoryHolder<Type>::freeStructure(_futureHistory.value(key)->pop());
        }

        virtual void undo(Type t){
            if(_passedHistory.contains(t))
                if(_passedHistory.value(t)->size()!=0){
                    HistoryHolder<Type> * toUndo = (_passedHistory.value(t))->pop();
                    HistoryHolder<Type> * globalHistory=NULL;
                    foreach(HistoryMaker<Type> * hM, _historyMakers){
                        hM->startHistory(t);
                        hM->undo(toUndo);
                        HistoryHolder<Type> * localHistory;
                        if( (localHistory = hM->retrieveHistory(t)) != NULL){
                            if (globalHistory==NULL){
                                globalHistory = localHistory;
                            }else{
                                globalHistory->getFirst()->setNextAction(localHistory);
                            }
                        }
                    }
                    if(_futureHistory.contains(t)){
                        _futureHistory.value(t)->push_back(globalHistory);
                    }else{
                        QStack<HistoryHolder<Type> *> * toFill = new QStack<HistoryHolder<Type> *>();
                        toFill->push(globalHistory);
                        _futureHistory.insert(t,toFill);
                    }
                }
        }

        virtual void redo(Type t){
            if(_futureHistory.contains(t))
                if(_futureHistory.value(t)->size()!=0){
                    HistoryHolder<Type> * toRedo = (_futureHistory.value(t))->pop();
                    HistoryHolder<Type> * globalHistory=NULL;
                    foreach(HistoryMaker<Type> * hM, _historyMakers){
                        hM->startHistory(t);
                        hM->undo(toRedo);
                        HistoryHolder<Type> * localHistory;
                        if( (localHistory = hM->retrieveHistory(t)) != NULL){
                            if (globalHistory==NULL){
                                globalHistory = localHistory;
                            }else{
                                globalHistory->getFirst()->setNextAction(localHistory);
                            }
                        }
                    }
                    if(_passedHistory.contains(t)){
                        _passedHistory.value(t)->push_back(globalHistory);
                    }else{
                        QStack<HistoryHolder<Type> *> * toFill = new QStack<HistoryHolder<Type> *>();
                        toFill->push(globalHistory);
                        _passedHistory.insert(t,toFill);
                    }
                }
        }

        virtual void addHistoryMaker(HistoryMaker<Type> * hM){
            _historyMakers.append(hM);
        }

        virtual int numberOfUndo(Type t){
            if(_passedHistory.contains(t))
                return _passedHistory.value(t)->size();
            else
                return 0;
        }

        virtual int numberOfRedo(Type t){
            if(_futureHistory.contains(t))
                return _futureHistory.value(t)->size();
            else
                return 0;
        }

        virtual void startHistory(Type t){

            if(_isStarted[t])
                return;

            _isStarted[t]=true;

            if(_futureHistory.contains(t)){
                _futureHistory.value(t)->clear();
            }

            foreach(HistoryMaker<Type> * hM, _historyMakers){
                hM->startHistory(t);
            }
        }

        virtual void stopHistory(Type t){

            if(!_isStarted[t])
                return;

            HistoryHolder<Type> * globalHistory = NULL;

            foreach(HistoryMaker<Type> * hM, _historyMakers){
                HistoryHolder<Type> * localHistory;
                if( (localHistory = hM->retrieveHistory(t)) != NULL){
                    if (globalHistory==NULL){
                        globalHistory = localHistory;
                    }else{
                        globalHistory->getFirst()->setNextAction(localHistory);
                    }
                }
            }

            if(_passedHistory.contains(t)){
                _passedHistory.value(t)->push_back(globalHistory);
            }else{
                QStack<HistoryHolder<Type> *> * toFill = new QStack<HistoryHolder<Type> *>();
                toFill->push(globalHistory);
                _passedHistory.insert(t,toFill);
            }
            _isStarted[t] = false;
        }
    };

}

#endif // HISTORYHOLDER_H
