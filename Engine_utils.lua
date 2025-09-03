--添加opengl依赖
print("Adding Attachment...")
add_requires("glfw", {configs = {git = true, shared = true}})
add_requires("glm", {configs = {git = true}})
--docking分支版本
add_requires("imgui", {version = "v1.91.8-docking"})
--日志库依赖
add_requires("spdlog", {version = "v1.15.2"})
--FreeType
add_requires("freetype")
--音频库
add_requires("openal-soft", {configs = {shared = true}})
add_requires("libsndfile")
--json读取库
add_requires("nlohmann_json")
--性能分析库
add_requires("tracy")

--禁用字符集警告
print("Done to add Attachment...")

---------------------------------
--------引擎运行函数添加----------
---------------------------------

--基本初始化
function init_engine() 
    -- 禁用字符集警告
    add_cxflags("/wd4828")

    -- 模式宏定义
    if is_mode("debug") then
        add_defines("DEBUG")
    else 
        add_defines("RELEASE")
    end
    --启用23
    set_optimize("fast") 
    set_languages("c++26", "cxxlatest")

    --文件复制
    after_build(function (target)
        print("Copping files...")
        if is_mode("debug") then
            os.cp("sharders", "build/windows/x64/debug")
            os.cp("picture", "build/windows/x64/debug")
            os.cp("Fonts", "build/windows/x64/debug")
            os.cp("Sounds", "build/windows/x64/debug")
        else
            os.cp("sharders", "build/windows/x64/release")
            os.cp("picture", "build/windows/x64/release")
            os.cp("Fonts", "build/windows/x64/release")
            os.cp("Sounds", "build/windows/x64/release")

        end

        print("Done to copy Attachment...")
    end)

end  
-------------------------------------
-------opengl接入函数-----------------
function add_opengl()
    add_packages("glfw")
    add_packages("glm")
    add_packages("imgui")
end

--引擎依赖库接入
function add_engineAtt() 
    add_packages("spdlog")
    add_packages("freetype")
    add_packages("openal-soft")
    add_packages("libsndfile")
    add_packages("nlohmann_json")
    if is_mode("debug") then
        add_packages("tracy")
    end
end
--------------------------------------
-----------模式匹配设置-------------------
function mode_choice()
    if is_mode("release") then
        add_linkdirs("$(buildir)/windows/x64/release")
        if is_plat("windows") then
            --release模式下关闭终端窗口
            add_ldflags("/SUBSYSTEM:WINDOWS", "/ENTRY:mainCRTStartup", {force = true})
            add_syslinks("xinput", "user32", "gdi32")
        end
    end
end
------------------------------------------
