#include "core.h"


int Core::start() {
    return 0;
}


int Core::stop() {
    return 0;
}


int Core::pause() {
    return 0;
}


int Core::execute() {
    bool operation_applied = false;
    for (set<Bubble*>::iterator connected_bubble = _current_bubble->GetConnectionBubbleSet()->begin();
         connected_bubble != _current_bubble->GetConnectionBubbleSet()->end();
         connected_bubble++) {
        set<BubbleConnectionLine*>* connection_set = (*_current_bubble->GetConnectionInfo())[(*connected_bubble)->GetBubbleId()];
        for (set<BubbleConnectionLine*>::iterator connection_description = connection_set->begin();
             connection_description != connection_set->end();
             connection_description++) {
            if (_turing_line->GetCurrentCell()->text() == (*connection_description)->GetSymbolBeforeLine()->text()) {
                _turing_line->GetCurrentCell()->setText((*connection_description)->GetSymbolAfterLine()->text());
                (*connection_description)->GetSelectedWay() == "L"
                    ? _turing_line->L()
                    : (*connection_description)->GetSelectedWay() == "R"
                        ? _turing_line->R()
                        : _turing_line->N();
                _current_bubble = *connected_bubble;
                operation_applied = true;
                break;
            }
        }

        if (operation_applied) {
            break;
        }
    }
    return operation_applied ? 0 : 1;
}


void Core::SetCurrentBubble(Bubble* current_bubble) {
    _current_bubble = current_bubble;
}


void Core::SetCurrentCellCode(int current_cell_code) {
    _current_cell_code = current_cell_code;
}


void Core::SetFinishBubbleId(int finish_bubble_id) {
    _finish_bubble_id = finish_bubble_id;
}

Core::Core(TuringLine* turing_line, QWidget* parent) : QObject(parent)
{
    _turing_line = turing_line;
}


int Core::Run() {
    if (_finish_bubble_id == -1) {
        return 1;
    }

    int execute_result = 0;
    int execute_comands_bound = 10000;
    while (_current_bubble->GetBubbleId() != _finish_bubble_id
           && execute_result == 0
           && execute_comands_bound > 0) {
        execute_result = execute();
        execute_comands_bound--;
    }

    return 0;
}


int Core::Pause() {
    return 0;
}
