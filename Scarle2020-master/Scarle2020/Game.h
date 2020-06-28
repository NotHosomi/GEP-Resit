//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <list>
#include "CommonStates.h"
#include "Effects.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Audio.h"

#include "PhysicsManager.h"
#include "FileManager.h"
#include "GameManager.h"
//Classes
#include "Weapons.h"

using std::list;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.

//Created using the template added to VS2019 by the vsix file from here:
//https://walbourn.github.io/direct3d-game-visual-studio-templates-redux/

// Forward declarations
class GameObject;
class GameObject2D;
class GameData;
class DrawData;
class DrawData2D;
class Camera;
class TPSCamera;
class Light;
class Sound;
class RenderTarget;

class Game
{
public:

    Game() noexcept;

    // Initialization and management
    void Initialize(HWND _window, int _width, int _height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int _width, int _height);

    // Properties
    void GetDefaultSize( int& _width, int& _height ) const;

    //SIMON'S ADDITIONAL STUFF

private:

    void Update(DX::StepTimer const& _timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

    //SIMON'S ADDITIONAL STUFF
    GameData* m_GD = NULL;			//Data to be shared to all Game Objects as they are ticked
    DrawData* m_DD = NULL;			//Data to be shared to all 3D Game Objects as they are drawn
    DrawData2D* m_DD2D = NULL;	    //Data to be passed by game to all 2D Game Objects via Draw 

    //Basic 3D renderers
    //SIMON:: Don't be scared  to remove my stuff you are not using. Remember it’s still here on the repo.
    Camera* m_cam = NULL; //principle camera
    TPSCamera* m_TPScam = NULL;//TPS cam
    Light* m_light = NULL; //base light

    //required for the CMO model rendering system
    DirectX::IEffectFactory* m_fxFactory = NULL;
    DirectX::CommonStates* m_states = NULL;

    list<GameObject*> m_GameObjects; //data structure to hold pointers to the 3D Game Objects
    list<GameObject2D*> m_GameObjects2D; //data structure to hold pointers to the 2D Game Objects

    //basic keyboard and mouse input system
    void ReadInput(); //Get current Mouse and Keyboard states
    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    std::unique_ptr<DirectX::Mouse> m_mouse;

    //sound stuff
	//This uses a simple system, but a better pipeline can be used using Wave Banks
	//See here: https://github.com/Microsoft/DirectXTK/wiki/Creating-and-playing-sounds Using wave banks Section
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
    list<Sound*>m_Sounds;

	void GamePlay();

	//worms
	void MovementInput();
	bool m_pressed[2] = { false, false }; //left , right
    int m_lastPressed = 0;
    //base line for 2D destrucable terrain
    RECT m_screenRect;
    RenderTarget* m_terrain = NULL;
    list<GameObject2D*> m_skyBox;
    GameObject2D* m_terrainBitMap = nullptr;
    list<GameObject2D*> m_alphaRemove;

    GameObject* pPlayer = nullptr;
    GameObject2D* pPlayer2d = nullptr;
	GameObject2D* m_water = nullptr; 

	//managers
    //SIMON:: Three things all called _SOMETHING_Manager might be expected to have a shared base class.
    //A more modular system would do this and then could data-wise include or not include for future projects.
	PhysicsManager* m_physicsManager = nullptr;
    FileManager* m_fileManager = nullptr;
    GameManager* m_gameManager = nullptr;
    Weapon OurWeapon;

	//gameplay
	enum ControlStates {
		LOCKED = 0,
		FREE,
		SELECTING_WEAPON,
		CHARGING,
		TARGETTING
	} ControlState;
	enum TurnStates
	{
		PREROUND = 0, // "are you ready", cannot act (5 secs or until input)
		ATTACKING,
		FLEEING,
		POSTROUND // Cycling through worms to display dmg and kill them
	} TurnState;

	void AttackMode();
	void Game::NewTurn();
	bool SelectWeapon(int weaponID);
	void Targetting(bool validate_target);

    GameObject2D* m_currentWeapon = nullptr;
    bool m_fired = false;
	int aiming_input = 0;
    bool shiftAiming = true;
	bool input_fire = false;

	int CurrentWeaponID = 0;
	int NumWeapons = 3; // increase with more added (currently just BAZOOKA and GRENADE)
	// timers
	float tmr_preround = 0;
	const float tmr_preround_duration = 5;
	float tmr_turn = 0;
	const float tmr_turn_duration = 60;
	float tmr_charge = 0;
	const float tmr_charge_duration = 2;
	float tmr_flee = 0;
	const float tmr_flee_duration = 5;

    //weaon testing
    GameObject2D* m_missile = nullptr;
    list<GameObject2D> projectiles;
    GameObject2D* m_arrow = nullptr;
    bool m_flipArrow = false;
    GameObject2D* m_mouseTarget = nullptr;
	bool pressed = false;
    
    Teams m_winColour;
    std::string m_winColourString = "";
    TextComponent* WinSateColour = nullptr;
};
