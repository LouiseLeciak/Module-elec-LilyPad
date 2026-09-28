DOXYFILE	:=	Doxyfile

FIRMWARE_DIR := firmware

.PHONY: all hex flash clean re doc debug

all hex flash clean re debug:
	$(MAKE) -C $(FIRMWARE_DIR) $@

doc:
	doxygen $(DOXYFILE)
