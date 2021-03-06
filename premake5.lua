project "glfw"
	version = "3.0.4"

	language "C"
	includedirs { "deps", "include" }
	files { "deps/**.c", "deps/**.h",
			"include/**.h",
			"src/**.c", "src/**.h" }
			
	excludes { "deps/EGL/eglext.h" }
	
	defines { "_GLFW_USE_OPENGL=1", "_GLFW_WGL=1" }
	
	--[[configuration "*DLL"
		kind ("SharedLib")
		defines { "_GLFW_BUILD_DLL=1" }
		links { "opengl32" }
	configuration "*Lib"
		kind ("StaticLib")]]
		
	kind ("StaticLib")
		
	configuration "windows"
		defines { "_GLFW_WIN32=1", "_GLFW_WGL=1" }
		excludes { "src/cocoa_*", "src/egl_*", "src/glx_*", "src/nsgl_*", "src/x11_*" }
		
	configuration "linux"
		defines { "_GLFW_X11=1", "_GLFW_GLX=1" }
		defines { "_GLFW_HAS_GLXGETPROCADDRESSARB=1" } -- HACK HACK: but modern (targeted) computers should support this
		excludes { "src/cocoa_*", "src/egl_*", "src/nsgl_*", "src/wgl_*", "src/win32_*" }
	configuration { }
	
	warnings "Off" --Disable warnings, third party libs flood the log
	
	targetdir( LDIR_THIRDPARTY_LIB )
	location( LDIR_THIRDPARTY_BUILD )

	suffix_macro ( version, true )
	
if _OPTIONS["tp-samples"] then
	include ( "examples/" ) --was broken and for premake4.. now its not even there :v
end