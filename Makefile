DOXYFILE	:=	Doxyfile

FIRMWARE_DIR := firmware

.PHONY: all hex flash clean re doc debug size

all hex flash clean re debug size:
	$(MAKE) -C $(FIRMWARE_DIR) $@

doc:
	doxygen $(DOXYFILE)
