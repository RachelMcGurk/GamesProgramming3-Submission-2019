#include "pch.h"
#include "Application.h"
#include "Common.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "RigidBody.h"

Application* Application::m_application = nullptr;
Application::Application()
{
}
void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), Log::Error);
		exit(-1);
	}
	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//creating window
	m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

	SDL_CaptureMouse(SDL_TRUE);

	joystick = SDL_JoystickOpen(0);
	haptic = SDL_HapticOpenFromJoystick(joystick);

	if (joystick == NULL)
	{
		LOG_DEBUG("No controller connected", Log::Warning);
	}
	else
	{
		LOG_DEBUG(SDL_JoystickName(joystick), Log::Trace);
	}

	if (haptic == NULL)
	{
		LOG_DEBUG("Controller does not support haptics", Log::Warning);
	}
	else
	{
		if (SDL_HapticRumbleInit(haptic) < 0)
		{
			LOG_DEBUG("No haptic connected", Log::Warning);
		}
	}

	OpenGlInit();
	GameInit();

}
void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();
	SDL_GL_SetSwapInterval(1);
	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) <<
			std::endl;
		exit(-1);
	}

	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));

	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));

	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//turn on back face culling
	//GL_ATTEMPT(glEnable(GL_CULL_FACE));
	//GL_ATTEMPT(glCullFace(GL_BACK));

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::GameInit()
{
	//loading all resources
	Resources::GetInstance()->AddModel("Cube.obj");
	Resources::GetInstance()->AddModel("heart.obj");
	Resources::GetInstance()->AddModel("head.obj");
	Resources::GetInstance()->AddModel("ball.obj");
	Resources::GetInstance()->AddModel("hand.obj");
	Resources::GetInstance()->AddModel("banana.obj");
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddTexture("head_texture.jpg");
	Resources::GetInstance()->AddTexture("heart_texture.jpg");
	Resources::GetInstance()->AddTexture("hand_Texture.jpg");
	Resources::GetInstance()->AddTexture("banana_texture.jpg");
	Resources::GetInstance()->AddTexture("basketball_texture.jpg");
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "simple");
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "blinnPhong_VERT.glsl", ASSET_PATH + "blinnPhong_FRAG.glsl"), "blinnPhong");

	//creates floor object
	Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("Cube.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("Wood.jpg"))
	);
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f, 100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));

	// creates camera component
	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	cc->Start();

	//creates array of heart objects
	for (int i = 0; i < 5; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("heart.obj"),
				Resources::GetInstance()->GetShader("blinnPhong"),
				Resources::GetInstance()->GetTexture("heart_texture.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -50.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(3.5f, 3.5f, 3.5f)));
		a->GetTransform()->SetScale(glm::vec3(0.8f, 0.8f, 0.8f));
	}

	for (int i = 0; i < 5; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("ball.obj"),
				Resources::GetInstance()->GetShader("blinnPhong"),
				Resources::GetInstance()->GetTexture("basketball_texture.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -10.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(4.8f, 4.8f, 4.8f)));
		a->GetTransform()->SetScale(glm::vec3(5.f, 5.f, 5.f));
	}

	for (int i = 0; i < 5; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("hand.obj"),
				Resources::GetInstance()->GetShader("blinnPhong"),
				Resources::GetInstance()->GetTexture("hand_Texture.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(-20.0f, 5.f * i, -10.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(3.5f, 3.5f, 3.5f)));
		a->GetTransform()->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	}

	for (int i = 0; i < 5; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("banana.obj"),
				Resources::GetInstance()->GetShader("blinnPhong"),
				Resources::GetInstance()->GetTexture("banana_texture.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(20.0f, 5.f * i, -10.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.8f, 1.8f, 1.8f)));
		a->GetTransform()->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
	}

	for (int i = 0; i < 5; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("head.obj"),
				Resources::GetInstance()->GetShader("blinnPhong"),
				Resources::GetInstance()->GetTexture("head_texture.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(20.0f, 5.f * i, -50.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(5.0f, 5.0f, 5.0f)));
		a->GetTransform()->SetScale(glm::vec3(0.8f, 0.8f, 0.8f));
	}
}

void Application::Loop()
{
	m_appState = AppState::RUNNING;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	INPUT->SetAxis(0, false);
	INPUT->SetAxis(1, false);
	INPUT->SetAxis(2, false);
	INPUT->SetAxis(3, false);
	INPUT->SetAxis(4, false);
	INPUT->SetAxis(5, false);
	INPUT->SetAxis(6, false);
	INPUT->SetAxis(7, false);

	INPUT->SetButton(0, false);
	INPUT->SetButton(1, false);
	INPUT->SetButton(2, false);
	INPUT->SetButton(3, false);

	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:
				INPUT->SetKey(event.key.keysym.sym, true);
				break;
				
			case SDL_KEYUP:
				INPUT->SetKey(event.key.keysym.sym, false);
				break;
				//record when the user releases a key
			case SDL_MOUSEMOTION:
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				glm::ivec2 movementPos = INPUT->GetMouseDelta();

				m_entities.at(1)->GetTransform()->RotateEulerAxis(movementPos.x * mouseSensitivity, glm::vec3(0, 1, 0));
				m_entities.at(1)->GetTransform()->RotateEulerAxis(movementPos.y * mouseSensitivity, m_entities.at(1)->GetTransform()->GetRight());
				break;

			case SDL_JOYAXISMOTION:
				if (event.jaxis.which == 0)
				{
					if (event.jaxis.axis == 0)
					{
						// below dead zone
						if (event.jaxis.value < -8000)
						{
							//left movement
							INPUT->SetAxis(0, true);
						}
						//above dead zone
						else if (event.jaxis.value > 8000)
						{
							//right movement
							INPUT->SetAxis(1, true);
						}
						else
						{
							INPUT->SetAxis(0, false);
							INPUT->SetAxis(1, false);
						}
					}
					if (event.jaxis.axis == 1)
					{
						if (event.jaxis.value < -8000)
						{
							//forawrd movement
							INPUT->SetAxis(2, true);
						}
						else if (event.jaxis.value > 8000)
						{
							//backward movement
							INPUT->SetAxis(3, true);
						}
						else
						{
							INPUT->SetAxis(2, false);
							INPUT->SetAxis(3, false);
						}
					}
					if (event.jaxis.axis == 2)
					{
						if (event.jaxis.value < -8000)
						{
							//left rotation
							INPUT->SetAxis(4, true);
						}
						else if (event.jaxis.value > 8000)
						{
							//right rotation
							INPUT->SetAxis(5, true);
						}
						else
						{
							INPUT->SetAxis(4, false);
							INPUT->SetAxis(5, false);
						}
					}
					if (event.jaxis.axis == 3)
					{
						if (event.jaxis.value < -8000)
						{
							//up rotation
							INPUT->SetAxis(6, true);
						}
						else if (event.jaxis.value > 8000)
						{
							//down rotation
							INPUT->SetAxis(7, true);
						}
						else
						{
							INPUT->SetAxis(6, false);
							INPUT->SetAxis(7, false);
						}
					}
				}
				break;
			case SDL_JOYBUTTONDOWN:
				if (event.jaxis.which == 0)
				{
					//L1
					if (event.jbutton.button == 9)
					{
						INPUT->SetButton(0, true);
					}
					//L2
					if (event.jbutton.button == 10)
					{
						INPUT->SetButton(1, true);
					}

					//triangle
					if (event.jbutton.button == 3)
					{
						INPUT->SetButton(2, true);
						m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0, 1, 0));
					}
					else
					{
						INPUT->SetButton(2, false);
					}
					// x
					if (event.jbutton.button == 0)
					{
						INPUT->SetButton(3, true);
						m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0, -1, 0));
					}
					else
					{
						INPUT->SetButton(3, false);
					}
				}
				break;
			case SDL_JOYBUTTONUP:
				if (event.jaxis.which == 0)
				{
					if (event.jbutton.button == 9)
					{
						INPUT->SetButton(0, false);
					}
					//L2
					if (event.jbutton.button == 10)
					{
						INPUT->SetButton(1, false);
					}

					if (event.jbutton.button == 3)
					{
						INPUT->SetButton(2, false);
					}
					// x
					if (event.jbutton.button == 0)
					{
						INPUT->SetButton(3, false);
					}
				}
				break;
			}
		}

		Movement();


		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		// update and render all entities
		Physics::GetInstance()->Update(deltaTime);

		//set uniforms for multiple shaders
		Resources::GetInstance()->GetShader("simple")->Use();
		Resources::GetInstance()->GetShader("simple")->setMat4("view", Application::GetInstance()->GetCamera()->GetView());
		Resources::GetInstance()->GetShader("simple")->setMat4("projection", Application::GetInstance()->GetCamera()->GetProj());
		Resources::GetInstance()->GetShader("simple")->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

		Resources::GetInstance()->GetShader("blinnPhong")->Use();
		Resources::GetInstance()->GetShader("blinnPhong")->setMat4("view", Application::GetInstance()->GetCamera()->GetView());
		Resources::GetInstance()->GetShader("blinnPhong")->setMat4("projection", Application::GetInstance()->GetCamera()->GetProj());
		Resources::GetInstance()->GetShader("blinnPhong")->setVec3("viewPos", Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition());
		Resources::GetInstance()->GetShader("blinnPhong")->setVec3("lightColour", glm::vec3(1.0f, 1.0f, 1.0f));
		Resources::GetInstance()->GetShader("blinnPhong")->setVec3("lightDir", glm::vec3(-5.0f, 6.0f, 7.0f));




		Update(deltaTime);
		Render();
		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Movement()
{
	//w - forward
	if (INPUT->GetKey(SDLK_w))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetForward());
	}
	//s - backwards
	else if (INPUT->GetKey(SDLK_s))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetForward());
	}
	//a - right
	else if (INPUT->GetKey(SDLK_a))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetRight());
	}
	//d - left
	else if (INPUT->GetKey(SDLK_d))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetRight());
	}

	//space - up
	if (INPUT->GetKey(SDLK_SPACE))
	{
		m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0, 1, 0));
	}
	//shift - down
	else if (INPUT->GetKey(SDLK_LSHIFT))
	{
		m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0, -1, 0));
	}

	//up - force
	if (INPUT->GetKey(SDLK_UP))
	{
		for (int i = 2; i < m_entities.size(); i++)
		{
			m_entities.at(i)->GetComponent<RigidBody>()->ApplyForce(btVector3(0, 5, 0));
		}
	}
	//down - torque
	if (INPUT->GetKey(SDLK_DOWN))
	{
		for (int i = 2; i < m_entities.size(); i++)
		{
			m_entities.at(i)->GetComponent<RigidBody>()->ApplyTorque(btVector3(0, 5, 0));
		}
	}

	//escape - quit
	if (INPUT->GetKey(SDLK_ESCAPE))
	{
		m_appState = AppState::QUITTING;
	}

	//left movement
	if (INPUT->GetAxis(0))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetRight());
	}
	//right movement
	else if (INPUT->GetAxis(1))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetRight());
	}

	//forward movement
	if (INPUT->GetAxis(2))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetForward());
	}
	//backward movement
	else if (INPUT->GetAxis(3))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetForward());
	}

	//left rotation
	if (INPUT->GetAxis(4))
	{
		m_entities.at(1)->GetTransform()->RotateEulerAxis(-1, glm::vec3(0, 1, 0));
	}
	//right rotation
	else if (INPUT->GetAxis(5))
	{
		m_entities.at(1)->GetTransform()->RotateEulerAxis(1, glm::vec3(0, 1, 0));
	}

	//left rotation
	if (INPUT->GetAxis(6))
	{
		m_entities.at(1)->GetTransform()->RotateEulerAxis(-1, m_entities.at(1)->GetTransform()->GetRight());
	}
	//right rotation
	else if (INPUT->GetAxis(7))
	{
		m_entities.at(1)->GetTransform()->RotateEulerAxis(1, m_entities.at(1)->GetTransform()->GetRight());
	}

	//L1
	if (INPUT->GetButton(0))
	{
		for (int i = 2; i < m_entities.size(); i++)
		{
			m_entities.at(i)->GetComponent<RigidBody>()->ApplyForce(btVector3(0, 10, 0));
		}
		SDL_HapticRumblePlay(haptic, 0.75f, 500);
	}
	//L2
	else if (INPUT->GetButton(1))
	{
		for (int i = 2; i < m_entities.size(); i++)
		{
			m_entities.at(i)->GetComponent<RigidBody>()->ApplyTorque(btVector3(0, 10, 0));
		}
		SDL_HapticRumblePlay(haptic, 0.75f, 500);
	}

	if (INPUT->GetButton(2))
	{
		m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0, 1, 0));
	}
	// x
	else if (INPUT->GetButton(3))
	{
		m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0, -1, 0));
	}
}

void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

void Application::Render()
{
	/* Clear context */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_JoystickClose(joystick);
	joystick = NULL;
	SDL_HapticClose(haptic);
	haptic = NULL;
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}
Application::~Application()
{
}
Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}
void Application::Run()
{
	Init();
	Loop();
	Quit();
}
