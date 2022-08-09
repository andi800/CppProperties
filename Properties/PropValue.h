#pragma once

#include <string>
#include <any>
#include <memory>
#include <functional>
#include "UndoRedoManager.h"


namespace Properties
{



    template<typename T>
    class PropValue : IUndoRedoProp
    {
    private:
        std::string _name = "";
        std::function<void(T)> _updateFunc = 0;
        std::shared_ptr<T> _value;
        std::shared_ptr<T> _defaultValue;
        UndoRedoManager& _undoRedoMan;

    public:
        explicit PropValue(const std::string& name, std::shared_ptr<T> value, UndoRedoManager& undoRedoMan) : _name(name), _value(value), _defaultValue(value), _undoRedoMan(undoRedoMan)
        {

        }

        explicit PropValue(const std::string& name, T value, UndoRedoManager& undoRedoMan) : PropValue(name, std::make_shared<T>(value), undoRedoMan)
        {

        }


        void setUpateFunction(std::function<void(T)> updateFunc) // TODO: better use shared_ptr<T> instead of T here as well? or const &
        {
            _updateFunc = updateFunc;
        }

        T get()
        {
            return *std::static_pointer_cast<T>(_value);
        }

        void set(std::shared_ptr<T> newValue)
        {
            if (_undoRedoMan.CurrentCommand != nullptr) {
                std::shared_ptr<Change> change = std::make_shared<Change>();
                change->NewObj = newValue;
                change->OldObj = _value;
                //change->Type = typeid(T);
                _undoRedoMan.CurrentCommand->add(this, change);
            }
            _value = newValue;
            if (_updateFunc && newValue) {
                _updateFunc(*std::static_pointer_cast<T>(newValue));
            }
        }

        void set(T newValue)
        {
            set(std::make_shared<T>(newValue));
        }


        // like set, but does not use undoredo manager
        /*void load(T newValue)
        {
            _value = newValue;
            if (_updateFunc) {
                _updateFunc(*std::static_pointer_cast<T>(newValue));
            }
        }*/

        void onUndo(std::shared_ptr<Change> change) override
        {
            _value = std::static_pointer_cast<T>(change->OldObj); // TODO: check for cast errors, zero ptrs?
            _updateFunc(*std::static_pointer_cast<T>(_value));
        }

        void onRedo(std::shared_ptr<Change> change) override
        {
            _value = std::static_pointer_cast<T>(change->NewObj);
            _updateFunc(*std::static_pointer_cast<T>(_value));  // TODO: check for cast errors, zero ptrs?
        }

        std::string getName() const override
        {
            return _name;
        }

        std::string convertToJson(const T& value);


        std::string toJson()
        {
            const auto ptr = _value.get();
            if (ptr != 0) {
                return convertToJson(*ptr);
            }
            return "";
        }

        void fromJson(std::string json)
        {

        }

    };




}
