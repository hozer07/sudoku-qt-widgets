#include "logelement.h"

ChangeValueLog::ChangeValueLog(coordinateType coord, uint8_t value, std::bitset<9> notes) :
    m_coordinate(coord),
    m_prevValue(value),
    m_notesTakenBeforeAction(notes)
{}

TakeNoteLog::TakeNoteLog(coordinateType coord, uint8_t value) :
    m_coordinate(coord),
    m_NoteValue(value)
{}
