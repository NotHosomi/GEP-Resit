//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <time.h>

//Scarle Headers
#include "DrawData.h"
#include "DrawData2D.h"
#include "ObjectList.h"
#include "GameData.h"
#include "GameState.h"
#include "RenderTarget.h"
#include "Worms.h"
#include "DestructionCircle.h"

//DXTK Headers
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "CommonStates.h"

#include <iostream>

#define WEAPONS true

extern void ExitGame();

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND _window, int _width, int _height)
{
    m_window = _window;
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateDevice();

    CreateResources();

    //set up keyboard and mouse system
    //documentation here: https://github.com/microsoft/DirectXTK/wiki/Mouse-and-keyboard-input
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(_window);
   // m_mouse->SetMode(Mouse::MODE_RELATIVE);
    //Hide the mouse pointer
    ShowCursor(true);

    //seed the random number generator
    srand((UINT)time(NULL));

    //set up systems for 2D rendering
    m_DD2D = new DrawData2D();
    m_DD2D->m_Sprites.reset(new SpriteBatch(m_d3dContext.Get()));
    m_DD2D->m_Font.reset(new SpriteFont(m_d3dDevice.Get(), L"..\\Fonts\\italic.spritefont"));
    m_states = new CommonStates(m_d3dDevice.Get());

    //set audio system
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);

    //set up systems for 3D rendering
    
    //set up DirectXTK Effects system
    m_fxFactory = new EffectFactory(m_d3dDevice.Get());
    //Tell the fxFactory to look to the correct build directory to pull stuff in from
    ((EffectFactory*)m_fxFactory)->SetDirectory(L"..\\Models");
    //init render system for VBGOs
    VBGO::Init(m_d3dDevice.Get());

    //find how big my window is to correctly calculate my aspect ratio
    float AR = (float)_width / (float)_height;

    //create a base camera
    m_cam = new Camera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3::Zero);
    m_cam->SetPos(Vector3(0.0f, 200.0f, 200.0f));
    m_GameObjects.push_back(m_cam);

    //create a base light
    m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
    m_GameObjects.push_back(m_light);

    //add Player
    pPlayer = new Player("BirdModelV1", m_d3dDevice.Get(), m_fxFactory);
    m_GameObjects.push_back(pPlayer);

    //add a secondary camera
    m_TPScam = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 10.0f, 50.0f));
    m_GameObjects.push_back(m_TPScam);

    //create DrawData struct and populate its pointers
    m_DD = new DrawData;
    m_DD->m_pd3dImmediateContext = nullptr;
    m_DD->m_states = m_states;
    m_DD->m_cam = m_cam;
    m_DD->m_light = m_light;
    
    //create GameData struct and populate its pointers
    m_GD = new GameData;
    m_GD->m_GS = GS_MAIN_MENU;

    TextGO2D* text = new TextGO2D("Test Text");
    text->SetPos(Vector2(100, 10));
    text->SetColour(Color((float*)&Colors::Yellow));
   // m_GameObjects2D.push_back(text);

    //Test Sounds
  //  Loop* loop = new Loop(m_audioEngine.get(), "NightAmbienceSimple_02");
  //  loop->SetVolume(0.1f);
 //   loop->Play();
   // m_Sounds.push_back(loop);

    TestSound* TS = new TestSound(m_audioEngine.get(), "Explo1");
   // m_Sounds.push_back(TS);

    m_terrain = new RenderTarget(m_d3dDevice.Get(), 1920, 1080);//set to size of viewport

    //skybox
    m_screenRect.left = 0;
    m_screenRect.top = 0;
    m_screenRect.right = 1920;
    m_screenRect.bottom = 1080;

    //how to create a shader resource view
    ImageGO2D* background = new ImageGO2D("skyBoxMountains", m_d3dDevice.Get());
    m_skyBox.push_back(background);
  
    m_terrainBitMap = new ImageGO2D("TerrianHD", m_d3dDevice.Get());

   // ID3D11ShaderResourceView* srv = m_d3dDevice->CreateShaderResourceView()
   // m_terrain->SetShaderResourceView(((ImageGO2D*)m_terrainBitMap)->GetShaderResource(), m_d3dContext.Get());
   
    //DestructionCircle* imageDestruction = new DestructionCircle("CircleHitAlpha_1", m_d3dDevice.Get(), 0.5f, Vector2(100, 250), m_gameManager->currentLevel()->objects);
   // m_alphaRemove.push_back(imageDestruction);

    m_arrow = new GameObject2D();

	//init managers
	m_water = new GameObject2D();
	m_water->SetPos(Vector2(0, 1000));
    m_gameManager = new GameManager(m_d3dDevice.Get());
	m_physicsManager = new PhysicsManager();
    m_fileManager = new FileManager();
    m_fileManager->readJSON("worms2d_levels.json", "WeaponConfig.json", m_d3dDevice.Get(), m_d3dContext.Get());
    m_gameManager->initGame(m_fileManager->getGameData(), m_fileManager->getLevels());
    m_gameManager->loadStartLevel();

    m_mouseTarget = new GameObject2D();
    m_mouseTarget->SetScale(0.2f);
    m_mouseTarget->SetSprite("TargetSight", m_d3dDevice.Get());
    m_mouseTarget->spriteComp()->setVisibility(false);

	NewTurn();
}
 
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& _timer)
{
    float elapsedTime = float(_timer.GetElapsedSeconds());
    m_GD->m_dt = elapsedTime;

    //this will update the audio engine but give us chance to do somehting else if that isn't working
    if (!m_audioEngine->Update())
    {
        if (m_audioEngine->IsCriticalError())
        {
            // We lost the audio device!
        }
    }
    else
    {
        //update sounds playing
        for (list<Sound*>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
        {
            (*it)->Tick(m_GD);
        }
    }

    ReadInput();

    //basically if youre not in menu (change to not be hard coded to only the first level)


	////pause all this below 
	//if(m_gameManager->pauseScene()->pause = false)???

	switch (m_GD->m_GS)
	{
	case GS_PLAY_MAIN_CAM:
		GamePlay();
		break;
	case GS_PAUSE:
		//show pause ui options
		if (m_mouse->GetState().leftButton && !pressed)
		{
			m_gameManager->updateScene(Vector2(m_mouse->GetState().x, m_mouse->GetState().y), m_GD, m_d3dDevice.Get(), m_alphaRemove);
			//m_gameManager->currentLevel()->objects.push_back(m_water);
			pressed = true;
		}
		else if (!m_mouse->GetState().leftButton && pressed)
		{
			pressed = false;
		}
		break;
	case GS_MAIN_MENU:
		//show menus e.g. load screen and then level editor screen
		if (m_mouse->GetState().leftButton && !pressed)
		{
			m_gameManager->updateScene(Vector2(m_mouse->GetState().x, m_mouse->GetState().y), m_GD, m_d3dDevice.Get(), m_alphaRemove);
			//m_gameManager->currentLevel()->objects.push_back(m_water);
			pressed = true;
		}
		else if (!m_mouse->GetState().leftButton && pressed)
		{
			pressed = false;
		}
        if (WinSateColour && m_gameManager->currentLevel()->id != m_gameManager->lastLevel()->id)
        {
            delete(WinSateColour);
            WinSateColour = nullptr;
        }
		break;
	}

    elapsedTime;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();
	//Code potentially for drawing into the Terrain RenderTarget
	//Draw stuff to the render texture
	if (m_gameManager->currentLevel()->terrain)//m_gameManager->currentLevel()->terrain)
	{
		m_terrain->Begin(m_d3dContext.Get());
		m_terrain->ClearRenderTarget(m_d3dContext.Get(), 0.f, 0.f, 0.f, 0.f);
		m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
		//draw Terrain
		m_DD2D->m_Sprites->Draw(m_gameManager->currentLevel()->terrain->spriteComp()->getTexture(), m_screenRect, nullptr, Colors::White);
		m_DD2D->m_Sprites->End();
		m_terrain->End(m_d3dContext.Get());
	}
    //set immediate context of the graphics device
    m_DD->m_pd3dImmediateContext = m_d3dContext.Get();

    //set which camera to be used
    m_DD->m_cam = m_cam;
    if (m_GD->m_GS == GS_PLAY_TPS_CAM)
    {
        m_DD->m_cam = m_TPScam;
    }

    //update the constant buffer for the rendering of VBGOs
    VBGO::UpdateConstantBuffer(m_DD);

    //SIMON:: There's a lot of special drawing of objects here that could possibly be pulled 
    //to the objects themselves, or more of a render pass idea.

    //Code potentially for digging from the Terrain
    //Destruction of the terrain
    m_terrain->Begin(m_d3dContext.Get());
    m_d3dContext->OMSetBlendState(m_terrain->GetDigBlend(), 0, 0xffffff);
    m_DD2D->m_Sprites->Begin(DirectX::SpriteSortMode_Deferred, m_terrain->GetDigBlend());
        //Draw Destruction here
    for (auto& point : m_alphaRemove)
    {
        point->Draw(m_DD2D);
    }
    m_DD2D->m_Sprites->End();
    m_terrain->End(m_d3dContext.Get());

    //draw the skybox at the back
    m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());

	
	if(m_gameManager->currentLevel()->background)
		m_DD2D->m_Sprites->Draw(m_gameManager->currentLevel()->background->spriteComp()->getTexture(), m_screenRect, nullptr, Colors::White);

	//draw rendertarget on top of skybox
	m_DD2D->m_Sprites->Draw(m_terrain->GetShaderResourceView(), XMFLOAT2(0.0f, 0.0f));
	
    m_DD2D->m_Sprites->End();

    // Draw sprite batch stuff 
    m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
	
	for (auto& obj : m_gameManager->currentLevel()->objects)
	{
        if (obj->spriteComp() && obj->spriteComp()->getVisibility() == false)
            continue;
        obj->Draw(m_DD2D);
	}
    if (m_arrow->spriteComp()->getVisibility() && m_GD->m_GS != GS_MAIN_MENU)
    {
        if(m_flipArrow)
            m_DD2D->m_Sprites->Draw(m_arrow->spriteComp()->getTexture(), m_arrow->GetPos(), nullptr, Colors::White, m_arrow->GetRot(), m_arrow->GetOrigin(), m_arrow->GetScale(), DirectX::SpriteEffects::SpriteEffects_FlipHorizontally);
        else
            m_DD2D->m_Sprites->Draw(m_arrow->spriteComp()->getTexture(), m_arrow->GetPos(), nullptr, Colors::White, m_arrow->GetRot(), m_arrow->GetOrigin(), m_arrow->GetScale());
    }
    if (m_mouseTarget->spriteComp()->getVisibility() && m_GD->m_GS != GS_MAIN_MENU)
        m_mouseTarget->Draw(m_DD2D);
	
    m_DD2D->m_Sprites->End();

    m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());

    for (auto& obj : m_gameManager->currentLevel()->objects)
    {
        if (obj->textComponent() && obj->spriteComp() && obj->spriteComp()->getVisibility())
        {
            TextComponent* tComp = obj->textComponent();
            tComp->Draw(m_DD2D, Vector2(obj->GetPos().x + obj->getWidth() / 5 + tComp->getXOffSet(), (obj->GetPos().y + obj->getHeight() / 5) + tComp->getYOffSet()), tComp->getColor(), obj->GetRot(), obj->GetOrigin(), tComp->getScale());
        }
    }

    if (WinSateColour)
        WinSateColour->Draw(m_DD2D, Vector2(500,600), Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0, Vector2(0,0), Vector2(3.0f, 3.0f));

    m_DD2D->m_Sprites->End();

    //Draw 3D Game Obejects
    for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
    {
       // (*it)->Draw(m_DD);
    }

    //drawing text screws up the Depth Stencil State, this puts it back again!
    m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    Color clearCol(0.0f, 0.3f, 0.8f, 0.0f);
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::Black);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int _width, int _height)
{
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& _width, int& _height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    _width = 800;
    _height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
   // creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
        ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            m_d3dDevice.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            m_swapChain.ReleaseAndGetAddressOf()
            ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}

void Game::ReadInput()
{
    m_GD->m_KBS = m_keyboard->GetState();
    m_GD->m_KBS_tracker.Update(m_GD->m_KBS);
    //quit game on hiting escape
    if (m_GD->m_KBS.Escape)
    {
        ExitGame();
    }
    m_GD->m_input.updateInputs(&m_GD->m_KBS_tracker);

 //  m_GD->m_MS = m_mouse->GetState();

    //lock the cursor to the centre of the window
    RECT window;
    GetWindowRect(m_window, &window);
    //SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);
}

void Game::GamePlay()
{
    m_physicsManager->UpdatePhysicsEngine(m_terrain, m_d3dContext.Get(), m_GD, m_gameManager->currentLevel()->objects, m_d3dDevice.Get(),
        m_alphaRemove, m_missile, m_gameManager->currentWorm());
    OurWeapon.MaintainProjectiles(m_gameManager->currentLevel()->objects, m_GD);
	
    switch (TurnState)
    {
    case TurnStates::PREROUND:
        tmr_preround += m_GD->m_dt;
        if (tmr_preround > tmr_charge_duration ||
            m_GD->m_input.any())
        {
            TurnState = TurnStates::ATTACKING;
        }
        break;
    case TurnStates::ATTACKING:
#pragma region ATK
        m_arrow->spriteComp()->setVisibility(true);
        m_arrow->SetOrigin(Vector2(m_gameManager->currentWeapon()->GetPos().x + m_gameManager->currentWeapon()->weaponComponent()->getWidth() / 2, m_gameManager->currentWeapon()->GetPos().y + m_gameManager->currentWeapon()->weaponComponent()->getHeight() / 2));
        //m_mouseTarget->spriteComp()->setVisibility(true);
        if (ControlState == ControlStates::FREE)
        {
            if (!m_gameManager->currentWorm())
                return;
            int x = m_gameManager->currentWorm()->GetPos().x + (m_gameManager->currentWorm()->getWidth() - m_gameManager->currentWeapon()->weaponComponent()->getWidth() * m_arrow->GetScale().x);
            int y = m_gameManager->currentWorm()->GetPos().y + (m_gameManager->currentWorm()->getHeight() / 1.2 - m_gameManager->currentWeapon()->weaponComponent()->getHeight() * m_arrow->GetScale().y);
            m_arrow->SetPos(Vector2(x, y));

            // Fetch player movement
            MovementInput();

            // SwitchWeapons
            WeaponSwitching();

            if (m_gameManager->currentWorm()->physicsComponent())
            {
                // Not moving, thus aim
                if (m_gameManager->currentWorm()->physicsComponent()->getVelocity() == Vector2(0, 0))
                {
                    AttackMode();
                    if (m_gameManager->currentWeapon()->weaponComponent()->getAttackType() == WeaponData::W_HOMINGMISSILE)
                    {
                        if (m_mouse->GetState().leftButton)
                        {
                            m_gameManager->currentWeapon()->weaponComponent()->setTarget(m_mouse->GetState().x, m_mouse->GetState().y);
                            m_gameManager->currentWeapon()->weaponComponent()->setTargetSet(true);
                        }
                        else if (!m_gameManager->currentWeapon()->weaponComponent()->getTargetSet())
                        {
                            m_mouseTarget->SetPos(Vector2(m_mouse->GetState().x, m_mouse->GetState().y));
                            m_mouseTarget->spriteComp()->setVisibility(true);
                        }
                    }
                    else
                    {
                        m_mouseTarget->spriteComp()->setVisibility(false);
                    }
                }
            }
        }
        else if (ControlState == ControlStates::CHARGING)
        {
            tmr_charge += m_GD->m_dt;
            if (m_GD->m_KBS_tracker.released.F)
            {
                if (m_gameManager->currentWeapon()->weaponComponent()->getAimType() == WeaponData::W_HOMINGMISSILE && !m_gameManager->currentWeapon()->weaponComponent()->getTargetSet())
                {
                    return;
                }
                m_gameManager->currentWeapon()->weaponComponent()->setCharge(tmr_charge / tmr_charge_duration);
                OurWeapon.fire(m_d3dDevice.Get(), m_gameManager->currentLevel()->objects, m_arrow->GetPos(), m_gameManager->currentWeapon());
                OurWeapon.incrementShotsFired();
                tmr_charge = 0;
                m_fired = true;
                int size = 1;

                m_currentWeapon = m_gameManager->currentLevel()->objects.back();
            }
            else if (m_fired && !m_currentWeapon->spriteComp()->getVisibility() && OurWeapon.getShotsFired() == m_currentWeapon->weaponComponent()->ammoCount())
            {
                TurnState = TurnStates::FLEEING;
                OurWeapon.resetShotsFired();
                m_fired = false;
            }
        }
#pragma endregion
        break;
    case TurnStates::FLEEING:
        m_arrow->spriteComp()->setVisibility(false);
        MovementInput();
        tmr_flee += m_GD->m_dt;
        if (tmr_flee >= tmr_flee_duration)
        {
            tmr_flee = 0;
            TurnState = TurnStates::POSTROUND;
        }
        break;
    case TurnStates::POSTROUND:
#pragma region POST

        // Camera track moving objects
        // Apply damage taken to worms, and trigger kill explosions
        // if only 1 team has living worms
        // { end game }
        // if all worms damageTaken == 0
        list<GameObject2D*> playersAlive;
        for (auto& player : m_gameManager->currentLevel()->objects)
        {
            if (player->playerComponent() && player->playerComponent()->health() > 0)
                playersAlive.push_back(player);
        }
        if (playersAlive.size() == 1)
        {
            //game ends
            // winner is the player whos in the list
            m_gameManager->loadLevel(m_gameManager->lastLevel()->id);
            m_GD->m_GS = GS_MAIN_MENU;
            m_winColour = playersAlive.front()->playerComponent()->getTeam();
            switch (m_winColour)
            {
            case Teams::BLUE:
                m_winColourString = "BLUE";
                break;
            case Teams::RED:
                m_winColourString = "RED";
                break;
            case Teams::PURPLE:
                m_winColourString = "PURPLE";
                break;
            case Teams::GREEN:
                m_winColourString = "GREEN";
                break;
            }
            WinSateColour = new TextComponent();
            WinSateColour->text(m_winColourString);
        }
        else
        {
            NewTurn();
            playersAlive.clear();
        }
#pragma endregion
        break;
    }
    
    if (m_mouse->GetState().leftButton)
	{
        // Not sure what this is for
		m_gameManager->updateScene(Vector2(m_mouse->GetState().x, m_mouse->GetState().y), m_GD, m_d3dDevice.Get(), m_alphaRemove);
	}

	for (list<GameObject2D*>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
	{
		(*it)->Tick(m_GD);
	}

	for (list<GameObject2D*>::iterator it = m_gameManager->currentLevel()->objects.begin(); it != m_gameManager->currentLevel()->objects.end(); it++)
	{
		(*it)->Tick(m_GD);
		if ((*it)->GetPos().y > m_water->GetPos().y && (*it)->playerComponent())
		{
			(*it)->spriteComp()->setVisibility(false);
            (*it)->playerComponent()->health(0);
			//kill worm
		}
	}
}



void Game::MovementInput()
{
	if (!m_gameManager->currentWorm()->physicsComponent())
	{
		return;
	}
    // absolute garbage!
#if 0
	//pressed
	if (m_GD->m_KBS_tracker.pressed.J)
	{
		m_pressed[0] = true;
        m_gameManager->currentWorm()->spriteComp()->animationComponent()->setFlipX(true);
	}
	else if (m_GD->m_KBS_tracker.pressed.K)
	{
		m_pressed[1] = true;
        m_gameManager->currentWorm()->spriteComp()->animationComponent()->setFlipX(false);
	}
	//released
	if (m_GD->m_KBS_tracker.released.J)
	{
		m_pressed[0] = false;
		m_gameManager->currentWorm()->physicsComponent()->setVelX(0);
	}
	else if (m_GD->m_KBS_tracker.released.K)
	{
		m_pressed[1] = false;
		m_gameManager->currentWorm()->physicsComponent()->setVelX(0);
	}
	//action
	if (m_pressed[0])
	{
		m_gameManager->currentWorm()->physicsComponent()->setVelX(-1);
        m_gameManager->currentWorm()->spriteComp()->animationComponent()->nextFrame(m_GD->m_dt);
	}
	else if (m_pressed[1])
	{
		m_gameManager->currentWorm()->physicsComponent()->setVelX(1);
        m_gameManager->currentWorm()->spriteComp()->animationComponent()->nextFrame(m_GD->m_dt);
	}
	//add into physics manager
	m_gameManager->currentWorm()->physicsComponent()->move(m_GD->m_dt, m_gameManager->currentWorm()->GetPos());
	// m_currentWorm->SetPos(vec);

	 //jump
	if (m_GD->m_KBS_tracker.pressed.I)
	{
		m_gameManager->currentWorm()->physicsComponent()->jump(m_GD->m_dt);
	}
#endif
    m_gameManager->currentWorm->physicsComponent()->playerMove(&m_GD->m_input, m_GD->m_dt);
}

void Game::WeaponSwitching()
{
    if (m_GD->m_input.checkKey(InputManager::IN_NEXT))
    {
        m_gameManager->currentWeapon()->weaponComponent()->setAimAngle(90);
        m_gameManager->nextWeapon();
        float w = 30.0f / m_gameManager->currentWeapon()->weaponComponent()->getWidth();
        float h = 30.0f / m_gameManager->currentWeapon()->weaponComponent()->getHeight();
        m_arrow->copySpriteComp(m_gameManager->currentWeapon()->weaponComponent()->spriteComp());
        m_arrow->SetScale(Vector2(w, h));
    }
    if (m_GD->m_input.checkKey(InputManager::IN_PREV))
    {
        //back a 
        m_gameManager->currentWeapon()->weaponComponent()->setAimAngle(90);
        m_gameManager->previousWeapon();
        float w = 30.0f / m_gameManager->currentWeapon()->weaponComponent()->getWidth();
        float h = 30.0f / m_gameManager->currentWeapon()->weaponComponent()->getHeight();
        m_arrow->copySpriteComp(m_gameManager->currentWeapon()->weaponComponent()->spriteComp());
        m_arrow->SetScale(Vector2(w, h));
    }
}

void Game::AttackMode()
{
	// AIMING
	if (m_gameManager->currentWeapon()->weaponComponent()->getAimType() == WeaponData::AimTypes::AIMED ||
        m_gameManager->currentWeapon()->weaponComponent()->getAimType() == WeaponData::AimTypes::TARGETTED)
	{
		// TODO: Fetch worm facing dir
		bool mirrored = false;
        aiming_input = 0;
		//pressed
		if (m_GD->m_KBS_tracker.pressed.W && shiftAiming)
		{
			aiming_input += 5;
            shiftAiming = false;
		}
		if (m_GD->m_KBS_tracker.pressed.S && shiftAiming)
		{
			aiming_input -= 5;
            shiftAiming = false;
		}
		//released
		if (m_GD->m_KBS_tracker.released.W || m_GD->m_KBS_tracker.released.S)
		{
			//aiming_input--;

		}
		if (m_GD->m_KBS_tracker.released.S)
		{
			//aiming_input++;
		}
        float newAngle =  newAngle = m_gameManager->currentWeapon()->weaponComponent()->getAimAngle() + aiming_input * m_GD->m_dt;
		if (mirrored)
		{
			newAngle -= 180;
		}
		if (newAngle > 179)
		{
			newAngle == 179;
		}
		else if (newAngle < 1)
		{
			newAngle == 1;
		}
		if (mirrored)
		{
			newAngle += 180;
		}
        if (m_arrow->GetRot() > 180 || m_arrow->GetRot() < 0)
            m_flipArrow = true;
        else
            m_flipArrow = false;

        if (!shiftAiming)
        {
            m_gameManager->currentWeapon()->weaponComponent()->setAimAngle(newAngle);
            shiftAiming = true;
            m_arrow->SetRot(newAngle);
        }
	}

	if ((m_GD->m_KBS_tracker.pressed.F && OurWeapon.getShotsFired() < m_gameManager->currentWeapon()->weaponComponent()->ammoCount()) || (m_gameManager->currentWeapon()->weaponComponent()->fireRate() != OurWeapon.getFireRateSoFar() && OurWeapon.getFireRateSoFar() != 0))
	{
		if (m_gameManager->currentWeapon()->weaponComponent()->requiresCharging())
		{
			ControlState = ControlStates::CHARGING;
			std::cout << "Charge Begun" << std::endl;
		}
		else
		{
            bool fire = m_gameManager->currentWeapon()->weaponComponent()->fireNextRound(m_GD->m_dt);
            if(m_gameManager->currentWeapon()->weaponComponent()->fireRate() != OurWeapon.getFireRateSoFar() && fire || OurWeapon.getFireRateSoFar() == 0)
			    OurWeapon.fire(m_d3dDevice.Get(), m_gameManager->currentLevel()->objects, m_arrow->GetPos(), m_gameManager->currentWeapon());
            if(m_gameManager->currentWeapon()->weaponComponent()->fireRate() == OurWeapon.getFireRateSoFar())
                OurWeapon.incrementShotsFired();
     
            if (OurWeapon.getShotsFired() == m_gameManager->currentWeapon()->weaponComponent()->ammoCount())
            {
                TurnState = TurnStates::FLEEING;
                OurWeapon.resetShotsFired();
            }
		}
	}
}


#ifdef WEAPONS

// Selecting a weapon from the weapon menu
bool Game::SelectWeapon(int weaponID)
{
#if 0
	if (Teams[OurWorm.TeamID].WeaponList[weaponID] == 0)
	{
		// This worm's team does not have this weapon to use
		return false;
	}
#endif
    //std::list<GameObject2D*> weapons = m_gameManager->gameData()->weapons;

	//OurWeapon.EquipWeapon(WepInfo);

	//if (WepInfo.m_aimType == WeaponData::AimTypes::TARGETTED || WepInfo.m_aimType == WeaponData::AimTypes::U_HOMINGMISSILE)
	//{
	//	ControlState = ControlStates::TARGETTING;
	//}
	return true;
}

/*
* Purpose: Selecting a coordinate for targetted weapons e.g. teleport
*/
void Game::Targetting(bool validate_target)
{
	// Gameloop for when in targetting mode
	// TODO: MOVING TARGET

	// Upon Valid target selected
	ControlState = ControlStates::FREE;
	if (m_gameManager->currentWeapon()->weaponComponent()->getAimType() == WeaponData::AimTypes::U_HOMINGMISSILE)
	{
        if(!m_gameManager->currentWeapon()->weaponComponent()->getTargetSet())
		    return;
	}
	OurWeapon.fire(m_d3dDevice.Get(), m_gameManager->currentLevel()->objects, m_arrow->GetPos(), m_gameManager->currentWeapon());
}
#endif

void Game::NewTurn()
{
	// TODO: currentWorm = next worm in queue
	TurnState = TurnStates::PREROUND;
	ControlState = ControlStates::FREE;
    m_pressed[0] = false;
    m_pressed[1] = false;
    m_gameManager->nextWeapon();
    m_gameManager->currentWeapon()->weaponComponent()->setAimAngle(90);
    float w = 30.0f / m_gameManager->currentWeapon()->weaponComponent()->getWidth();
    float h = 30.0f / m_gameManager->currentWeapon()->weaponComponent()->getHeight();
    m_arrow->copySpriteComp(m_gameManager->currentWeapon()->weaponComponent()->spriteComp());
    m_arrow->SetScale(Vector2(w, h));
	// TODO fetch which direction current worm is facing
	bool mirrored = true;
	if (mirrored)
	{
        m_gameManager->currentWeapon()->weaponComponent()->setAimAngle(270);
	}
	tmr_preround = 0;
	tmr_turn = 0;
	tmr_charge = 0;
	tmr_flee = 0;
    m_gameManager->nextPlayer();
}