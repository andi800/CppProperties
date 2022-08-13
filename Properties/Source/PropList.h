#pragma once

#include <string>
#include <any>
#include <memory>
#include <functional>
#include "UndoRedoManager.h"


namespace Properties
{


    template<typename T>
    class PropList : IUndoRedoProp
    {
    private:
        std::string _name = "";
        std::function<void(T)> _updateFunc = 0;
        std::vector<std::shared_ptr<T>> _list;
//        std::shared_ptr<T> _value;
        //std::shared_ptr<T> _defaultValue;
        UndoRedoManager* _undoRedoMan = 0;
        PropertyManager* _propertyManager = 0;

        enum ListOperations
        {
            OperationAppend, OperationRemove, OperationClear
        };


    public:
        explicit PropList(const std::string& name, UndoRedoManager* undoRedoMan, PropertyManager* propertyManager)
            : _name(name), _undoRedoMan(undoRedoMan), _propertyManager(propertyManager)
        {
            if (propertyManager != nullptr) {
                propertyManager->add(this);
            }
        }


        const std::vector<std::shared_ptr<T>>& get() const
        {
            return _list;
        }

        void setUpateFunction(std::function<void(T)> updateFunc) // TODO: better use shared_ptr<T> instead of T here as well? or const &
        {
            _updateFunc = updateFunc;
        }

        void append(std::shared_ptr<T> newValue)
        {
            if (_undoRedoMan != nullptr && _undoRedoMan->CurrentCommand != nullptr) {
                std::shared_ptr<Change> change = std::make_shared<Change>();
                change->NewObj = newValue;
                //change->OldObj = 0; // _value;
                //change->Type = typeid(T);
                change->Param = OperationAppend; // ListADD
                _undoRedoMan->CurrentCommand->add(this, change);
            }
            //_value = newValue;
            _list.push_back(newValue);
            if (_updateFunc && newValue) {
                _updateFunc(*std::static_pointer_cast<T>(newValue));
            }


            
        }

        void append(const T& newValue)
        {
            append(std::make_shared<T>(newValue));
        }

        void clear()
        {
            if (_undoRedoMan != nullptr && _undoRedoMan->CurrentCommand != nullptr) {
                std::shared_ptr<Change> change = std::make_shared<Change>();
                std::shared_ptr< std::vector<std::shared_ptr<T>>> backupList = std::make_shared<std::vector<std::shared_ptr<T>>>();
                //change->NewObj = backupList;
                change->OldObj = backupList; // _value;
                backupList->swap(_list); // this will automatically clear _list

                //change->Type = typeid(T);
                change->Param = OperationClear; // ListADD
                _undoRedoMan->CurrentCommand->add(this, change);
            }
            else 
            {
                _list.clear();
            }
            
            if (_updateFunc && newValue) {
                _updateFunc(*std::static_pointer_cast<T>(newValue));
            }

        }

        void remove(int index)
        {

        }

        void insert(const& T newValue, int index)
        {

        }


        /*
        T get()
        {
            return *std::static_pointer_cast<T>(_value);
        }

        void set(std::shared_ptr<T> newValue)
        {
            if (_undoRedoMan != nullptr && _undoRedoMan->CurrentCommand != nullptr) {
                std::shared_ptr<Change> change = std::make_shared<Change>();
                change->NewObj = newValue;
                change->OldObj = _value;
                //change->Type = typeid(T);
                _undoRedoMan->CurrentCommand->add(this, change);
            }
            _value = newValue;
            if (_updateFunc && newValue) {
                _updateFunc(*std::static_pointer_cast<T>(newValue));
            }
        }

        void set(const T& newValue)
        {
            set(std::make_shared<T>(newValue));
        }
        */


        void onUndo(std::shared_ptr<Change> change) override
        {
            switch (change->Param)
            {
            case OperationAppend:
                _list.pop_back();
                break;
            case OperationRemove:
                break;
            case OperationClear:
                std::shared_ptr< std::vector<std::shared_ptr<T>>> &backupList = change->OldObj;
                _list.swap(*_backupList);
                break;
            default:
                return;
            }
            //_value = std::static_pointer_cast<T>(change->OldObj); // TODO: check for cast errors, zero ptrs?
            if (_updateFunc) {
                _updateFunc(*std::static_pointer_cast<T>(_value));
            }
        }

        void onRedo(std::shared_ptr<Change> change) override
        {
            switch (change->Param)
            {
            case OperationAppend:
                _list.push_back(change->NewObj);
                break;
            case OperationRemove:
                break;
            case OperationClear:
                std::shared_ptr< std::vector<std::shared_ptr<T>>>& backupList = change->OldObj;
                _list.swap(*_backupList);
                break;
            default:
                return;
            }
            //_value = std::static_pointer_cast<T>(change->NewObj);
            if (_updateFunc) {
                _updateFunc(*std::static_pointer_cast<T>(_value));  // TODO: check for cast errors, zero ptrs?
            }
        }

        std::string getName() const override
        {
            return _name;
        }

        //std::string convertToJson(const T& value) const;


        std::string toJson() const override
        {
          /*  const auto ptr = _value.get();
            if (ptr != 0) {
                return convertToJson(*ptr);
            }*/
            return "";
        }

        void fromJson(std::string json) override
        {

        }

    };







}
