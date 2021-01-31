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

enum ShaderStage
{
    SHADER_STAGE_VERTEX = 0,
    SHADER_STAGE_TESS_CONTROL,
    SHADER_STAGE_TESS_EVALUATION,
    SHADER_STAGE_GEOMETRY,
    SHADER_STAGE_FRAGMENT,
    SHADER_STAGE_COMPUTE
};
enum ProgramParameter
{
    PROGRAM_PARAMETER_BINARY_RETRIEVABLE_HINT = 0,
    PROGRAM_PARAMETER_SEPARABLE
};

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

enum BufferType
{
    BUFFER_TYPE_ARRAY = 0, //vertex
    BUFFER_TYPE_ATOMIC_COUNTER,
    BUFFER_TYPE_COPY_READ,  //transfer src
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
    BUFFER_TYPE_UNIFORM,            //uniform
};
enum BufferStorageFlagBits
{
    BUFFER_STORAGE_DYNAMIC_STORAGE_BIT = 0x1,
    BUFFER_STORAGE_MAP_READ_BIT = 0x2,
    BUFFER_STORAGE_MAP_WRITE_BIT = 0x4,
    BUFFER_STORAGE_MAP_PERSISTENT_BIT = 0x8,
    BUFFER_STORAGE_MAP_COHERENT_BIT = 0x10,
    BUFFER_STORAGE_CLIENT_STORAGE_BIT = 0x20
};
typedef Flags BufferStorageFlags;

GLenum Map(DataType dataType);
GLenum Map(BufferType bufferType);
GLenum Map(ShaderStage shaderStage);
GLenum Map(ShaderBinaryFormat shaderBinaryFormat);
GLenum Map(ProgramParameter para);