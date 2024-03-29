/*
-----------------------------------------------------------------------------
This source file is part of the �aQtree� cooperation project between the Institute of Animation, Brainpets GbR and University of Konstanz.
Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation & University of Konstanz and brainpets GbR

www.aQtree.de

aQtree nodes function in FRAPPER, the framework application developed at the Institute of Animation.
FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper
Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

uniform sampler2D inputMap;
uniform sampler2D indicationMap;
uniform vec4 pixelSize;

	
varying vec4 vertexScreenSpace;
			
vec3 saturate(vec3 color, float intensity)
{
	float value = (color.r + color.g + color.b) / 3.0;
	vec3 tone = color - value;
	return value + tone * intensity;
}

vec3 aquaColorMatch(vec3 color)
{
	float value = (color.r + color.g + color.b) / 3.0;
	vec3 tone = (color - value) * 4.0;
	float maxToneValue = max(tone.r, max(tone.g, tone.b));
	return 1.0 + tone - maxToneValue;
}
						 
void main (void)
{
	vec2 texCoord = vertexScreenSpace.xy / vertexScreenSpace.w * 0.5 + vec2(0.5);

/*	vec3 colorDiff = texture2D(inputMap, texCoord).rgb - texture2D(indicationMap, texCoord).rgb;
	
	//gl_FragColor.rgb = vec3(0.5) + colorDiff;
	gl_FragColor.rgb = saturate(texture2D(inputMap, texCoord).rgb, 2.0).rgb;
	
	gl_FragColor.a = length(colorDiff); 
	
*/

	vec3 input = texture2D(inputMap, texCoord).rgb;
	vec3 indication = texture2D(indicationMap, texCoord).rgb;

	//aqua color difference
	vec3 maskNeg = clamp(input - indication, -1.0, 0.0);
	vec3 aquaColorNeg = aquaColorMatch(maskNeg);

	vec3 maskPos = clamp(input - indication, 0.0, 1.0);
	vec3 aquaColorPos = aquaColorMatch(maskPos);


	//color map differences
	float weightNeg = abs(clamp(length(input) - length(indication), -1.0, 0.0));
	vec3 inputColorNeg = input;

	float weightPos = clamp(length(input) - length(indication), 0.0, 1.0);
	vec3 inputColorPos = input;

	//final color
	//gl_FragColor.rgb = saturate(mix(aquaColorPos, inputColorPos, 0.7), 1.5);	
	//gl_FragColor.rgb = saturate(mix(aquaColorNeg, inputColorNeg, 0.7), 1.5);
	
	gl_FragColor.a = clamp(pow(weightNeg, 0.4), 0.0, 0.7);
	//gl_FragColor.a = clamp(pow(weightPos, 0.4), 0.0, 0.5) * (1.0 - length(inputColorPos));
	//gl_FragColor.a = clamp(pow(weightNeg, 0.4), 0.0, 1.0);
	
	
	gl_FragColor.rgb = saturate(mix(aquaColorPos, inputColorPos, 0.7), 1.5);
	gl_FragColor.a = clamp(pow(weightNeg, 0.4) * 2.0, 0.0, 1.0);
	
}

