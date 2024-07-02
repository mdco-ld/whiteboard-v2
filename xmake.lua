if is_plat("linux") then
	add_requires("raylib", { system = true })
else
	add_requires("raylib")
end

target("wb")
	set_kind("binary")
	add_files("src/wb/*.cpp")
	add_files("src/cmd/wb.cpp")
	add_packages("raylib")
	add_includedirs("src/include")
	set_warnings("all")
	if is_mode("debug") then
		add_defines("DEBUG_MODE")
	end
target_end()

target("tests")
	set_kind("binary")
	add_files("src/wb/*.cpp")
	add_files("src/cmd/tests.cpp")
	add_includedirs("src/include")
	set_warnings("all")
	if is_mode("debug") then
		add_defines("DEBUG_MODE")
	end
target_end()
