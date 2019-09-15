writeKbuild:
	@printf 'obj-m += exfat.o\nexfat-objs := %s\n' "$(strip $(exfat-objs))" > $(KBUILD_PATH)
