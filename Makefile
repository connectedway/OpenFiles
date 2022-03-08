
macos-smb: macos-smb-config macos-smb-build

macos-smb-config:
	cmake -Bbuild-macos-smb -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/macos-smb

macos-smb-build:
	cmake --build build-macos-smb

macos-smb-test:
	cd build-macos-smb; OPEN_FILES_HOME=./configs/carwin_debug.xml ctest

macos-smb-clean:
	rm -rf build-macos-smb


androidsim-smb: androidsim-smb-config androidsim-smb-build

androidsim-smb-config:
	cmake -Bbuild-androidsim-smb -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/23.1.7779620/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 -DANDROID_PLATFORM=android-23 -DCMAKE_SYSTEM_VERSION=23 -DOPENFILE_CONFIG=./configs/android-smb

androidsim-smb-build:
	cmake --build build-androidsim-smb

androidsim-smb-clean:
	rm -rf build-androidsim-smb

