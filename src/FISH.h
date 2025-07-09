#pragma once
//第三方库
#include <ft2build.h>
#include <freetype/freetype.h>
#include <imgui.h>
//基本
#include "FISH/Time.h"
#include "FISH/FISHMemory.h"
#include "FISH/Keycodes.h"
#include "FISH/MouseButtonCodes.h"
#include "FISH/Application.h"
#include "FISH/input.h"
#include "FISH/Controller.h"
#include "FISH/Layer.h"
#include "FISH/Log.h"
#include "FISH/ImGui/ImGuiLayer.h"
#include "FISH/Events/Events.h"

//按键码
#include "FISH/keycodes.h"
#include "FISH/MouseButtonCodes.h"


//渲染基本
#include "FISH/REnderer/TextureHandle.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/Shader.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/BaseShape.h"

//渲染扩展
#include "FISH/Renderer/Font.h"
#include "FISH/Renderer/Animation.h"

//物体基类
#include "FISH/Object/Object.h"
#include "FISH/Object/Mesh.h"
#include "FISH/Object/SkyBox.h"
#include "FISH/Object/Botton.h"
#include "FISH/Object/Light.h"
#include "FISH/Object/AmbientLight.h"
#include "FISH/Object/DirectionLight.h"
#include "FISH/Object/PointLight.h"
#include "FISH/Object/SpotLight.h"
#include "FISH/Object/Camera.h"
#include "FISH/Object/perspectiveCamera.h"
//物理和游戏基本
#include "FISH/physics/ObjectCast.h"
#include "FISH/Game/GameObject.h"
#include "FISH/physics/RayTest.h"
#include "FISH/physics/CastTest.h"
//渲染
#include "FISH/Renderer/Renderstatus.h"
#include "FISH/Renderer/Renderer.h"

//声音
#include "FISH/Sound.h"

//入口
#include "FISH/Events/Application.h"
#include "FISH/EntryPoint.h"

