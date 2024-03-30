macos_FLAGS=
linux_FLAGS=
win_FLAGS=
# arm64-v8a/x86_64
android_arm64-v8a_FLAGS=-DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/25.1.8937393/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-25 -DCMAKE_SYSTEM_VERSION=25
android_x86_64_FLAGS=-DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/25.1.8937393/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 -DANDROID_PLATFORM=android-25 -DCMAKE_SYSTEM_VERSION=25
android_x86_FLAGS=-DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/25.1.8937393/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86 -DANDROID_PLATFORM=android-25 -DCMAKE_SYSTEM_VERSION=25
android_armeabi-v7a_FLAGS=-DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/25.1.8937393/build/cmake/android.toolchain.cmake -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-25 -DCMAKE_SYSTEM_VERSION=25

.PHONY: info-header
info-header:
	@echo ""
	@echo "Openfile target builds:"
	@echo ""

.PHONY:	info
info:   info-header

# add_target os,debug,smb,cipher,jni
# where:
#    os = linux,android,macos,win
#    debug = debug,nodebug
#    smb = nosmb,smbclient,smbserver
#    cipher = openssl,gnutls,mbedtls
#    jni = jni,nojni
define add_target

.PHONY: $(1)-$(2)-$(3)-$(4)-$(5)-full
$(1)-$(2)-$(3)-$(4)-$(5)-full: \
    $(1)-$(2)-$(3)-$(4)-$(5)-clean \
    $(1)-$(2)-$(3)-$(4)-$(5)-config \
    $(1)-$(2)-$(3)-$(4)-$(5)-build \
    $(1)-$(2)-$(3)-$(4)-$(5)-install \
    $(1)-$(2)-$(3)-$(4)-$(5)-test

.PHONY: $(1)-$(2)-$(3)-$(4)-$(5)-reinstall
$(1)-$(2)-$(3)-$(4)-$(5)-reinstall: \
    $(1)-$(2)-$(3)-$(4)-$(5)-build \
    $(1)-$(2)-$(3)-$(4)-$(5)-install

.PHONY: $(1)-$(2)-$(3)-$(4)-$(5)-info
$(1)-$(2)-$(3)-$(4)-$(5)-info:
	@echo "    $(1)-$(2)-$(3)-$(4)-$(5)"

.PHONY: $(1)-$(2)-$(3)-$(4)-$(5)-clean
$(1)-$(2)-$(3)-$(4)-$(5)-clean:
	rm -rf build-$(1)-$(2)-$(3)-$(4)-$(5)

.PHONY: $(1)-$(2)-$(3)-$(4)-$(5)-config
$(1)-$(2)-$(3)-$(4)-$(5)-config:
	cmake -Bbuild-$(1)-$(2)-$(3)-$(4)-$(5) \
	    -DCMAKE_BUILD_TYPE=Debug \
	    -DOPENFILE_NAMING=./build/naming.cfg \
	    -DOPENFILE_PLATFORM=./build/$(1)-platform.cfg \
	    -DOPENFILE_BEHAVIOR=./build/$(1)-behavior.cfg \
	    -DOPENFILE_SIZING=./build/sizing.cfg \
	    -DOPENFILE_DEBUG=./build/$(2).cfg \
	    -DOPENFILE_SMB=./build/$(3).cfg \
	    -DOPENFILE_CIPHER=./build/$(4).cfg \
	    -DOPENFILE_JNI=./build/$(5).cfg \
	    -DSMB_CONFIG=./configs/default.cfg \
	    -DSMB_CONFIG1=./configs/deprecated.cfg \
	    $($(1)_FLAGS)

.PHONY:	$(1)-$(2)-$(3)-$(4)-$(5)-build
$(1)-$(2)-$(3)-$(4)-$(5)-build:
	cmake --build build-$(1)-$(2)-$(3)-$(4)-$(5)

.PHONY:	$(1)-$(2)-$(3)-$(4)-$(5)-install
$(1)-$(2)-$(3)-$(4)-$(5)-install:
	if [ "$(1)" != "android_arm64-v8a" ] && \
	[ "$(1)" != "android_armeabi-v7a" ] && \
	[ "$(1)" != "android_x86_64" ] && \
	[ "$(1)" != "android_x86" ]; then \
		sudo cmake --install build-$(1)-$(2)-$(3)-$(4)-$(5); \
		sudo cp configs/$(1)-$(2)-$(3).xml /etc/openfiles.xml; \
	fi

.PHONY:	$(1)-$(2)-$(3)-$(4)-$(5)-uninstall
$(1)-$(2)-$(3)-$(4)-$(5)-uninstall:
	if [ "$(1)" != "android_arm64-v8a" ] && \
	[ "$(1)" != "android_armeabi-v7a" ] && \
	[ "$(1)" != "android_x86_64" ] && \
	[ "$(1)" != "android_x86" ]; then \
		sudo rm /etc/openfiles.xml; \
		@xargs rm < build-$(1)-$(2)-$(3)-$(4)-$(5)/install_manifset.txt \
		2> /dev/null || true; \
		sudo @rmdir /usr/local/bin/openfiles 2> /dev/null || true; \
	fi

.PHONY:	$(1)-$(2)-$(3)-$(4)-$(5)-test
$(1)-$(2)-$(3)-$(4)-$(5)-test:
	if [ "$(1)" != "android_arm64-v8a" ] && \
	[ "$(1)" != "android_armeabi-v7a" ] && \
	[ "$(1)" != "android_x86_64" ] && \
	[ "$(1)" != "android_x86" ]; then \
		OPEN_FILES_HOME=`pwd`/configs/$(1)-$(2)-$(3).xml; \
		cd build-$(1)-$(2)-$(3)-$(4)-$(5); \
		ctest; \
	fi

.PHONY: $(1)-full
$(1)-full: $(1)-$(2)-$(3)-$(4)-$(5)-full

.PHONY: $(1)-config
$(1)-config:  $(1)-$(2)-$(3)-$(4)-$(5)-config

.PHONY: $(1)-build
$(1)-build:  $(1)-$(2)-$(3)-$(4)-$(5)-build

.PHONY: $(1)-clean
$(1)-clean:  $(1)-$(2)-$(3)-$(4)-$(5)-clean

.PHONY: info
info:   $(1)-$(2)-$(3)-$(4)-$(5)-info

endef

.PHONY: android-full
android-full:	\
	android_arm64-v8a-full \
	android_armeabi-v7a-full \
	android_x86_64-full \
	android_x86-full

.PHONY: android-clean
android-clean:	\
	android_arm64-v8a-clean \
	android_armeabi-v7a-clean \
	android_x86_64-clean \
	android_x86-clean

# add_target os,debug,smb,cipher,jni
$(eval $(call add_target,macos,debug,nosmb,gnutls,nojni))
$(eval $(call add_target,macos,debug,smbclient,gnutls,nojni))
$(eval $(call add_target,macos,debug,smbserver,gnutls,nojni))
$(eval $(call add_target,macos,nodebug,smbclient,gnutls,nojni))
$(eval $(call add_target,macos,nodebug,smbserver,gnutls,nojni))

$(eval $(call add_target,macos,debug,nosmb,openssl,nojni))
$(eval $(call add_target,macos,debug,smbclient,openssl,nojni))
$(eval $(call add_target,macos,debug,smbserver,openssl,nojni))
$(eval $(call add_target,macos,nodebug,smbclient,openssl,nojni))
$(eval $(call add_target,macos,nodebug,smbserver,openssl,nojni))

$(eval $(call add_target,macos,debug,nosmb,mbedtls,nojni))
$(eval $(call add_target,macos,debug,smbclient,mbedtls,nojni))
$(eval $(call add_target,macos,debug,smbserver,mbedtls,nojni))
$(eval $(call add_target,macos,nodebug,smbclient,mbedtls,nojni))
$(eval $(call add_target,macos,nodebug,smbserver,mbedtls,nojni))

$(eval $(call add_target,android_arm64-v8a,debug,smbserver,mbedtls,jni))
$(eval $(call add_target,android_arm64-v8a,nodebug,smbserver,mbedtls,jni))
$(eval $(call add_target,android_armeabi-v7a,debug,smbserver,mbedtls,jni))
$(eval $(call add_target,android_armeabi-v7a,nodebug,smbserver,mbedtls,jni))
$(eval $(call add_target,android_x86_64,debug,smbserver,mbedtls,jni))
$(eval $(call add_target,android_x86_64,nodebug,smbserver,mbedtls,jni))
$(eval $(call add_target,android_x86,debug,smbserver,mbedtls,jni))
$(eval $(call add_target,android_x86,nodebug,smbserver,mbedtls,jni))

$(eval $(call add_target,linux,debug,nosmb,openssl,nojni))
$(eval $(call add_target,linux,debug,smbclient,openssl,nojni))
$(eval $(call add_target,linux,debug,smbclient,mbedtls,nojni))
$(eval $(call add_target,linux,debug,smbclient,gnutls,nojni))
$(eval $(call add_target,linux,debug,smbserver,openssl,nojni))
$(eval $(call add_target,linux,debug,smbserver,mbedtls,nojni))
$(eval $(call add_target,linux,debug,smbserver,gnutls,nojni))
$(eval $(call add_target,linux,nodebug,smbclient,openssl,nojni))
$(eval $(call add_target,linux,nodebug,smbserver,openssl,nojni))

$(eval $(call add_target,win,debug,nosmb,openssl,nojni))
$(eval $(call add_target,win,debug,smbserver,openssl,nojni))

#
# Alias for Linux Client Production Target
#
linux-smb-client-info: 
	@echo ""
	@echo "    linux-smb-client (alias for linux-nodebug-smbclient-openssl-nojni)"
linux-smb-client-full: linux-nodebug-smbclient-openssl-nojni-full
linux-smb-client-clean: linux-nodebug-smbclient-openssl-nojni-clean
linux-smb-client-config: linux-nodebug-smbclient-openssl-nojni-config
linux-smb-client-build: linux-nodebug-smbclient-openssl-nojni-build
linux-smb-client-install: linux-nodebug-smbclient-openssl-nojni-install
linux-smb-client-uninstall: linux-nodebug-smbclient-openssl-nojni-uninstall
linux-smb-client-test: linux-nodebug-smbclient-openssl-nojni-test
linux-smb-client-reinstall: linux-nodebug-smbclient-openssl-nojni-reinstall

linux-smb-client-awsntlm1: linux-nodebug-smbclient-openssl-nojni-build
	OPEN_FILES_HOME=`pwd`/configs/linux-nodebug-smbclient-awsntlm.xml; \
	cd build-linux-nodebug-smbclient-openssl-nojni/of_smb_fs/test; \
	ctest

linux-smb-client-awsntlm2: linux-nodebug-smbclient-openssl-nojni-build
	OPEN_FILES_HOME=`pwd`/configs/linux-nodebug-smbclient-awsntlmdfs.xml; \
	cd build-linux-nodebug-smbclient-openssl-nojni/of_smb_fs/test; \
	ctest

linux-smb-client-awstest: linux-smb-client-awsntlm1 linux-smb-client-awsntlm2
#
# Alias for Linux Server Production Target
#
linux-smb-server-info: 
	@echo ""
	@echo "    linux-smb-server (alias for linux-nodebug-smbserver-openssl-nojni)"
linux-smb-server-full: linux-nodebug-smbserver-openssl-nojni-full
linux-smb-server-clean: linux-nodebug-smbserver-openssl-nojni-clean
linux-smb-server-config: linux-nodebug-smbserver-openssl-nojni-config
linux-smb-server-build: linux-nodebug-smbserver-openssl-nojni-build
linux-smb-server-install: linux-nodebug-smbserver-openssl-nojni-install
linux-smb-server-uninstall: linux-nodebug-smbserver-openssl-nojni-uninstall
linux-smb-server-test: linux-nodebug-smbserver-openssl-nojni-test
linux-smb-server-reinstall: linux-nodebug-smbserver-openssl-nojni-reinstall

linux-smb-server-v2.02-test: linux-debug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/linux-debug-smbserver-v2.02.xml \
	$(CURDIR)/build-linux-debug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

linux-smb-server-v2.10-test: linux-debug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/linux-debug-smbserver-v2.10.xml \
	$(CURDIR)/build-linux-debug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

linux-smb-server-v3.02-test: linux-debug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/linux-debug-smbserver-v3.02.xml \
	$(CURDIR)/build-linux-debug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

linux-smb-server-v3.11-ccm-test: linux-debug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/linux-debug-smbserver-v3.11-ccm.xml \
	$(CURDIR)/build-linux-debug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

linux-smb-server-v3.11-gcm-test: linux-debug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/linux-debug-smbserver-v3.11-gcm.xml \
	$(CURDIR)/build-linux-debug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

linux-smb-server-session-encrypt-test: linux-debug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/linux-debug-smbserver-session-encrypt.xml \
	$(CURDIR)/build-linux-debug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

linux-smb-server-tree-noencrypt-test: linux-debug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/linux-debug-smbserver-tree-noencrypt.xml \
	$(CURDIR)/build-linux-debug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

linux-test-all: \
	linux-full \
	linux-smb-server-v2.02-test \
	linux-smb-server-v2.10-test \
	linux-smb-server-v3.02-test \
	linux-smb-server-v3.11-ccm-test \
	linux-smb-server-v3.11-gcm-test \
	linux-smb-server-session-encrypt-test \
	linux-smb-server-tree-noencrypt-test

info:	linux-smb-client-info linux-smb-server-info

#
# Alias for Macos Client Production Target
#
macos-smb-client-info: 
	@echo ""
	@echo "    macos-smb-client (alias for macos-nodebug-smbclient-openssl-nojni)"
macos-smb-client-full: macos-nodebug-smbclient-openssl-nojni-full
macos-smb-client-clean: macos-nodebug-smbclient-openssl-nojni-clean
macos-smb-client-config: macos-nodebug-smbclient-openssl-nojni-config
macos-smb-client-build: macos-nodebug-smbclient-openssl-nojni-build
macos-smb-client-install: macos-nodebug-smbclient-openssl-nojni-install
macos-smb-client-uninstall: macos-nodebug-smbclient-openssl-nojni-uninstall
macos-smb-client-test: macos-nodebug-smbclient-openssl-nojni-test
macos-smb-client-reinstall: macos-nodebug-smbclient-openssl-nojni-reinstall

#
# Alias for Macos Server Production Target
#
macos-smb-server-info: 
	@echo ""
	@echo "    macos-smb-server (alias for macos-nodebug-smbserver-openssl-nojni)"
macos-smb-server-full: macos-nodebug-smbserver-openssl-nojni-full
macos-smb-server-clean: macos-nodebug-smbserver-openssl-nojni-clean
macos-smb-server-config: macos-nodebug-smbserver-openssl-nojni-config
macos-smb-server-build: macos-nodebug-smbserver-openssl-nojni-build
macos-smb-server-install: macos-nodebug-smbserver-openssl-nojni-install
macos-smb-server-uninstall: macos-nodebug-smbserver-openssl-nojni-uninstall
macos-smb-server-test: macos-nodebug-smbserver-openssl-nojni-test
macos-smb-server-reinstall: macos-nodebug-smbserver-openssl-nojni-reinstall

macos-smb-server-v2.02-test: macos-nodebug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/macos-nodebug-smbserver-v2.02.xml \
	$(CURDIR)/build-macos-nodebug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

macos-smb-server-v2.10-test: macos-nodebug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/macos-nodebug-smbserver-v2.10.xml \
	$(CURDIR)/build-macos-nodebug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

macos-smb-server-v3.02-test: macos-nodebug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/macos-nodebug-smbserver-v3.02.xml \
	$(CURDIR)/build-macos-nodebug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

macos-smb-server-v3.11-ccm-test: macos-nodebug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/macos-nodebug-smbserver-v3.11-ccm.xml \
	$(CURDIR)/build-macos-nodebug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

macos-smb-server-v3.11-gcm-test: macos-nodebug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/macos-nodebug-smbserver-v3.11-gcm.xml \
	$(CURDIR)/build-macos-nodebug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

macos-smb-server-session-encrypt-test: macos-nodebug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/macos-nodebug-smbserver-session-encrypt.xml \
	$(CURDIR)/build-macos-nodebug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

macos-smb-server-tree-noencrypt-test: macos-nodebug-smbserver-openssl-nojni-build
	OPEN_FILES_HOME=$(CURDIR)/configs/macos-nodebug-smbserver-tree-noencrypt.xml \
	$(CURDIR)/build-macos-nodebug-smbserver-openssl-nojni/of_smb_fs/test/test_fs_smb

macos-test-all: \
	macos-full \
	macos-smb-server-v2.02-test \
	macos-smb-server-v2.10-test \
	macos-smb-server-v3.02-test \
	macos-smb-server-v3.11-ccm-test \
	macos-smb-server-v3.11-gcm-test \
	macos-smb-server-session-encrypt-test \
	macos-smb-server-tree-noencrypt-test

info:	macos-smb-client-info macos-smb-server-info

help:
	@echo "make <target>-clean: Cleans the build directory"
	@echo "make <target>-config: Cleans the build"
	@echo "make <target>-build: Builds the target"
	@echo "make <target>-install: Installs the target on the system"
	@echo "make <target>-uninstall: Uninstall the target from the system"
	@echo "make <target>-test: Run Unit Tests on Target"
	@echo "make <target>-full: Cleans, Configs, Builds, Installs and Tests"
	@echo "make <target>-reinstall: Builds and Reinstalls"
	@echo ""
	@echo "make help: This text"
	@echo "make info: List Targets"
	@echo ""
	@echo "make <os>-full: cleans, configs, builds all os targets"
	@echo "make <os>-clean: cleans all os targets"
	@echo "make <os>-config: Configures all os targets"
	@echo "make <os>-build: Builds all os targets"
	@echo ""	
	@echo "make <os>-init: Initialize git repos for <os>"
	@echo "make <os>-update: Update git repos for <os>"
	@echo "make <os>-smb-init: Initialize git repos for <os> plus smb"
	@echo "make <os>-smb-update: Update git repos for <os> plus smb"
	@echo "NOTE: For smb-server, use git directly"
	@echo ""
	@echo "Where:"
	@echo "  <target> is a tupple of <os>-<debug>-<smb>-<cipher>-<jni>"
	@echo "  <os> is macos,linux,android,win"
	@echo "  <debug> is debug,nodebug"
	@echo "  <smb> is nosmb,smbclient,smbserver (server includes client)"
	@echo "  <cipher> is openssl,gnutls,mbedtls"
	@echo "  <jni> is jni,nojni"
	@echo ""
	@echo "Supported Targets shown with 'make info'"

all-init:
	git submodule init of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_pipe

all-update:
	git submodule update of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_pipe

smb-init:
	git submodule init of_smb of_smb_fs of_smb_client of_security \
	of_smb_browser of_netbios

smb-update:
	git submodule update of_smb of_smb_fs of_smb_client of_security \
	of_smb_browser of_netbios

macos-init: all-init
	git submodule init of_core_fs_darwin of_core_darwin

macos-update: all-update
	git submodule update of_core_fs_darwin of_core_darwin

macos-smb-init: macos-init smb-init

macos-smb-update: macos-update smb-update

linux-init: all-init
	git submodule init of_core_fs_linux of_core_linux

linux-update: all-update
	git submodule update of_core_fs_linux of_core_linux

linux-smb-init: linux-init smb-init

linux-smb-update: linux-update smb-update

win-init: all-init
	git submodule init of_core_fs_windows of_core_windows

win-update: all-update
	git submodule update of_core_fs_windows of_core_windows

win-smb-init: win-init smb-init

win-smb-update: win-update smb-update

