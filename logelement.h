#ifndef LOGELEMENT_H
#define LOGELEMENT_H

#include <cstdint>
#include <utility>
#include <variant>
#include <bitset>

class ChangeValueLog;
class TakeNoteLog;

using LogElement = std::variant<ChangeValueLog, TakeNoteLog>;

namespace{
    using coordinateType = std::pair<uint8_t, uint8_t>;
}

class ChangeValueLog{
    public:
        explicit ChangeValueLog(coordinateType, uint8_t, std::bitset<9>);
        coordinateType m_coordinate;
        uint8_t m_prevValue;
        std::bitset<9> m_notesTakenBeforeAction;
};

class TakeNoteLog{
    public:
        explicit TakeNoteLog(coordinateType, uint8_t);
        coordinateType m_coordinate;
        uint8_t m_NoteValue;
};

class UndoProcessor;

#endif // LOGELEMENT_H
