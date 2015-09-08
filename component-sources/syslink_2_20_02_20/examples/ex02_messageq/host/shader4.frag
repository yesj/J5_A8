/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */
precision lowp float;
#ifdef GL_IMG_texture_stream2
#extension GL_IMG_texture_stream2 : enable
#endif
uniform sampler2D streamtexture;
varying vec2 oTextureCoord1;
varying float outAlpha;
vec4 tempcolor;

void main() {
	tempcolor=texture2D(streamtexture,oTextureCoord1);
	gl_FragColor =vec4( tempcolor.bgr, outAlpha);
}
