workspace("barberdule")
configurations({ "Debug", "Release" })
platforms({ "Win64", "Linux" })

project("barberdule")
kind("ConsoleApp")
language("C")
targetdir("build/bin/%{cfg.system}-%{cfg.buildcfg}")
files({ "src/**.h", "src/**.c" })

includedirs({ "deps/raygui/src/", "deps/raylib-linux/include/" })
libdirs({ "deps/raylib-linux/lib" })
links({ "raylib" })

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")

filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")

filter("platforms:Linux")
system("linux")
architecture("x86_64")
links({
	"raylib",
	"pthread",
	"dl",
	"m",
	"rt",
	"GL",
	"X11",
	"Xrandr", -- Dependencias adicionales necesarias
	"Xinerama", -- para X11 y OpenGL
	"Xi",
	"Xxf86vm",
	"Xcursor",
	"raylib",
})
linkoptions({ "-static-libgcc" }) -- Quitar "-static" si prefieres bibliotecas dinámicas.

filter("platforms:Win64")
system("windows")
architecture("x86_64")
includedirs({ "C:/msys64/mingw64/include/glib-2.0", "C:/msys64/mingw64/lib/glib-2.0/glib/" })
includedirs({ "C:/msys64/mingw64/lib/glib-2.0/include" })
libdirs({ "C:/msys64/mingw64/lib" })
links({ "winmm", "kernel32", "opengl32", "gdi32", "glib-2.0" })
linkoptions({ "-static-libgcc" })
