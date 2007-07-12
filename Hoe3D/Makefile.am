AUTOMAKE_OPTIONS = subdir-objects

#lib_LIBRARIES = ../bin/Hoe3D.so
bin_PROGRAMS = ../bin/Hoe3D.so

src/icreate_parser.c: src/icreate_parser.l
	flex -o"src/icreate_parser.c" src/icreate_parser.l
src/material_parser.c: src/material_parser.l
	flex -o"src/material_parser.c" src/material_parser.l


___bin_Hoe3D_so_SOURCES = \
src/camera.cpp \
src/cmdexec.cpp \
src/config.cpp \
src/filesystem.cpp \
src/freetype.cpp \
src/gridsurface.cpp \
src/heightmap.cpp \
src/hoe.cpp \
src/hoe2d.cpp \
src/hoe_font.cpp \
src/hoe_format.cpp \
src/hoe_index.cpp \
src/hoe_info.cpp \
src/hoe_input.cpp \
src/hoe_input_di.cpp \
src/hoe_input_winapi.cpp \
src/hoe_input_xwin.cpp \
src/hoe_main.cpp \
src/hoe_material.cpp \
src/hoe_model.cpp \
src/hoe_picture.cpp \
src/hoe_stream.cpp \
src/hoe_texture.cpp \
src/hoe_time.cpp \
src/hoefile.cpp \
src/icreate.cpp \
src/iget.cpp \
src/light.cpp \
src/light_system.cpp \
src/map_utils.cpp \
src/material_system.cpp \
src/model_generator.cpp \
src/model_loader.cpp \
src/object_controller.cpp \
src/object_inspector.cpp \
src/particle_emitor.cpp \
src/physics.cpp \
src/ref.cpp \
src/ref_base.cpp \
src/ref_opengl.cpp \
src/ref_opengl_ext.cpp \
src/ref_utils.cpp \
src/render_target.cpp \
src/resmgr.cpp \
src/scene.cpp \
src/scene_base.cpp \
src/scene_env.cpp \
src/scene_graph.cpp \
src/scene_part.cpp \
src/shader_vertex.cpp \
src/shared.cpp \
src/sound.cpp \
src/sound_al.cpp \
src/sound_ds.cpp \
src/states.cpp \
src/texture_converter.cpp \
src/texture_convjpg.cpp \
src/texture_convpng.cpp \
src/texture_loader.cpp \
src/texture_system.cpp \
src/unicode.cpp \
src/utils.cpp \
src/video.cpp \
src/icreate_parser.c \
src/material_parser.c

___bin_Hoe3D_so_CPPFLAGS = -I../HoeCore/include -DGL_GLEXT_PROTOTYPES1 $(FT2_CFLAGS) $(OGG_CFLAGS) $(VORBIS_CFLAGS)
___bin_Hoe3D_so_LDFLAGS = -shared -lGL -ljpeg -lpng -lopenal -lHoeCore -lXxf86vm -lXxf86dga -lvorbis -lvorbisfile  @LEXLIB@ $(FT2_LIBS) $(OGG_LIBS) $(VORBIS_LIBS) $(VORBISFILE_LIBS) $(VORBISENC_LIBS) $(XVID_LIBS) -lxvidcore



