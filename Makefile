
macos-smb: macos-smb-config macos-smb-build

macos-smb-config:
	cmake -Bbuild-macos-smb -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/macos-smb

macos-smb-build:
	cmake --build build-macos-smb

macos-smb-test:
	cd build-macos-smb; OPEN_FILES_HOME=./configs/darwin_debug.xml ctest

macos-smb-clean:
	rm -rf build-macos-smb


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
#	cd build-linux-smb; OPEN_FILES_HOME=./configs/linux_debug.xml ctest
	OPEN_FILES_HOME=./configs/linux_debug.xml \
            ./build-linux-smb/of_smb_fs/test/test_fs_smb

linux-smb-clean:
	rm -rf build-linux-smb

linux-smbfs: linux-smbfs-config linux-smbfs-build

linux-smbfs-config:
	cmake -Bbuild-linux-smbfs -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/linux-smbfs

linux-smbfs-build:
	cmake --build build-linux-smbfs

linux-smbfs-test:
#	cd build-linux-smbfs; OPEN_FILES_HOME=./configs/linux_debug.xml ctest
	OPEN_FILES_HOME=./configs/linux_debug.xml \
            ./build-linux-smbfs/of_smb_fs/test/test_fs_smb

linux-smbfs-clean:
	rm -rf build-linux-smbfs

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


