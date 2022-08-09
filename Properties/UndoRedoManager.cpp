#include <stdint.h>
#include <vector>
#include <string>
#include "UndoRedoManager.h"
using namespace std;

namespace Properties
{



    UndoRedoManager::UndoRedoManager()
    {

    }


    void UndoRedoManager::beginCommand(string name)
    {
        if (CurrentCommand == nullptr)
        {
            CurrentCommand = make_shared<Command>(name);
        }
    }

    void UndoRedoManager::endCommand()
    {
        if (!CurrentCommand) {
            return;
        }

        // Check if current command can be merged with last one in history list
        if (HistoryIndex >= 0 && HistoryIndex < History.size())
        {
            bool isMerged = History[HistoryIndex]->mergeCommands(*CurrentCommand);
            if (isMerged) {
                CurrentCommand = 0;
                return;
            }
        }

        // Remove all items from HistoryIndex to the end -> undo/redo not possible anymore for these changes
        HistoryIndex++;
        History.erase(History.begin() + HistoryIndex, History.end());    // History.RemoveRange(HistoryIndex, History.Count - HistoryIndex);
        History.push_back(CurrentCommand);
        CurrentCommand = 0;
    }

    void UndoRedoManager::clearHistory()
    {
        History.clear();
        HistoryIndex = -1;
        CurrentCommand = 0;
    }

    void UndoRedoManager::undo()
    {
        if (CurrentCommand != 0)
        {
            endCommand();     // close current command if existent
        }
        if (HistoryIndex >= 0 && HistoryIndex < History.size() && History.size() > 0)
        {
            History[HistoryIndex]->undo();
            HistoryIndex--;
        }
    }

    void UndoRedoManager::redo()
    {
        if (CurrentCommand != 0) {
            return; // no redo possible
        }
        if (HistoryIndex >= -1 && HistoryIndex < History.size() - 1 && History.size() > 0) {
            HistoryIndex++;
            History[HistoryIndex]->redo();
        }
    }
}