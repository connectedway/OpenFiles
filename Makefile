
git-url: \
	git-url.of_core_darwin \
	git-url.of_core_cheap \
	git-url.of_core_binheap \
	git-url.of_core \
	git-url.Unity \
	git-url.of_core_fs_darwin \
	git-url.of_core_fs_bookmarks \
	git-url.of_core_fs_linux \
	git-url.of_core_linux \
	git-url.of_core_windows \
	git-url.of_core_fs_windows \
	git-url.of_core_android \
	git-url.of_core_fs_android \
	git-url.of_core_jni \
	git-url.of_smb_jni \
	git-url.of_smb \
	git-url.of_smb_fs \
	git-url.of_smb_client \
	git-url.of_security \
	git-url.of_smb_browser \
	git-url.of_smb_server \
	git-url.of_resolver_fs \
	git-url.of_core_fs_pipe \
	git-url.of_netbios

git-url.of_core_darwin:
	cd of_core_darwin; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_darwin.git; \
	cd ..

git-url.of_core_cheap:
	cd of_core_cheap; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_cheap.git; \
	cd ..

git-url.of_core_binheap:
	cd of_core_binheap; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_binheap.git; \
	cd ..

git-url.of_core:
	cd of_core; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core.git; \
	cd ..

git-url.Unity:
	cd Unity; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/Unity.git; \
	cd ..

git-url.of_core_fs_darwin:
	cd of_core_fs_darwin; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_fs_darwin.git; \
	cd ..

git-url.of_core_fs_bookmarks:
	cd of_core_fs_bookmarks; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_fs_bookmarks.git; \
	cd ..

git-url.of_core_fs_linux:
	cd of_core_fs_linux; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_fs_linux.git; \
	cd ..

git-url.of_core_linux:
	cd of_core_linux; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_linux.git; \
	cd ..

git-url.of_core_windows:
	cd of_core_windows; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_windows.git; \
	cd ..

git-url.of_core_fs_windows:
	cd of_core_fs_windows; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_fs_windows.git; \
	cd ..

git-url.of_core_android:
	cd of_core_android; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_android.git; \
	cd ..

git-url.of_core_fs_android:
	cd of_core_fs_android; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_fs_android.git; \
	cd ..

git-url.of_core_jni:
	cd of_core_jni; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_jni.git; \
	cd ..

git-url.of_smb_jni:
	cd of_smb_jni; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_smb_jni.git; \
	cd ..

git-url.of_smb:
	cd of_smb; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_smb.git; \
	cd ..

git-url.of_smb_fs:
	cd of_smb_fs; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_smb_fs.git; \
	cd ..

git-url.of_smb_client:
	cd of_smb_client; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_smb_client.git; \
	cd ..

git-url.of_security:
	cd of_security; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_security.git; \
	cd ..

git-url.of_smb_browser:
	cd of_smb_browser; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_smb_browser.git; \
	cd ..

git-url.of_smb_server:
	cd of_smb_server; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_smb_server.git; \
	cd ..

git-url.of_resolver_fs:
	cd of_resolver_fs; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_resolver_fs.git; \
	cd ..

git-url.of_core_fs_pipe:
	cd of_core_fs_pipe; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_core_fs_pipe.git; \
	cd ..

git-url.of_netbios:
	cd of_netbios; \
	git remote set-url origin \
		ssh://git@github.com/connectedway/of_netbios.git; \
	cd ..

macos-smb: macos-smb-config macos-smb-build

macos-smb-config:
	cmake -Bbuild-macos-smb -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/macos-smb

macos-smb-build:
	cmake --build build-macos-smb

macos-smb-test:
	cd build-macos-smb; OPEN_FILES_HOME=./configs/darwin_debug.xml ctest

macos-smb-clean:
	rm -rf build-macos-smb

macos-smbfs: macos-smbfs-config macos-smbfs-build

macos-smbfs-config:
	cmake -Bbuild-macos-smbfs -DCMAKE_BUILD_TYPE=Debug -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 -DOPENFILE_CONFIG=./configs/macos-smbfs

macos-smbfs-build:
	cmake --build build-macos-smbfs

macos-smbfs-test:
	OPEN_FILES_HOME=./configs/darwin_debug.xml \
            ./build-macos-smbfs/of_smb_fs/test/test_fs_smb

macos-smbfs-clean:
	rm -rf build-macos-smbfs

macos-smbfs-install:
	cmake --install build-macos-smbfs
	cp configs/darwin_debug.xml /etc/openfiles.xml

macos-smbfs-uninstall:
	rm /etc/openfiles.xml
	@xargs rm < build-macos-smbfs/install_manifest.txt 2> /dev/null || true
	@rmdir /usr/local/bin/openfiles 2> /dev/null || true

macos-smbfs-init:
	git submodule init of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
	git submodule init of_smb of_smb_fs of_smb_client of_security \
	of_smb_browser of_netbios

macos-smbfs-update:
	git submodule update of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
	git submodule update of_smb of_smb_fs of_smb_client of_security \
	of_smb_browser of_netbios

androidsim-smb: androidsim-smb-config androidsim-smb-build

androidsim-smb-config:
	cmake -Bbuild-androidsim-smb -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/23.1.7779620/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 -DANDROID_PLATFORM=android-23 -DCMAKE_SYSTEM_VERSION=23 -DOPENFILE_CONFIG=./configs/android-smb

androidsim-smb-build:
	cmake --build build-androidsim-smb

androidsim-smb-clean:
	rm -rf build-androidsim-smb

linux-smb: linux-smb-config linux-smb-build

linux-smb-config:
	cmake -Bbuild-linux-smb -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/linux-smb

linux-smb-build:
	cmake --build build-linux-smb

linux-smb-test:
	OPEN_FILES_HOME=./configs/linux_debug.xml \
            ./build-linux-smb/of_smb_fs/test/test_fs_smb

linux-smb-clean:
	rm -rf build-linux-smb

linux-smbfs: linux-smbfs-config linux-smbfs-build

linux-smbfs-config:
	cmake -Bbuild-linux-smbfs -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/linux-smbfs -DMBEDTLS_ROOT_DIR=/usr/local

linux-smbfs-build:
	cmake --build build-linux-smbfs

linux-smbfs-install:
	cmake --install build-linux-smbfs
	cp configs/linux_debug.xml /etc/openfiles.xml

linux-smbfs-uninstall:
	rm /etc/openfiles.xml
	@xargs rm < build-linux-smbfs/install_manifest.txt 2> /dev/null || true
	@rmdir /usr/local/bin/openfiles 2> /dev/null || true

linux-smbfs-test:
#	cd build-linux-smbfs; OPEN_FILES_HOME=./configs/linux_debug.xml ctest
	OPEN_FILES_HOME=./configs/linux_debug.xml \
            ./build-linux-smbfs/of_smb_fs/test/test_fs_smb

linux-smbfs-clean:
	rm -rf build-linux-smbfs

linux-smbfs-init:
	git submodule init of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
	git submodule init of_smb of_smb_fs of_smb_client of_security \
	of_smb_browser of_netbios

linux-smbfs-update:
	git submodule update of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
	git submodule update of_smb of_smb_fs of_smb_client of_security \
	of_smb_browser of_netbios

yocto-smbfs: yocto-smbfs-config yocto-smbfs-build

yocto-smbfs-config:
	cmake -Bbuild-yocto-smbfs -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/yocto-smbfs -DMBEDTLS_ROOT_DIR=/usr/local

yocto-smbfs-build:
	cmake --build build-yocto-smbfs

yocto-smbfs-install:
	cmake --install build-yocto-smbfs

yocto-smbfs-test:
	OPEN_FILES_HOME=./configs/linux_debug.xml \
            ./build-yocto-smbfs/of_smb_fs/test/test_fs_smb

yocto-smbfs-clean:
	rm -rf build-yocto-smbfs

linux-smbloop: linux-smbloop-config linux-smbloop-build

linux-smbloop-config:
	cmake -Bbuild-linux-smbloop -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/linux-smbloop -DMBEDTLS_ROOT_DIR=/usr/local

linux-smbloop-build:
	cmake --build build-linux-smbloop

linux-smbloop-install:
	cmake --install build-linux-smbloop
	cp configs/linux_loop.xml /etc/openfiles.xml

linux-smbloop-uninstall:
	rm /etc/openfiles.xml
	@xargs rm < build-linux-smbloop/install_manifest.txt 2> /dev/null || true
	@rmdir /usr/local/bin/openfiles 2> /dev/null || true

linux-smbloop-test:
#	cd build-linux-smbfs; OPEN_FILES_HOME=./configs/linux_debug.xml ctest
	OPEN_FILES_HOME=./configs/linux_loop.xml \
            ./build-linux-smbloop/of_smb_fs/test/test_fs_smb

linux-smbloop-clean:
	rm -rf build-linux-smbloop

linux-smbloop-init:
	git submodule init of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
	git submodule init of_smb of_smb_fs of_smb_client of_security \
	of_smb_browser of_smb_server of_netbios

linux-smbloop-update:
	git submodule update of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
	git submodule update of_smb of_smb_fs of_smb_client of_security \
	of_smb_browser of_smb_server of_netbios


linux: linux-config linux-build

linux-config:
	cmake -Bbuild-linux -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/linux

linux-build:
	cmake --build build-linux

linux-install:
	cmake --install build-linux

linux-uninstall:
	@xargs rm < build-linux/install_manifest.txt 2> /dev/null || true
	@rmdir /usr/local/bin/openfiles 2> /dev/null || true

linux-init:
	git submodule init of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe

linux-update:
	git submodule update of_core_cheap of_core_binheap of_core Unity \
	of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe

linux-test:
	cd build-linux; OPEN_FILES_HOME=./configs/linux_debug.xml ctest

linux-clean:
	rm -rf build-linux

linux-perf: linux-perf-config linux-perf-build

linux-perf-config:
	cmake -Bbuild-linux-perf -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/linux-perf

linux-perf-build:
	cmake --build build-linux-perf

linux-perf-test:
	OPEN_FILES_HOME=./configs/linux_debug.xml \
            ./build-linux-perf/of_core/test/test_fs_linux

linux-perf-clean:
	rm -rf build-linux-perf


