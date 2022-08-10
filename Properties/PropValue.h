#pragma once

#include <string>
#include <any>
#include <memory>
#include <functional>
#include "UndoRedoManager.h"


namespace Properties
{

    class PropertyManager
    {
        std::vector<IUndoRedoProp*> _list; // TODO: or better use map?

        std::vector<PropertyManager*> _childManagers;

        std::string _name;

    public:
        PropertyManager(const std::string &name = "") : _name(name)
        {
        }

        void add(IUndoRedoProp* newProp)
        {
            _list.push_back(newProp);
        }

        void clear()
        {
            _list.clear();
            _childManagers.clear();
        }

        std::string toJson() const
        {
            for(const auto &it : _list) {
                printf("PropName: %s -> JSON: %s\n", it->getName().c_str(), it->toJson().c_str());
            }
            for (const auto* it : _childManagers) {
                printf("CHILD %s\n", it->name().c_str());
                it->toJson();
            }
            return "";
        }

        void fromJson(std::string json)
        {

        }

        void addChild(PropertyManager *childMan)
        {
            _childManagers.push_back(childMan);
        }

        std::string name() const
        {
            return _name;
        }
    };


    template<typename T>
    class PropValue : IUndoRedoProp
    {
    private:
        std::string _name = "";
        std::function<void(T)> _updateFunc = 0;
        std::shared_ptr<T> _value;
        std::shared_ptr<T> _defaultValue;
        UndoRedoManager * _undoRedoMan = 0;
        PropertyManager * _propertyManager = 0;

    public:
        explicit PropValue(const std::string& name, std::shared_ptr<T> value, UndoRedoManager* undoRedoMan, PropertyManager* propertyManager)
            : _name(name), _value(value), _defaultValue(value), _undoRedoMan(undoRedoMan), _propertyManager(propertyManager)
        {
            if (propertyManager != nullptr) {
                propertyManager->add(this);
            }
        }

        explicit PropValue(const std::string& name, T value, UndoRedoManager *undoRedoMan, PropertyManager *propertyManager)
            : PropValue(name, std::make_shared<T>(value), undoRedoMan, propertyManager)
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

        std::string convertToJson(const T& value) const;


        std::string toJson() const override
        {
            const auto ptr = _value.get();
            if (ptr != 0) {
                return convertToJson(*ptr);
            }
            return "";
        }

        void fromJson(std::string json) override
        {

        }

    };







}
