DOXYFILE	:=	Doxyfile

FIRMWARE_DIR := firmware

.PHONY: all hex flash clean re doc debug size default halloween releases

all hex flash clean re debug size default halloween candy bw releases:
	$(MAKE) -C $(FIRMWARE_DIR) $@

doc:
	doxygen $(DOXYFILE)
