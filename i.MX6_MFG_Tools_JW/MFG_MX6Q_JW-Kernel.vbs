Set wshShell = CreateObject("WScript.shell")
wshShell.run "mfgtool2.exe -c ""linux"" -l ""Hybus-Linux4.1.15_MX6Q-eMMC-Kernel"" -s ""board=sabresd"" -s ""folder=sabreauto"" -s ""nand=nand"" -s ""soc=6dl"" -s ""nanddtb=gpmi-weim"" -s ""part_uboot=1"" -s ""part_kernel=2"" -s ""part_dtb=3"" -s ""part_rootfs=4""  "
Set wshShell = Nothing