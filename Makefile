# Kernel release name and product name.

RELEASE_NAME=Big Navajo
RELEASE_PRODUCT=Ne.app.NeKernel
RELEASE_VERSION=v1.0.0

.PHONY: all
all:
	@echo "[ReleaseInfo]"
	@echo "Release=\"${RELEASE_NAME}-${RELEASE_VERSION}-${RELEASE_PRODUCT}\""
	@echo "Vendor=\"Ne.app\""
	@echo "Profile=\"Desktop\""
