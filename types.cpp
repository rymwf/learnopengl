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
constexpr GLenum glDataTypeArray[]{
	GL_BYTE,
	GL_UNSIGNED_BYTE,
	GL_SHORT,
	GL_UNSIGNED_SHORT,
	GL_INT,
	GL_UNSIGNED_INT,
	GL_FLOAT,
	GL_HALF_FLOAT,
	GL_DOUBLE,
	GL_INT_2_10_10_10_REV,
	GL_UNSIGNED_INT_2_10_10_10_REV,
	GL_UNSIGNED_INT_10F_11F_11F_REV};

constexpr GLenum glShaderBinaryFormatArray[]{
	GL_SHADER_BINARY_FORMAT_SPIR_V};

constexpr GLenum glBufferBindTargetArray[]{
	GL_ARRAY_BUFFER, //vertex
	GL_ATOMIC_COUNTER_BUFFER,
	GL_COPY_READ_BUFFER,  //transfer src
	GL_COPY_WRITE_BUFFER, //transfer dst
	GL_DISPATCH_INDIRECT_BUFFER,
	GL_DRAW_INDIRECT_BUFFER, //indirect
	GL_ELEMENT_ARRAY_BUFFER, //index
	GL_PARAMETER_BUFFER,	 //indirect multidraw
	GL_PIXEL_PACK_BUFFER,
	GL_PIXEL_UNPACK_BUFFER,
	GL_QUERY_BUFFER,
	GL_SHADER_STORAGE_BUFFER, //storage
	GL_TEXTURE_BUFFER,
	GL_TRANSFORM_FEEDBACK_BUFFER, //transform feedback
	GL_UNIFORM_BUFFER			  //uniform
};

constexpr GLenum glShaderStageArray[]{
	GL_VERTEX_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_GEOMETRY_SHADER,
	GL_FRAGMENT_SHADER,
	GL_COMPUTE_SHADER};

constexpr GLbitfield glShaderStageFlagBitArray[]{
	GL_VERTEX_SHADER_BIT,		   // 0x00000001
	GL_FRAGMENT_SHADER_BIT,		   // 0x00000002
	GL_GEOMETRY_SHADER_BIT,		   // 0x00000004
	GL_TESS_CONTROL_SHADER_BIT,	   // 0x00000008
	GL_TESS_EVALUATION_SHADER_BIT, // 0x00000010
	GL_COMPUTE_SHADER_BIT,		   // 0x00000020
	GL_ALL_SHADER_BITS,			   // 0xFFFFFFFF
};

constexpr GLenum glProgramParameterArray[]{
	GL_PROGRAM_BINARY_RETRIEVABLE_HINT,
	GL_PROGRAM_SEPARABLE};

constexpr GLbitfield glBufferStorageFlagBitArray[]{
	GL_MAP_READ_BIT,		// 0x0001
	GL_MAP_WRITE_BIT,		// 0x0002
	GL_MAP_PERSISTENT_BIT,	// 0x0040
	GL_MAP_COHERENT_BIT,	// 0x0080
	GL_DYNAMIC_STORAGE_BIT, // 0x0100
	GL_CLIENT_STORAGE_BIT,	// 0x0200
};
constexpr GLenum glMutableStorageUsageArray[]{
	GL_STREAM_DRAW,
	GL_STREAM_READ,
	GL_STREAM_COPY,
	GL_DYNAMIC_DRAW,
	GL_DYNAMIC_READ,
	GL_DYNAMIC_COPY,
	GL_STATIC_DRAW,
	GL_STATIC_READ,
	GL_STATIC_COPY,
};
constexpr GLbitfield glBufferMapFlagBitArray[]{
	GL_MAP_READ_BIT,			  // 0x0001
	GL_MAP_WRITE_BIT,			  // 0x0002
	GL_MAP_INVALIDATE_RANGE_BIT,  // 0x0004
	GL_MAP_INVALIDATE_BUFFER_BIT, // 0x0008
	GL_MAP_FLUSH_EXPLICIT_BIT,	  // 0x0010
	GL_MAP_UNSYNCHRONIZED_BIT,	  // 0x0020
	GL_MAP_PERSISTENT_BIT,		  // 0x0040
	GL_MAP_COHERENT_BIT,		  // 0x0080
};
constexpr GLbitfield glMemoryBarrierFlagBitArray[]{
	GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,	 // 0x00000001
	GL_ELEMENT_ARRAY_BARRIER_BIT,		 // 0x00000002
	GL_UNIFORM_BARRIER_BIT,				 // 0x00000004
	GL_TEXTURE_FETCH_BARRIER_BIT,		 // 0x00000008
	GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,	 // 0x00000020
	GL_COMMAND_BARRIER_BIT,				 // 0x00000040
	GL_PIXEL_BUFFER_BARRIER_BIT,		 // 0x00000080
	GL_TEXTURE_UPDATE_BARRIER_BIT,		 // 0x00000100
	GL_BUFFER_UPDATE_BARRIER_BIT,		 // 0x00000200
	GL_FRAMEBUFFER_BARRIER_BIT,			 // 0x00000400
	GL_TRANSFORM_FEEDBACK_BARRIER_BIT,	 // 0x00000800
	GL_ATOMIC_COUNTER_BARRIER_BIT,		 // 0x00001000
	GL_SHADER_STORAGE_BARRIER_BIT,		 // 0x00002000
	GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT, // 0x00004000
	GL_QUERY_BUFFER_BARRIER_BIT,		 // 0x00008000
	GL_ALL_BARRIER_BITS,				 // 0xFFFFFFFF
};
GLenum glImageTypeArray[]{
	GL_TEXTURE_1D_ARRAY,
	GL_TEXTURE_2D_ARRAY,
	GL_TEXTURE_3D,
};
GLenum glImageViewTypeArray[]{
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP,
	GL_TEXTURE_1D_ARRAY,
	GL_TEXTURE_2D_ARRAY,
	GL_TEXTURE_CUBE_MAP_ARRAY,
};

GLenum glImageTilingArray[]{
	GL_OPTIMAL_TILING_EXT,
	GL_LINEAR_TILING_EXT,
};

GLenum glFilterArray[]{
	GL_NEAREST,
	GL_LINEAR,
};

GLenum glSamplerWrapModeArray[]{
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
};
GLenum glCompareOpArray[]{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS};

GLenum glImageLayoutArray[]{
	GL_NONE,										  //| VK_IMAGE_LAYOUT_UNDEFINED                                      |
	GL_LAYOUT_GENERAL_EXT,							  //| VK_IMAGE_LAYOUT_GENERAL                                        |
	GL_LAYOUT_COLOR_ATTACHMENT_EXT,					  //| VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL                       |
	GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT,			  //| VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT                       |
	GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT,			  //| VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL                |
	GL_LAYOUT_SHADER_READ_ONLY_EXT,					  //| VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL                       |
	GL_LAYOUT_TRANSFER_SRC_EXT,						  //| VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL                           |
	GL_LAYOUT_TRANSFER_DST_EXT,						  //| VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL                           |
//	GL_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_EXT, //| VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR |
//	GL_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_EXT, //| VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR
};

GLenum glComponentSwizzleArray[]{
	GL_NONE,
	GL_ZERO,
	GL_ONE,
	GL_RED,
	GL_GREEN,
	GL_BLUE,
	GL_ALPHA};

GLenum Map(DataType dataType)
{
	return glDataTypeArray[static_cast<size_t>(dataType)];
}
GLenum Map(BufferBindTarget target)
{
	return glBufferBindTargetArray[static_cast<size_t>(target)];
}
GLenum Map(ShaderBinaryFormat shaderBinaryFormat)
{
	return glShaderBinaryFormatArray[static_cast<size_t>(shaderBinaryFormat)];
}
GLbitfield Map(ShaderStageFlagBits flagbit)
{
	return flagbit;
}
GLenum Map(ImageType imageType, bool multisample)
{
	if (multisample)
		return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	else
		return glImageTypeArray[static_cast<size_t>(imageType)];
}
GLenum Map(SamplerWrapMode wrapMode)
{
	return glSamplerWrapModeArray[static_cast<size_t>(wrapMode)];
}
GLenum Map(CompareOp op)
{
	return glCompareOpArray[static_cast<size_t>(op)];
}
GLenum Map(Filter filter)
{
	return glFilterArray[static_cast<size_t>(filter)];
}
GLenum Map(ImageTiling tiling)
{
	return glImageTilingArray[static_cast<size_t>(tiling)];
}
GLenum Map(ImageLayout imageLayout)
{
	return glImageLayoutArray[static_cast<size_t>(imageLayout)];
}
GLenum Map(BufferMutableStorageUsage usage)
{
	return glMutableStorageUsageArray[static_cast<size_t>(usage)];
}
GLenum Map(ImageViewType viewType, bool multisample)
{
	if (multisample)
	{
		if (viewType == IMAGE_VIEW_TYPE_2D)
			return GL_TEXTURE_2D_MULTISAMPLE;
		else if (viewType == IMAGE_VIEW_TYPE_2D_ARRAY)
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		else
			throw std::runtime_error("wrong image view type");
	}
	else
		return glImageViewTypeArray[static_cast<size_t>(viewType)];
}
GLenum Map(ComponentSwizzle swizzle)
{
	return glComponentSwizzleArray[static_cast<size_t>(swizzle)];
}