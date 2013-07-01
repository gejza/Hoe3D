
FLEX_TARGET(CreateParser src/icreate_parser.l  ${CMAKE_CURRENT_SOURCE_DIR}/src/icreate_parser.yy.cpp)
#FLEX_TARGET(LangParser src/lang_parser.l  ${CMAKE_CURRENT_SOURCE_DIR}/src/lang_parser.yy.cpp)

set(HOE3D_LIBS ${HOE3D_LIBS} ${FLEX_LIBRARIES} ${FL_LIBRARY})

IF(WIN32)
	set(HOE3D_LIBS ${Hoe3D_LIBS} Winmm)
ENDIF()

IF(UNIX)
	set(HOE3D_LIBS ${Hoe3D_LIBS} ${X11_X11_LIB} ${X11_Xxf86vm_LIB} Xxf86dga dl)
ENDIF()

if(USE_OPENGL)
	#########################################################
	# FIND OPENGL
	#########################################################
	find_package(OpenGL REQUIRED)
	find_package(OpenAL REQUIRED)
	include_directories(${OpenGL_INCLUDE_DIRS})
	link_directories(${OpenGL_LIBRARY_DIRS})
	add_definitions(${OpenGL_DEFINITIONS})
	if(NOT OPENGL_FOUND)
		message(ERROR " OPENGL not found!")
	else()
		add_definitions(-D_USE_OPENGL_)
		set(HOE3D_LIBS ${Hoe3D_LIBS} ${OPENGL_LIBRARIES} ${OPENAL_LIBRARY})
		set(HOE3D_SOURCES ${HOE3D_SOURCES}
			src/ref_opengl.cpp
			src/ref_opengl.h
			src/ref_opengl.inl
			src/ref_opengl_ext.cpp
			src/ref_opengl_ext.h
			src/sound_al.cpp
			src/hoe_input_xwin.cpp
		)
	endif(NOT OPENGL_FOUND)
endif()
if(USE_DIRECTX)
	#########################################################
	# FIND OPENGL
	#########################################################
	find_package(DirectX REQUIRED)
	include_directories(${DirectX_INCLUDE_DIR})
	add_definitions(-D_USE_D3D9_ -D_HOE_DS8_)
	set(HOE3D_LIBS ${HOE3D_LIBS} ${DirectX_LIBRARY} ${DirectX_DXERR9_LIBRARY} ${DirectX_DXGUID_LIBRARY} ${DirectX_DINPUT8_LIBRARY})
	set(HOE3D_SOURCES ${HOE3D_SOURCES}
		#src/ref_d3d8.cpp
		#src/ref_d3d8.h
		#src/ref_d3d8.inl
		src/ref_d3d9.cpp
		src/ref_d3d9.h
		src/ref_d3d9.inl
		src/ref_d3d9_debug.cpp
		#src/ref_d3dmobile.cpp
		#src/ref_d3dmobile.h
		#src/ref_d3dmobile.inl
		#src/ref_dd.cpp
		#src/ref_dd.h
		src/sound_ds.cpp
		src/hoe_input_di.cpp
	)
endif()

FIND_PACKAGE(Freetype)
INCLUDE_DIRECTORIES(${FREETYPE_INCLUDE_DIR_ft2build} ${FREETYPE_INCLUDE_DIR_freetype2})

#find_package(Vorbis)
#find_package(VorbisFile)
FIND_PACKAGE(OggVorbis)
SET(HOE3D_LIBS ${HOE3D_LIBS} ${OGG_LIBRARY} ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY})

FIND_PACKAGE(xvid)
SET(HOE3D_LIBS ${HOE3D_LIBS} ${XVID_LIBRARY})

add_library(Hoe3D SHARED

../include/hoe3d.h
../include/hoeelements.h
../include/hoefs.h
../include/hoeinput.h
../include/hoeinputkeys.h
../include/hoeinterfaces.h
../include/hoeobject.h
../include/hoeresources.h
../include/hoescene.h
../include/hoesystems.h
../include/hfmt/hmap_file.h
../include/hfmt/hmodel_file.h
../include/hfmt/hres_file.h
../include/hfmt/htex_file.h

src/StdAfx.h
src/2d.cpp
src/2d.h
src/camera.cpp
src/camera.h
src/cmdexec.cpp
src/cmdexec.h
src/config.cpp
src/config.h
#src/config_asm.asm
#src/filesystem.cpp
src/filesystem.h
#src/freetype.cpp
src/freetype.h
src/glext.h
src/gridsurface.cpp
src/gridsurface.h
#src/heightmap.cpp
src/heightmap.h
src/hoe.cpp
src/hoe.h
#src/hoe2d.cpp
src/hoe2d.h
#src/hoe2d_main.cpp
src/hoe3d.cpp
src/hoe3d.h
src/hoe_font.cpp
src/hoe_font.h
src/hoe_format.cpp
src/hoe_index.cpp
src/hoe_index.h
src/hoe_info.cpp
src/hoe_info.h
src/hoe_input.cpp
src/hoe_input.h
src/hoe_input_di.h
src/hoe_input_keymap.h
src/hoe_input_winapi.cpp
src/hoe_input_winapi.h
src/hoe_input_xwin.h
src/hoe_main.cpp
#src/hoe_map.cpp
src/hoe_map.h
#src/hoe_map_file.h
#src/hoe_map_part.cpp
#src/hoe_map_part.h
src/hoe_material.cpp
src/hoe_material.h
src/hoe_model.cpp
src/hoe_model.h
src/hoe_picture.cpp
src/hoe_picture.h
src/hoe_stream.cpp
src/hoe_stream.h
src/hoe_texture.cpp
src/hoe_texture.h
src/hoe_time.cpp
src/hoe_time.h
src/hoe_tokens.h
#src/hoefile.cpp
src/hoefile.h
src/icreate.cpp
src/icreate.h
src/icreate_parser.l
src/iget.cpp
src/light.cpp
src/light.h
src/light_system.cpp
src/light_system.h
src/map_utils.cpp
src/map_utils.h
src/material_parser.h
src/material_parser.l
src/material_system.cpp
src/material_system.h
src/model_generator.cpp
src/model_generator.h
src/model_loader.cpp
src/model_loader.h
src/model_shader.h
src/object_controller.cpp
src/object_controller.h
src/object_inspector.cpp
src/object_inspector.h
src/particle_emitor.cpp
src/particle_emitor.h
src/physics.cpp
src/physics.h
src/ref.cpp
src/ref.h
src/ref_base.cpp
src/ref_base.h
src/ref_utils.cpp
src/ref_utils.h
src/render_target.cpp
src/render_target.h
src/scene.cpp
src/scene.h
src/scene_base.cpp
src/scene_base.h
src/scene_env.cpp
src/scene_env.h
src/scene_graph.cpp
src/scene_graph.h
src/scene_part.cpp
src/scene_part.h
src/shader_vertex.cpp
src/shader_vertex.h
src/shared.cpp
src/shared.h
src/sound.cpp
src/sound.h
src/sound_al.h
src/sound_ds.h
#src/sound_mobile.cpp
src/sound_mobile.h
src/sound_ogg.cpp
src/sound_ogg.h
src/states.cpp
src/states.h
src/texture_converter.cpp
src/texture_converter.h
src/texture_hx.h
src/texture_loader.cpp
src/texture_loader.h
src/texture_system.cpp
src/texture_system.h
src/unicode.cpp
src/unicode.h
src/utils.cpp
src/utils.h
src/video.cpp
src/video.h


#src/Octree.cpp
#src/Octree.h
#src/QuadSurface.cpp
#src/QuadSurface.h
#src/Surface.cpp
#src/Surface.h
#src/skybox.cpp
#src/skybox.h
#src/cursor.cpp
#src/cursor.h

${HOE3D_SOURCES}

${FLEX_CreateParser_OUTPUTS}
)

target_link_libraries(Hoe3D ${HOE3D_LIBS})

# src/structures.cpp

