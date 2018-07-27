#include "GameController.h"
#include "MainMenu.h"
#include "StandardComponents.h"
#include "SpaceLevel.h"
#include "IsometricLevel.h"
#include "Persistance.h"

void MainMenu::Load()
{
	//LevelSprites = new SpriteManager(gfx);
	int32 windowHeight = gfx->GetContext()->GetPixelSize().height;
	int32 windowWidth = gfx->GetContext()->GetPixelSize().width;

	// setup looping menu music
	MainMenuMusic = new cSound("herdNest.ogg");
	MainMenuMusic->m_Sound.setVolume(50.0f);
	MainMenuMusic->m_Sound.play();
	MainMenuMusic->m_Sound.setLoop(true);

	// space game button
	buttonTemplate = EntityManager->CreateTemplate("UI", "Button");
	eSpaceShooterStartBtn = new TEntityUID;
	*eSpaceShooterStartBtn = EntityManager->CreateEntity("Button", "SpaceBtn");

	cEntity* btnEntity = EntityManager->GetEntity(*eSpaceShooterStartBtn);

	cRenderComponent* eButtonRenderer =  new cRenderComponent(btnEntity, EntityManager->GetNewUID(), "button.png", gfx, 1);
	
	btnEntity->AddComponent(eButtonRenderer);

	cColliderComponent* eBoxCollider = new cColliderComponent(btnEntity, EntityManager->GetNewUID(), Tijss::ColliderType::Box, 1);

	btnEntity->AddComponent(eBoxCollider);
	
	cAnimatorComponent* eAnimation = new cAnimatorComponent(btnEntity, EntityManager->GetNewUID(), { 1, 3 }, { 300, 90 });
	eAnimation->SelectAnimation({ 0,0 }, { 0,0});
	btnEntity->AddComponent(eAnimation);

	btnEntity->m_Transform.SetPosition(windowWidth / 2, (windowHeight / 2) - 100.0f);

	cTextComponent* eText = new cTextComponent(btnEntity, EntityManager->GetNewUID(), gfx, "Gabriola", 52.0f, "Space Game");
	eText->ChangeColour({ 1,1,1,1 });
	eText->SetPosition(-90.0f, -60.0f);
	btnEntity->AddComponent(eText);

	// isometric button
	eIsometricStartBtn = new TEntityUID;
	*eIsometricStartBtn = EntityManager->CreateEntity("Button", "IsometricBtn");

	btnEntity = EntityManager->GetEntity(*eIsometricStartBtn);

	eButtonRenderer = new cRenderComponent(btnEntity, EntityManager->GetNewUID(), "button.png", gfx, 1);

	btnEntity->AddComponent(eButtonRenderer);

	eBoxCollider = new cColliderComponent(btnEntity, EntityManager->GetNewUID(), Tijss::ColliderType::Box, 1);

	btnEntity->AddComponent(eBoxCollider);

	eAnimation = new cAnimatorComponent(btnEntity, EntityManager->GetNewUID(), { 1, 3 }, { 300, 90 });
	eAnimation->SelectAnimation({ 0,0 }, { 0,0 });
	btnEntity->AddComponent(eAnimation);

	btnEntity->m_Transform.SetPosition(windowWidth / 2, (windowHeight / 2));

	eText = new cTextComponent(btnEntity, EntityManager->GetNewUID(), gfx, "Gabriola", 52.0f, "Isometric");
	eText->ChangeColour({ 1,1,1,1 });
	eText->SetPosition(-90.0f, -60.0f);
	btnEntity->AddComponent(eText);

	// quit button
	eExitButton = new TEntityUID;
	*eExitButton = EntityManager->CreateEntity("Button", "ExitBtn");

	btnEntity = EntityManager->GetEntity(*eExitButton);

	eButtonRenderer = new cRenderComponent(btnEntity, EntityManager->GetNewUID(), "button.png", gfx, 1);

	btnEntity->AddComponent(eButtonRenderer);

	eBoxCollider = new cColliderComponent(btnEntity, EntityManager->GetNewUID(), Tijss::ColliderType::Box, 1);

	btnEntity->AddComponent(eBoxCollider);

	eAnimation = new cAnimatorComponent(btnEntity, EntityManager->GetNewUID(), { 1, 3 }, { 300, 90 });
	eAnimation->SelectAnimation({ 0,0 }, { 0,0 });
	btnEntity->AddComponent(eAnimation);

	btnEntity->m_Transform.SetPosition(windowWidth / 2, (windowHeight / 2) + 100.0f);

	eText = new cTextComponent(btnEntity, EntityManager->GetNewUID(), gfx, "Gabriola", 52.0f, "Quit Demo");
	eText->ChangeColour({ 1,1,1,1 });
	eText->SetPosition(-90.0f, -60.0f);
	btnEntity->AddComponent(eText);
	//StartButton = LevelSprites->AddSprite("button.png", 1);
	//StartButton->MakeAnimated(3, 1, 0.5f, 300, 90);
	//StartButton->transform.SetPosition(windowWidth / 2, windowHeight / 2);
	//StartButton->animation.SelectAnimation({ 0, 0 }, {0,0});
	//StartButton->collider = CollisionType::Box;

	//QuitButton = LevelSprites->AddSprite("button.png", 1);
	//QuitButton->MakeAnimated(3, 1, 0.5f, 300, 90);
	//QuitButton->transform.SetPosition(windowWidth / 2, (windowHeight / 2) + QuitButton->transform.size.y);
	//QuitButton->animation.SelectAnimation({ 0, 0 }, { 0,0 });
	//QuitButton->collider = CollisionType::Box;

	Persistant::PersistantData* m_PersistentData = Persistant::PersistantData::instance();

	string persistentText = m_PersistentData->GetString("LastOption");

	TEntityUID textEnt = EntityManager->CreateEntity("Background", "PersistentText");
	cEntity* textEntity = EntityManager->GetEntity(textEnt);


	cTextComponent* textComp = new cTextComponent(textEntity, EntityManager->GetNewUID(), gfx, "Gabriola", 52.0f, persistentText);
	textEntity->AddComponent(textComp);

	EntityManager->CreateTemplate("UI", "Background");
	TEntityUID backgroundUID = EntityManager->CreateEntity("Background", "Backdrop");
	cEntity* background = EntityManager->GetEntity(backgroundUID);

	cRenderComponent* backgroundRenderer = new cRenderComponent(background, EntityManager->GetNewUID(), "landscape.png", gfx, 2);
	background->AddComponent(backgroundRenderer);

}

bool MainMenu::Update(double timeTotal, double timeDelta)
{
	frameTime = timeDelta;
	EntityManager->UpdateAllEntities(frameTime);

	cAnimatorComponent* spaceBtnAnim = (cAnimatorComponent*)EntityManager->GetEntity(*eSpaceShooterStartBtn)->GetComponent("AnimatorComponent");
	cAnimatorComponent* isomBtnAnim = (cAnimatorComponent*)EntityManager->GetEntity(*eIsometricStartBtn)->GetComponent("AnimatorComponent");
	cAnimatorComponent* exitBtnAnim = (cAnimatorComponent*)EntityManager->GetEntity(*eExitButton)->GetComponent("AnimatorComponent");

	spaceBtnAnim->SelectAnimation({ 0,0 }, { 0,0 });
	isomBtnAnim->SelectAnimation({ 0,0 }, { 0,0 });
	exitBtnAnim->SelectAnimation({ 0,0 }, { 0,0 });

	cColliderComponent* btnCollider = (cColliderComponent*)EntityManager->GetEntity(*eSpaceShooterStartBtn)->GetComponent("Collider");
	sFloat2 mousePos;
	mousePos.x = GetMousePos().x;
	mousePos.y = GetMousePos().y;
	//

	if (btnCollider->DetectPointCollision(mousePos))
	{
		spaceBtnAnim->SelectAnimation({ 2, 0 }, { 2, 0 });
		if (KeyHit(LeftMouseButton))
		{
			spaceBtnAnim->SelectAnimation({ 1, 0 }, { 1, 0 });

			GameController::SwitchLevel(new SpaceLevel());
			Persistant::PersistantData* m_PersistentData = Persistant::PersistantData::instance();
			m_PersistentData->SetString("LastOption", "SpaceGame");
			return true;
		}
	}

	btnCollider = (cColliderComponent*)EntityManager->GetEntity(*eIsometricStartBtn)->GetComponent("Collider");

	if (btnCollider->DetectPointCollision(mousePos))
	{
		isomBtnAnim->SelectAnimation({ 2, 0 }, { 2, 0 });
		if (KeyHit(LeftMouseButton))
		{
			isomBtnAnim->SelectAnimation({ 1, 0 }, { 1, 0 });

			GameController::SwitchLevel(new IsometricLevel());
			Persistant::PersistantData* m_PersistentData = Persistant::PersistantData::instance();
			m_PersistentData->SetString("LastOption", "IsometricScene");
			return true;
		}
	}

	btnCollider = (cColliderComponent*)EntityManager->GetEntity(*eExitButton)->GetComponent("Collider");

	if (btnCollider->DetectPointCollision(mousePos))
	{
		exitBtnAnim->SelectAnimation({ 2, 0 }, { 2, 0 });
		if (KeyHit(LeftMouseButton))
		{
			exitBtnAnim->SelectAnimation({ 1, 0 }, { 1, 0 });

			return false;
		}
	}

	return true;	// return false to quit application
}

void MainMenu::Unload()
{
	EntityManager->DestroyAllEntities();
	delete buttonTemplate;
	delete eSpaceShooterStartBtn;
	delete eIsometricStartBtn;
	delete eExitButton;
	delete MainMenuMusic;
}

void MainMenu::Render()
{
	gfx->ClearScreen(0.0f, 0.0f, 0.5f);
	EntityManager->DrawAllEntities(gfx);
}