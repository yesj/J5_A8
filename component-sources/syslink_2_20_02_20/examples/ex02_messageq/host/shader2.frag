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
uniform samplerStreamIMG streamtexture;
varying vec2 oTextureCoord1;
vec4 tempcolor;

void main() {
	tempcolor=textureStreamIMG(streamtexture,oTextureCoord1);
	gl_FragColor =vec4( tempcolor.rgb, 1.0);
//        gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
