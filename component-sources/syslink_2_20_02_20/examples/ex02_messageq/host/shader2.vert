/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

attribute vec4 av4position;
attribute vec2 aTextureCoord1;

uniform mat4 mvp;

varying vec2 oTextureCoord1;

void main() {
	oTextureCoord1 = aTextureCoord1;
	gl_Position = mvp * av4position;
}

