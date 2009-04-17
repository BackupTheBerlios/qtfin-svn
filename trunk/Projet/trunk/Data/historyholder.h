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
        QStack<void *> _toStore;
    public:
        HistoryHolder(Type t): _next(NULL), _t(t) {
            _first=this;
        }

        void push(void * data){
            _toStore.push(data);
        }

        void * pop(){
            if(!_toStore.isEmpty())
                return _toStore.pop();
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
        QMap< Type, QStack<HistoryHolder<Type> *> *> _passedHistory;
        QMap< Type, QStack<HistoryHolder<Type> *> *> _futureHistory;

    public:

        virtual ~HistoryTakeCarer() {
            foreach(Type key,_passedHistory.keys())
                delete _passedHistory.value(key);

            foreach(Type key,_futureHistory.keys())
                delete _futureHistory.value(key);
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
            if(_futureHistory.contains(t))
                _futureHistory.value(t)->clear();

            foreach(HistoryMaker<Type> * hM, _historyMakers){
                hM->startHistory(t);
            }
        }

        virtual void stopHistory(Type t){
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
        }
    };

}

#endif // HISTORYHOLDER_H
