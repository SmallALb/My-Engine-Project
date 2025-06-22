
set_project("FISHengine")


--���־�������
-- add_cxflags("-w")
add_cxflags("/wd4326")
add_cxflags("/wd4251")
add_cxflags("/wd4828")
add_cxflags("/wd4834")
add_cxflags("/wd4477")
add_cxflags("/wd4313")
add_cxflags("/wd4244")
add_cxflags("/wd4312")
add_cxflags("/wd4305")
add_cxflags("/wd4267")
add_cxflags("/utf-8")



--���opengl����
add_requires("glfw", {configs = {git = true, shared = true, vs_runtime = "MD"}})
add_requires("glm", {configs = {git = true}})
--docking��֧�汾
add_requires("imgui", {version = "v1.91.8-docking"})
--��־������
add_requires("spdlog", {version = "v1.15.2"})
--opengl���뺯��
function add_opengl()
    add_packages("glfw")
    add_packages("glm")
    add_packages("imgui")
end

add_rules("mode.debug", "mode.release")

--ģʽ�궨��
if is_mode("debug") then
    add_defines("DEBUG")
else 
    add_defines("RELEASE")
end

set_optimize("fast") 
set_warnings("all", "error")
set_languages("c++23", "cxxlatest")

--���涯̬��
target("FISH")
    set_kind("shared")
    add_includedirs("src")
    add_includedirs("src/Platform/OpenGL/glad")
    add_defines("GLFW_INCLUDE_NONE")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_files("src/**.c")
    --����Ϊ��̬��ĺ��־
    add_defines("FISH_DLL")
    add_packages("spdlog")
    add_opengl()
    if  is_plat("windows") then 
        add_rules("utils.symbols.export_all", {
            export_classes = true,
            export_auto_implib = true})
        add_ldflags("-Wl,--export-all-symbols")
        add_ldflags("--out-implib=libFISH.dll.a")
    end
    if  is_mode("release") then
        add_linkdirs("$(buildir)/windows/x64/release")
    end

--��ں���
target("ENTRY")
    set_kind("binary")
    add_files("ENTRY.cpp")
    add_deps("FISH")
    add_links("FISH")
    add_opengl()
    add_packages("spdlog")
    add_includedirs("src")
    if is_mode("release") then
        add_linkdirs("$(buildir)/windows/x64/release")
        if is_plat("windows") then
            --releaseģʽ�¹ر��ն˴���
            add_ldflags("/SUBSYSTEM:WINDOWS", "/ENTRY:mainCRTStartup", {force = true})
        end
    end

--��Դ�ļ�����
after_build(function (target) 
    if is_mode("debug") then
        os.cp("sharders", "build/windows/x64/debug")
        os.cp("picture", "build/windows/x64/debug")
    else
        os.cp("sharders", "build/windows/x64/release")
        os.cp("picture", "build/windows/x64/release")
    end
end)
