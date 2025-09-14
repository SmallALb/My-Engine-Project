includes("Engine_utils.lua")

set_project("FISHengine")
init_engine()
add_rules("mode.debug", "mode.release")


--���涯̬��
target("FISH")
    set_kind("shared")
    add_includedirs("src")
    add_includedirs("src/Platform/OpenGL/glad")
    --�궨��
    add_defines("GLFW_INCLUDE_NONE")
    add_defines("FT2_BUILD_LIBRARY")  -- ȷ�� FreeType ʹ���ڲ�ʵ��
    add_defines("FT_CONFIG_OPTION_SYSTEM_ZLIB=0")  -- ����ϵͳ ZLIB
    add_defines("FISH_DLL") --����Ϊ��̬��ĺ��־
    --ָ�������ļ�
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_files("src/**.c")
    --���Ӱ����⣩
    add_engineAtt()
    add_opengl()
    if  is_plat("windows") then 
        add_rules("utils.symbols.export_all", {
            export_classes = true,
            export_auto_implib = true})
        add_ldflags("-Wl,--export-all-symbols")
        add_syslinks("xinput", "user32", "gdi32")
    end
    if  is_mode("release") then
        add_linkdirs("$(buildir)/windows/x64/release")
    end

target("TESTRENDER")
    set_kind("binary")
    add_files("RenderTest.cpp")
    add_deps("FISH")
    add_links("FISH")
    add_opengl()
    add_engineAtt()
    add_includedirs("src")
    mode_choice()

--��������src�е�ͷ�ļ�
after_build(function (target) 
    if not os.isdir("build/include") then
            os.mkdir("build/include")
    end
    
    --ͷ�ļ�����
    for _, header in ipairs(os.files("src/**.h")) do
        --�ҵ������src������ļ�
        local Rpath = path.relative(header, "src")
        --ƴ�������build��·��
        local Dpath = path.join("build/include", Rpath)
        if not os.isdir(Dpath) then 
            os.mkdir(path.directory(Dpath));
        end
        os.cp(header, Dpath)
    end

    --���ļ�����
    --debug��
    if not os.isdir("build/include/lib-debug") then 
        os.mkdir("build/include/lib-debug");
    end
    --release��
    if not os.isdir("build/include/lib-release") then 
        os.mkdir("build/include/lib-release");
    end

    if is_mode("debug") then
        os.cp("build/windows/x64/debug/FISH.lib", "build/include/lib-debug")
        os.cp("build/windows/x64/debug/FISH.dll", "build/include/lib-debug")
    else
        os.cp("build/windows/x64/release/FISH.lib", "build/include/lib-release")
        os.cp("build/windows/x64/release/FISH.dll", "build/include/lib-release")
    end

end)