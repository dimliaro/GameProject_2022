#include "defines.h"
#include "game.h"
#include <sgg/graphics.h>

void Game::draw()
{
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	
	if (m_state == STATE_INIT)
	{
		graphics::setFont("FreeSans.ttf");
		graphics::drawText(CANVAS_WIDTH/2, CANVAS_HEIGHT/2,0.5f,"Loading assets..",br);
		m_state = STATE_LOADING;
		return;
	}

	

	//Draw Background
	br.outline_opacity = 0.0f;
	br.texture = ASSET_PATH + std::string("main_menu.png");
	SETCOLOR(br.fill_color, 1.0f, 1.0f,1.0f);

	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
	for (auto player : m_players)
	{
		player->draw();
	}

	for (auto ev : m_events) {
		ev->draw();
	}

	graphics::Brush reticle;
	if (m_state == STATE_MOVING)
	{
		
		SETCOLOR(reticle.fill_color, 0.0f, 0.0f, 0.0f);
		reticle.fill_opacity = 0.3f;
		reticle.outline_opacity = 0.7f;
		SETCOLOR(reticle.outline_color, 0.0f, 0.0f, 0.0f);
		graphics::drawDisk(m_init_pos_x, m_init_pos_y, PLAYER_MOTION_RANGE, reticle);
	}
	else if (m_state==STATE_ATTACKING)
	{
		SETCOLOR(reticle.fill_color, 1.0f, 0.0f, 0.0f);
		reticle.fill_opacity = 0.3f;
		reticle.outline_opacity = 0.7f;
		SETCOLOR(reticle.outline_color, 1.0f, 0.0f, 0.0f);
		graphics::drawDisk(m_init_pos_x, m_init_pos_y, PLAYER_ATTACK_RANGE, reticle);
	}

	if (m_state == STATE_MOVING || m_state == STATE_ATTACKING)
	{
		reticle.fill_opacity = 1.0f;
		reticle.outline_width = 2.0f;
		graphics::drawDisk(m_init_pos_x, m_init_pos_y, 0.1f, reticle);
		graphics::drawLine(m_init_pos_x, m_init_pos_y, m_active_player->getPosX(), m_active_player->getPosY(), reticle);
	}
}
void Game::update()
{
	if (m_state == STATE_INIT)
	{
		return;
	}
	if (m_state == STATE_LOADING)
	{
		init();
		m_state = STATE_IDLE;
		return;
	}

	processEvents();
	

	for (auto player : m_players)
	{
		player->update();
	}
	graphics::MouseState ms;
	graphics::getMouseState(ms);

	float mx = graphics::windowToCanvasX(ms.cur_pos_x);
	float my = graphics::windowToCanvasY(ms.cur_pos_y);
	//highlight player
	Player* curr_player = nullptr;
	for (auto player : m_players)
	{
		if (player->contains(mx, my))
		{
			player->setHighlight(player->contains(mx, my));
			curr_player = player;
		}
		else
			player->setHighlight(false);

		
	}
	//active player
	if (ms.button_left_pressed && curr_player)
	{
		m_active_player = curr_player;
		m_active_player->setActive(true);
		for (auto player : m_players)
		{
			if (player !=  m_active_player)
			{
				player->setActive(false);
			}
		}
		m_init_pos_x = m_active_player->getPosX();
		m_init_pos_y = m_active_player->getPosY();
	}

	//mouse dragging
	if (ms.dragging && m_active_player)
	{
		m_state = STATE_MOVING;
		if(distance(m_init_pos_x,m_init_pos_y, mx, my)< PLAYER_MOTION_RANGE)
		{
			m_active_player->setPosX(mx);
			m_active_player->setPosY(my);

			addEvent(new SmokeEvent(mx,my));
		}

		m_valid_action = true;

		m_target_player = nullptr;

		for (auto p : m_players)
		{ 
			if (p == m_active_player)
				continue;
			if (p->intersect(m_active_player)) {

				m_state = STATE_ATTACKING;
				m_target_player = p;
			}
		}

		if (m_state == STATE_ATTACKING)
		{
			if (m_target_player != nullptr && distance(m_target_player->getPosX(), m_target_player->getPosY(),
				m_init_pos_x, m_init_pos_y) < PLAYER_ATTACK_RANGE)
				m_valid_action = true;
			else
				m_valid_action = false;
		}

	}
	if (ms.button_left_released && curr_player)
	{
		if (!m_valid_action)
		{
			//m_active_player->setPosX(m_init_pos_x);
			//m_active_player->setPosY(m_init_pos_y);
			float x_in = m_active_player->getPosX();
			float y_in = m_active_player->getPosY();
			float x_out = m_init_pos_x;
			float y_out = m_init_pos_y;
			addEvent(new PlayerMotionEvent(x_in, y_in, x_out, y_out, m_active_player));

		}
		m_state = STATE_IDLE;

		if (m_valid_action && m_target_player)
		{
			Player* tgt = m_target_player;
			m_players.remove_if([tgt](Player* pl) {return pl == tgt; });
			delete m_target_player;
		}

		m_target_player = nullptr;
		m_active_player->setActive(false);
		m_active_player = nullptr;
	}


}

void Game::init()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			//dimiourgia paiktwn
			Player* p = new Player();
			m_players.push_front(p);

			p->setPosX(CANVAS_WIDTH * (i+0.5f) / 4.0f);
			p->setPosY(CANVAS_HEIGHT * (j+0.5f) / 2.0f);
		}

	}
	graphics::preloadBitmaps(ASSET_PATH);
	//sleep(2000)
}

Game::~Game()
{
	for (auto p : m_players)
	{
		delete p;
	}
	m_players.clear();
}

Game* Game::getInstance()
{
	if (!m_instance) // an den yparxei to instance tote ftiakse ena instance Games ton soro 
		m_instance = new Game();
	else
		return m_instance;
}

void Game::processEvents()
{
	for (auto e : m_events) {
		e->update();
	}

	m_events.remove_if([](Event* ev) {return !ev->active(); });

}

void Game::addEvent(Event* evt)
{
	m_events.push_front(evt);
}

Game* Game::m_instance = nullptr;


