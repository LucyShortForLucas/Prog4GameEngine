#include "States.h"
#include <utility>

StateMachine::StateMachine(std::unique_ptr<IState>&& initialState):
	m_CurrentState(std::move(initialState)){}


void StateMachine::Update() {
	std::unique_ptr<IState> nextState{};

	switch (m_CurrentState->Update(nextState)) {
	case StateCode::Running:
		return;

	case StateCode::Transition:
		m_CurrentState = std::move(nextState);
		return;

	case StateCode::Push:
		m_StateStack.emplace(std::move(m_CurrentState));
		m_CurrentState = std::move(nextState);
		return;

	case StateCode::Pull:
		m_CurrentState = std::move(m_StateStack.top());
		m_StateStack.pop();
		return;
	}

}