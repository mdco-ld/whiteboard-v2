add_requires("raylib", { system = true })

target("wb")
	set_kind("binary")
	add_files("src/*.cpp")
	add_packages("raylib")
	add_includedirs("src")
	if is_mode("debug") then
		add_defines("DEBUG_MODE")
	end
target_end()
