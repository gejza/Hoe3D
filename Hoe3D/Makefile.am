AUTOMAKE_OPTIONS = subdir-objects

#lib_LIBRARIES = ../bin/Hoe3D.so
bin_PROGRAMS = ../bin/Hoe2D.so ../bin/Hoe3D.so

src/icreate_parser.yy.cpp: src/icreate_parser.l ../HoeCore/template/hoe_flex.skl
	flex -S"../HoeCore/template/hoe_flex.skl" -o"src/icreate_parser.yy.cpp" src/icreate_parser.l
src/material_parser.yy.cpp: src/material_parser.l ../HoeCore/template/hoe_flex.skl
	flex -S"../HoeCore/template/hoe_flex.skl" -o"src/material_parser.yy.cpp" src/material_parser.l

Hoe2D: ../bin/Hoe2D.so
Hoe3D: ../bin/Hoe3D.so

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
src/light.cpp \
src/light_system.cpp \
src/map_utils.cpp \
src/material_system.cpp \
src/model_generator.cpp \
src/particle_emitor.cpp \
src/physics.cpp \
src/ref.cpp \
src/ref_base.cpp \
src/ref_opengl.cpp \
src/ref_opengl_ext.cpp \
src/ref_utils.cpp \
src/render_target.cpp \
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
src/unicode.cpp \
src/utils.cpp \
src/video.cpp \
src/icreate_parser.yy.cpp 
#src/material_parser.yy.cpp
#src/model_loader.cpp \
#src/object_controller.cpp \
#src/object_inspector.cpp \
#src/texture_converter.cpp \
#src/texture_loader.cpp \
#src/texture_system.cpp \
#

___bin_Hoe3D_so_CPPFLAGS = -I../HoeCore/include -I../HoeResource/include -DGL_GLEXT_PROTOTYPES1 $(FT2_CFLAGS) $(OGG_CFLAGS) $(VORBIS_CFLAGS)
___bin_Hoe3D_so_LDFLAGS = -shared -lGL -ljpeg -lpng -lopenal -lHoeCore -lXxf86vm -lXxf86dga -lvorbis -lvorbisfile  @LEXLIB@ $(FT2_LIBS) $(OGG_LIBS) $(VORBIS_LIBS) $(VORBISFILE_LIBS) $(VORBISENC_LIBS) $(XVID_LIBS) -lxvidcore

___bin_Hoe2D_so_SOURCES = \
src/camera.cpp \
src/cmdexec.cpp \
src/config.cpp \
src/freetype.cpp \
src/gridsurface.cpp \
src/hoe.cpp \
src/hoe2d.cpp \
src/hoe_font.cpp \
src/hoe_format.cpp \
src/hoe_index.cpp \
src/hoe_info.cpp \
src/hoe_input.cpp \
src/hoe_input_winapi.cpp \
src/hoe_input_xwin.cpp \
src/hoe2d_main.cpp \
src/hoe_picture.cpp \
src/hoe_time.cpp \
src/ref.cpp \
src/ref_base.cpp \
src/ref_opengl.cpp \
src/ref_opengl_ext.cpp \
src/ref_utils.cpp \
src/scene.cpp \
src/scene_base.cpp \
src/shared.cpp \
src/sound.cpp \
src/sound_al.cpp \
src/unicode.cpp \
src/utils.cpp \ 
src/icreate_parser.yy.cpp

___bin_Hoe2D_so_CPPFLAGS = -I../HoeCore/include -I../HoeResource/include -DHOE2D  -DGL_GLEXT_PROTOTYPES1 $(FT2_CFLAGS) $(OGG_CFLAGS) $(VORBIS_CFLAGS)
___bin_Hoe2D_so_LDFLAGS = -shared -lGL -ljpeg -lpng -lopenal -lHoeCore -lXxf86vm -lXxf86dga -lvorbis -lvorbisfile  @LEXLIB@ $(FT2_LIBS) $(OGG_LIBS) $(VORBIS_LIBS) $(VORBISFILE_LIBS) $(VORBISENC_LIBS) $(XVID_LIBS) -lxvidcore



