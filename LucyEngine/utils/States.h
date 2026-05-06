#pragma once

#include <memory>
#include <stack>

enum class StateCode {
	Running,
	Transition,
	Push,
	Pull
};

class IState {
public:
	virtual StateCode Update(std::unique_ptr<IState>& nextState) = 0;
};

class StateMachine {
public:
	StateMachine(std::unique_ptr<IState>&& initialState);

	void Update();

private:
	std::unique_ptr<IState> m_CurrentState{};
	std::stack<std::unique_ptr<IState>> m_StateStack{};
};