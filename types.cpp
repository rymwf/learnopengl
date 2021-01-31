/**
 * @file types.cpp
 * @author yangzs
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "types.h"
constexpr GLenum glFormatEnums[]{
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_HALF_FLOAT,
    GL_FLOAT,
    GL_DOUBLE,
    GL_INT_2_10_10_10_REV,
    GL_UNSIGNED_INT_2_10_10_10_REV,
    GL_UNSIGNED_INT_10F_11F_11F_REV};

constexpr GLenum glShaderBinaryFormatEnums[]{
    GL_SHADER_BINARY_FORMAT_SPIR_V};

constexpr GLenum glBufferTypeEnums[]{
    GL_ARRAY_BUFFER, //vertex
    GL_ATOMIC_COUNTER_BUFFER,
    GL_COPY_READ_BUFFER,  //transfer src
    GL_COPY_WRITE_BUFFER, //transfer dst
    GL_DISPATCH_INDIRECT_BUFFER,
    GL_DRAW_INDIRECT_BUFFER, //indirect
    GL_ELEMENT_ARRAY_BUFFER, //index
    GL_PARAMETER_BUFFER,
    GL_PIXEL_PACK_BUFFER,
    GL_PIXEL_UNPACK_BUFFER,
    GL_QUERY_BUFFER,
    GL_SHADER_STORAGE_BUFFER, //storage
    GL_TEXTURE_BUFFER,
    GL_TRANSFORM_FEEDBACK_BUFFER, //transform feedback
    GL_UNIFORM_BUFFER,            //uniform
};

constexpr GLenum glShaderStageEnums[]{
    GL_VERTEX_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_GEOMETRY_SHADER,
    GL_FRAGMENT_SHADER,
    GL_COMPUTE_SHADER};

constexpr GLenum glProgramParameterEnums[]{
    GL_PROGRAM_BINARY_RETRIEVABLE_HINT,
    GL_PROGRAM_SEPARABLE};

GLenum Map(DataType dataType)
{
    return glFormatEnums[static_cast<size_t>(dataType)];
}
GLenum Map(BufferType bufferType)
{
    return glBufferTypeEnums[static_cast<size_t>(bufferType)];
}
GLenum Map(ShaderStage shaderStage)
{
    return glShaderStageEnums[static_cast<size_t>(shaderStage)];
}
GLenum Map(ShaderBinaryFormat shaderBinaryFormat)
{
    return glShaderBinaryFormatEnums[static_cast<size_t>(shaderBinaryFormat)];
}
GLenum Map(ProgramParameter para)
{
    return glProgramParameterEnums[static_cast<size_t>(para)];
}