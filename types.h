/**
 * @file types.h
 * @author yangzs
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include <glad/glad.h>

typedef unsigned int Flags;

enum DataType
{
	DATA_TYPE_BYTE = 0,
	DATA_TYPE_BYTE_UNSIGNED,
	DATA_TYPE_SHORT,
	DATA_TYPE_SHORT_UNSIGNED,
	DATA_TYPE_INT,
	DATA_TYPE_INT_UNSIGNED,
	DATA_TYPE_FLOAT,
	DATA_TYPE_FLOAT_HALF,
	DATA_TYPE_DOUBLE,
	DATA_TYPE_INT_2_10_10_10_REV,
	DATA_TYPE_UNSIGNED_INT_2_10_10_10_REV,
	DATA_TYPE_UNSIGNED_INT_10F_11F_11F_REV
};

enum ShaderBinaryFormat
{
	SHADER_BINARY_FORMAT_SPIR_V = 0
};

enum ShaderStageFlagBits
{
	SHADER_STAGE_VERTEX_BIT = 0x00000001,
	SHADER_STAGE_FRAGMENT_BIT = 0x00000002,
	SHADER_STAGE_GEOMETRY_BIT = 0x00000004,
	SHADER_STAGE_TESS_CONTROL_BIT = 0x00000008,
	SHADER_STAGE_TESS_EVALUATION_BIT = 0x00000010,
	SHADER_STAGE_COMPUTE_BIT = 0x00000020,
	SHADER_STAGE_ALL_BITS = 0xFFFFFFFF,
};
using ShaderStageFlags = Flags;

enum ProgramInterface
{
	PROGRAME_INTERFACE_UNIFORM,
	PROGRAME_INTERFACE_UNIFORM_BLOCK,
	PROGRAME_INTERFACE_ATOMIC_COUNTER_BUFFER,
	PROGRAME_INTERFACE_PROGRAM_INPUT,
	PROGRAME_INTERFACE_PROGRAM_OUTPUT,
	PROGRAME_INTERFACE_VERTEX_SUBROUTINE,
	PROGRAME_INTERFACE_TESS_CONTROL_SUBROUTINE,
	PROGRAME_INTERFACE_TESS_EVALUATION_SUBROUTINE,
	PROGRAME_INTERFACE_GEOMETRY_SUBROUTINE,
	PROGRAME_INTERFACE_FRAGMENT_SUBROUTINE,
	PROGRAME_INTERFACE_COMPUTE_SUBROUTINE,
	PROGRAME_INTERFACE_VERTEX_SUBROUTINE_UNIFORM,
	PROGRAME_INTERFACE_TESS_CONTROL_SUBROUTINE_UNIFORM,
	PROGRAME_INTERFACE_TESS_EVALUATION_SUBROUTINE_UNIFORM,
	PROGRAME_INTERFACE_GEOMETRY_SUBROUTINE_UNIFORM,
	PROGRAME_INTERFACE_FRAGMENT_SUBROUTINE_UNIFORM,
	PROGRAME_INTERFACE_COMPUTE_SUBROUTINE_UNIFORM,
	PROGRAME_INTERFACE_TRANSFORM_FEEDBACK_VARYING,
	PROGRAME_INTERFACE_TRANSFORM_FEEDBACK_BUFFER,
	PROGRAME_INTERFACE_BUFFER_VARIABLE,
	PROGRAME_INTERFACE_SHADER_STORAGE_BLOCK
};

enum BufferBindTarget
{
	BUFFER_TYPE_ARRAY = 0, //vertex
	BUFFER_TYPE_ATOMIC_COUNTER,
	BUFFER_TYPE_COPY_READ,	//transfer src
	BUFFER_TYPE_COPY_WRITE, //transfer dst
	BUFFER_TYPE_DISPATCH_INDIRECT,
	BUFFER_TYPE_DRAW_INDIRECT, //indirect
	BUFFER_TYPE_ELEMENT_ARRAY, //index
	BUFFER_TYPE_PARAMETER,
	BUFFER_TYPE_PIXEL_PACK,
	BUFFER_TYPE_PIXEL_UNPACK,
	BUFFER_TYPE_QUERY,
	BUFFER_TYPE_SHADER_STORAGE, //storage
	BUFFER_TYPE_TEXTURE,
	BUFFER_TYPE_TRANSFORM_FEEDBACK, //transform feedback
	BUFFER_TYPE_UNIFORM,			//uniform
};
enum BufferStorageFlagBits
{
	BUFFER_STORAGE_MAP_READ_BIT = 0x1,
	BUFFER_STORAGE_MAP_WRITE_BIT = 0x2,
	BUFFER_STORAGE_MAP_PERSISTENT_BIT = 0x40,
	BUFFER_STORAGE_MAP_COHERENT_BIT = 0x80,
	BUFFER_STORAGE_DYNAMIC_STORAGE_BIT = 0x100,
	BUFFER_STORAGE_CLIENT_STORAGE_BIT = 0x200,
};
using BufferStorageFlags=Flags;

enum BufferMapFlagBits
{
	BUFFER_MAP_READ_BIT = 0x1,
	BUFFER_MAP_WRITE_BIT = 0x2,
	BUFFER_MAP_INVALIDATE_RANGE_BIT = 0x4,
	BUFFER_MAP_INVALIDATE_BUFFER_BIT = 0x8,
	BUFFER_MAP_FLUSH_EXPLICIT_BIT = 0x10,
	BUFFER_MAP_UNSYNCHRONIZED_BIT = 0x20,
	BUFFER_MAP_PERSISTENT_BIT = 0x40,
	BUFFER_MAP_COHERENT_BIT = 0x80,
};
using BufferMapFlags=Flags;

enum MemoryBarrierFlagBits
{
	MEMORY_BARRIER_VERTEX_ATTRIB_ARRAY_BIT = 0x00000001,
	MEMORY_BARRIER_ELEMENT_ARRAY_BIT = 0x00000002,
	MEMORY_BARRIER_UNIFORM_BIT = 0x00000004,
	MEMORY_BARRIER_TEXTURE_FETCH_BIT = 0x00000008,
	MEMORY_BARRIER_SHADER_IMAGE_ACCESS_BIT = 0x00000020,
	MEMORY_BARRIER_COMMAND_BIT = 0x00000040,
	MEMORY_BARRIER_PIXEL_BUFFER_BIT = 0x00000080,
	MEMORY_BARRIER_TEXTURE_UPDATE_BIT = 0x00000100,
	MEMORY_BARRIER_BUFFER_UPDATE_BIT = 0x00000200,
	MEMORY_BARRIER_FRAMEBUFFER_BIT = 0x00000400,
	MEMORY_BARRIER_TRANSFORM_FEEDBACK_BIT = 0x00000800,
	MEMORY_BARRIER_ATOMIC_COUNTER_BIT = 0x00001000,
	MEMORY_BARRIER_SHADER_STORAGE_BIT = 0x00002000,
	MEMORY_BARRIER_CLIENT_MAPPED_BUFFER_BIT = 0x00004000,
	MEMORY_BARRIER_QUERY_BUFFER_BIT = 0x00008000,
	MEMORY_BARRIER_ALL_BITS = 0xFFFFFFFF,
};
using MemoryBarrierFlags=Flags;

GLenum Map(DataType dataType);
GLenum Map(BufferBindTarget target);
GLenum Map(ShaderBinaryFormat shaderBinaryFormat);