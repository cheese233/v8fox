TARGET_V8 = x64.release.sample

all: server

depot_tools:
	git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git

v8/README.md: 
	git submodule update --init --recursive

define export-depot_tools-PATH
export PATH=$(shell pwd)/depot_tools:$$PATH
endef

.ONESHELL:
v8/buildtools: v8/README.md depot_tools
	$(export-depot_tools-PATH)
	gclient config https://github.com/v8/v8.git
	cd v8
	./build/install-build-deps.sh
	cd ..
	gclient sync

.ONESHELL:
v8/out.gn/$(TARGET_V8): v8/README.md
	$(export-depot_tools-PATH)
	cd v8
	tools/dev/v8gen.py $(TARGET_V8)
	gn args out.gn/$(TARGET_V8)
	ninja -C out.gn/$(TARGET_V8) v8_monolith

v8: v8/out.gn/$(TARGET_V8)

CXXFLAGS += -Iv8 -Iv8/include -Iv8/out.gn/$(TARGET_V8)/gen \
	-fno-rtti -lv8_monolith -lv8_libbase -lv8_libplatform -ldl -Lv8/out.gn/$(TARGET_V8)/obj/ \
	-pthread -std=c++20 -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX

server: v8 main.cpp
	$(CXX) main.cpp -o server $(CXXFLAGS)

.PHONY:
clean:
	$(RM) *.o
	$(RM) server

ifeq ($(LANG),)
.PHONY:
install: server
	cp ./v8fox_server.windows.json ./v8fox_server.json
	reg add HKEY_LOCAL_MACHINE\SOFTWARE\Mozilla\NativeMessagingHosts\v8fox_server /d $(shell pwd)\\v8fox_server.json
else
INSTALL_PATH := /usr/lib/mozilla/native-messaging-hosts/v8fox_server.json
.PHONY:
install: server
	echo '$(subst __PWD__,$(shell pwd),$(shell cat ./v8fox_server.linux.json) )' > $(INSTALL_PATH)
endif