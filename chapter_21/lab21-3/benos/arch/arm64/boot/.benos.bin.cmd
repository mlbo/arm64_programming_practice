cmd_arch/arm64/boot/benos.bin := aarch64-linux-gnu-objcopy -O binary -R .note -R .note.gnu.build-id -R .comment -S  benos.strip arch/arm64/boot/benos.bin
