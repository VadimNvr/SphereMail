#include "cScene.h"

#define KNIGTH_ICON				"../data/textures/knight_icon.png"
#define WIZARD_ICON				"../data/textures/wizard_icon.png"

#define STATUS_BAR_TEXTURE		"../data/textures/bar.png"

#define KNIGHT_SET				"../data/textures/knight.png"
#define WIZARD_SET				"../data/textures/wizard.png"
#define MONSTER_WARRIOR_SET		"../data/textures/monster1.png"
#define MONSTER_ARCHER_SET		"../data/textures/monster2.png"
#define TILE_SET				"../data/textures/tiles3.png"
#define FIREBALL_SET			"../data/textures/fireball.png"
#define ARROW_SET				"../data/textures/arrow.png"

#define KNIGHT_WIDTH			64
#define KNIGHT_HEIGHT			64
#define WIZARD_WIDTH			64
#define WIZARD_HEIGHT			64
#define MONSTER_WARRIOR_WIDTH	64
#define MONSTER_WARRIOR_HEIGHT	64
#define MONSTER_ARCHER_WIDTH	64
#define MONSTER_ARCHER_HEIGHT	64

#define TILE_WIDTH				80
#define TILE_HEIGHT				80
#define FIREBALL_WIDTH			32
#define FIREBALL_HEIGHT			32
#define ARROW_WIDTH				32
#define ARROW_HEIGHT			32

#define TILE_CLIP_NUMBER		13


Scene::Scene(const std::string& f_settings, const std::string& f_map, bool isBoy, bool isGirl)
{
	if (!isBoy && !isGirl)
	{
		char msg_err[256];
		sprintf(msg_err, "No character is chosen!\n");
		throw msg_err;
	}

	m_knight = NULL; m_knight_bar = NULL;
	m_wizard = NULL; m_wizard_bar = NULL;
	m_knight_icon = m_wizard_icon = m_bar = NULL;
	m_goblin_set = m_chobin_set = NULL;
	m_knight_set = m_wizard_set = NULL;
	m_fireball_set = m_arrow_set = NULL;
	m_tile_set = NULL;

	m_camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	try
	{
		m_knight_icon = new LTexture(KNIGTH_ICON);
		m_wizard_icon = new LTexture(WIZARD_ICON);
		m_bar = new LTexture(STATUS_BAR_TEXTURE);
	}
	catch (char* msg_err)
	{
		deleteTextures();
		throw msg_err;
	}

	try
	{
		m_goblin_set = new LTextureSet(MONSTER_WARRIOR_SET, MONSTER_WARRIOR_WIDTH, MONSTER_WARRIOR_HEIGHT);
		m_chobin_set = new LTextureSet(MONSTER_ARCHER_SET, MONSTER_ARCHER_WIDTH, MONSTER_ARCHER_HEIGHT);
		m_knight_set = new LTextureSet(KNIGHT_SET, KNIGHT_WIDTH, KNIGHT_HEIGHT);
		m_wizard_set = new LTextureSet(WIZARD_SET, WIZARD_WIDTH, WIZARD_HEIGHT);
		m_tile_set = new LTextureSet(TILE_SET, TILE_WIDTH, TILE_HEIGHT);
		m_fireball_set = new LTextureSet(FIREBALL_SET, FIREBALL_WIDTH, FIREBALL_HEIGHT);
		m_arrow_set = new LTextureSet(ARROW_SET, ARROW_WIDTH, ARROW_WIDTH);
	}
	catch (char* msg_err)
	{
		deleteTextureSets();
		deleteTextures();
		throw msg_err;
	}

	std::ifstream file;
	int x = 0, y = 0, n = 0, m = 0;
	Vector2D pos;

	try
	{
		file.open(f_settings.c_str());
		if (!file.is_open())
		{
			char msg_err[256];
			sprintf(msg_err, "Unable to load map file: %s!\n", f_map.c_str());
			throw msg_err;
		}

		file >> n;
		for (int i = 0; i < n; i++)
		{
			file >> m >> x >> y;
			pos = Vector2D(x * Tile::m_width, y * Tile::m_height);
			switch (m)
			{
			case 0:
				m_monsters.push_back(new Goblin(m_goblin_set, pos));
				break;
			case 1:
				m_monsters.push_back(new Chobin(m_chobin_set, m_arrow_set, pos));
				break;
			default:
				char msg_err[256];
				sprintf(msg_err, "Error loading monster: Invalid tile type at %d!\n", i);
				throw msg_err;
			}
		}

		if (isBoy && isGirl)
		{
			file >> x >> y;
			pos = Vector2D(x * Tile::m_width, y * Tile::m_height);
			m_knight = new Knight(m_knight_set, pos, (SDL_Keycode*)knight_keys);

			file >> x >> y;
			pos = Vector2D(x * Tile::m_width, y * Tile::m_height);
			m_wizard = new Wizard(m_wizard_set, m_fireball_set, pos, (SDL_Keycode*)wizard_keys);
		}
		else if (isBoy)
		{
			file >> x >> y;
			pos = Vector2D(x * Tile::m_width, y * Tile::m_height);
			m_knight = new Knight(m_knight_set, pos, (SDL_Keycode*)knight_keys);
		}
		else if (isGirl)
		{
			file >> x >> y;
			pos = Vector2D(x * Tile::m_width, y * Tile::m_height);
			m_wizard = new Wizard(m_wizard_set, m_fireball_set, pos, (SDL_Keycode*)knight_keys);
		}

		file.close();
	}
	catch (char* msg_err)
	{
		if (!file.is_open()) file.close();
		deleteCreatures();
		deleteTextureSets();
		deleteTextures();
		throw msg_err;
	}

	try
	{
		file.open(f_map.c_str());
		if (!file.is_open())
		{
			char msg_err[256];
			sprintf(msg_err, "Unable to load map file!\n");
			throw msg_err;
		}

		file >> m_level.m_width >> m_level.m_height;
		m_level.m_width_px = m_level.m_width * Tile::m_width;
		m_level.m_height_px = m_level.m_height * Tile::m_height;

		for (int y = 0; y < m_level.m_height; y++)
		{
			for (int x = 0; x < m_level.m_width; x++)
			{
				file >> n;
				if (n >= 0 && n <= TILE_CLIP_NUMBER)
				{
					Vector2D pos(x * Tile::m_width, y * Tile::m_height);
					m_tile_map.push_back(new Tile(m_tile_set, pos, n));
				}
				else
				{
					char msg_err[256];
					sprintf(msg_err, "Error loading map: Invalid tile type at %d!\n", x + y * m_level.m_width);
					throw msg_err;
				}
			}
		}
	}
	catch (char* msg_err)
	{
		if (!file.is_open()) file.close();
		deleteTiles();
		deleteCreatures();
		deleteTextureSets();
		deleteTextures();
		throw msg_err;
	}

	file >> m_finish_area.x >> m_finish_area.y >> m_finish_area.w >> m_finish_area.h;
	m_finish_area.x *= Tile::m_width;
	m_finish_area.y *= Tile::m_height;
	m_finish_area.w *= Tile::m_width;
	m_finish_area.h *= Tile::m_height;

	file.close();

	if (m_knight && m_wizard)
	{
		pos = Vector2D(3, m_camera.h - 16 - 3);
		m_knight_bar = new StatusBar(m_bar, m_knight_icon, pos);
		pos = Vector2D(m_camera.w - 114 - 3, m_camera.h - 16 - 3);
		m_wizard_bar = new StatusBar(m_bar, m_wizard_icon, pos);
	}
	else if (m_knight)
	{
		pos = Vector2D(3, m_camera.h - 16 - 3);
		m_knight_bar = new StatusBar(m_bar, m_knight_icon, pos);
	}
	else if (m_wizard)
	{
		pos = Vector2D(3, m_camera.h - 16 - 3);
		m_wizard_bar = new StatusBar(m_bar, m_wizard_icon, pos);
	}
}

void Scene::prepare()
{
	for (auto p_elem = m_monsters.begin(); p_elem != m_monsters.end(); )
	{
		if ((*p_elem)->isReadyToBeDeleted())
		{
			m_camera;
			delete *p_elem;
			p_elem = m_monsters.erase(p_elem);
		}
		else
		{
			p_elem++;
		}
	}

	for (auto p_elem = m_bullets.begin(); p_elem != m_bullets.end(); )
	{
		if ((*p_elem)->isReadyToBeDeleted())
		{
			m_camera;
			delete *p_elem;
			p_elem = m_bullets.erase(p_elem);
		}
		else
		{
			p_elem++;
		}
	}
}

void Scene::setCamera()
{
	if (m_knight && m_wizard)
	{
		// think of it
		int knight_pos_x = (m_knight->getBox().x + Knight::m_width / 2) - SCREEN_WIDTH / 2;
		int knight_pos_y = (m_knight->getBox().y + Knight::m_height / 2) - SCREEN_HEIGHT / 2;
		int wizard_pos_x = (m_wizard->getBox().x + Wizard::m_width / 2) - SCREEN_WIDTH / 2;
		int wizard_pos_y = (m_wizard->getBox().y + Wizard::m_height / 2) - SCREEN_HEIGHT / 2;
		m_camera.x = (knight_pos_x + wizard_pos_x) / 2;
		m_camera.y = (knight_pos_y + wizard_pos_y) / 2;

	}
	else if (m_knight)
	{
		m_camera.x = (m_knight->getBox().x + Knight::m_width / 2) - SCREEN_WIDTH / 2;
		m_camera.y = (m_knight->getBox().y + Knight::m_height / 2) - SCREEN_HEIGHT / 2;
	}
	else if (m_wizard)
	{
		m_camera.x = (m_wizard->getBox().x + Wizard::m_width / 2) - SCREEN_WIDTH / 2;
		m_camera.y = (m_wizard->getBox().y + Wizard::m_height / 2) - SCREEN_HEIGHT / 2;
	}

	if (m_camera.x < 0)
	{
		m_camera.x = 0;
	}
	else if (m_camera.x > m_level.m_width_px - m_camera.w)
	{
		m_camera.x = m_level.m_width_px - m_camera.w;
	}

	if (m_camera.y < 0)
	{
		m_camera.y = 0;
	}
	else if (m_camera.y > m_level.m_height_px - m_camera.h)
	{
		m_camera.y = m_level.m_height_px - m_camera.h;
	}
}

void Scene::update()
{
	std::vector<Creature*> players;
	if (m_knight)
	{
		players.push_back(m_knight);
	}
	if (m_wizard)
	{
		players.push_back(m_wizard);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * CREATURE MOVEMENT * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::vector<RObject*> obstacles;
	obstacles.insert(obstacles.end(), m_tile_map.begin(), m_tile_map.end());
	obstacles.insert(obstacles.end(), m_monsters.begin(), m_monsters.end());
	obstacles.insert(obstacles.end(), players.begin(), players.end());

	if (m_knight)
	{
		m_knight->move(m_level, obstacles, &m_camera);
	}
	if (m_wizard)
	{
		m_wizard->move(m_level, obstacles, &m_camera);
	}

	obstacles.clear();
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * BULLETS  MOVEMENT * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::vector<RObject*> obstacles_player;
	obstacles_player.insert(obstacles_player.end(), m_tile_map.begin(), m_tile_map.end());
	obstacles_player.insert(obstacles_player.end(), m_monsters.begin(), m_monsters.end());

	std::vector<RObject*> obctacles_monster;
	obctacles_monster.insert(obctacles_monster.end(), m_tile_map.begin(), m_tile_map.end());
	obctacles_monster.insert(obctacles_monster.end(), players.begin(), players.end());

	for (auto p_elem = m_bullets.begin(); p_elem != m_bullets.end(); p_elem++)
	{
		FireBall* tmp1 = dynamic_cast<FireBall*>(*p_elem);
		if (tmp1)
		{
			tmp1->move(m_level, obstacles_player);
		}
		Arrow* tmp2 = dynamic_cast<Arrow*>(*p_elem);
		if (tmp2)
		{
			tmp2->move(m_level, obctacles_monster);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * CREATURE ATTACK * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (m_knight)
	{
		m_knight->attack(m_monsters);
	}
	if (m_wizard)
	{
		m_wizard->attack<FireBall>(m_bullets);
	}
	for (auto p_elem = m_monsters.begin(); p_elem != m_monsters.end(); p_elem++)
	{
		Goblin* tmp1 = dynamic_cast<Goblin*>(*p_elem);
		if (tmp1)
		{
			tmp1->attack(players);
		}
		Chobin* tmp2 = dynamic_cast<Chobin*>(*p_elem);
		if (tmp2)
		{
			tmp2->attack<Arrow>(m_bullets);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * BULLETS  ATTACK * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	for (auto p_elem = m_bullets.begin(); p_elem != m_bullets.end(); p_elem++)
	{
		FireBall* tmp1 = dynamic_cast<FireBall*>(*p_elem);
		if (tmp1)
		{
			tmp1->attack(m_monsters);
		}
		Arrow* tmp2 = dynamic_cast<Arrow*>(*p_elem);
		if (tmp2)
		{
			tmp2->attack(players);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * ENERGY RELOAD FOR PLAYERS * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (m_timer.getTime() >= 40)
	{
		if (m_knight)
		{
			m_knight->reloadEnergy(m_knight->getEnergy() + 1);
		}
		if (m_wizard)
		{
			m_wizard->reloadEnergy(m_wizard->getEnergy() + 1);
		}
		m_timer.resetTimer();
	}
}

bool Scene::isLevelFinished() const
{
	if (m_knight && m_wizard)
	{
		return checkCollision(m_knight->getBox(), m_finish_area) &&
			checkCollision(m_wizard->getBox(), m_finish_area);
	}
	else if (m_knight)
	{
		return checkCollision(m_knight->getBox(), m_finish_area);
	}
	else if (m_wizard)
	{
		return checkCollision(m_wizard->getBox(), m_finish_area);
	}
    
    return 0;
}

bool Scene::isGameOver() const
{
	if (m_knight && m_wizard)
	{
		return !(m_knight->isAlive() && m_wizard->isAlive());
	}
	else if (m_knight)
	{
		return !m_knight->isAlive();
	}
	else if (m_wizard)
	{
		return !m_wizard->isAlive();
	}
    
    return 0;
}

void Scene::render()
{
	setCamera();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * BACKGROUND  RENDERING * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	for (auto p_elem = m_tile_map.begin(); p_elem != m_tile_map.end(); p_elem++)
	{
		(*p_elem)->render(m_camera);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * CREATURE    RENDERING * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	std::vector<Creature*> creatures = m_monsters;
	if (m_knight)
	{
		creatures.push_back(m_knight);
	}
	if (m_wizard)
	{
		creatures.push_back(m_wizard);
	}
	std::sort(creatures.begin(), creatures.end(), creatures[0]->comparator);

	for (auto p_elem = creatures.begin(); p_elem != creatures.end(); p_elem++)
	{
		(*p_elem)->render(m_camera);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * BULLETS   RENDERING * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	for (auto p_elem = m_bullets.begin(); p_elem != m_bullets.end(); p_elem++)
	{
		(*p_elem)->render(m_camera);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * PLAYERS' STATUSBARS RENDERING * * * * * * * * * * * * * * * * * *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	if (m_knight)
	{
		m_knight_bar->setEnergyBarLength(static_cast<float>(m_knight->getEnergy()) / m_knight->getEnergyMax());
		m_knight_bar->setHealthBarLength(static_cast<float>(m_knight->getHealth()) / m_knight->getHealthMax());
		m_knight_bar->render();
	}
	if (m_wizard)
	{
		m_wizard_bar->setEnergyBarLength(static_cast<float>(m_wizard->getEnergy()) / m_wizard->getEnergyMax());
		m_wizard_bar->setHealthBarLength(static_cast<float>(m_wizard->getHealth()) / m_wizard->getHealthMax());
		m_wizard_bar->render();
	}
}

Scene::~Scene()
{
	deleteBullets();
	deleteTiles();
	deleteCreatures();
	deleteTextureSets();
	deleteTextures();
}

void Scene::deleteTextures()
{
	if (m_bar) delete m_bar;
	if (m_wizard_icon) delete m_wizard_icon;
	if (m_knight_icon) delete m_knight_icon;
}

void Scene::deleteTextureSets()
{
	if (m_fireball_set) delete m_fireball_set;
	if (m_arrow_set) delete m_arrow_set;
	if (m_tile_set) delete m_tile_set;
	if (m_wizard_set) delete m_wizard_set;
	if (m_knight_set) delete m_knight_set;
	if (m_goblin_set) delete m_goblin_set;
	if (m_chobin_set) delete m_chobin_set;
}

void Scene::deleteTiles()
{
	for (auto p_elem = m_tile_map.begin(); p_elem != m_tile_map.end(); p_elem++)
	{
		delete *p_elem;
	}
	m_tile_map.clear();
}

void Scene::deleteBullets()
{
	for (auto p_elem = m_bullets.begin(); p_elem != m_bullets.end(); p_elem++)
	{
		delete *p_elem;
	}
	m_bullets.clear();
}

void Scene::deleteCreatures()
{
	for (auto p_elem = m_monsters.begin(); p_elem != m_monsters.end(); p_elem++)
	{
		delete *p_elem;
	}
	m_monsters.clear();
	if (m_wizard)
	{
		delete m_wizard;
		delete m_wizard_bar;
	}
	if (m_knight)
	{
		delete m_knight;
		delete m_knight_bar;
	}
}