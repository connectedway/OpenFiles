
macos-smb: macos-smb-config macos-smb-build

macos-smb-config:
	cmake -Bbuild-macos-smb -DCMAKE_BUILD_TYPE=Debug -DOPENFILE_CONFIG=./configs/macos-smb

macos-smb-build:
	cmake --build build-macos-smb

macos-smb-test:
	cd build-macos-smb; OPEN_FILES_HOME=./configs/carwin_debug.xml ctest

macos-smb-clean:
	rm -rf build-macos-smb

