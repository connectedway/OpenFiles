macos_FLAGS=
linux_FLAGS=
win_FLAGS=
android_FLAGS=-DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/23.1.7779620/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 -DANDROID_PLATFORM=android-23 -DCMAKE_SYSTEM_VERSION=23 

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
	    -DOPENFILE_DEP1=./build/deprecated.cfg \
	    -DSMB_CONFIG=./configs/default.cfg \
	    -DSMB_CONFIG1=./configs/deprecated.cfg \
	    $($(1)_FLAGS)

.PHONY:	$(1)-$(2)-$(3)-$(4)-$(5)-build
$(1)-$(2)-$(3)-$(4)-$(5)-build:
	cmake --build build-$(1)-$(2)-$(3)-$(4)-$(5)

.PHONY:	$(1)-$(2)-$(3)-$(4)-$(5)-install
$(1)-$(2)-$(3)-$(4)-$(5)-install:
	sudo cmake --install build-$(1)-$(2)-$(3)-$(4)-$(5)
	sudo cp configs/$(1)-$(2)-$(3).xml /etc/openfiles.xml

.PHONY:	$(1)-$(2)-$(3)-$(4)-$(5)-uninstall
$(1)-$(2)-$(3)-$(4)-$(5)-uninstall:
	sudo rm /etc/openfiles.xml
	@xargs rm < build-$(1)-$(2)-$(3)-$(4)-$(5)/install_manifset.txt \
	2> /dev/null || true
	sudo @rmdir /usr/local/bin/openfiles 2> /dev/null || true

.PHONY:	$(1)-$(2)-$(3)-$(4)-$(5)-test
$(1)-$(2)-$(3)-$(4)-$(5)-test:
	cd build-$(1)-$(2)-$(3)-$(4)-$(5); \
	OPEN_FILES_HOME=./configs/$(1)-$(2)-$(3).xml \
	ctest

#
# a true full doesn't make sense.  Can't install on more than one target
#
.PHONY: $(1)-full
$(1)-full: $(1)-$(2)-$(3)-$(4)-$(5)-clean \
	$(1)-$(2)-$(3)-$(4)-$(5)-config \
	$(1)-$(2)-$(3)-$(4)-$(5)-build \

.PHONY: $(1)-config
$(1)-config:  $(1)-$(2)-$(3)-$(4)-$(5)-config

.PHONY: $(1)-build
$(1)-build:  $(1)-$(2)-$(3)-$(4)-$(5)-build

.PHONY: $(1)-clean
$(1)-clean:  $(1)-$(2)-$(3)-$(4)-$(5)-clean

.PHONY: info
info:   $(1)-$(2)-$(3)-$(4)-$(5)-info

endef

# add_target os,debug,smb,cipher,jni
$(eval $(call add_target,macos,debug,nosmb,openssl,nojni))
$(eval $(call add_target,macos,debug,smbserver,openssl,nojni))

$(eval $(call add_target,android,debug,smbserver,mbedtls,jni))
$(eval $(call add_target,android,nodebug,smbserver,mbedtls,jni))

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

linux-smb-client-info: 
	@echo ""
	@echo "    linux-smb-client (alias for linux-nodebug-smbclient-openssl-nojni)"
linux-smb-client-full: linux-nodebug-smbclient-openssl-nojni-full
linux-smb-client-clean: linux-nodebug-smbclient-openssl-nojni-clean
linux-smb-client-build: linux-nodebug-smbclient-openssl-nojni-build
linux-smb-client-install: linux-nodebug-smbclient-openssl-nojni-install
linux-smb-client-uninstall: linux-nodebug-smbclient-openssl-nojni-uninstall
linux-smb-client-test: linux-nodebug-smbclient-openssl-nojni-test
linux-smb-client-reinstall: linux-nodebug-smbclient-openssl-nojni-reinstall

info:	linux-smb-client-info

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

