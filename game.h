#pragma once
#include <list>
#include "player.h"
#include "event.h"
class Game
{
public:
	enum game_state_t{STATE_INIT,STATE_LOADING, STATE_IDLE, STATE_MOVING, STATE_ATTACKING};
protected:

	static Game* m_instance;

	std::list <Player*> m_players;
	std::list<Event*> m_events;
	Player* m_active_player = nullptr;
	Player* m_tgt_player = nullptr;
	game_state_t m_state = STATE_INIT;
	float m_init_pos_x;
	float m_init_pos_y;
	bool m_valid_action = false;

	Game() {}
public:
	void draw();
	void update();
	void init();

	~Game();
	static void releaseInstance() { if (m_instance)delete m_instance; m_instance = nullptr; }
	static Game* getInstance();


	void processEvents();
	void addEvent(Event* evt);
};