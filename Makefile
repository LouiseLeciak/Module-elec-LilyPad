DOXYFILE	:=	Doxyfile

FIRMWARE_DIR := firmware

.PHONY: all hex flash clean re doc

all hex flash clean re:
	$(MAKE) -C $(FIRMWARE_DIR) $@

doc:
	doxygen $(DOXYFILE)
