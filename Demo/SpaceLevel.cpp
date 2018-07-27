#include "GameController.h"
#include "SpaceLevel.h"
#include "StandardComponents.h"
#include "MainMenu.h"
#include "Input.h"
#include "ProjectileComponent.h"

void cPlayer::Init(cEntity * ship, cEntity * projectile, float32 speed)
{
	m_Fired = false;
	m_ProjectileSprite = projectile;
	m_ProjectileSprite->m_Active = false;

	m_ShipSprite = ship;
	m_Speed = speed;
	m_ShootSound = new cSound("shoot.wav");
}

void cPlayer::UserInput(float32 updateTime)
{
	if (KeyHeld(Key_A))
	{
		m_ShipSprite->m_Transform.Move(-1 * m_Speed * updateTime, 0);
	}

	if (KeyHeld(Key_D))
	{
		m_ShipSprite->m_Transform.Move(1 * m_Speed * updateTime, 0);
	}

	if (KeyHeld(Key_W))
	{
		m_ShipSprite->m_Transform.Move(0, -1 * m_Speed * updateTime);
	}

	if (KeyHeld(Key_S))
	{
		m_ShipSprite->m_Transform.Move(0, 1 * m_Speed * updateTime);
	}

	if (KeyHit(Key_Space))
	{
		if (!m_Fired)
		{
			m_ProjectileSprite->m_Active = true;
			m_ShootSound->m_Sound.play();
			m_Fired = true;
		}
	}
	if (!m_ProjectileSprite->m_Active || !m_Fired)
	{
		m_ProjectileSprite->m_Transform.position = m_ShipSprite->m_Transform.position;
		m_Fired = false;
	}
}

cNPC::cNPC(cEntity * ship, cEntity * projectile)
{
	m_Fired = false;
	m_ProjectileSprite = projectile;
	m_ShipSprite = ship;
	m_ProjectileSprite->m_Active = false;
	m_ShootSound = new cSound("ionHit.wav");
}

void cNPC::Update()
{
	int randomChance = rand() % 50 + 1;

	if (!m_Fired && randomChance == 1)
	{
		m_ProjectileSprite->m_Active = true;
		m_ShootSound->m_Sound.play();
		m_Fired = true;
	}
	else if (!m_ProjectileSprite->m_Active)
	{
		m_ProjectileSprite->m_Transform.position = m_ShipSprite->m_Transform.position;
		m_Fired = false;
	}
}

void SpaceLevel::Load()
{
	int32 windowHeight = gfx->GetContext()->GetPixelSize().height;
	int32 windowWidth = gfx->GetContext()->GetPixelSize().width;

	// environment setup
	cEntityTemplate sceneryTemp("Sprite", "Scenery");
	TEntityUID background = EntityManager->CreateEntity("Scenery", "Background");
	cEntity* backgroundEntity = EntityManager->GetEntity(background);
	cRenderComponent* backgroundRender = new cRenderComponent(backgroundEntity, EntityManager->GetNewUID(), "bg5.jpg", gfx, 4);
	backgroundEntity->AddComponent(backgroundRender);

	cEntityTemplate shipTemp("Sprite", "Ship");

	TEntityUID playerShip = EntityManager->CreateEntity("Ship", "Player");
	cEntity* playerEntity = EntityManager->GetEntity(playerShip);
	cRenderComponent* shipRender = new cRenderComponent(playerEntity, EntityManager->GetNewUID(), "6.png", gfx, 2);
	playerEntity->AddComponent(shipRender);
	playerEntity->m_Transform.SetPosition(windowWidth / 2, windowHeight / 2);

	cColliderComponent* playerCollider = new cColliderComponent(playerEntity, EntityManager->GetNewUID(), Tijss::ColliderType::Box, 2);
	playerEntity->AddComponent(playerCollider);

	cEntityTemplate laserTemp("Sprite", "Projectile");

	TEntityUID playerProject = EntityManager->CreateEntity("Projectile", "PlayerProjectile");
	cEntity* projectileEntity = EntityManager->GetEntity(playerProject);
	cRenderComponent* laserRender = new cRenderComponent(projectileEntity, EntityManager->GetNewUID(), "laserY.png", gfx, 3);
	projectileEntity->AddComponent(laserRender);

	cColliderComponent* laserCollider = new cColliderComponent(projectileEntity, EntityManager->GetNewUID(), Tijss::ColliderType::Point, 3);
	projectileEntity->AddComponent(laserCollider);

	projectileEntity->m_Transform.SetPosition(playerEntity->m_Transform.position);
	cProjectileComponent* laserMove = new cProjectileComponent(projectileEntity, EntityManager->GetNewUID(), 600.0f, true);
	projectileEntity->AddComponent(laserMove);

	m_PlayerClass.Init(playerEntity, projectileEntity, 750.0f);

	for (int i = 0; i < 7; ++i)
	{
		string npcName = "NPC" + i;
		TEntityUID npcShip = EntityManager->CreateEntity("Ship", npcName);
		cEntity* npcEntity = EntityManager->GetEntity(npcShip);
		shipRender = new cRenderComponent(npcEntity, EntityManager->GetNewUID(), "6.png", gfx, 3);
		npcEntity->AddComponent(shipRender);
		cColliderComponent* npcCollider = new cColliderComponent(npcEntity, EntityManager->GetNewUID(), Tijss::ColliderType::Box, 3);
		npcEntity->AddComponent(npcCollider);
		npcEntity->m_Transform.Rotate(180);
		npcEntity->m_Transform.Move(150 * (i + 1), 0);

		TEntityUID npcProjectile = EntityManager->CreateEntity("Projectile", "NPCProjectile");
		cEntity* npcLaser = EntityManager->GetEntity(npcProjectile);
		cRenderComponent* NPClaserRender = new cRenderComponent(npcLaser, EntityManager->GetNewUID(), "laserY.png", gfx, 2);
		npcLaser->AddComponent(NPClaserRender);

		cColliderComponent* NPClaserCollider = new cColliderComponent(npcLaser, EntityManager->GetNewUID(), Tijss::ColliderType::Point, 2);
		npcLaser->AddComponent(NPClaserCollider);

		npcLaser->m_Transform.SetPosition(npcEntity->m_Transform.position);
		cProjectileComponent* NPClaserMove = new cProjectileComponent(npcLaser, EntityManager->GetNewUID(), 500.0f, false);
		npcLaser->AddComponent(NPClaserMove);

		m_NPCs[i] = new cNPC(npcEntity, npcLaser);
	}
}
void SpaceLevel::Unload()
{
	EntityManager->DestroyAllEntities();
	delete EntityManager;
}

void SpaceLevel::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.5f);
	EntityManager->DrawAllEntities(gfx);
}

bool SpaceLevel::Update(double timeTotal, double timeDelta)
{
	EntityManager->UpdateAllEntities(timeDelta);
	m_PlayerClass.UserInput(timeDelta);

	for (int i = 0; i < 7; ++i)
	{
		m_NPCs[i]->Update();
	}

	if (KeyHit(Key_Escape))
	{
		GameController::SwitchLevel(new MainMenu());
	}
	return true;
}