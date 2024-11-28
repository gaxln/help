# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_win64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LDDEPS +=
LINKCMD = $(CC) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_win64)
TARGETDIR = build/bin/windows-Debug
TARGET = $(TARGETDIR)/barberdule.exe
OBJDIR = obj/Win64/Debug
DEFINES += -DDEBUG
INCLUDES += -Ideps/raygui/src -Ideps/raylib-linux/include -IC:/msys64/mingw64/include/glib-2.0 -IC:/msys64/mingw64/lib/glib-2.0/glib -IC:/msys64/mingw64/lib/glib-2.0/include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g
LIBS += -lraylib -lwinmm -lkernel32 -lopengl32 -lgdi32 -lglib-2.0
ALL_LDFLAGS += $(LDFLAGS) -Ldeps/raylib-linux/lib -LC:/msys64/mingw64/lib -L/usr/lib64 -m64 -static-libgcc

else ifeq ($(config),debug_linux)
TARGETDIR = build/bin/linux-Debug
TARGET = $(TARGETDIR)/barberdule
OBJDIR = obj/Linux/Debug
DEFINES += -DDEBUG
INCLUDES += -Ideps/raygui/src -Ideps/raylib-linux/include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -g
LIBS += -lpthread -ldl -lm -lrt -lGL -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lraylib
ALL_LDFLAGS += $(LDFLAGS) -Ldeps/raylib-linux/lib -L/usr/lib64 -m64 -static-libgcc

else ifeq ($(config),release_win64)
TARGETDIR = build/bin/windows-Release
TARGET = $(TARGETDIR)/barberdule.exe
OBJDIR = obj/Win64/Release
DEFINES += -DNDEBUG
INCLUDES += -Ideps/raygui/src -Ideps/raylib-linux/include -IC:/msys64/mingw64/include/glib-2.0 -IC:/msys64/mingw64/lib/glib-2.0/glib -IC:/msys64/mingw64/lib/glib-2.0/include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2
LIBS += -lraylib -lwinmm -lkernel32 -lopengl32 -lgdi32 -lglib-2.0
ALL_LDFLAGS += $(LDFLAGS) -Ldeps/raylib-linux/lib -LC:/msys64/mingw64/lib -L/usr/lib64 -m64 -s -static-libgcc

else ifeq ($(config),release_linux)
TARGETDIR = build/bin/linux-Release
TARGET = $(TARGETDIR)/barberdule
OBJDIR = obj/Linux/Release
DEFINES += -DNDEBUG
INCLUDES += -Ideps/raygui/src -Ideps/raylib-linux/include
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -O2
LIBS += -lpthread -ldl -lm -lrt -lGL -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lraylib
ALL_LDFLAGS += $(LDFLAGS) -Ldeps/raylib-linux/lib -L/usr/lib64 -m64 -s -static-libgcc

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/application.o
GENERATED += $(OBJDIR)/client_form.o
GENERATED += $(OBJDIR)/colors.o
GENERATED += $(OBJDIR)/controller.o
GENERATED += $(OBJDIR)/grid.o
GENERATED += $(OBJDIR)/main.o
GENERATED += $(OBJDIR)/model.o
GENERATED += $(OBJDIR)/schedule.o
GENERATED += $(OBJDIR)/view.o
OBJECTS += $(OBJDIR)/application.o
OBJECTS += $(OBJDIR)/client_form.o
OBJECTS += $(OBJDIR)/colors.o
OBJECTS += $(OBJDIR)/controller.o
OBJECTS += $(OBJDIR)/grid.o
OBJECTS += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/model.o
OBJECTS += $(OBJDIR)/schedule.o
OBJECTS += $(OBJDIR)/view.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking barberdule
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning barberdule
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CC) -x c-header $(ALL_CFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/application.o: src/application.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/colors.o: src/colors.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/client_form.o: src/components/client_form.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/grid.o: src/components/grid.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/schedule.o: src/components/schedule.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/controller.o: src/controller.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main.o: src/main.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/model.o: src/model.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/view.o: src/view.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif