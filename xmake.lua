includes("Engine_utils.lua")

set_project("FISHengine")
init_engine()
add_rules("mode.debug", "mode.release")


--引擎动态库
target("FISH")
    set_kind("shared")
    add_includedirs("src")
    add_includedirs("src/Platform/OpenGL/glad")
    --宏定义
    add_defines("GLFW_INCLUDE_NONE")
    add_defines("FT2_BUILD_LIBRARY")  -- 确保 FreeType 使用内部实现
    add_defines("FT_CONFIG_OPTION_SYSTEM_ZLIB=0")  -- 禁用系统 ZLIB
    add_defines("FISH_DLL") --编译为动态库的宏标志
    --指定编译文件
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_files("src/**.c")
    --链接包（库）
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

--入口函数
target("ENTRY")
    set_kind("binary")
    add_files("ENTRY.cpp")
    add_deps("FISH")
    add_links("FISH")
    add_opengl()
    add_engineAtt()
    add_includedirs("src")
    mode_choice()

--测试碰撞
target("TESTCAST")
    set_kind("binary")
    add_files("CASTTEST.cpp")
    add_deps("FISH")
    add_links("FISH")
    add_opengl()
    add_engineAtt()
    add_includedirs("src")
    mode_choice()

--导出所有src中的头文件
after_build(function (target) 
    if not os.isdir("build/include") then
            os.mkdir("build/include")
    end
    
    --头文件导出
    for _, header in ipairs(os.files("src/**.h")) do
        --找到相对于src的相对文件
        local Rpath = path.relative(header, "src")
        --拼接相对于build的路径
        local Dpath = path.join("build/include", Rpath)
        if not os.isdir(Dpath) then 
            os.mkdir(path.directory(Dpath));
        end
        os.cp(header, Dpath)
    end

    --库文件导出
    --debug用
    if not os.isdir("build/include/lib-debug") then 
        os.mkdir("build/include/lib-debug");
    end
    --release用
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