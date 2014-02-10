char volume_compute_ray_color_frag[] = "#version 110\n"
                                       "\n"
                                       "uniform vec4 viewportDimensions;\n"
                                       "uniform sampler3D volumeTexture;\n"
                                       "uniform vec3 volumeCellSize;\n"
                                       "\n"
                                       "uniform float SampleRatioValue;\n"
                                       "uniform float TransparencyValue;\n"
                                       "varying mat4 texgen_withProjectionMatrixInverse;\n"
                                       "\n"
                                       "// forward declare, probided by volume_accumulateSamples*.frag shaders\n"
                                       "vec4 accumulateSamples(vec3 ts, vec3 te, vec3 dt, float scale, float cutoff, int num_iterations);\n"
                                       "\n"
                                       "vec4 accumulateSegment(vec3 ts, vec3 te)\n"
                                       "{\n"
                                       "    const int max_iterations = 8192;\n"
                                       "\n"
                                       "    float density = volumeCellSize.x;\n"
                                       "    if (volumeCellSize.y<density) density = volumeCellSize.y;\n"
                                       "    if (volumeCellSize.z<density) density = volumeCellSize.z;\n"
                                       "    density /= SampleRatioValue;\n"
                                       "\n"
                                       "    int num_iterations = int(ceil(length((te-ts).xyz)/density));\n"
                                       "\n"
                                       "\n"
                                       "    vec4 baseColor = vec4(1.0,1.0,1.0,1.0);\n"
                                       "\n"
                                       "    // clamp to 2 to max_iterations range.\n"
                                       "    if (num_iterations<2) num_iterations = 2;\n"
                                       "    if (num_iterations>max_iterations)\n"
                                       "    {\n"
                                       "        num_iterations = max_iterations;\n"
                                       "        baseColor.r = 0.0;\n"
                                       "    }\n"
                                       "\n"
                                       "    // traverse from front to back\n"
                                       "    vec3 deltaTexCoord=(ts-te).xyz/float(num_iterations-1);\n"
                                       "    float stepLength = length(deltaTexCoord);\n"
                                       "\n"
                                       "    //float scale = 0.5/sampleRatio;\n"
                                       "    float scale = stepLength/length(volumeCellSize);\n"
                                       "    if (scale>1.0) scale = 1.0;\n"
                                       "\n"
                                       "    scale *= TransparencyValue;\n"
                                       "\n"
                                       "    float cutoff = 1.0-1.0/256.0;\n"
                                       "\n"
                                       "    vec4 fragColor;\n"
                                       "\n"
                                       "    fragColor = accumulateSamples(ts, te, deltaTexCoord, scale, cutoff, num_iterations);\n"
                                       "\n"
                                       "    fragColor *= baseColor;\n"
                                       "\n"
                                       "    return fragColor;\n"
                                       "}\n"
                                       "\n"
                                       "vec3 clampToUnitCube(vec3 ts, vec3 te)\n"
                                       "{\n"
                                       "    if (te.x>=0.0 && te.x<=1.0 &&\n"
                                       "        te.y>=0.0 && te.y<=1.0 &&\n"
                                       "        te.z>=0.0 && te.z<=1.0)\n"
                                       "    {\n"
                                       "        // do nothing... te inside volume\n"
                                       "        return te;\n"
                                       "    }\n"
                                       "    else\n"
                                       "    {\n"
                                       "        if (te.x<0.0)\n"
                                       "        {\n"
                                       "            float r = -te.x / (ts.x-te.x);\n"
                                       "            te = te + (ts-te)*r;\n"
                                       "        }\n"
                                       "\n"
                                       "        if (te.x>1.0)\n"
                                       "        {\n"
                                       "            float r = (1.0-te.x) / (ts.x-te.x);\n"
                                       "            te = te + (ts-te)*r;\n"
                                       "        }\n"
                                       "\n"
                                       "        if (te.y<0.0)\n"
                                       "        {\n"
                                       "            float r = -te.y / (ts.y-te.y);\n"
                                       "            te = te + (ts-te)*r;\n"
                                       "        }\n"
                                       "\n"
                                       "        if (te.y>1.0)\n"
                                       "        {\n"
                                       "            float r = (1.0-te.y) / (ts.y-te.y);\n"
                                       "            te = te + (ts-te)*r;\n"
                                       "        }\n"
                                       "\n"
                                       "        if (te.z<0.0)\n"
                                       "        {\n"
                                       "            float r = -te.z / (ts.z-te.z);\n"
                                       "            te = te + (ts-te)*r;\n"
                                       "        }\n"
                                       "\n"
                                       "        if (te.z>1.0)\n"
                                       "        {\n"
                                       "            float r = (1.0-te.z) / (ts.z-te.z);\n"
                                       "            te = te + (ts-te)*r;\n"
                                       "        }\n"
                                       "    }\n"
                                       "    return te;\n"
                                       "}\n"
                                       "\n"
                                       "\n"
                                       "vec4 computeRayColor(float px, float py, float depth_start, float depth_end)\n"
                                       "{\n"
                                       "    float viewportWidth = viewportDimensions[2];\n"
                                       "    float viewportHeight = viewportDimensions[3];\n"
                                       "\n"
                                       "    px -= viewportDimensions.x;\n"
                                       "    py -= viewportDimensions.y;\n"
                                       "\n"
                                       "    // start and end clip space coords\n"
                                       "    vec4 start_clip = vec4((px/viewportWidth)*2.0-1.0, (py/viewportHeight)*2.0-1.0, (depth_start)*2.0-1.0, 1.0);\n"
                                       "    vec4 end_clip = vec4((px/viewportWidth)*2.0-1.0, (py/viewportHeight)*2.0-1.0, (depth_end)*2.0-1.0, 1.0);\n"
                                       "\n"
                                       "    vec4 start_texcoord = texgen_withProjectionMatrixInverse * start_clip;\n"
                                       "    vec4 end_texcoord = texgen_withProjectionMatrixInverse * end_clip;\n"
                                       "\n"
                                       "    start_texcoord.xyz = start_texcoord.xyz / start_texcoord.w;\n"
                                       "    start_texcoord.w = 1.0;\n"
                                       "\n"
                                       "    end_texcoord.xyz = end_texcoord.xyz / end_texcoord.w;\n"
                                       "    end_texcoord.w = 1.0;\n"
                                       "\n"
                                       "    vec3 clamped_start_texcoord = clampToUnitCube(end_texcoord.xyz, start_texcoord.xyz);\n"
                                       "    vec3 clamped_end_texcoord = clampToUnitCube(start_texcoord.xyz, end_texcoord.xyz);\n"
                                       "\n"
                                       "    return accumulateSegment(clamped_start_texcoord, clamped_end_texcoord);\n"
                                       "}\n"
                                       "\n";
