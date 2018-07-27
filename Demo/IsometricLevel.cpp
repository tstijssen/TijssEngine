#include "GameController.h"
#include "IsometricLevel.h"
#include "StandardComponents.h"
#include "MoveComponent.h"
#include "MainMenu.h"

void IsometricLevel::Load()
{
	windowHeight = gfx->GetContext()->GetPixelSize().height;
	windowWidth = gfx->GetContext()->GetPixelSize().width;

	// player setup
	cEntityTemplate playerTemp("Character", "PlayerTemplate");
	TEntityUID playerCharacter = EntityManager->CreateEntity("PlayerTemplate", "Player");

	playerEntity = EntityManager->GetEntity(playerCharacter);

	cRenderComponent* playerSprite =  new cRenderComponent(playerEntity, EntityManager->GetNewUID(), "minotaur.png", gfx, 3, true);

	playerEntity->AddComponent(playerSprite);

	cColliderComponent* playerCollider = new cColliderComponent(playerEntity, EntityManager->GetNewUID(), Tijss::ColliderType::Circle, 3, 100.0f);
	playerEntity->AddComponent(playerCollider);

	cAnimatorComponent* playerAnimation = new cAnimatorComponent(playerEntity, EntityManager->GetNewUID(), { 8, 24 }, { 128, 128 });
	playerAnimation->SelectAnimation({ 0,5 }, { 3,5});
	playerEntity->AddComponent(playerAnimation);

	cMoveComponent* playerMovement = new cMoveComponent(playerEntity, EntityManager->GetNewUID(), 100.0f, gfx->GetCamera());
	playerEntity->AddComponent(playerMovement);

	playerEntity->m_Transform.SetPosition(windowWidth / 2, (windowHeight / 2));

	//// environment setup
	cEntityTemplate sceneryTemp("Sprite", "Scenery");

	//TEntityUID background = EntityManager->CreateEntity("Scenery", "Background");
	//cEntity* backgroundEntity = EntityManager->GetEntity(background);
	//cRenderComponent* backgroundRender = new cRenderComponent(backgroundEntity, EntityManager->GetNewUID(), "grass.png", gfx, 4);
	//backgroundEntity->AddComponent(backgroundRender);
	//backgroundEntity->m_Transform.scale = { 0.8f, 0.5f };
	//backgroundEntity->m_Transform.SetPosition({100, 215 });

	//// second tile
	//TEntityUID background2 = EntityManager->CreateEntity("Scenery", "Background2");
	//cEntity* backgroundEntity2 = EntityManager->GetEntity(background2);
	//backgroundRender = new cRenderComponent(backgroundEntity2, EntityManager->GetNewUID(), "grass.png", gfx, 4);
	//backgroundEntity2->AddComponent(backgroundRender);
	//backgroundEntity2->m_Transform.scale = { 0.8f, 0.5f };
	//backgroundEntity2->m_Transform.SetPosition({ 900, 215 });

	//// third tile
	//TEntityUID background3 = EntityManager->CreateEntity("Scenery", "Background3");
	//cEntity* backgroundEntity3 = EntityManager->GetEntity(background3);
	//backgroundRender = new cRenderComponent(backgroundEntity3, EntityManager->GetNewUID(), "grass.png", gfx, 4);
	//backgroundEntity3->AddComponent(backgroundRender);
	//backgroundEntity3->m_Transform.scale = { 0.8f, 0.5f };
	//backgroundEntity3->m_Transform.SetPosition({ 100, 725 });

	//// fourth tile
	//TEntityUID background4 = EntityManager->CreateEntity("Scenery", "Background4");
	//cEntity* backgroundEntity4 = EntityManager->GetEntity(background4);
	//backgroundRender = new cRenderComponent(backgroundEntity4, EntityManager->GetNewUID(), "grass.png", gfx, 4);
	//backgroundEntity4->AddComponent(backgroundRender);
	//backgroundEntity4->m_Transform.scale = { 0.8f, 0.5f };
	//backgroundEntity4->m_Transform.SetPosition({ 900, 725 });

	TEntityUID textUID = EntityManager->CreateEntity("Scenery", "TextTest");
	cEntity* textEnt = EntityManager->GetEntity(textUID);
	textEnt->m_Transform.SetPosition(windowWidth / 2, (windowHeight / 2));
	cTextComponent* textComp = new cTextComponent(textEnt, EntityManager->GetNewUID(), gfx, "Gabriola", 20.0f, "Press Escape to go back, WASD to move.");
	textComp->ChangeColour({ 1,1,1,1 });
	textEnt->AddComponent(textComp);

	// trees
	for (int i = 0; i < 10; ++i)
	{
		string treeName = "Tree" + std::to_string(i);
		TEntityUID treeUID = EntityManager->CreateEntity("Scenery", treeName);
		cEntity* treeEnt = EntityManager->GetEntity(treeUID);
		cRenderComponent* treeRender = new cRenderComponent(treeEnt, EntityManager->GetNewUID(), "modeTree2.png", gfx, 3, true);
		treeEnt->AddComponent(treeRender);

		cColliderComponent* treeCollider = new cColliderComponent(treeEnt, EntityManager->GetNewUID(), Circle, 3, 5.0f);
		treeEnt->AddComponent(treeCollider);
		int randX = rand() % 1000;
		int randY = rand() % 800;
		treeEnt->m_Transform.SetPosition(randX, randY);
	}

}
void IsometricLevel::Unload()
{
	EntityManager->DestroyAllEntities();
	delete EntityManager;
	delete playerEntity;
}
void IsometricLevel::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.5f);
	EntityManager->DrawAllEntities(gfx);
}
bool IsometricLevel::Update(double timeTotal, double timeDelta)
{
	EntityManager->UpdateAllEntities(timeDelta);

	if (KeyHit(Key_Escape))
	{
		return false;
	}
	return true;
}