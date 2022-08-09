#pragma once

#include <memory>
#include <typeindex>
#include <vector>

namespace Properties
{

    class Command;


    class Change
    {
    public:
        std::shared_ptr<void> OldObj;
        std::shared_ptr<void> NewObj;
        //    std::type_index Type;
    };


    class IUndoRedoProp
    {
    public:
        virtual void onUndo(std::shared_ptr<Change> change) = 0;
        virtual void onRedo(std::shared_ptr<Change> change) = 0;
        virtual std::string getName() const = 0;
    };


    class UndoRedoManager
    {
    public:
        std::shared_ptr<Command> CurrentCommand = nullptr;
        std::vector<std::shared_ptr<Command>> History;
        int HistoryIndex = -1;

        UndoRedoManager();
        void beginCommand(std::string name);
        void endCommand();
        void clearHistory();
        void undo();
        void redo();

    };


    class Command
    {
    public:
        bool _isMergeEnabled = false;
        std::vector<std::pair<IUndoRedoProp*, std::shared_ptr<Change>>> _list;
        std::string _name;

        // CTOR
        Command(std::string name = "", bool isMergeEnabled = false) : _name(name)
        {
            _isMergeEnabled = isMergeEnabled;
        }

        void add(IUndoRedoProp* item, std::shared_ptr<Change> change)
        {
            // TODO: Check if can be merged -> not here, better in TODO, but for single property changes inside one commit maybe here?
            _list.push_back(std::make_pair(item, change));
        }
        void undo()
        {
            for (auto& it : _list) {
                IUndoRedoProp* prop = it.first;
                prop->onUndo(it.second);
            }
        }

        void redo()
        {
            for (auto& it : _list) {
                IUndoRedoProp* prop = it.first;
                prop->onRedo(it.second);
            }
        }

        bool mergeCommands(const Command& newCommand) const
        {
            if (!_isMergeEnabled || !newCommand._isMergeEnabled || _list.size() != newCommand._list.size() || _list.size() != 1)
            {
                // Note: for now, only commands with a single Change be merged
                return false;
            }
            const std::pair<IUndoRedoProp*, std::shared_ptr<Change>>& el = _list[0];
            const std::pair<IUndoRedoProp*, std::shared_ptr<Change>>& newItem = newCommand._list[0];
            IUndoRedoProp* elProp = el.first;
            IUndoRedoProp* newProp = newItem.first;

            if (elProp == newProp) { // change of same property
                el.second->NewObj = newItem.second->NewObj; // merge: change gets newObj from new value
                return true;
            }
            return false;
        }

    };



}