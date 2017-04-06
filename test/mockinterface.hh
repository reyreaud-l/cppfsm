#pragma once

#include <string>
#include "interface.hh"
#include "gmock/gmock.h"

namespace fsm
{
class MockInterface : public interface
{
  public:
    MOCK_METHOD0(start, void());
    MOCK_METHOD1(transit, void(const Transition&));
    MOCK_METHOD1(transit, void(const State&));
    MOCK_METHOD1(transit, void(int));
    MOCK_METHOD1(add_states, void(State&));
    MOCK_METHOD1(add_transitions, void(Transition&));
    MOCK_METHOD1(set_start_state, void(State&));
    MOCK_METHOD1(set_machine, void(std::string));
    MOCK_METHOD0(name_get, std::string());
    MOCK_METHOD0(id_get, int());
};
}
