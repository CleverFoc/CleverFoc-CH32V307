add_rules("mode.debug", "mode.release")


toolchain("wch_riscv")
    set_kind("standalone")
    set_sdkdir("./tools/RISC-V Embedded GCC")
toolchain_end()


target("CleverFoc_CH32V307")
    set_kind("binary")
    add_files("startup/*.S")
    add_files("peripheral/src/*.c")
    add_files("core/*.c")
    add_files("src/*.c")
    add_files("src/driver/*.c")
    add_files("src/current/*.c")
    add_files("src/position/*.c")
    add_includedirs("src")
    add_includedirs("peripheral/inc")
    add_includedirs("core")
    set_arch("riscv")
    set_plat("cross")
    set_toolchains('wch_riscv')
    set_extension(".elf")

    add_cxxflags("-Werror")
    add_cxxflags("-std=gnu99")

    add_ldflags('--specs=nosys.specs')
    add_ldflags('--specs=nano.specs',{force = true})
    add_ldflags('-Wl,--gc-sections','-nostartfiles')
    add_ldflags('-T', path.join(os.projectdir(),'ld','Link.ld'),{force = true})
    
    -- After compilation, the .elf file is converted to .hex
    after_build(function (target)
        -- Get the path of the SDK
        local executeable_name = "riscv-none-embed-objcopy"
        local sdk_path = "./tools/RISC-V Embedded GCC"
        if is_host("windows") then
            executeable_name = executeable_name .. ".exe"
        end
        -- Convert .elf to .hex
        os.execv(path.join(sdk_path,'bin',executeable_name), {"-O", "ihex", target:targetfile(),path.join(path.directory(target:targetfile()),path.basename(target:targetfile()) .. ".hex")})
    end)
    
    
