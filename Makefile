FIRMWARE_DIR := firmware

.PHONY: all hex flash clean re doc

all hex flash clean re doc:
	$(MAKE) -C $(FIRMWARE_DIR) $@
