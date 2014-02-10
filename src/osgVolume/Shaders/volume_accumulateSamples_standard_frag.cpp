char volume_accumulateSamples_standard_frag[] = "#version 110\n"
                                                "\n"
                                                "uniform sampler3D volumeTexture;\n"
                                                "\n"
                                                "uniform float AlphaFuncValue;\n"
                                                "\n"
                                                "vec4 accumulateSamples(vec3 ts, vec3 te, vec3 dt, float scale, float cutoff, int num_iterations)\n"
                                                "{\n"
                                                "    vec3 texcoord = te.xyz;\n"
                                                "    vec4 fragColor = vec4(0.0, 0.0, 0.0, 0.0);\n"
                                                "\n"
                                                "    while(num_iterations>0 && fragColor.a<cutoff)\n"
                                                "    {\n"
                                                "        vec4 color = texture3D( volumeTexture, texcoord);\n"
                                                "\n"
                                                "        if (color.a>AlphaFuncValue)\n"
                                                "        {\n"
                                                "            float r = color.a * ((1.0-fragColor.a)*scale);\n"
                                                "            fragColor.rgb += color.rgb*r;\n"
                                                "            fragColor.a += r;\n"
                                                "        }\n"
                                                "\n"
                                                "        texcoord += dt;\n"
                                                "\n"
                                                "        --num_iterations;\n"
                                                "    }\n"
                                                "\n"
                                                "    if (num_iterations>0) fragColor.a = 1.0;\n"
                                                "\n"
                                                "    return fragColor;\n"
                                                "}\n"
                                                "\n";
